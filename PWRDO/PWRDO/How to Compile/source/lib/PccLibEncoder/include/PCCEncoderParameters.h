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
#ifndef PCCEncoderParameters_h
#define PCCEncoderParameters_h

#include "PCCCommon.h"
#include "PCCMath.h"

namespace pcc {
class PCCContext;

class PCCEncoderParameters {
 public:
  PCCEncoderParameters();
  ~PCCEncoderParameters();
  void              print();
  bool              check();
  void              completePath();
  static void       constructAspsRefList( PCCContext& context, size_t aspsIdx, size_t afpsIdx );
  void              initializeContext( PCCContext& context );
  size_t            startFrameNumber_;
  std::string       configurationFolder_;
  std::string       uncompressedDataFolder_;
  std::string       compressedStreamPath_;
  std::string       reconstructedDataPath_;
  PCCColorTransform colorTransform_;
  std::string       colorSpaceConversionPath_;
  std::string       videoEncoderPath_;
  bool              use3dmc_;
  std::string       videoEncoderAuxPath_;
  std::string       videoEncoderOccupancyMapPath_;
  std::string       colorSpaceConversionConfig_;
  std::string       inverseColorSpaceConversionConfig_;
  size_t            nbThread_;
  size_t            frameCount_;
  size_t            groupOfFramesSize_;
  std::string       uncompressedDataPath_;

  // packing
  size_t minimumImageWidth_;
  size_t minimumImageHeight_;

  // video encoding
  int geometryQP_;
  int textureQP_;

  // segmentation
  size_t nnNormalEstimation_;
  bool   gridBasedRefineSegmentation_;
  size_t maxNNCountRefineSegmentation_;
  size_t iterationCountRefineSegmentation_;
  size_t voxelDimensionRefineSegmentation_;
  size_t searchRadiusRefineSegmentation_;
  size_t occupancyResolution_;
  bool   enablePatchSplitting_;
  size_t maxPatchSize_;
  size_t log2QuantizerSizeX_;
  size_t log2QuantizerSizeY_;
  size_t minPointCountPerCCPatchSegmentation_;
  size_t maxNNCountPatchSegmentation_;
  size_t surfaceThickness_;
  size_t minLevel_;  // 8,16,32,64
  double maxAllowedDist2RawPointsDetection_;
  double maxAllowedDist2RawPointsSelection_;
  double lambdaRefineSegmentation_;
  size_t mapCountMinus1_;

  // occupancy map encoding
  size_t      maxCandidateCount_;
  size_t      occupancyPrecision_;
  std::string occupancyMapVideoEncoderConfig_;
  size_t      occupancyMapQP_;
  size_t      EOMFixBitCount_;
  bool        occupancyMapRefinement_;

  // smoothing
  size_t neighborCountSmoothing_;
  double radius2Smoothing_;
  double radius2BoundaryDetection_;
  double thresholdSmoothing_;
  bool   gridSmoothing_;
  size_t gridSize_;
  bool   flagGeometrySmoothing_;
  size_t postprocessSmoothingFilter_;

  // Patch Expansion (m47772, CE2.12)
  bool patchExpansion_;

  // color smoothing
  double thresholdColorSmoothing_;
  double thresholdColorDifference_;
  double thresholdColorVariation_;
  size_t cgridSize_;
  bool   flagColorSmoothing_;

  // color pre-smoothing
  double thresholdColorPreSmoothing_;
  double thresholdColorPreSmoothingLocalEntropy_;
  double radius2ColorPreSmoothing_;
  size_t neighborCountColorPreSmoothing_;
  bool   flagColorPreSmoothing_;

  // coloring
  size_t bestColorSearchRange_;
  // Improved color transfer
  int    numNeighborsColorTransferFwd_;
  int    numNeighborsColorTransferBwd_;
  bool   useDistWeightedAverageFwd_;
  bool   useDistWeightedAverageBwd_;
  bool   skipAvgIfIdenticalSourcePointPresentFwd_;
  bool   skipAvgIfIdenticalSourcePointPresentBwd_;
  double distOffsetFwd_;
  double distOffsetBwd_;
  double maxGeometryDist2Fwd_;
  double maxGeometryDist2Bwd_;
  double maxColorDist2Fwd_;
  double maxColorDist2Bwd_;
  // Exclude color outliers
  bool   excludeColorOutlier_;
  double thresholdColorOutlierDist_;

