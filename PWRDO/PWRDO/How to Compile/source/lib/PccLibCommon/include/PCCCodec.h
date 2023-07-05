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
#ifndef PCCCodec_h
#define PCCCodec_h

#include "tbb/compat/condition_variable"
#include "PCCCommon.h"
#include "PCCImage.h"
#include "PCCMath.h"
#include "PCCVideo.h"
#include "PCCContext.h"

namespace pcc {
class PCCPatch;
} /* namespace pcc */

namespace pcc {

class PCCContext;
class PCCFrameContext;
class PCCGroupOfFrames;
class PCCPointSet3;
template <typename T, size_t N>
class PCCVideo;
typedef pcc::PCCVideo<uint16_t, 3> PCCVideoTexture;
typedef pcc::PCCVideo<uint16_t, 3> PCCVideoGeometry;

template <typename T, size_t N>
class PCCImage;
typedef pcc::PCCImage<uint16_t, 3> PCCImageGeometry;
typedef pcc::PCCImage<uint8_t, 3>  PCCImageOccupancyMap;

struct GeneratePointCloudParameters {
  size_t      occupancyResolution_;
  size_t      occupancyPrecision_;
  bool        enableSizeQuantization_;
  bool        gridSmoothing_;
  size_t      gridSize_;
  size_t      neighborCountSmoothing_;
  double      radius2Smoothing_;
  double      radius2BoundaryDetection_;
  double      thresholdSmoothing_;
  size_t      rawPointColorFormat_;
  size_t      nbThread_;
  bool        multipleStreams_;
  bool        absoluteD1_;
  size_t      surfaceThickness_;
  double      thresholdColorSmoothing_;
  size_t      cgridSize_;
  double      thresholdColorDifference_;
  double      thresholdColorVariation_;
  bool        flagGeometrySmoothing_;
  bool        flagColorSmoothing_;
  bool        enhancedOccupancyMapCode_;
  size_t      EOMFixBitCount_;
  size_t      thresholdLossyOM_;
  bool        removeDuplicatePoints_;
  size_t      mapCountMinus1_;
  bool        pointLocalReconstruction_;
  bool        singleMapPixelInterleaving_;
  std::string path_;
  bool        useAdditionalPointsPatch_;
  size_t      plrlNumberOfModes_;
  size_t      geometryBitDepth3D_;
  size_t      geometry3dCoordinatesBitdepth_;
  bool        pbfEnableFlag_;
  int16_t     pbfPassesCount_;
  int16_t     pbfFilterSize_;
  int16_t     pbfLog2Threshold_;
};

#ifdef CODEC_TRACE
#define TRACE_CODEC( fmt, ... ) trace( fmt, ##__VA_ARGS__ );
#else
#define TRACE_CODEC( fmt, ... ) ;
#endif

class PCCCodec {
 public:
  PCCCodec();
  ~PCCCodec();

  void generatePointCloud( PCCGroupOfFrames&                   reconstructs,
                           PCCContext&                         context,
                           const GeneratePointCloudParameters& params,
                           std::vector<std::vector<uint32_t>>& partitions,
                           bool                                bDecoder );

  void generatePointCloud( PCCPointSet3&                       reconstruct,
                           PCCContext&                         context,
                           PCCFrameContext&                    frame,
                           const GeneratePointCloudParameters& params,
                           std::vector<uint32_t>&              partition,
                           bool                                bDecoder );

  bool colorPointCloud( PCCGroupOfFrames&                     reconstructs,
                        PCCContext&                           context,
                        const uint8_t                         attributeCount,
                        const PCCColorTransform               colorTransform,
                        const std::vector<std::vector<bool>>& absoluteT1List,
                        const size_t                          multipleStreams,
                        const GeneratePointCloudParameters&   params );

  bool colorPointCloud( PCCPointSet3&                       reconstruct,
                        PCCContext&                         context,
                        PCCFrameContext&                    frame,
                        const std::vector<bool>&            absoluteT1List,
                        const size_t                        multipleStreams,
                        const uint8_t                       attributeCount,
                        const GeneratePointCloudParameters& params );

  void smoothPointCloudPostprocess( PCCPointSet3&                       reconstruct,
                                    PCCContext&                         context,
                                    const PCCColorTransform             colorTransform,
                                    const GeneratePointCloudParameters& params,
                                    std::vector<uint32_t>&              partition );
  void colorSmoothing( PCCPointSet3&                       reconstruct,
                       PCCContext&                         context,
                       const PCCColorTransform             colorTransform,
                       const GeneratePointCloudParameters& params );

  void generateRawPointsGeometryfromVideo( PCCContext& context, PCCFrameContext& frame, size_t frameIndex );

  void generateRawPointsTexturefromVideo( PCCContext& context, PCCFrameContext& frame, size_t frameIndex );

