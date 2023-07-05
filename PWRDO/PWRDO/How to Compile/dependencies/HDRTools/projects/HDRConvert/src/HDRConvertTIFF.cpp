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
 * \file HDRConvertTIFF.cpp
 *
 * \brief
 *    HDRConvertTIFF class source files for performing HDR (TIFF) format conversion
 *
 * \author
 *     - Alexis Michael Tourapis         <atourapis@apple.com>
 *************************************************************************************
 */

#include <time.h>
#include <string.h>
#include <math.h>
#include "HDRConvertTIFF.H"

HDRConvertTIFF::HDRConvertTIFF(ProjectParameters *inputParams) {
  m_noOfFrameStores   = 7;
  m_oFrameStore       = NULL;
  m_iFrameStore       = NULL;
  m_dFrameStore       = NULL;
  
  for (int index = 0; index < m_noOfFrameStores; index++) {
    m_pFrameStore[index]     =  NULL;
    m_pDFrameStore[index]    =  NULL;
  }

  m_convertProcess           =  NULL;
  m_colorTransform           =  NULL;
  m_normalizeFunction        =  NULL;
  m_outputTransferFunction   =  NULL;
  m_inputTransferFunction    =  NULL;
  m_inputFrame               =  NULL;
  m_outputFrame              =  NULL;
  m_convertTo420             =  NULL;

  m_addNoise                 =  NULL;
  m_colorSpaceConvert        =  NULL;
  m_frameScale               =  NULL;
  m_scaledFrame              =  NULL;

  m_frameFilterNoise0        =  NULL;
  m_frameFilterNoise1        =  NULL;
  m_frameFilterNoise2        =  NULL;

  m_useMinMax                =  inputParams->m_useMinMax;

  m_filterInFloat            =  inputParams->m_filterInFloat;
    
  m_inputFile                = &inputParams->m_inputFile;
  m_outputFile               = &inputParams->m_outputFile;
  m_startFrame               =  m_inputFile->m_startFrame;
   
  m_cropOffsetLeft           =  inputParams->m_cropOffsetLeft;
  m_cropOffsetTop            =  inputParams->m_cropOffsetTop;
  m_cropOffsetRight          =  inputParams->m_cropOffsetRight;
  m_cropOffsetBottom         =  inputParams->m_cropOffsetBottom;
  
  m_croppedFrameStore        =  NULL;

  m_bUseWienerFiltering      =  inputParams->m_bUseWienerFiltering;
  m_bUse2DSepFiltering       =  inputParams->m_bUse2DSepFiltering;
  m_bUseNLMeansFiltering     =  inputParams->m_bUseNLMeansFiltering;
  m_b2DSepMode               =  inputParams->m_b2DSepMode;

  m_convertIQuantize         =  NULL;
  
  m_srcDisplayGammaAdjust    =  NULL;
  m_outDisplayGammaAdjust    =  NULL;
  m_toneMapping              =  NULL;
}