  // lossless
  bool noAttributes_;
  bool losslessGeo_;
  bool losslessGeo444_;

  // raw points video
  bool        useRawPointsSeparateVideo_;
  std::string geometryMPConfig_;
  std::string textureMPConfig_;

  // scale and bias
  float             modelScale_;
  PCCVector3<float> modelOrigin_;

  // patch sampling resolution
  size_t      levelOfDetailX_;
  size_t      levelOfDetailY_;
  std::string geometryConfig_;
  std::string geometryD0Config_;
  std::string geometryD1Config_;
  std::string textureConfig_;
  std::string textureT0Config_;
  std::string textureT1Config_;
  bool        keepIntermediateFiles_;
  bool        absoluteD1_;
  int         qpAdjD1_;
  bool        absoluteT1_;
  int         qpAdjT1_;
  bool        multipleStreams_;
  bool        constrainedPack_;

  // dilation
  bool groupDilation_;
  bool textureDilationOffLossless_;

  // EOM
  bool enhancedOccupancyMapCode_;

  // Lossy occupancy Map coding
  size_t offsetLossyOM_;
  size_t thresholdLossyOM_;
  bool   prefilterLossyOM_;

  // reconstruction
  bool   removeDuplicatePoints_;
  bool   pointLocalReconstruction_;
  size_t patchSize_;
  size_t plrlNumberOfModes_;
  bool   singleMapPixelInterleaving_;

  // visual quality
  bool   patchColorSubsampling_;
  bool   deltaCoding_;
  bool   surfaceSeparation_;
  bool   highGradientSeparation_;
  double minGradient_;
  size_t minNumHighGradientPoints_;

  // Flexible Patch Packing
  size_t packingStrategy_;
  size_t textureBGFill_;
  size_t safeGuardDistance_;
  bool   useEightOrientations_;

  // Lossy raw points Patch
  bool   lossyRawPointsPatch_;
  double minNormSumOfInvDist4MPSelection_;
  int    lossyRawPointPatchGeoQP_;

  // GPA
  int globalPatchAllocation_;
  // GTP
  int    globalPackingStrategyGOF_;
  bool   globalPackingStrategyReset_;
  double globalPackingStrategyThreshold_;
  // low delay encoding
  bool lowDelayEncoding_;
  // 3D geometry padding
  size_t geometryPadding_;

  // EOM
  bool   enhancedPP_;
  double minWeightEPP_;

  // Additional Projection Plane
  int    additionalProjectionPlaneMode_;
  double partialAdditionalProjectionPlane_;

  // 3D and 2D bit depths
  size_t geometry3dCoordinatesBitdepth_;
  size_t geometryNominal2dBitdepth_;

  // Partitions and tiles
  bool             enablePointCloudPartitioning_;
  std::vector<int> roiBoundingBoxMinX_;
  std::vector<int> roiBoundingBoxMaxX_;
  std::vector<int> roiBoundingBoxMinY_;
  std::vector<int> roiBoundingBoxMaxY_;
  std::vector<int> roiBoundingBoxMinZ_;
  std::vector<int> roiBoundingBoxMaxZ_;
  int              numTilesHor_;
  double           tileHeightToWidthRatio_;
  int              numCutsAlong1stLongestAxis_;
  int              numCutsAlong2ndLongestAxis_;
  int              numCutsAlong3rdLongestAxis_;
  int              numROIs_;

  // Sort raw points by Morton code
  bool   mortonOrderSortRawPoints_;
  size_t textureRawSeparateVideoWidth_;

  // Patch block filtering
  bool    pbfEnableFlag_;
  int16_t pbfPassesCount_;
  int16_t pbfFilterSize_;
  int16_t pbfLog2Threshold_;

  //
  bool   patchPrecedenceOrderFlag_;
  size_t maxNumRefAtlasList_;
  size_t maxNumRefAtlasFrame_;
};

};  // namespace pcc

#endif /* PCCEncoderParameters_h */
