/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * <OWNER> = Apple Inc.
 * <ORGANIZATION> = Apple Inc.
 * <YEAR> = 2014
 *
 * Copyright (c) 2014, Apple Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the <ORGANIZATION> nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 *************************************************************************************
 * \file HDRMetricsFrame.cpp
 *
 * \brief
 *    HDRMetricsFrame class source files for computing various distortion metrics
 *
 * \author
 *     - Alexis Michael Tourapis         <atourapis@apple.com>
 *************************************************************************************
 */

#include <time.h>
#include <string.h>
#include <math.h>
#include <vector>
#include "HDRMetricsFrame.H"

//-----------------------------------------------------------------------------
// constructor /de-constructor
//-----------------------------------------------------------------------------


HDRMetricsFrame::HDRMetricsFrame(ProjectParameters *inputParams)
{
  m_numberOfClips = 2;
  m_frameStore       = new hdrtoolslib::Frame*         [m_numberOfClips];
  m_cropFrameStore   = new hdrtoolslib::Frame*         [m_numberOfClips];
  m_windowFrameStore = new hdrtoolslib::Frame*         [m_numberOfClips];
  m_inputFrame       = new hdrtoolslib::Input*         [m_numberOfClips];
  m_inputFile        = new hdrtoolslib::IOVideo*       [m_numberOfClips];
  m_startFrame       = new int            [m_numberOfClips];
  m_cropOffsetLeft   = new int            [m_numberOfClips];
  m_cropOffsetTop    = new int            [m_numberOfClips];
  m_cropOffsetRight  = new int            [m_numberOfClips];
  m_cropOffsetBottom = new int            [m_numberOfClips];
  
  for (int index = 0; index < m_numberOfClips; index++) {
    m_frameStore[index]       = NULL;
    m_cropFrameStore[index]   = NULL;
    m_windowFrameStore[index] = NULL;
    m_inputFrame[index]       = NULL;
    m_inputFile[index]        = &inputParams->m_inputFile[index];
    m_startFrame[index]       = m_inputFile[index]->m_startFrame;
    m_cropOffsetLeft[index]   = inputParams->m_cropOffsetLeft  [index];
    m_cropOffsetTop[index]    = inputParams->m_cropOffsetTop   [index];
    m_cropOffsetRight[index]  = inputParams->m_cropOffsetRight [index];
    m_cropOffsetBottom[index] = inputParams->m_cropOffsetBottom[index];
  }
  
  m_distortionMetric = new hdrtoolslib::DistortionMetric*[hdrtoolslib::DIST_METRICS];
  m_windowDistortionMetric = new hdrtoolslib::DistortionMetric*[hdrtoolslib::DIST_METRICS];
  
  m_enableWindow  = false;
  m_windowMinPosX = 0;
  m_windowMaxPosX = 0;
  m_windowMinPosY = 0;
  m_windowMaxPosY = 0;
  
  m_windowWidth   = 0;
  m_windowHeight  = 0;
  // Copy input distortion parameters
  m_distortionParameters = inputParams->m_distortionParameters;

  for (int index = hdrtoolslib::DIST_NULL; index < hdrtoolslib::DIST_METRICS; index++) {
    m_distortionMetric[index] = NULL;
    m_windowDistortionMetric[index] = NULL;
  }
}

HDRMetricsFrame::~HDRMetricsFrame()
{
  if (m_frameStore != NULL) {
    delete [] m_frameStore;
    m_frameStore = NULL;
  }
  if (m_cropFrameStore != NULL) {
    delete [] m_cropFrameStore;
    m_cropFrameStore = NULL;
  }
  if (m_windowFrameStore != NULL) {
    delete [] m_windowFrameStore;
    m_windowFrameStore = NULL;
  }
  if (m_inputFrame != NULL) {
    delete [] m_inputFrame;
    m_inputFrame = NULL;
  }
  if (m_inputFile != NULL) {
    delete [] m_inputFile;
    m_inputFile = NULL;
  }
  if (m_startFrame != NULL) {
    delete [] m_startFrame;
    m_startFrame = NULL;
  }

  // Cropping parameters
  if (m_cropOffsetLeft != NULL) {
    delete [] m_cropOffsetLeft;
    m_cropOffsetLeft = NULL;
  }
  if (m_cropOffsetTop != NULL) {
    delete [] m_cropOffsetTop;
    m_cropOffsetTop = NULL;
  }
  if (m_cropOffsetRight != NULL) {
    delete [] m_cropOffsetRight;
    m_cropOffsetRight = NULL;
  }
  if (m_cropOffsetBottom != NULL) {
    delete [] m_cropOffsetBottom;
    m_cropOffsetBottom = NULL;
  }
  
  // Distortion parameters
  if (m_distortionMetric != NULL) {
    delete [] m_distortionMetric;
    m_distortionMetric = NULL;
  }
  if (m_windowDistortionMetric != NULL) {
    delete [] m_windowDistortionMetric;
    m_windowDistortionMetric = NULL;
  }
}

