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
#include "PccAppDecoder.h"

using namespace std;
using namespace pcc;
using pcc::chrono::StopwatchUserTime;

int main( int argc, char* argv[] ) {
  std::cout << "PccAppDecoder v" << TMC2_VERSION_MAJOR << "." << TMC2_VERSION_MINOR << std::endl << std::endl;

  PCCDecoderParameters decoderParams;
  PCCMetricsParameters metricsParams;
  if ( !parseParameters( argc, argv, decoderParams, metricsParams ) ) { return -1; }
  if ( decoderParams.nbThread_ > 0 ) { tbb::task_scheduler_init init( static_cast<int>( decoderParams.nbThread_ ) ); }

  // Timers to count elapsed wall/user time
  pcc::chrono::Stopwatch<std::chrono::steady_clock> clockWall;
  pcc::chrono::StopwatchUserTime                    clockUser;

  clockWall.start();
  int ret = decompressVideo( decoderParams, metricsParams, clockUser );
  clockWall.stop();
  
  using namespace std::chrono;
  using ms       = milliseconds;
  auto totalWall = duration_cast<ms>( clockWall.count() ).count();
  std::cout << "Processing time (wall): " << (totalWall / 1000.0 ) << " s\n";

  auto totalUserSelf = duration_cast<ms>( clockUser.self.count() ).count();
  std::cout << "Processing time (user.self): " << ( totalUserSelf / 1000.0 ) << " s\n";

  auto totalUserChild = duration_cast<ms>( clockUser.children.count() ).count();
  std::cout << "Processing time (user.children): " << ( totalUserChild / 1000.0 ) << " s\n";

  std::cout << "Peak memory: " << getPeakMemory() << " KB\n";
  return ret;
}

//---------------------------------------------------------------------------
// :: Command line / config parsing helpers

namespace pcc {
static std::istream& operator>>( std::istream& in, PCCColorTransform& val ) {
  unsigned int tmp;
  in >> tmp;
  val = PCCColorTransform( tmp );
  return in;
}
}  // namespace pcc

//---------------------------------------------------------------------------
// :: Command line / config parsing