  void generateRawPointsGeometryfromVideo( PCCContext& context );
  void generateRawPointsTexturefromVideo( PCCContext& context );

#ifdef CODEC_TRACE
  template <typename... Args>
  void trace( const char* pFormat, Args... eArgs ) {
    if ( trace_ ) {
      FILE* output = traceFile_ ? traceFile_ : stdout;
      fprintf( output, pFormat, eArgs... );
      fflush( output );
    }
  }

  template <typename T>
  void traceVector( std::vector<T>    data,
                    const size_t      width,
                    const size_t      height,
                    const std::string string,
                    const bool        hexa = false ) {
    if ( trace_ ) {
      if ( data.size() == 0 ) { data.resize( width * height, 0 ); }
      trace( "%s: %zu %zu \n", string.c_str(), width, height );
      for ( size_t v0 = 0; v0 < height; ++v0 ) {
        for ( size_t u0 = 0; u0 < width; ++u0 ) {
          if ( hexa ) {
            trace( "%2x", (int)( data[v0 * width + u0] ) );
          } else {
            trace( "%3d", (int)( data[v0 * width + u0] ) );
          }
        }
        trace( "\n" );
      }
    }
  }

  void setTrace( bool trace ) { trace_ = trace; }
  bool getTrace() { return trace_; }
  bool openTrace( std::string file ) {
    if ( traceFile_ ) {
      fclose( traceFile_ );
      traceFile_ = NULL;
    }
    if ( ( traceFile_ = fopen( file.c_str(), "w" ) ) == NULL ) { return false; }
    return true;
  }
  void closeTrace() {
    if ( traceFile_ ) {
      fclose( traceFile_ );
      traceFile_ = NULL;
    }
  }
#endif
 protected:
  void generateOccupancyMap( PCCFrameContext&            frame,
                             const PCCImageOccupancyMap& videoFrame,
                             const size_t                occupancyPrecision,
                             const size_t                thresholdLossyOM,
                             const bool                  enhancedOccupancyMapForDepthFlag );

  void generateBlockToPatchFromBoundaryBox( PCCContext& context, const size_t occupancyResolution );

  void generateBlockToPatchFromBoundaryBox( PCCContext&      context,
                                            PCCFrameContext& frame,
                                            const size_t     occupancyResolution );

  void generateBlockToPatchFromOccupancyMapVideo( PCCContext&  context,
                                                  const size_t occupancyResolution,
                                                  const size_t occupancyPrecision );

  void generateBlockToPatchFromOccupancyMapVideo( PCCContext&           context,
                                                  PCCFrameContext&      frame,
                                                  PCCImageOccupancyMap& occupancyMapImage,
                                                  const size_t          occupancyResolution,
                                                  const size_t          occupancyPrecision );

  int getDeltaNeighbors( const PCCImageGeometry& frame,
                         const PCCPatch&         patch,
                         const int               xOrg,
                         const int               yOrg,
                         const int               neighboring,
                         const int               threshold,
                         const bool              projectionMode );

  std::vector<PCCPoint3D> generatePoints( const GeneratePointCloudParameters&  params,
                                          PCCFrameContext&                     frame,
                                          const std::vector<PCCVideoGeometry>& videoMultiple,
                                          const size_t                         videoFrameIndex,
                                          const size_t                         patchIndex,
                                          const size_t                         u,
                                          const size_t                         v,
                                          const size_t                         x,
                                          const size_t                         y,
                                          const bool                           interpolate,
                                          const bool                           filling,
                                          const size_t                         minD1,
                                          const size_t                         neighbor );
  // PCCPatchType            getCurrPatchType( PCCTileType tileType, uint8_t patchMode );
  inline double entropy( std::vector<uint8_t>& Data, int N ) {
    std::vector<size_t> count;
    count.resize( 256, 0 );
    for ( size_t i = 0; i < N; ++i ) { ++count[size_t( Data[i] )]; }
    double s = 0;
    for ( size_t i = 0; i < 256; ++i ) {
      if ( count[i] ) {
        double p = double( count[i] ) / double( N );
        s += -p * std::log2( p );
      }
    }
    return s;
  }

  inline double median( std::vector<uint16_t>& Data, int N ) {
    std::sort( Data.begin(), Data.end() );

    if ( N % 2 == 0 )
      return ( double( Data[N / 2] ) + double( Data[N / 2 - 1] ) ) / 2.0;
    else
      return double( Data[N / 2] );
  }

  inline double mean( std::vector<uint16_t>& Data, int N ) {
    double s = 0.0;
    for ( size_t i = 0; i < N; ++i ) { s += double( Data[i] ); }
    return s / double( N );
  }

 private:
  void smoothPointCloud( PCCPointSet3&                      reconstruct,
                         const std::vector<uint32_t>&       partition,
                         const GeneratePointCloudParameters params );

  void createSubReconstruct( const PCCPointSet3&                 reconstruct,
                             const std::vector<uint32_t>&        partition,
                             PCCFrameContext&                    frame,
                             const GeneratePointCloudParameters& params,
                             const size_t                        frameCount,
                             PCCPointSet3&                       subReconstruct,
                             std::vector<uint32_t>&              subPartition,
                             std::vector<size_t>&                subReconstructIndex );