//-----------------------------------------------------------------------------
// allocate memory - create frame stores
//-----------------------------------------------------------------------------
void HDRMetricsFrame::allocateFrameStores(hdrtoolslib::Input *inputFrame, hdrtoolslib::Frame **frameStore)
{
  // delete frame store if previous allocated
  if ((*frameStore) != NULL)
    delete (*frameStore);
  
  // create frame memory as necessary
  // Input. This has the same format as the Input file.
  using ::hdrtoolslib::Y_COMP;
  (*frameStore)  = new hdrtoolslib::Frame(inputFrame->m_width[Y_COMP], inputFrame->m_height[Y_COMP], inputFrame->m_isFloat, inputFrame->m_colorSpace, inputFrame->m_colorPrimaries, inputFrame->m_chromaFormat, inputFrame->m_sampleRange, inputFrame->m_bitDepthComp[Y_COMP], inputFrame->m_isInterlaced, inputFrame->m_transferFunction, inputFrame->m_systemGamma);
  (*frameStore)->clear();
  
  
}

//-----------------------------------------------------------------------------
// initialize
//-----------------------------------------------------------------------------
void HDRMetricsFrame::init (ProjectParameters *inputParams)
{
  std::vector <int>  width (m_numberOfClips);
  std::vector <int>  height(m_numberOfClips);
  
  using ::hdrtoolslib::Frame;
  using ::hdrtoolslib::Y_COMP;
  for (int index = 0; index < m_numberOfClips; index++) {
    // Input frame objects initialization
    hdrtoolslib::IOFunctions::openFile (m_inputFile[index]);
    if (m_inputFile[index]->m_isConcatenated == false && strlen(m_inputFile[index]->m_fTail) == 0) {
      // Update number of frames to be processed
      inputParams->m_numberOfFrames = 1;
    }
    
    // create memory for reading the input filesource
    
    m_inputFrame[index] = hdrtoolslib::Input::create(m_inputFile[index], &inputParams->m_source[index], inputParams);
    if (m_inputFile[index]->m_videoType == hdrtoolslib::VIDEO_YUV)
      allocateFrameStores(m_inputFrame[index], &m_frameStore[index]);
    else
    {
      // Read first frame just to see if there is need to update any parameters. This is very important for OpenEXR files
      if (m_inputFrame[index]->readOneFrame(m_inputFile[index], 0, m_inputFile[index]->m_fileHeader, m_startFrame[index]) == true) {
        allocateFrameStores(m_inputFrame[index], &m_frameStore[index]);
      }
      else {
        fprintf(stderr, "Error reading input file. Process exiting.\n");
        destroy();
        exit(EXIT_FAILURE);
      }
    }
    
    if (m_cropOffsetLeft[index] != 0 || m_cropOffsetTop[index] != 0 || m_cropOffsetRight[index] != 0 || m_cropOffsetBottom[index] != 0) {
      width[index]  = m_inputFrame[index]->m_width[Y_COMP]  - m_cropOffsetLeft[index] + m_cropOffsetRight[index];
      height[index] = m_inputFrame[index]->m_height[Y_COMP] - m_cropOffsetTop[index]  + m_cropOffsetBottom[index];
      m_cropFrameStore[index]  = new Frame(width[index], height[index], m_inputFrame[index]->m_isFloat, m_inputFrame[index]->m_colorSpace, m_inputFrame[index]->m_colorPrimaries, m_inputFrame[index]->m_chromaFormat, m_inputFrame[index]->m_sampleRange, m_inputFrame[index]->m_bitDepthComp[Y_COMP], m_inputFrame[index]->m_isInterlaced, m_inputFrame[index]->m_transferFunction, m_inputFrame[index]->m_systemGamma);
      m_cropFrameStore[index]->clear();
    }
    else {
      width[index]  = m_inputFrame[index]->m_width[Y_COMP];
      height[index] = m_inputFrame[index]->m_height[Y_COMP];
    }
  }
  
  if (m_frameStore[0]->equalType(m_frameStore[1]) == false) {
    fprintf(stderr, "Error. Input sources of different type.\n");
    destroy();
    exit(EXIT_FAILURE);
  }
  
  for (int index = hdrtoolslib::DIST_NULL; index < hdrtoolslib::DIST_METRICS; index++) {
    m_enableMetric[index] = inputParams->m_enableMetric[index];
    if (m_enableMetric[index] == true)
      m_distortionMetric[index] = hdrtoolslib::DistortionMetric::create(&m_frameStore[0]->m_format, index, &m_distortionParameters, false);
  }

  m_enableWindow  = inputParams->m_enableWindow;

  if (m_enableWindow) {
    m_windowMinPosX = inputParams->m_windowMinPosX;
    m_windowMaxPosX = inputParams->m_windowMaxPosX;
    m_windowMinPosY = inputParams->m_windowMinPosY;
    m_windowMaxPosY = inputParams->m_windowMaxPosY;

    m_windowWidth   = hdrtoolslib::iAbs(m_windowMaxPosX - m_windowMinPosX + 1);
    m_windowHeight  = hdrtoolslib::iAbs(m_windowMaxPosY - m_windowMinPosY + 1);
    
    m_windowFrameStore[0]  = new Frame(m_windowWidth, m_windowHeight, m_inputFrame[0]->m_isFloat, m_inputFrame[0]->m_colorSpace, m_inputFrame[0]->m_colorPrimaries, m_inputFrame[0]->m_chromaFormat, m_inputFrame[0]->m_sampleRange, m_inputFrame[0]->m_bitDepthComp[Y_COMP], m_inputFrame[0]->m_isInterlaced, m_inputFrame[0]->m_transferFunction, m_inputFrame[0]->m_systemGamma);
    m_windowFrameStore[0]->clear();
    m_windowFrameStore[1]  = new Frame(m_windowWidth, m_windowHeight, m_inputFrame[1]->m_isFloat, m_inputFrame[1]->m_colorSpace, m_inputFrame[1]->m_colorPrimaries, m_inputFrame[1]->m_chromaFormat, m_inputFrame[1]->m_sampleRange, m_inputFrame[1]->m_bitDepthComp[Y_COMP], m_inputFrame[1]->m_isInterlaced,  m_inputFrame[1]->m_transferFunction, m_inputFrame[1]->m_systemGamma);
    m_windowFrameStore[1]->clear();

    for (int index = hdrtoolslib::DIST_NULL; index < hdrtoolslib::DIST_METRICS; index++) {
      m_enableWindowMetric[index] = inputParams->m_enableWindowMetric[index];
      if (m_enableWindowMetric[index] == true)
        m_windowDistortionMetric[index] = hdrtoolslib::DistortionMetric::create(&m_windowFrameStore[0]->m_format, index, &m_distortionParameters, true);
    }
  }
}