void HDRConvertTIFF::deleteMemory(){
  // delete frame stores if previous allocated
  for (int i = 0; i < m_noOfFrameStores; i++) {
    if (m_pFrameStore[i] != NULL) {
      delete m_pFrameStore[i];
      m_pFrameStore[i] = NULL;
    }
    if (m_pDFrameStore[i] != NULL) {
      delete m_pDFrameStore[i];
      m_pDFrameStore[i] = NULL;
    }
  }
  // output frame objects
  if (m_oFrameStore != NULL) {
    delete m_oFrameStore;
    m_oFrameStore = NULL;
  }
  // input frame objects
  if (m_iFrameStore != NULL) {
    delete m_iFrameStore;
    m_iFrameStore = NULL;
  }
    
  if (m_colorTransform != NULL) {
    delete m_colorTransform;
    m_colorTransform = NULL;
  }
  
  if (m_colorSpaceConvert != NULL) {
    delete m_colorSpaceConvert;
    m_colorSpaceConvert = NULL;
  }
  
  if (m_frameFilterNoise0 != NULL) {
    delete m_frameFilterNoise0;
    m_frameFilterNoise0 = NULL;
  }
  
  if (m_frameFilterNoise1 != NULL) {
    delete m_frameFilterNoise1;
    m_frameFilterNoise1 = NULL;
  }
  
  if (m_frameFilterNoise2 != NULL) {
    delete m_frameFilterNoise2;
    m_frameFilterNoise2 = NULL;
  }
  
  if (m_convertTo420 != NULL) {
    delete m_convertTo420;
    m_convertTo420 = NULL;
  }

   if (m_dFrameStore != NULL) {
    delete m_dFrameStore;
    m_dFrameStore = NULL;
  }
  
  if (m_frameScale != NULL) {
    delete m_frameScale;
    m_frameScale = NULL;
  }
  
  if (m_scaledFrame != NULL) {
    delete m_scaledFrame;
    m_scaledFrame = NULL;
  }
  
  if (m_convertProcess != NULL) {
    delete m_convertProcess;
    m_convertProcess = NULL;
  }
  
  if (m_convertIQuantize != NULL) {
    delete m_convertIQuantize;
    m_convertIQuantize = NULL;
  }
  
    // Cropped frame store
  if (m_croppedFrameStore != NULL) {
    delete m_croppedFrameStore;
    m_croppedFrameStore = NULL;
  }
  

  if (m_srcDisplayGammaAdjust != NULL) {
    delete m_srcDisplayGammaAdjust;
    m_srcDisplayGammaAdjust = NULL;
  }
  
  if (m_outDisplayGammaAdjust != NULL) {
    delete m_outDisplayGammaAdjust;
    m_outDisplayGammaAdjust = NULL;
  }
  if (m_toneMapping != NULL) {
    delete m_toneMapping;
    m_toneMapping = NULL;
  }
}

//-----------------------------------------------------------------------------
// deallocate memory - destroy objects
//-----------------------------------------------------------------------------

void HDRConvertTIFF::destroy() {
  deleteMemory();
  
    if (m_addNoise != NULL) {
    delete m_addNoise;
    m_addNoise = NULL;
  }

  if (m_inputFrame != NULL) {
    delete m_inputFrame;
    m_inputFrame = NULL;
  }
  
  if (m_outputFrame != NULL) {
    delete m_outputFrame;
    m_outputFrame = NULL;
  }
  
  if (m_normalizeFunction != NULL) {
    delete m_normalizeFunction;
    m_normalizeFunction = NULL;
  }
  if (m_outputTransferFunction != NULL) {
    delete m_outputTransferFunction;
    m_outputTransferFunction = NULL;
  }
  if (m_inputTransferFunction != NULL) {
    delete m_inputTransferFunction;
    m_inputTransferFunction = NULL;
  }
  
  if (m_frameFilterNoise0 != NULL) {
    delete m_frameFilterNoise0;
    m_frameFilterNoise0 = NULL;
  }
  
  if (m_frameFilterNoise1 != NULL) {
    delete m_frameFilterNoise1;
    m_frameFilterNoise1 = NULL;
  }
  
  if (m_frameFilterNoise2 != NULL) {
    delete m_frameFilterNoise2;
    m_frameFilterNoise2 = NULL;
  }
  
  hdrtoolslib::IOFunctions::closeFile(m_inputFile);
  hdrtoolslib::IOFunctions::closeFile(m_outputFile);
}

//-----------------------------------------------------------------------------
// allocate memory - create objects
//-----------------------------------------------------------------------------