  void createSpecificLayerReconstruct( const PCCPointSet3&                 reconstruct,
                                       const std::vector<uint32_t>&        partition,
                                       PCCFrameContext&                    frame,
                                       const GeneratePointCloudParameters& params,
                                       const size_t                        frameCount,
                                       PCCPointSet3&                       subReconstruct,
                                       std::vector<uint32_t>&              subPartition,
                                       std::vector<size_t>&                subReconstructIndex );

  void updateReconstruct( PCCPointSet3&              reconstruct,
                          const PCCPointSet3&        subReconstruct,
                          const std::vector<size_t>& subReconstructIndex );

  void smoothPointCloudGrid( PCCPointSet3&                       reconstruct,
                             const std::vector<uint32_t>&        partition,
                             const GeneratePointCloudParameters& params,
                             uint16_t                            gridWidth,
                             std::vector<int>&                   cellIndex );

  void addGridCentroid( PCCPoint3D&                     point,
                        uint32_t                        patchIdx,
                        std::vector<uint16_t>&          count,
                        std::vector<PCCVector3<float>>& center,
                        std::vector<uint32_t>&          partition,
                        std::vector<bool>&              doSmooth,
                        uint8_t                         gridSize,
                        uint16_t                        gridWidth,
                        int                             cellId );

  void addGridColorCentroid( PCCPoint3D&                         point,
                             PCCVector3D&                        color,
                             uint32_t                            patchIdx,
                             std::vector<uint16_t>&              colorGridCount,
                             std::vector<PCCVector3<float>>&     colorCenter,
                             std::vector<uint32_t>&              colorPartition,
                             std::vector<bool>&                  colorDoSmooth,
                             uint8_t                             colorGrid,
                             std::vector<std::vector<uint16_t>>& colorLum,
                             const GeneratePointCloudParameters& params,
                             int                                 cellId );

  bool gridFilteringColor( PCCPoint3D&                         curPos,
                           PCCVector3D&                        colorCentroid,
                           int&                                colorCount,
                           std::vector<uint16_t>&              colorGridCount,
                           std::vector<PCCVector3<float>>&     colorCenterGrid,
                           std::vector<bool>&                  colorDoSmooth,
                           uint8_t                             gridSize,
                           PCCVector3D&                        curPosColor,
                           const GeneratePointCloudParameters& params,
                           std::vector<int>&                   cellIndex );

  void smoothPointCloudColorLC( PCCPointSet3&                       reconstruct,
                                const GeneratePointCloudParameters& params,
                                std::vector<int>&                   cellIndex );

  bool gridFiltering( const std::vector<uint32_t>&    partition,
                      PCCPointSet3&                   pointCloud,
                      PCCPoint3D&                     curPoint,
                      PCCVector3D&                    centroid,
                      int&                            count,
                      std::vector<uint16_t>&          gridCount,
                      std::vector<PCCVector3<float>>& center,
                      std::vector<bool>&              doSmooth,
                      uint8_t                         gridSize,
                      uint16_t                        gridWidth,
                      std::vector<int>&               cellIndex );

  bool gridFilteringTransfer( const std::vector<uint32_t>& partition,
                              PCCPointSet3&                pointCloud,
                              PCCPoint3D&                  curPoint,
                              PCCVector3D&                 centroid,
                              int&                         count,
                              std::vector<int>&            gridCount,
                              std::vector<PCCVector3D>&    center,
                              std::vector<bool>&           doSmooth,
                              int                          gridSize,
                              int                          gridWidth,
                              std::vector<PCCVector3D>&    colorGrid,
                              PCCVector3D&                 color );

  void identifyBoundaryPoints( const std::vector<uint32_t>& occupancyMap,
                               const size_t                 x,
                               const size_t                 y,
                               const size_t                 imageWidth,
                               const size_t                 imageHeight,
                               const size_t                 pointIndex,
                               std::vector<uint32_t>&       BPflag,
                               PCCPointSet3&                reconstruct );

#ifdef CODEC_TRACE
  void printChecksum( PCCPointSet3& ePointcloud, std::string eString );
#endif
  std::vector<uint16_t>              geoSmoothingCount_;
  std::vector<PCCVector3<float>>     geoSmoothingCenter_;
  std::vector<bool>                  geoSmoothingDoSmooth_;
  std::vector<uint32_t>              geoSmoothingPartition_;
  std::vector<uint16_t>              colorSmoothingCount_;
  std::vector<PCCVector3<float>>     colorSmoothingCenter_;
  std::vector<bool>                  colorSmoothingDoSmooth_;
  std::vector<uint32_t>              colorSmoothingPartition_;
  std::vector<std::vector<uint16_t>> colorSmoothingLum_;
#ifdef CODEC_TRACE
  bool  trace_;
  FILE* traceFile_;
#else
#ifdef BITSTREAM_TRACE
  bool  trace_;
  FILE* traceFile_;
#endif
#endif
};

};  // namespace pcc

#endif /* PCCCodec_h */