//-----------------------------------------------------------------------------
// deallocate memory - destroy objects
//-----------------------------------------------------------------------------
void HDRMetricsFrame::destroy()
{
  for (int index = 0; index < m_numberOfClips; index++) {
    if (m_enableWindow) {
      if (m_windowFrameStore[index] != NULL) {
        delete m_windowFrameStore[index];
        m_windowFrameStore[index] = NULL;
      }
    }
    if (m_frameStore[index] != NULL) {
      delete m_frameStore[index];
      m_frameStore[index] = NULL;
    }
    
    if (m_cropFrameStore[index] != NULL) {
      delete m_cropFrameStore[index];
      m_cropFrameStore[index] = NULL;
    }
    
    if (m_inputFrame[index] != NULL) {
      delete m_inputFrame[index];
      m_inputFrame[index] = NULL;
    }
    hdrtoolslib::IOFunctions::closeFile(m_inputFile[index]);
  }
  
  for (int index = hdrtoolslib::DIST_NULL; index < hdrtoolslib::DIST_METRICS; index++) {
    if (m_distortionMetric[index] != NULL) {
      delete m_distortionMetric[index];
      m_distortionMetric[index] = NULL;
    }
    if (m_windowDistortionMetric[index] != NULL) {
      delete m_windowDistortionMetric[index];
      m_windowDistortionMetric[index] = NULL;
    }
  }
}