void HDRConvertTIFF::allocateFrameStores(ProjectParameters *inputParams, hdrtoolslib::FrameFormat   *input, hdrtoolslib::FrameFormat   *output) {
  
  int width, height;
  
  using ::hdrtoolslib::Frame;
  using ::hdrtoolslib::Y_COMP;
  deleteMemory();
  // Create output file
  hdrtoolslib::IOFunctions::openFile (m_outputFile, OPENFLAGS_WRITE, OPEN_PERMISSIONS);
  
  // create frame memory as necessary
  // Input. This has the same format as the Input file.
  m_iFrameStore  = new Frame(m_inputFrame->m_width[Y_COMP], m_inputFrame->m_height[Y_COMP], m_inputFrame->m_isFloat, m_inputFrame->m_colorSpace, m_inputFrame->m_colorPrimaries, m_inputFrame->m_chromaFormat, m_inputFrame->m_sampleRange, m_inputFrame->m_bitDepthComp[Y_COMP], m_inputFrame->m_isInterlaced, m_inputFrame->m_transferFunction, m_inputFrame->m_systemGamma);
  m_iFrameStore->clear();
  
  // update input parameters to avoid any possible errors later on
  *input = m_iFrameStore->m_format;
  // frame rate is not passed into the frame store so lets copy this here back to the input
  input->m_frameRate = m_inputFrame->m_frameRate;

  if (m_cropOffsetLeft != 0 || m_cropOffsetTop != 0 || m_cropOffsetRight != 0 || m_cropOffsetBottom != 0) {
    m_cropWidth = width  = m_inputFrame->m_width[Y_COMP]  - m_cropOffsetLeft + m_cropOffsetRight;
    m_cropHeight = height = m_inputFrame->m_height[Y_COMP] - m_cropOffsetTop  + m_cropOffsetBottom;
    m_croppedFrameStore  = new Frame(width, height, m_inputFrame->m_isFloat, m_inputFrame->m_colorSpace, m_inputFrame->m_colorPrimaries, m_inputFrame->m_chromaFormat, m_inputFrame->m_sampleRange, m_inputFrame->m_bitDepthComp[Y_COMP], m_inputFrame->m_isInterlaced, m_inputFrame->m_transferFunction, m_inputFrame->m_systemGamma);
    m_croppedFrameStore->clear();
  }
  else {
    m_cropWidth = width  = m_inputFrame->m_width[Y_COMP];
    m_cropHeight = height = m_inputFrame->m_height[Y_COMP];
  }

  // Since this is a TIFF image, we should convert from fixed to float
  m_pFrameStore[0]   = new Frame(width, height, true, m_inputFrame->m_colorSpace, m_inputFrame->m_colorPrimaries, m_inputFrame->m_chromaFormat, m_inputFrame->m_sampleRange, m_inputFrame->m_bitDepthComp[Y_COMP], m_inputFrame->m_isInterlaced, m_inputFrame->m_transferFunction, m_inputFrame->m_systemGamma);
  m_pFrameStore[0]->clear();
  
  // Frame store for the inversion of PQ TF
  m_pFrameStore[1]   = new Frame(width, height, true, m_inputFrame->m_colorSpace, m_inputFrame->m_colorPrimaries, m_inputFrame->m_chromaFormat, m_inputFrame->m_sampleRange, m_inputFrame->m_bitDepthComp[Y_COMP], m_inputFrame->m_isInterlaced, m_inputFrame->m_transferFunction, m_inputFrame->m_systemGamma);
  m_pFrameStore[1]->clear();

  // Output. Since we don't support scaling, lets reset the width and height here.
  //m_outputFile->m_format.m_height[Y_COMP] = output->m_height[Y_COMP] = height;
  //m_outputFile->m_format.m_width [Y_COMP] = output->m_width [Y_COMP] = width;
  
  m_outputFile->m_format.m_height[Y_COMP] = output->m_height[Y_COMP];
  m_outputFile->m_format.m_width [Y_COMP] = output->m_width [Y_COMP];

  // Processing also happens here in the same space (for now). Likely we should change this into floats, or have this depending on the process we are doing (int to floats or floats to ints).
  // Currently lets set this to the same format as the input store.
  m_pFrameStore[2]   = new Frame(output->m_width [Y_COMP], output->m_height[Y_COMP], true, m_inputFrame->m_colorSpace, output->m_colorPrimaries, m_inputFrame->m_chromaFormat, m_inputFrame->m_sampleRange, m_inputFrame->m_bitDepthComp[Y_COMP], m_inputFrame->m_isInterlaced, m_inputFrame->m_transferFunction, m_inputFrame->m_systemGamma);
  m_pFrameStore[2]->clear();
  
  // Also creation of frame store for the normalized images
  m_pFrameStore[3]   = new Frame(output->m_width [Y_COMP], output->m_height[Y_COMP], true, m_inputFrame->m_colorSpace, output->m_colorPrimaries, m_inputFrame->m_chromaFormat, m_inputFrame->m_sampleRange, m_inputFrame->m_bitDepthComp[Y_COMP], m_inputFrame->m_isInterlaced, m_inputFrame->m_transferFunction, m_inputFrame->m_systemGamma);
  m_pFrameStore[3]->clear();
  // and of the transfer function converted pictures
  m_pFrameStore[4]   = new Frame(output->m_width [Y_COMP], output->m_height[Y_COMP], true, m_inputFrame->m_colorSpace, output->m_colorPrimaries, m_inputFrame->m_chromaFormat, m_inputFrame->m_sampleRange, m_inputFrame->m_bitDepthComp[Y_COMP], m_inputFrame->m_isInterlaced, output->m_transferFunction, output->m_systemGamma);
  m_pFrameStore[4]->clear();
  
  m_outputFrame = hdrtoolslib::Output::create(m_outputFile, output);

  // Frame store for altering color space
  if (m_inputFrame->m_colorSpace == hdrtoolslib::CM_XYZ && output->m_colorSpace != hdrtoolslib::CM_XYZ && output->m_colorPrimaries != hdrtoolslib::CP_NONE ) {
    m_pFrameStore[6]   = new Frame(width, height, true, hdrtoolslib::CM_RGB, output->m_colorPrimaries, m_inputFrame->m_chromaFormat, m_inputFrame->m_sampleRange, m_inputFrame->m_bitDepthComp[Y_COMP], m_inputFrame->m_isInterlaced, m_inputFrame->m_transferFunction, m_inputFrame->m_systemGamma);
  }
  else {
    m_pFrameStore[6]   = new Frame(width, height, true, m_inputFrame->m_colorSpace, output->m_colorPrimaries, m_inputFrame->m_chromaFormat, m_inputFrame->m_sampleRange, m_inputFrame->m_bitDepthComp[Y_COMP], m_inputFrame->m_isInterlaced, m_inputFrame->m_transferFunction, m_inputFrame->m_systemGamma);
  }
  m_pFrameStore[6]->clear();

  if (output->m_chromaFormat != m_inputFrame->m_chromaFormat) {
    if (m_filterInFloat == true) {
      m_pFrameStore[5] = new Frame(output->m_width[Y_COMP], output->m_height[Y_COMP], true, output->m_colorSpace, output->m_colorPrimaries, output->m_chromaFormat, output->m_sampleRange, output->m_bitDepthComp[Y_COMP], output->m_isInterlaced, output->m_transferFunction, output->m_systemGamma);
    }
    else {
      m_pFrameStore[5] = new Frame(output->m_width[Y_COMP], output->m_height[Y_COMP], output->m_isFloat, output->m_colorSpace, output->m_colorPrimaries, m_inputFrame->m_chromaFormat, output->m_sampleRange, output->m_bitDepthComp[Y_COMP], output->m_isInterlaced, output->m_transferFunction, output->m_systemGamma);
    }
    m_pFrameStore[5]->clear();
  }
  else {
    m_pFrameStore[5] = NULL;
  }
  
  m_oFrameStore  = new Frame(output->m_width[Y_COMP], output->m_height[Y_COMP], output->m_isFloat, output->m_colorSpace, output->m_colorPrimaries, output->m_chromaFormat, output->m_sampleRange, output->m_bitDepthComp[Y_COMP], output->m_isInterlaced, output->m_transferFunction, output->m_systemGamma);
  m_oFrameStore->clear();
  
  // Format conversion process
  m_convertIQuantize = hdrtoolslib::Convert::create(&m_iFrameStore->m_format, &m_pFrameStore[0]->m_format, &inputParams->m_cvParams);
  m_convertProcess   = hdrtoolslib::Convert::create(&m_pFrameStore[2]->m_format, output, &inputParams->m_cvParams);
  
  // Initial color space conversion in linear space (if data were in different RGB space than target)
  if (m_iFrameStore->m_colorSpace == hdrtoolslib::CM_XYZ && m_oFrameStore->m_colorSpace != hdrtoolslib::CM_XYZ && m_oFrameStore->m_colorPrimaries != hdrtoolslib::CP_NONE) {
    m_colorSpaceConvert = hdrtoolslib::ColorTransform::create(m_iFrameStore->m_colorSpace, m_iFrameStore->m_colorPrimaries, hdrtoolslib::CM_RGB, m_oFrameStore->m_colorPrimaries, inputParams->m_transformPrecision, inputParams->m_useHighPrecisionTransform, hdrtoolslib::CLT_NULL, 0, 0);
    m_colorTransform = hdrtoolslib::ColorTransform::create(hdrtoolslib::CM_RGB, m_oFrameStore->m_colorPrimaries, m_oFrameStore->m_colorSpace, m_oFrameStore->m_colorPrimaries, inputParams->m_transformPrecision, inputParams->m_useHighPrecisionTransform, inputParams->m_closedLoopConversion, 0, output->m_iConstantLuminance);
  }
  else {
    m_colorSpaceConvert = hdrtoolslib::ColorTransform::create(m_iFrameStore->m_colorSpace, m_iFrameStore->m_colorPrimaries, m_iFrameStore->m_colorSpace, m_oFrameStore->m_colorPrimaries, inputParams->m_transformPrecision, inputParams->m_useHighPrecisionTransform, hdrtoolslib::CLT_NULL, input->m_iConstantLuminance, 0);
    m_colorTransform = hdrtoolslib::ColorTransform::create(m_iFrameStore->m_colorSpace, m_oFrameStore->m_colorPrimaries, m_oFrameStore->m_colorSpace, m_oFrameStore->m_colorPrimaries, inputParams->m_transformPrecision, inputParams->m_useHighPrecisionTransform, inputParams->m_closedLoopConversion, 0, output->m_iConstantLuminance, output->m_transferFunction, output->m_bitDepthComp[Y_COMP], output->m_sampleRange, inputParams->m_chromaDownsampleFilter, inputParams->m_chromaUpsampleFilter, inputParams->m_useAdaptiveDownsampling, inputParams->m_useAdaptiveUpsampling, inputParams->m_useMinMax, inputParams->m_closedLoopIterations, output->m_chromaFormat, output->m_chromaLocation, inputParams->m_filterInFloat, inputParams->m_enableTFLUTs, &inputParams->m_ctParams);
  }
  
  
  m_frameScale   = hdrtoolslib::FrameScale::create(width, height, output->m_width[Y_COMP], output->m_height[Y_COMP], &inputParams->m_fsParams, inputParams->m_chromaDownsampleFilter, output->m_chromaLocation[hdrtoolslib::FP_FRAME], inputParams->m_useMinMax);
  //m_scaledFrame = new Frame(output->m_width[Y_COMP], output->m_height[Y_COMP], true, CM_RGB, output->m_colorPrimaries, CF_444, output->m_sampleRange, output->m_bitDepthComp[Y_COMP], output->m_isInterlaced, TF_NORMAL, 1.0);

  //ChromaFormat chromaFormat = (m_inputFrame->m_colorPrimaries != output->m_colorPrimaries || m_inputFrame->m_colorSpace == CM_RGB)? CF_444 : (input->m_chromaFormat == CF_400 ? CF_400 : output->m_chromaFormat);

  m_scaledFrame = new Frame(output->m_width[Y_COMP], output->m_height[Y_COMP], true, m_inputFrame->m_colorSpace, m_inputFrame->m_colorPrimaries, m_inputFrame->m_chromaFormat, output->m_sampleRange, output->m_bitDepthComp[Y_COMP], output->m_isInterlaced, hdrtoolslib::TF_NORMAL, 1.0);

  // Chroma subsampling
  // We may wish to create a single convert class that uses as inputs the output resolution as well the input and output chroma format, and the downsampling/upsampling method. That would make the code easier to handle.
  // To be done later.
  m_convertTo420 = hdrtoolslib::ConvertColorFormat::create(output->m_width[Y_COMP], output->m_height[Y_COMP], m_inputFrame->m_chromaFormat, output->m_chromaFormat, inputParams->m_chromaDownsampleFilter, m_inputFrame->m_chromaLocation, output->m_chromaLocation, inputParams->m_useAdaptiveDownsampling, inputParams->m_useMinMax);
  
  if (m_bUseWienerFiltering == true) {
    m_frameFilterNoise0 = hdrtoolslib::FrameFilter::create(output->m_width[Y_COMP], output->m_height[Y_COMP], hdrtoolslib::FT_WIENER2D);
  }
  
  if (m_bUse2DSepFiltering == true) {
    m_frameFilterNoise1 = hdrtoolslib::FrameFilter::create(output->m_width[Y_COMP], output->m_height[Y_COMP], hdrtoolslib::FT_2DSEP, m_b2DSepMode);
  }
  
  if (m_bUseNLMeansFiltering == true) {
    m_frameFilterNoise2 = hdrtoolslib::FrameFilter::create(output->m_width[Y_COMP], output->m_height[Y_COMP], hdrtoolslib::FT_NLMEANS);
  }
}