bool parseParameters( int                   argc,
                      char*                 argv[],
                      PCCDecoderParameters& decoderParams,
                      PCCMetricsParameters& metricsParams ) {
  namespace po = df::program_options_lite;

  bool   print_help = false;
  size_t ignore     = 1024;

  // The definition of the program/config options, along with default values.
  //
  // NB: when updating the following tables:
  //      (a) please keep to 80-columns for easier reading at a glance,
  //      (b) do not vertically align values -- it breaks quickly
  //
  // clang-format off
  po::Options opts;
  opts.addOptions()
    ( "help", print_help, false, "This help text")
    ( "c,config", po::parseConfigFile, "Configuration file name")

    // i/o
    ( "compressedStreamPath",
      decoderParams.compressedStreamPath_,
      decoderParams.compressedStreamPath_,
    "Output(encoder)/Input(decoder) compressed bitstream")
    ( "reconstructedDataPath",
      decoderParams.reconstructedDataPath_,
      decoderParams.reconstructedDataPath_,
    "Output decoded pointcloud. Multi-frame sequences may be represented by %04i")

    // sequence configuration
    ( "startFrameNumber",
      decoderParams.startFrameNumber_,
      decoderParams.startFrameNumber_,"Fist frame number in sequence to encode/decode")

    // colour space conversion
    ( "colorTransform",
      decoderParams.colorTransform_,
      decoderParams.colorTransform_,"The colour transform to be applied:\n"
    "  0: none\n"
    "  1: RGB to YCbCr (Rec.709)")
    ( "colorSpaceConversionPath",
      decoderParams.colorSpaceConversionPath_,
      decoderParams.colorSpaceConversionPath_,
    "Path to the HDRConvert. If unset, an internal color space onversion is used")
    ( "inverseColorSpaceConversionConfig",
      decoderParams.inverseColorSpaceConversionConfig_,
      decoderParams.inverseColorSpaceConversionConfig_,
    "HDRConvert configuration file used for YUV420 to RGB444 conversion")
    ( "videoDecoderPath",
      decoderParams.videoDecoderPath_,
      decoderParams.videoDecoderPath_, "HM video decoder executable")
    ( "videoDecoderOccupancyMapPath",
      decoderParams.videoDecoderOccupancyMapPath_,
      decoderParams.videoDecoderOccupancyMapPath_,
    "HM lossless video decoder executable for occupancy map")
    ( "nbThread",
      decoderParams.nbThread_,
      decoderParams.nbThread_,
    "Number of thread used for parallel processing")
    ( "postprocessSmoothingFilterType",
      decoderParams.postprocessSmoothingFilter_,
      decoderParams.postprocessSmoothingFilter_,
    "Exclude geometry smoothing from attribute transfer")
    ( "keepIntermediateFiles",
      decoderParams.keepIntermediateFiles_,
      decoderParams.keepIntermediateFiles_,
    "Keep intermediate files: RGB, YUV and bin")

    // visual quality
    ( "patchColorSubsampling",
      decoderParams.patchColorSubsampling_, 
      false,
    "Enable per-patch color up-sampling");

    opts.addOptions()
    ( "computeChecksum", 
      metricsParams.computeChecksum_,
      metricsParams.computeChecksum_, "Compute checksum")
    ( "computeMetrics", 
      metricsParams.computeMetrics_,
      metricsParams.computeMetrics_, "Compute metrics")
    ( "uncompressedDataFolder", 
      metricsParams.uncompressedDataFolder_,
      metricsParams.uncompressedDataFolder_,
    "Folder where the uncompress input data are stored, use for cfg relative  paths.")
    ( "startFrameNumber", 
      metricsParams.startFrameNumber_,
      metricsParams.startFrameNumber_,
       "Fist frame number in sequence to encode/decode")
    ( "frameCount", 
      metricsParams.frameCount_, 
      metricsParams.frameCount_,
       "Number of frames to encode")
    ( "groupOfFramesSize", 
      metricsParams.groupOfFramesSize_,
      metricsParams.groupOfFramesSize_, "Random access period")
    ( "uncompressedDataPath", 
      metricsParams.uncompressedDataPath_,
      metricsParams.uncompressedDataPath_,
    "Input pointcloud to encode. Multi-frame sequences may be represented by "
    "%04i")
    ( "reconstructedDataPath", 
      metricsParams.reconstructedDataPath_,
      metricsParams.reconstructedDataPath_,
      "Output decoded pointcloud. Multi-frame sequences may be represented by %04i")
    ( "normalDataPath", 
      metricsParams.normalDataPath_,
      metricsParams.normalDataPath_,
    "Input pointcloud to encode. Multi-frame sequences may be represented by %04i")
    ( "resolution", 
      metricsParams.resolution_,
      metricsParams.resolution_, "Specify the intrinsic resolution")
    ( "dropdups", 
      metricsParams.dropDuplicates_, 
      metricsParams.dropDuplicates_,
      "0(detect), 1(drop), 2(average) subsequent points with same coordinates")
    ( "neighborsProc", 
      metricsParams.neighborsProc_,
      metricsParams.neighborsProc_,
      "0(undefined), 1(average), 2(weighted average), 3(min), 4(max) neighbors with same geometric distance")
    ( "nbThread", 
      metricsParams.nbThread_, 
      metricsParams.nbThread_,
      "Number of thread used for parallel processing")
    ( "minimumImageHeight", ignore, ignore, "Ignore parameter")
    ( "flagColorPreSmoothing", ignore, ignore, "Ignore parameter")
    ( "surfaceSeparation", ignore, ignore, "Ignore parameter");

  // clang-format on
  po::setDefaults( opts );
  po::ErrorReporter        err;
  const list<const char*>& argv_unhandled = po::scanArgv( opts, argc, (const char**)argv, err );
  for ( const auto arg : argv_unhandled ) { printf( "Unhandled argument ignored: %s \n", arg ); }

  if ( argc == 1 || print_help ) {
    po::doHelp( std::cout, opts, 78 );
    return false;
  }

  decoderParams.completePath();
  decoderParams.print();
  if ( !decoderParams.check() ) {
    printf( "Error Input parameters are not correct \n" );
    return false;
  }
  metricsParams.completePath();
  metricsParams.print();
  if ( !metricsParams.check( true ) ) {
    printf( "Error Input parameters are not correct \n" );
    return false;
  }
  metricsParams.startFrameNumber_ = decoderParams.startFrameNumber_;

  // report the current configuration (only in the absence of errors so
  // that errors/warnings are more obvious and in the same place).
  return !err.is_errored;
}