//-----------------------------------------------------------------------------
// main filtering function
//-----------------------------------------------------------------------------
void HDRMetricsFrame::process( ProjectParameters *inputParams ) {
  using ::hdrtoolslib::Y_COMP;
  int frameNumber;
  int iCurrentFrameToProcess = 0;
  //float fDistance0 = inputParams->m_source[0].m_frameRate / inputParams->m_source[1].m_frameRate;
  std::vector <hdrtoolslib::Frame *>  currentFrame(m_numberOfClips);
  int index;
  
  clock_t clk;  
  bool errorRead = false;

  // Now process all frames
  for (frameNumber = 0; frameNumber < inputParams->m_numberOfFrames; frameNumber ++) {
    clk = clock();
    // read frames
    for (index = 0; index < m_numberOfClips; index++) {
      // Current frame to process depends on frame rate of current sequence versus frame rate of the first sequence
      iCurrentFrameToProcess = int(frameNumber * inputParams->m_source[index].m_frameRate / inputParams->m_metricRate);
      if (m_inputFrame[index]->readOneFrame(m_inputFile[index], iCurrentFrameToProcess, m_inputFile[index]->m_fileHeader, m_startFrame[index]) == true) {
        // If the size of the images has changed, then reallocate space appropriately
        if ((m_inputFrame[index]->m_width[Y_COMP] != m_frameStore[index]->m_width[Y_COMP]) || (m_inputFrame[index]->m_height[Y_COMP] != m_frameStore[index]->m_height[Y_COMP])) {
          allocateFrameStores(m_inputFrame[index], &m_frameStore[index]);
        }
        
        // Now copy input frame buffer to processing frame buffer for any subsequent processing
        m_frameStore[index]->m_frameNo = frameNumber;
        m_inputFrame[index]->copyFrame(m_frameStore[index]);
        currentFrame[index] = m_frameStore[index];
        if (m_cropFrameStore[index] != NULL) {
          m_cropFrameStore[index]->copy(m_frameStore[index], m_cropOffsetLeft[index], m_cropOffsetTop[index], m_frameStore[index]->m_width[Y_COMP] + m_cropOffsetRight[index], m_frameStore[index]->m_height[Y_COMP] + m_cropOffsetBottom[index], 0, 0);
          currentFrame[index] = m_cropFrameStore[index];
        }
        if (m_enableWindow == true) {
          m_windowFrameStore[index]->copy(currentFrame[index], m_windowMinPosX, m_windowMinPosY, m_windowMaxPosX, m_windowMaxPosY, 0, 0);
        }
      }
      else {
        inputParams->m_numberOfFrames = frameNumber;
        errorRead = true;
        break;
      }
    }
    
    if (errorRead == false && currentFrame[0]->equalType(currentFrame[1]) == false) {
      fprintf(stderr, "Error. Input sources of different type.\n");
      errorRead = true;
      break;
    }
    
    if (errorRead == true) {
      break;
    }
    else if (inputParams->m_silentMode == false) {
      printf("%06d ", frameNumber );
    }
    
    for (int index = hdrtoolslib::DIST_NULL; index < hdrtoolslib::DIST_METRICS; index++) {
      if (m_enableMetric[index] == true) {
        m_distortionMetric[index]->computeMetric(currentFrame[0], currentFrame[1]);
        if (inputParams->m_silentMode == false) {
          m_distortionMetric[index]->reportMetric();
        }
      }
    }
    // Window statistics
    if (m_enableWindow == true) {
      printf("  |");
      for (int index = hdrtoolslib::DIST_NULL; index < hdrtoolslib::DIST_METRICS; index++) {
        if (m_enableWindowMetric[index] == true) {
          m_windowDistortionMetric[index]->computeMetric(m_windowFrameStore[0], m_windowFrameStore[1]);
          if (inputParams->m_silentMode == false) {
            m_windowDistortionMetric[index]->reportMetric();
          }
        }
      }
    }
    clk = clock() - clk;
    if (inputParams->m_silentMode == false){
      printf("%7.3f", 1.0 * clk / CLOCKS_PER_SEC);
      printf("\n");
      fflush(stdout);
    }
    else {
      printf("Processing Frame : %d\r", frameNumber);
      fflush(stdout);
    }
  } //end for frameNumber
}

