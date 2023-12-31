/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * <OWNER> = Apple Inc.
 * <ORGANIZATION> = Apple Inc.
 * <YEAR> = 2015
 *
 * Copyright (c) 2015, Apple Inc.
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
 * \file FrameScale.H
 *
 * \brief
 *    Base Class Header for Scaling
 *
 * \author
 *     - Alexis Michael Tourapis         <atourapis@apple.com>
 *
 *************************************************************************************
 */


#ifndef __FrameScale_H__
#define __FrameScale_H__

#include "Global.H"
#include "Parameters.H"
#include "Frame.H"
#include "ScaleFilter.H"

namespace hdrtoolslib {

class FrameScaleParams {
public:
  ScalingModes m_mode;
  int          m_lanczosLobes;
  int          m_xOffset;
  int          m_yOffset;
  
  FrameScaleParams() {
    m_mode = SC_NN;
    m_lanczosLobes = 6;
    m_xOffset = 0;
    m_yOffset = 0;
  }
};


class FrameScale {
private:
protected:
  int m_iMaxX, m_iMaxY, m_iMinX, m_iMinY;
  int m_oMaxX, m_oMaxY, m_oMinX, m_oMinY;

  double m_factorY;
  double m_factorX;
  double m_offsetY;
  double m_offsetX;
    
  std::vector<double> m_filterCoeffsX;
  std::vector<double> m_filterCoeffsY;
  std::vector<int>    m_filterOffsetsX;
  std::vector<int>    m_filterOffsetsY;
  
  int            m_filterTapsX;
  int            m_filterTapsY;
  
  void PrepareFilterCoefficients(double *factorCoeffs, int *filterOffsets, double factor, int filterTaps, double offset, int oSize);
  void PrepareFilterCoefficients(double *factorCoeffs, int *filterOffsets, double factor, int filterTaps, double offset, int oSize, int lobes);

  void SetFilterLimits(int *filterOffsets, int filterTaps, int iDimension, int oDimension, double factor, int *iMin, int *iMax, int *oMin, int *oMax);

  
  virtual double FilterTap( double dist ) { return 0.0; };
  virtual double FilterTap( double dist, double piScaled, double factor, int lobes ) { return 0.0; };


  void filter( imgpel *input, imgpel *output, int iSizeX, int iSizeY, int oSizeX, int oSizeY, int vMin, int vMax );
  void filter( uint16 *input, uint16 *output, int iSizeX, int iSizeY, int oSizeX, int oSizeY, int vMin, int vMax );
  void filter( float *input, float *output, int iSizeX, int iSizeY, int oSizeX, int oSizeY, double vMin, double vMax );
  imgpel filter( imgpel *input, double *filterX, double *filterY, int posX, int posY, int iSizeX, int vMin, int vMax);
  uint16 filter( uint16 *input, double *filterX, double *filterY, int posX, int posY, int iSizeX, int vMin, int vMax);
  float filter( float *input, double *filterX, double *filterY, int posX, int posY, int iSizeX, double vMin, double vMax);

public:
  // Construct/Deconstruct
 
  virtual ~FrameScale() {};
  
  static   FrameScale *create(int iWidth, int iHeight, int oWidth, int oHeight, FrameScaleParams *params, int filter, ChromaLocation chromaLocationType = CL_ZERO, int useMinMax = 0);
  
  virtual void process(Frame *out,  const Frame *inp) = 0;
  

};
} // namespace hdrtoolslib

#endif