void HDRConvertTIFF::init (ProjectParameters *inputParams) {
  hdrtoolslib::FrameFormat   *input  = &inputParams->m_source;
  hdrtoolslib::FrameFormat   *output = &inputParams->m_output;
  
   // Input frame objects initialization
  hdrtoolslib::IOFunctions::openFile (m_inputFile);

  if (m_inputFile->m_isConcatenated == false && strlen(m_inputFile->m_fTail) == 0) {
    // Update number of frames to be processed
    inputParams->m_numberOfFrames = 1;
  }

  // create memory for reading the input filesource
  
  m_inputFrame = hdrtoolslib::Input::create(m_inputFile, input, inputParams);
  m_inputFile->m_format = *input;
    
  // Read first frame just to see if there is need to update any parameters (resolution of TIFF files is in the file).
  if (m_inputFrame->readOneFrame(m_inputFile, 0, m_inputFile->m_fileHeader, m_startFrame) == true) {
    allocateFrameStores(inputParams, input, output);
  }
  else {
    fprintf(stderr, "Error reading input file. Process exiting.\n");
    destroy();
    exit(EXIT_FAILURE);
  }
  
  using ::hdrtoolslib::TransferFunction;
  m_inputTransferFunction  = TransferFunction::create(input->m_transferFunction, true, inputParams->m_srcNormalScale, input->m_systemGamma, inputParams->m_srcMinValue, inputParams->m_srcMaxValue, inputParams->m_enableTFunctionLUT);
  if ( output->m_iConstantLuminance !=0 || (output->m_transferFunction != hdrtoolslib::TF_NULL && output->m_transferFunction != hdrtoolslib::TF_POWER && ( inputParams->m_useSingleTransferStep == false || (output->m_transferFunction != hdrtoolslib::TF_PQ && output->m_transferFunction != hdrtoolslib::TF_HPQ && output->m_transferFunction != hdrtoolslib::TF_HPQ2 && output->m_transferFunction != hdrtoolslib::TF_APQ && output->m_transferFunction != hdrtoolslib::TF_APQS && output->m_transferFunction != hdrtoolslib::TF_MPQ  && output->m_transferFunction != hdrtoolslib::TF_AMPQ  && output->m_transferFunction != hdrtoolslib::TF_PH  && output->m_transferFunction != hdrtoolslib::TF_APH && output->m_transferFunction != hdrtoolslib::TF_HLG  && output->m_transferFunction != hdrtoolslib::TF_NORMAL)) )) {
    m_useSingleTransferStep = false;
    m_normalizeFunction = TransferFunction::create(hdrtoolslib::TF_NORMAL, false, inputParams->m_outNormalScale, output->m_systemGamma, inputParams->m_outMinValue, inputParams->m_outMaxValue);
    m_outputTransferFunction  = TransferFunction::create(output->m_transferFunction, false, inputParams->m_outNormalScale, output->m_systemGamma, inputParams->m_outMinValue, inputParams->m_outMaxValue, inputParams->m_enableTFunctionLUT);
  }
  else {
    m_useSingleTransferStep = true;
    m_normalizeFunction = NULL;
    m_outputTransferFunction  = TransferFunction::create(output->m_transferFunction, true, (inputParams->m_enableLegacy && output->m_transferFunction == hdrtoolslib::TF_NULL && output->m_transferFunction != input->m_transferFunction) ? 1.0f : inputParams->m_outNormalScale, output->m_systemGamma, inputParams->m_outMinValue, inputParams->m_outMaxValue, inputParams->m_enableTFunctionLUT);
  }

  m_srcDisplayGammaAdjust = hdrtoolslib::DisplayGammaAdjust::create(input->m_displayAdjustment,  m_useSingleTransferStep ? inputParams->m_srcNormalScale : 1.0f, input->m_systemGamma);
  m_outDisplayGammaAdjust = hdrtoolslib::DisplayGammaAdjust::create(output->m_displayAdjustment, m_useSingleTransferStep ? inputParams->m_outNormalScale : 1.0f, output->m_systemGamma);
  
  if (input->m_displayAdjustment == hdrtoolslib::DA_HLG) {
    m_inputTransferFunction->setNormalFactor(1.0);
  }
  if (output->m_displayAdjustment == hdrtoolslib::DA_HLG) {
    m_outputTransferFunction->setNormalFactor(1.0);
  }

  m_toneMapping = hdrtoolslib::ToneMapping::create(inputParams->m_toneMapping, &inputParams->m_tmParams);
}