//-----------------------------------------------------------------------------
// header output
//-----------------------------------------------------------------------------
void HDRMetricsFrame::outputHeader(ProjectParameters *inputParams) {
  printf("================================================================================================================\n");
  for (int index = 0; index < m_numberOfClips; index++) {
    printf("Input%d: %s\n", index, inputParams->m_inputFile[index].m_fName);
    using ::hdrtoolslib::Y_COMP;
    printf("W x H:  (%dx%d) ~ (%dx%d - %dx%d) => (%dx%d)\n", m_inputFrame[index]->m_width[Y_COMP], m_inputFrame[index]->m_height[Y_COMP],
           m_cropOffsetLeft[index], m_cropOffsetTop[index], m_cropOffsetRight[index], m_cropOffsetBottom[index],
           m_inputFrame[index]->m_width[Y_COMP] - m_cropOffsetLeft[index] + m_cropOffsetRight[index], m_inputFrame[index]->m_height[Y_COMP] - m_cropOffsetTop[index] + m_cropOffsetBottom[index]);
    m_inputFrame[index]->printFormat();

    if (index < m_numberOfClips - 1) {
      printf("----------------------------------------------------------------------------------------------------------------\n");
    }
    else {
      printf("================================================================================================================\n");
    }
  }

  if (m_enableWindow) {
    printf(" Cropped Window Statistics Enabled from rectangle position (%d,%d) and size (%dx%d)\n", hdrtoolslib::iMin(m_windowMinPosX,m_windowMaxPosX), hdrtoolslib::iMin(m_windowMinPosY,m_windowMaxPosY), m_windowWidth, m_windowHeight);
    printf("================================================================================================================\n");
  }
  
  if (1 || inputParams->m_silentMode == false) {
    for (int index = hdrtoolslib::DIST_NULL; index < hdrtoolslib::DIST_METRICS; index++) {
      if (m_enableMetric[index] == true)
        m_distortionMetric[index]->printSeparator();
    }
    if (m_enableWindow) {
      printf("---");
      for (int index = hdrtoolslib::DIST_NULL; index < hdrtoolslib::DIST_METRICS; index++) {
        if (m_enableWindowMetric[index] == true)
          m_windowDistortionMetric[index]->printSeparator();
      }
    }
    // Separators for 'Frame#  Time'
    printf("---------------\n");

    printf("Frame#  ");  // 8
    for (int index = hdrtoolslib::DIST_NULL; index < hdrtoolslib::DIST_METRICS; index++) {
      if (m_enableMetric[index] == true)
        m_distortionMetric[index]->printHeader();
    }
    if (m_enableWindow) {
      printf(" | ");
      for (int index = hdrtoolslib::DIST_NULL; index < hdrtoolslib::DIST_METRICS; index++) {
        if (m_enableWindowMetric[index] == true)
          m_windowDistortionMetric[index]->printHeader();
      }
    }
    
    printf("  Time "); // 7
    // Separators for 'Frame#  Time'
    printf("\n---------------");
    for (int index = hdrtoolslib::DIST_NULL; index < hdrtoolslib::DIST_METRICS; index++) {
      if (m_enableMetric[index] == true)
        m_distortionMetric[index]->printSeparator();
    }
    if (m_enableWindow) {
      printf("---");
      for (int index = hdrtoolslib::DIST_NULL; index < hdrtoolslib::DIST_METRICS; index++) {
        if (m_enableWindowMetric[index] == true)
          m_windowDistortionMetric[index]->printSeparator();
      }
    }
    printf("\n");
  }

}

