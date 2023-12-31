/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2017, ISO/IEC
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
 *  * Neither the name of the ISO/IEC nor the names of its contributors may
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
#ifndef PCC_BITSTREAM_PATCHDATAUNIT_H
#define PCC_BITSTREAM_PATCHDATAUNIT_H

#include "PCCBitstreamCommon.h"
#include "PCCPointLocalReconstructionData.h"

namespace pcc {

// 7.3.7.3  Patch data unit syntax
class PatchDataUnit {
 public:
  PatchDataUnit() :
      pos2dX_( 0 ),
      pos2dY_( 0 ),
      size2dXMinus1_( 0 ),
      size2dYMinus1_( 0 ),
      pos3dX_( 0 ),
      pos3dY_( 0 ),
      pos3dMinZ_( 0 ),
      pos3dDeltaMaxZ_( 0 ),
      projectionId_( 0 ),
      orientationIndex_( 0 ),
      lodEnableFlag_( false ),
      lodScaleXminus1_( 0 ),
      lodScaleY_( 0 ),
      patchIndex_( 0 ),
      frameIndex_( 0 ) {}
  ~PatchDataUnit() {}

  PatchDataUnit&                operator=( const PatchDataUnit& ) = default;
  size_t                        get2dPosX() { return pos2dX_; }
  size_t                        get2dPosY() { return pos2dY_; }
  int64_t                       get2dSizeXMinus1() { return size2dXMinus1_; }
  int64_t                       get2dSizeYMinus1() { return size2dYMinus1_; }
  size_t                        get3dPosX() { return pos3dX_; }
  size_t                        get3dPosY() { return pos3dY_; }
  size_t                        get3dPosMinZ() { return pos3dMinZ_; }
  size_t                        get3dPosDeltaMaxZ() { return pos3dDeltaMaxZ_; }
  size_t                        getProjectionId() { return projectionId_; }
  size_t                        getOrientationIndex() { return orientationIndex_; }
  bool                          getLodEnableFlag() { return lodEnableFlag_; }
  uint8_t                       getLodScaleXminus1() { return lodScaleXminus1_; }
  uint8_t                       getLodScaleY() { return lodScaleY_; }
  PointLocalReconstructionData& getPointLocalReconstructionData() { return pointLocalReconstructionData_; }

  size_t getPatchIndex() { return patchIndex_; }
  size_t getFrameIndex() { return frameIndex_; }
  void   setPatchIndex( size_t value ) { patchIndex_ = value; }
  void   setFrameIndex( size_t value ) { frameIndex_ = value; }
  void   set2dPosX( size_t value ) { pos2dX_ = value; }
  void   set2dPosY( size_t value ) { pos2dY_ = value; }
  void   set2dSizeXMinus1( uint64_t value ) { size2dXMinus1_ = value; }
  void   set2dSizeYMinus1( uint64_t value ) { size2dYMinus1_ = value; }
  void   set3dPosX( size_t value ) { pos3dX_ = value; }
  void   set3dPosY( size_t value ) { pos3dY_ = value; }
  void   set3dPosMinZ( size_t value ) { pos3dMinZ_ = value; }
  void   set3dPosDeltaMaxZ( size_t value ) { pos3dDeltaMaxZ_ = value; }
  void   setProjectionId( size_t value ) { projectionId_ = value; }
  void   setOrientationIndex( size_t value ) { orientationIndex_ = value; }
  void   setLodEnableFlag( bool value ) { lodEnableFlag_ = value; }
  void   setLodScaleXminus1( uint8_t value ) { lodScaleXminus1_ = value; }
  void   setLodScaleY( uint8_t value ) { lodScaleY_ = value; }
  void setPointLocalReconstructionData( PointLocalReconstructionData value ) { pointLocalReconstructionData_ = value; }

 private:
  size_t                       pos2dX_;
  size_t                       pos2dY_;
  uint64_t                     size2dXMinus1_;
  uint64_t                     size2dYMinus1_;
  size_t                       pos3dX_;
  size_t                       pos3dY_;
  size_t                       pos3dMinZ_;
  size_t                       pos3dDeltaMaxZ_;
  size_t                       projectionId_;
  size_t                       orientationIndex_;
  bool                         lodEnableFlag_;
  uint8_t                      lodScaleXminus1_;
  uint8_t                      lodScaleY_;
  PointLocalReconstructionData pointLocalReconstructionData_;
  size_t                       patchIndex_;
  size_t                       frameIndex_;
};

};  // namespace pcc

#endif  //~PCC_BITSTREAM_PATCHDATAUNIT_H