int decompressVideo( const PCCDecoderParameters& decoderParams,
                     const PCCMetricsParameters& metricsParams,
                     StopwatchUserTime&          clock ) {
  PCCBitstream     bitstream;
  PCCBitstreamStat bitstreamStat;
#ifdef BITSTREAM_TRACE
  bitstream.setTrace( true );
  bitstream.openTrace( removeFileExtension( decoderParams.compressedStreamPath_ ) + "_samplestream_read.txt" );
  bitstream.setTraceFile( bitstream.getTraceFile() );
#endif
  if ( !bitstream.initialize( decoderParams.compressedStreamPath_ ) ) { return -1; }
  // if ( !bitstream.readHeader() ) { return -1; }
  bitstreamStat.setHeader( bitstream.size() );
  size_t      frameNumber = decoderParams.startFrameNumber_;
  PCCMetrics  metrics;
  PCCChecksum checksum;
  metrics.setParameters( metricsParams );
  checksum.setParameters( metricsParams );
  std::vector<std::vector<uint8_t>> checksumsRec;
  std::vector<std::vector<uint8_t>> checksumsDec;
  if ( metricsParams.computeChecksum_ ) { checksum.read( decoderParams.compressedStreamPath_ ); }
  PCCDecoder decoder;
  decoder.setParameters( decoderParams );

  SampleStreamV3CUnit ssvu;
  PCCBitstreamReader  bitstreamReader;
  size_t              headerSize = pcc::PCCBitstreamReader::read( bitstream, ssvu );
  bitstreamStat.incrHeader( headerSize );
#ifdef BITSTREAM_TRACE
  size_t index = 0;
#endif
  bool bMoreData = true;
  while ( bMoreData ) {
    PCCGroupOfFrames reconstructs;
    PCCContext       context;
    context.setBitstreamStat( bitstreamStat );
    clock.start();

    PCCBitstreamReader bitstreamReader;
#ifdef BITSTREAM_TRACE
    PCCBitstream bitstream;
    bitstream.setTrace( true );
    bitstream.openTrace( stringFormat( "%s_GOF%u_hls_decode.txt",
                                       removeFileExtension( decoderParams.compressedStreamPath_ ).c_str(), index++ ) );
    bitstreamReader.setTraceFile( bitstream.getTraceFile() );
#endif
    if ( bitstreamReader.decode( ssvu, context ) == 0 ) { return 0; }
#ifdef BITSTREAM_TRACE
    bitstream.closeTrace();
#endif
    // allocate atlas structure
    context.resizeAtlas( context.getVps().getAtlasCountMinus1() + 1 );
    for ( uint32_t atlId = 0; atlId < context.getVps().getAtlasCountMinus1() + 1; atlId++ ) {
      context.getAtlas( atlId ).allocateVideoFrames( context, 0 );
      // first allocating the structures, frames will be added as the V3C
      // units are being decoded ???
      context.setAtlasIndex( atlId );
      int retDecoding = decoder.decode( context, reconstructs,  clock, atlId );
      //clock.stop();
      if ( retDecoding != 0 ) { return retDecoding; }
      if ( metricsParams.computeChecksum_ ) { checksum.computeDecoded( reconstructs ); }
      if ( metricsParams.computeMetrics_ ) {
        PCCGroupOfFrames sources;
        PCCGroupOfFrames normals;
        if ( !sources.load( metricsParams.uncompressedDataPath_, frameNumber,
                            frameNumber + reconstructs.getFrameCount(), decoderParams.colorTransform_ ) ) {
          return -1;
        }
        if ( !metricsParams.normalDataPath_.empty() ) {
          if ( !normals.load( metricsParams.normalDataPath_, frameNumber, frameNumber + reconstructs.getFrameCount(),
                              COLOR_TRANSFORM_NONE, true ) ) {
            return -1;
          }
        }
        metrics.compute( sources, reconstructs, normals );
        sources.clear();
        normals.clear();
      }
      if ( !decoderParams.reconstructedDataPath_.empty() ) {
        reconstructs.write( decoderParams.reconstructedDataPath_, frameNumber );
      } else {
        frameNumber += reconstructs.getFrameCount();
      }
      bMoreData = ( ssvu.getV3CUnitCount() > 0 );
    }
  }
  bitstreamStat.trace();
  if ( metricsParams.computeMetrics_ ) { metrics.display(); }
  if ( metricsParams.computeChecksum_ ) {
    if ( !checksum.compareRecDec() ) { return -1; }
  }
  return 0;
}