//-----------------------------------------------------------------------------
// footer output
//-----------------------------------------------------------------------------
void HDRMetricsFrame::outputFooter(ProjectParameters *inputParams) {
  int j;
  clock_t clk = clock();
  FILE* f = hdrtoolslib::IOFunctions::openFile(inputParams->m_logFile, "at");

  using ::hdrtoolslib::DIST_METRICS;
  using ::hdrtoolslib::DIST_NULL;
  if (f != NULL) {
    fprintf(f, "%s ", inputParams->m_inputFile[0].m_fName);
    fprintf(f, "%s ", inputParams->m_inputFile[1].m_fName);
    fprintf(f, "%d ", inputParams->m_numberOfFrames);
    fprintf(f, "%f \n", 1.0 * clk / inputParams->m_numberOfFrames / CLOCKS_PER_SEC);
  }
  //if (inputParams->m_silentMode == false){
  if (1) {
    for (j = DIST_NULL; j < DIST_METRICS; j++) {
      if (m_enableMetric[j] == true)
        m_distortionMetric[j]->printSeparator();
    }
    if (m_enableWindow) {
      printf("---");
      for (int index = DIST_NULL; index < DIST_METRICS; index++) {
        if (m_enableWindowMetric[index] == true)
          m_windowDistortionMetric[index]->printSeparator();
      }
    }
    
    // Separators for 'Frame#  Time'
    printf("---------------\n");
    
    //printf("%06d ", inputParams->m_numberOfFrames);
    printf("D_Avg  ");

    for (j = DIST_NULL; j < DIST_METRICS; j++) {
      if (m_enableMetric[j] == true)
        m_distortionMetric[j]->reportSummary();
    }
    if (m_enableWindow) {
      printf("  |");
      for (int index = DIST_NULL; index < DIST_METRICS; index++) {
        if (m_enableWindowMetric[index] == true)
          m_windowDistortionMetric[index]->reportSummary();
      }
    }
    printf("%7.3f", clk / ((float) inputParams->m_numberOfFrames * (float) CLOCKS_PER_SEC));
    printf("\n---------------");
    for (j = DIST_NULL; j < DIST_METRICS; j++) {
      if (m_enableMetric[j] == true)
        m_distortionMetric[j]->printSeparator();
    }
    if (m_enableWindow) {
      printf("---");
      for (int index = DIST_NULL; index < DIST_METRICS; index++) {
        if (m_enableWindowMetric[index] == true)
          m_windowDistortionMetric[index]->printSeparator();
      }
    }

    // Minimum
    printf("\nD_Min  ");
    for (j = DIST_NULL; j < DIST_METRICS; j++) {
      if (m_enableMetric[j] == true)
        m_distortionMetric[j]->reportMinimum();
    }
    if (m_enableWindow) {
      printf("  |");
      for (int index = DIST_NULL; index < DIST_METRICS; index++) {
        if (m_enableWindowMetric[index] == true)
          m_windowDistortionMetric[index]->reportMinimum();
      }
    }
    // Minimum
    printf("\nD_Max  ");
    for (j = DIST_NULL; j < DIST_METRICS; j++) {
      if (m_enableMetric[j] == true)
        m_distortionMetric[j]->reportMaximum();
    }
    if (m_enableWindow) {
      printf("  |");
      for (int index = DIST_NULL; index < DIST_METRICS; index++) {
        if (m_enableWindowMetric[index] == true)
          m_windowDistortionMetric[index]->reportMaximum();
      }
    }
  }
  
  // Separators for 'Frame#  Time'
  printf("\n---------------");
  for (j = DIST_NULL; j < DIST_METRICS; j++) {
    if (m_enableMetric[j] == true)
      m_distortionMetric[j]->printSeparator();
  }
  if (m_enableWindow) {
    printf("---");
    for (int index = DIST_NULL; index < DIST_METRICS; index++) {
      if (m_enableWindowMetric[index] == true)
        m_windowDistortionMetric[index]->printSeparator();
    }
  }

  
  printf("\nTotal of %d frames processed in %5.3f seconds\n",inputParams->m_numberOfFrames, 1.0 * clk / CLOCKS_PER_SEC);
  printf("Processing Speed: %3.2ffps\n", (float) inputParams->m_numberOfFrames * CLOCKS_PER_SEC / clk);
  for (j = DIST_NULL; j < DIST_METRICS; j++) {
    if (m_enableMetric[j] == true)
      m_distortionMetric[j]->printSeparator();
  }
  if (m_enableWindow) {
    printf("---");
    for (int index = DIST_NULL; index < DIST_METRICS; index++) {
      if (m_enableWindowMetric[index] == true)
        m_windowDistortionMetric[index]->printSeparator();
    }
  }
  printf("---------------\n");

  hdrtoolslib::IOFunctions::closeFile(f);
}