//-----------------------------------------------------------------------------
// main filtering function
//-----------------------------------------------------------------------------

void HDRConvertTIFF::process( ProjectParameters *inputParams ) {
  int frameNumber;
  int iCurrentFrameToProcess = 0;
  float fDistance0 = inputParams->m_source.m_frameRate / inputParams->m_output.m_frameRate;
  hdrtoolslib::Frame *currentFrame, *processFrame, *linearFrame;
  hdrtoolslib::FrameFormat   *input  = &inputParams->m_source;
  hdrtoolslib::FrameFormat   *output = &inputParams->m_output;

  clock_t clk;  
  bool errorRead = false;

  // Now process all frames
  using ::hdrtoolslib::Y_COMP;
  for (frameNumber = 0; frameNumber < inputParams->m_numberOfFrames; frameNumber ++) {
    clk = clock();
    iCurrentFrameToProcess = int(frameNumber * fDistance0);

    // read frames
    m_iFrameStore->m_frameNo = frameNumber;
    if (m_inputFrame->readOneFrame(m_inputFile, iCurrentFrameToProcess, m_inputFile->m_fileHeader, m_startFrame) == true) {
      // If the size of the images has changed, then reallocate space appropriately
      if ((m_inputFrame->m_width[Y_COMP] != m_iFrameStore->m_width[Y_COMP]) || (m_inputFrame->m_height[Y_COMP] != m_iFrameStore->m_height[Y_COMP])) {
        // Since we do not support scaling, width and height are also reset here
        output->m_height[Y_COMP] = m_inputFrame->m_height[Y_COMP];
        output->m_width [Y_COMP] = m_inputFrame->m_width [Y_COMP];

        allocateFrameStores(inputParams, input, output) ;
      }
      // Now copy input frame buffer to processing frame buffer for any subsequent processing
      m_inputFrame->copyFrame(m_iFrameStore);
    }
    else {
      inputParams->m_numberOfFrames = frameNumber;
      errorRead = true;
      break;
    }

    if (errorRead == true) {
      break;
    }
    else if (inputParams->m_silentMode == false) {
      printf("%05d ", frameNumber );
    }

    currentFrame = m_iFrameStore;
    if (m_croppedFrameStore != NULL) {
      m_croppedFrameStore->copy(m_iFrameStore, m_cropOffsetLeft, m_cropOffsetTop, m_iFrameStore->m_width[Y_COMP] + m_cropOffsetRight, m_iFrameStore->m_height[Y_COMP] + m_cropOffsetBottom, 0, 0);
      
      currentFrame = m_croppedFrameStore;
    }
    processFrame = m_pFrameStore[0];

    // Convert fixed to floating precision
    m_convertIQuantize->process(processFrame, currentFrame);
    
    currentFrame = processFrame;
    processFrame = m_pFrameStore[1];
    // forward transfer function on the image data
    m_inputTransferFunction->forward(processFrame, currentFrame);
    currentFrame = processFrame;

    m_srcDisplayGammaAdjust->forward(currentFrame);

    // At this point we have linear data.
    // We may wish here to convert the data into a different colorspace, i.e. from P3 to BT.2020.
    // This can be extended later to also support XYZ to RGB.
    processFrame = m_pFrameStore[6];
    m_colorSpaceConvert->process(processFrame, currentFrame);
    currentFrame = processFrame;

    // Apply denoising if specified (using Wiener2D currently)
    if (m_bUseWienerFiltering == true)
      m_frameFilterNoise0->process(currentFrame);

    // Separable denoiser      
    if (m_bUse2DSepFiltering == true)
      m_frameFilterNoise1->process(currentFrame);

    // NLMeans denoiser      
    if (m_bUseNLMeansFiltering == true)
      m_frameFilterNoise2->process(currentFrame);

    m_toneMapping->process(currentFrame);
    m_frameScale->process(m_scaledFrame, currentFrame);
    currentFrame = m_scaledFrame;
    linearFrame = currentFrame;

    // Apply transfer function
    if (!(output->m_iConstantLuminance != 0 && (output->m_colorSpace == hdrtoolslib::CM_YCbCr || output->m_colorSpace == hdrtoolslib::CM_ICtCp))) {
      if ( m_useSingleTransferStep == false ) {
        processFrame = m_pFrameStore[3];
        m_normalizeFunction->inverse(processFrame, currentFrame);
        currentFrame = processFrame;
        
        processFrame = m_pFrameStore[4];
        m_outDisplayGammaAdjust->inverse(currentFrame);
        m_outputTransferFunction->inverse (processFrame, currentFrame);
        currentFrame = processFrame;
      }
      else {
        processFrame = m_pFrameStore[4];
        m_outDisplayGammaAdjust->inverse(currentFrame);
        m_outputTransferFunction->inverse (processFrame, currentFrame);
        currentFrame = processFrame;
      }
    }
    else {
      processFrame = m_pFrameStore[3];
      m_normalizeFunction->inverse(processFrame, currentFrame);
      currentFrame = processFrame;
    }
    currentFrame->m_hasAlternate = true;
    currentFrame->m_altFrame = linearFrame;  
    currentFrame->m_altFrameNorm = inputParams->m_outNormalScale;      

    // Output to m_pFrameStore[2] memory with appropriate color space conversion
    processFrame = m_pFrameStore[2];
    m_colorTransform->process(processFrame, currentFrame);
    currentFrame = processFrame;

    if (m_oFrameStore->m_chromaFormat != currentFrame->m_chromaFormat) {
      // Now perform appropriate conversion to the output format (from float to fixed or vice versa)
      processFrame = m_pFrameStore[5];
      if (m_filterInFloat == true) {
        m_convertTo420->process  (processFrame, currentFrame);
        currentFrame = processFrame;
        processFrame = m_oFrameStore;
        m_convertProcess->process(processFrame, currentFrame);
      }
      else {
        m_convertProcess->process(processFrame, currentFrame);
        currentFrame = processFrame;
        processFrame = m_oFrameStore;
        m_convertTo420->process  (processFrame, currentFrame);
      }
      
      currentFrame = processFrame;
    }
    else {
      // Now perform appropriate conversion to the output format (from float to fixed or vice versa)
      processFrame = m_oFrameStore;
      m_convertProcess->process(processFrame, currentFrame);
      currentFrame = processFrame;
    }

    // frame output
    m_outputFrame->copyFrame(currentFrame);
    m_outputFrame->writeOneFrame(m_outputFile, frameNumber, m_outputFile->m_fileHeader, 0);

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
// footer output
//-----------------------------------------------------------------------------

void HDRConvertTIFF::outputFooter(ProjectParameters *inputParams) {
  clock_t clk = clock();
  FILE* f = hdrtoolslib::IOFunctions::openFile(inputParams->m_logFile, "at");

  if (f != NULL) {
    fprintf(f, "%s ", inputParams->m_inputFile.m_fName);
    fprintf(f, "%s ", inputParams->m_outputFile.m_fName);
    fprintf(f, "%d ", inputParams->m_numberOfFrames);
    fprintf(f, "%f \n", 1.0 * clk / inputParams->m_numberOfFrames / CLOCKS_PER_SEC);
  }

  printf("\n================================================================================================================\n");
  printf("Total of %d frames processed in %7.3f seconds\n",inputParams->m_numberOfFrames, 1.0 * clk / CLOCKS_PER_SEC);
  printf("Conversion Speed: %3.2ffps\n", (float) inputParams->m_numberOfFrames * CLOCKS_PER_SEC / clk);

  hdrtoolslib::IOFunctions::closeFile(f);
}

