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
#ifndef PCC_BITSTREAM_COMMON_H
#define PCC_BITSTREAM_COMMON_H

#define NOMINMAX
#include <limits>
#include <assert.h>
#include <chrono>
#include <cmath>
#include <vector>
#include <list>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstring>
#include <sys/stat.h>
#include <memory>
#include <queue>
#include <algorithm>
#include <map>
#include "PCCConfig.h"
#if defined( WIN32 )
#include <windows.h>
#endif
#if defined( __APPLE__ ) && defined( __MACH__ )
#include <unistd.h>
#include <sys/resource.h>
#include <mach/mach.h>
#endif

#define SINGLEPASS_WRITING 1

namespace pcc {

// ******************************************************************* //
// Trace modes to validate new syntax
// ******************************************************************* //
// #define BITSTREAM_TRACE

// ******************************************************************* //
// Common constants
// ******************************************************************* //
#define EXPAND_RANGE_CONDITIONAL 0
#define EXPAND_RANGE_ENCODER 1
#define ATLASIDXPCC 0

const uint32_t PCC_UNDEFINED_INDEX     = -1;
const bool     printDetailedInfo       = false;
const uint8_t  MAX_NUM_ATTR_PARTITIONS = 64;
enum PCCVideoType {
  VIDEO_OCCUPANCY = 0,
  VIDEO_GEOMETRY,
  VIDEO_GEOMETRY_D0,
  VIDEO_GEOMETRY_D1,
  VIDEO_GEOMETRY_D2,
  VIDEO_GEOMETRY_D3,
  VIDEO_GEOMETRY_D4,
  VIDEO_GEOMETRY_D5,
  VIDEO_GEOMETRY_D6,
  VIDEO_GEOMETRY_D7,
  VIDEO_GEOMETRY_D8,
  VIDEO_GEOMETRY_D9,
  VIDEO_GEOMETRY_D10,
  VIDEO_GEOMETRY_D11,
  VIDEO_GEOMETRY_D12,
  VIDEO_GEOMETRY_D13,
  VIDEO_GEOMETRY_D14,
  VIDEO_GEOMETRY_D15,
  VIDEO_GEOMETRY_RAW,
  VIDEO_TEXTURE,
  VIDEO_TEXTURE_T0,
  VIDEO_TEXTURE_T1  = VIDEO_TEXTURE_T0 + MAX_NUM_ATTR_PARTITIONS,
  VIDEO_TEXTURE_T2  = VIDEO_TEXTURE_T1 + MAX_NUM_ATTR_PARTITIONS,
  VIDEO_TEXTURE_T3  = VIDEO_TEXTURE_T2 + MAX_NUM_ATTR_PARTITIONS,
  VIDEO_TEXTURE_T4  = VIDEO_TEXTURE_T3 + MAX_NUM_ATTR_PARTITIONS,
  VIDEO_TEXTURE_T5  = VIDEO_TEXTURE_T4 + MAX_NUM_ATTR_PARTITIONS,
  VIDEO_TEXTURE_T6  = VIDEO_TEXTURE_T5 + MAX_NUM_ATTR_PARTITIONS,
  VIDEO_TEXTURE_T7  = VIDEO_TEXTURE_T6 + MAX_NUM_ATTR_PARTITIONS,
  VIDEO_TEXTURE_T8  = VIDEO_TEXTURE_T7 + MAX_NUM_ATTR_PARTITIONS,
  VIDEO_TEXTURE_T9  = VIDEO_TEXTURE_T8 + MAX_NUM_ATTR_PARTITIONS,
  VIDEO_TEXTURE_T10 = VIDEO_TEXTURE_T9 + MAX_NUM_ATTR_PARTITIONS,
  VIDEO_TEXTURE_T11 = VIDEO_TEXTURE_T10 + MAX_NUM_ATTR_PARTITIONS,
  VIDEO_TEXTURE_T12 = VIDEO_TEXTURE_T11 + MAX_NUM_ATTR_PARTITIONS,
  VIDEO_TEXTURE_T13 = VIDEO_TEXTURE_T12 + MAX_NUM_ATTR_PARTITIONS,
  VIDEO_TEXTURE_T14 = VIDEO_TEXTURE_T13 + MAX_NUM_ATTR_PARTITIONS,
  VIDEO_TEXTURE_T15 = VIDEO_TEXTURE_T14 + MAX_NUM_ATTR_PARTITIONS,
  VIDEO_TEXTURE_RAW = VIDEO_TEXTURE_T15 + MAX_NUM_ATTR_PARTITIONS,
  NUM_VIDEO_TYPE    = VIDEO_TEXTURE_RAW + MAX_NUM_ATTR_PARTITIONS
};
enum PCCMetadataType { METADATA_GOF = 0, METADATA_FRAME, METADATA_PATCH };
enum PCCPatchOrientation {
  PATCH_ORIENTATION_DEFAULT = 0,
  PATCH_ORIENTATION_SWAP    = 1,
  PATCH_ORIENTATION_ROT90   = 2,
  PATCH_ORIENTATION_ROT180  = 3,
  PATCH_ORIENTATION_ROT270  = 4,
  PATCH_ORIENTATION_MIRROR  = 5,
  PATCH_ORIENTATION_MROT90  = 6,
  PATCH_ORIENTATION_MROT180 = 7,
  PATCH_ORIENTATION_MROT270 = 8  // similar to SWAP, not used
};                               // switched SWAP with ROT90 positions

enum V3CUnitType {
  V3C_VPS = 0,       // 0: Sequence parameter set
  V3C_AD,            // 1: Patch Data Group
  V3C_OVD,           // 2: Occupancy Video Data
  V3C_GVD,           // 3: Geometry Video Data
  V3C_AVD,           // 4: Attribute Video Data
  V3C_RSVD_05,       // 05: Reserved
  V3C_RSVD_06,       // 06: Reserved
  V3C_RSVD_07,       // 07: Reserved
  V3C_RSVD_08,       // 08: Reserved
  V3C_RSVD_09,       // 09: Reserved
  V3C_RSVD_10,       // 10: Reserved
  V3C_RSVD_11,       // 11: Reserved
  V3C_RSVD_12,       // 12: Reserved
  V3C_RSVD_13,       // 13: Reserved
  V3C_RSVD_14,       // 14: Reserved
  V3C_RSVD_15,       // 15: Reserved
  V3C_RSVD_16,       // 16: Reserved
  V3C_RSVD_17,       // 17: Reserved
  V3C_RSVD_18,       // 18: Reserved
  V3C_RSVD_19,       // 19: Reserved
  V3C_RSVD_20,       // 20: Reserved
  V3C_RSVD_21,       // 21: Reserved
  V3C_RSVD_22,       // 22: Reserved
  V3C_RSVD_23,       // 23: Reserved
  V3C_RSVD_24,       // 24: Reserved
  V3C_RSVD_25,       // 25: Reserved
  V3C_RSVD_26,       // 26: Reserved
  V3C_RSVD_27,       // 27: Reserved
  V3C_RSVD_28,       // 28: Reserved
  V3C_RSVD_29,       // 29: Reserved
  V3C_RSVD_30,       // 30: Reserved
  V3C_RSVD_31,       // 32: Reserved
  NUM_V3C_UNIT_TYPE  // undefined
};

enum PCCCodecID { CODEC_HEVC = 0 };

enum PCCCodecGroup {
  CODEC_GROUP_AVC_PROGRESSIVE_HIGH = 0,
  CODEC_GROUP_HEVC_MAIN10,
  CODEC_GROUP_HEVC444,
  CODEC_GROUP_MP4RA
};

enum PCCTileType {
  P_TILE = 0,  // Inter atlas tile
  I_TILE,      //  Intra atlas tile)
  SKIP_TILE,   // SKIP atlas tile
  RESERVED_3   // 3 to N (N not defined?)
};

enum PCCPatchModeITile {
  I_INTRA = 0,    // Non-predicted patch mode
  I_RAW,          // RAW Point Patch mode
  I_EOM,          // EOM Point Patch mode
  I_RESERVED_3,   // I_RESERVED Reserved modes
  I_RESERVED_4,   // I_RESERVED Reserved modes
  I_RESERVED_5,   // I_RESERVED Reserved modes
  I_RESERVED_6,   // I_RESERVED Reserved modes
  I_RESERVED_7,   // I_RESERVED Reserved modes
  I_RESERVED_8,   // I_RESERVED Reserved modes
  I_RESERVED_9,   // I_RESERVED Reserved modes
  I_RESERVED_10,  // I_RESERVED Reserved modes
  I_RESERVED_11,  // I_RESERVED Reserved modes
  I_RESERVED_12,  // I_RESERVED Reserved modes
  I_RESERVED_13,  // I_RESERVED Reserved modes
  I_END           // Patch termination mode
};

enum PCCPatchModePTile {
  P_SKIP = 0,     // Patch Skip mode
  P_MERGE,        // Patch Merge mode
  P_INTER,        // Inter predicted Patch mode
  P_INTRA,        // Non-predicted Patch mode
  P_RAW,          // RAW Point Patch mode
  P_EOM,          // EOM Point Patch mode
  P_RESERVED_6,   // Reserved modes
  P_RESERVED_7,   // Reserved modes
  P_RESERVED_8,   // Reserved modes
  P_RESERVED_9,   // Reserved modes
  P_RESERVED_10,  // Reserved modes
  P_RESERVED_11,  // Reserved modes
  P_RESERVED_12,  // Reserved modes
  P_RESERVED_13,  // Reserved modes
  P_END,          // Patch termination mode
};

enum PCCPatchType {
  INTRA_PATCH = 0,
  INTER_PATCH,
  MERGE_PATCH,
  SKIP_PATCH,
  RAW_PATCH,
  EOM_PATCH,
  END_PATCH,
  ERROR_PATCH
};

static PCCPatchType getPatchType( PCCTileType tileType, uint8_t patchMode ) {
  if ( tileType == SKIP_TILE ) {
    return SKIP_PATCH;
  } else if ( tileType == P_TILE ) {
    if ( patchMode == P_SKIP ) {
      return SKIP_PATCH;
    } else if ( patchMode == P_MERGE ) {
      return MERGE_PATCH;
    } else if ( patchMode == P_INTRA ) {
      return INTRA_PATCH;
    } else if ( patchMode == P_INTER ) {
      return INTER_PATCH;
    } else if ( patchMode == P_RAW ) {
      return RAW_PATCH;
    } else if ( patchMode == P_EOM ) {
      return EOM_PATCH;
    }
  } else if ( tileType == I_TILE ) {
    if ( patchMode == I_INTRA ) {
      return INTRA_PATCH;
    } else if ( patchMode == I_RAW ) {
      return RAW_PATCH;
    } else if ( patchMode == I_EOM ) {
      return EOM_PATCH;
    }
  }
  return ERROR_PATCH;
}

enum SeiPayloadType {
  BUFFERING_PERIOD                 = 0,   //  0: bufferingPeriod
  ATLAS_FRAME_TIMING               = 1,   //  1: atlasFrameTiming
  FILLER_PAYLOAD                   = 2,   //  2: fillerPayload
  USER_DATAREGISTERED_ITUTT35      = 3,   //  3: userDataRegisteredItuTT35
  USER_DATA_UNREGISTERED           = 4,   //  4: userDataUnregistered
  RECOVERY_POINT                   = 5,   //  5: recoveryPoint
  NO_DISPLAY                       = 6,   //  6: noDisplay
  TIME_CODE                        = 7,   //  7: timeCode
  REGIONAL_NESTING                 = 8,   //  8: regionalNesting
  SEI_MANIFEST                     = 9,   //  9: seiManifest
  SEI_PREFIX_INDICATION            = 10,  // 10: seiPrefixIndication
  ATTRIBUTE_TRANSFORMATION_PARAMS  = 11,  // 11: attributeTransformationParams
  ACTIVE_SUB_BITSTREAMS            = 12,  // 12: activeSubBitstreams
  COMPONENT_CODEC_MAPPING          = 13,  // 13: componentCodecMapping
  SCENE_OBJECT_INFORMATION         = 14,  // 14: scene object information m52705
  OBJECT_LABEL_INFORMATION         = 15,  // 15: Object label information
  PATCH_INFORMATION                = 16,  // 16: Patch information SEI message syntax
  VOLUMETRIC_RECTANGLE_INFORMATION = 17,  // 17: Volumetric rectangle information
  ATLAS_INFORMATION                = 18,  // 18: atlas_information
  VIEWPORT_CAMERA_PARAMETERS       = 19,  // 17: viewport camera parameters
  VIEWPORT_POSITION                = 20,  // 20: viewport position
  GEOMETRY_SMOOTHING               = 64,  // 17: geometry smoothing
  ATTRIBUTE_SMOOTHING              = 65,  // 20: attribute smoothing
  RESERVED_SEI_MESSAGE             = 127  // xx: reservedSeiMessage
};

enum NalUnitType {
  NAL_TRAIL_N = 0,      // 0 Coded tile of a non-TSA, non STSA trailing atlas frame ACL
  NAL_TRAIL_R,          // 1 Coded tile of a non-TSA, non STSA trailing atlas frame ACL
  NAL_TSA_N,            // 2 Coded tile of a TSA atlas frame ACL
  NAL_TSA_R,            // 3 Coded tile of a TSA atlas frame ACL
  NAL_STSA_N,           // 4 Coded tile of a STSA atlas frame ACL
  NAL_STSA_R,           // 5 Coded tile of a STSA atlas frame ACL
  NAL_RADL_N,           // 6 Coded tile of a RADL atlas frame ACL
  NAL_RADL_R,           // 7 Coded tile of a RADL atlas frame ACL
  NAL_RASL_N,           // 8 Coded tile of a RASL atlas frame ACL
  NAL_RASL_R,           // 9 Coded tile of a RASL atlas frame ACL
  NAL_SKIP_N,           // 10 Coded tile of a skipped atlas frame ACL
  NAL_SKIP_R,           // 11 Coded tile of a skipped atlas frame ACL
  NAL_RSV_ACL_N12,      // 12 Reserved non-IRAP sub-layer non-reference ACL NAL unit types ACL
  NAL_RSV_ACL_N14,      // 14 Reserved non-IRAP sub-layer non-reference ACL NAL unit types ACL
  NAL_RSV_ACL_R13,      // 13 Reserved non-IRAP sub-layer reference ACL NAL unit types ACL
  NAL_RSV_ACL_R15,      // 15 Reserved non-IRAP sub-layer reference ACL NAL unit types ACL
  NAL_BLA_W_LP,         // 16 Coded tile of a BLA atlas frame ACL
  NAL_BLA_W_RADL,       // 17 Coded tile of a BLA atlas frame ACL
  NAL_BLA_N_LP,         // 18 Coded tile of a BLA atlas frame ACL
  NAL_GBLA_W_LP,        // 19 Coded tile of a GBLA atlas frame ACL
  NAL_GBLA_W_RADL,      // 20 Coded tile of a GBLA atlas frame ACL
  NAL_GBLA_N_LP,        // 21 Coded tile of a GBLA atlas frame ACL
  NAL_IDR_W_RADL,       // 22 Coded tile of an IDR atlas frame ACL
  NAL_IDR_N_LP,         // 23 Coded tile of an IDR atlas frame ACL
  NAL_GIDR_W_RADL,      // 24 Coded tile of a GIDR atlas frame ACL
  NAL_GIDR_N_LP,        // 25 Coded tile of a GIDR atlas frame ACL
  NAL_CRA,              // 26 Coded tile of a CRA atlas frame ACL
  NAL_GCRA,             // 27 Coded tile of a GCRA atlas frame ACL
  NAL_RSV_IRAP_ACL_28,  // 28 Reserved IRAP ACL NAL unit types ACL
  NAL_RSV_IRAP_ACL_29,  // 29 Reserved IRAP ACL NAL unit types ACL
  NAL_RSV_ACL_30,       // 30 Reserved non-IRAP ACL NAL unit types ACL
  NAL_RSV_ACL_31,       // 31 Reserved non-IRAP ACL NAL unit types ACL
  NAL_RSV_ACL_32,       // 32 Reserved non-IRAP ACL NAL unit types ACL
  NAL_RSV_ACL_33,       // 33 Reserved non-IRAP ACL NAL unit types ACL
  NAL_RSV_ACL_34,       // 34 Reserved non-IRAP ACL NAL unit types ACL
  NAL_RSV_ACL_35,       // 35 Reserved non-IRAP ACL NAL unit types ACL
  NAL_ASPS,             // 36 Atlas sequence parameter set non-ACL
  NAL_AFPS,             // 37 Atlas frame parameter set non-ACL
  NAL_AUD,              // 38 Access unit delimiter non-ACL
  NAL_V3C_AUD,          // 39 V3C access unit delimiter non-ACL
  NAL_EOS,              // 40 End of sequence non-ACL
  NAL_EOB,              // 41 End of bitstream non-ACL
  NAL_FD,               // 42 Filler non-ACL
  NAL_PREFIX_NSEI,      // 43 Non-essential supplemental enhancement information non-ACL
  NAL_SUFFIX_NSEI,      // 44 Non-essential supplemental enhancement information non-ACL
  NAL_PREFIX_ESEI,      // 45 Essential supplemental enhancement information non-ACL
  NAL_SUFFIX_ESEI,      // 46 Essential supplemental enhancement information non-ACL
  NAL_AAPS,             // 47 Atlas adaptation parameter set non-ACL
  NAL_FOC,              // 48 Frame order count non-ACL
  NAL_RSV_NACL_49,      // 49 Reserved non-ACL NAL unit types non-ACL
  NAL_RSV_NACL_50,      // 50 Reserved non-ACL NAL unit types non-ACL
  NAL_RSV_NACL_51,      // 51 Reserved non-ACL NAL unit types non-ACL
  NAL_RSV_NACL_52,      // 52 Reserved non-ACL NAL unit types non-ACL
  NAL_UNSPEC_53,        // 53 Unspecified non-ACL NAL unit types non-ACL
  NAL_UNSPEC_54,        // 54 Unspecified non-ACL NAL unit types non-ACL
  NAL_UNSPEC_55,        // 55 Unspecified non-ACL NAL unit types non-ACL
  NAL_UNSPEC_56,        // 56 Unspecified non-ACL NAL unit types non-ACL
  NAL_UNSPEC_57,        // 57 Unspecified non-ACL NAL unit types non-ACL
  NAL_UNSPEC_58,        // 58 Unspecified non-ACL NAL unit types non-ACL
  NAL_UNSPEC_59,        // 59 Unspecified non-ACL NAL unit types non-ACL
  NAL_UNSPEC_60,        // 60 Unspecified non-ACL NAL unit types non-ACL
  NAL_UNSPEC_61,        // 61 Unspecified non-ACL NAL unit types non-ACL
  NAL_UNSPEC_62,        // 62 Unspecified non-ACL NAL unit types non-ACL
  NAL_UNSPEC_63         // 63 Unspecified non-ACL NAL unit types non-ACL
};

// ******************************************************************* //
// Static functions
// ******************************************************************* //
static inline std::string toString( NalUnitType type ) {
  switch ( type ) {
    case NAL_ASPS: return std::string( "NAL_ASPS" ); break;
    case NAL_AFPS: return std::string( "NAL_AFPS" ); break;
    case NAL_AUD: return std::string( "NAL_AUD" ); break;
    case NAL_TRAIL_N: return std::string( "NAL_TRAIL_N" ); break;
    case NAL_TRAIL_R: return std::string( "NAL_TRAIL_R" ); break;
    case NAL_TSA_N: return std::string( "NAL_TSA_N" ); break;
    case NAL_TSA_R: return std::string( "NAL_TSA_R" ); break;
    case NAL_STSA_N: return std::string( "NAL_STSA_N" ); break;
    case NAL_STSA_R: return std::string( "NAL_STSA_R" ); break;
    case NAL_RADL_N: return std::string( "NAL_RADL_N" ); break;
    case NAL_RADL_R: return std::string( "NAL_RADL_R" ); break;
    case NAL_RASL_N: return std::string( "NAL_RASL_N" ); break;
    case NAL_RASL_R: return std::string( "NAL_RASL_R" ); break;
    case NAL_SKIP_N: return std::string( "NAL_SKIP_N" ); break;
    case NAL_SKIP_R: return std::string( "NAL_SKIP_R" ); break;
    case NAL_PREFIX_ESEI: return std::string( "NAL_PREFIX_ESEI" ); break;
    case NAL_PREFIX_NSEI: return std::string( "NAL_PREFIX_NSEI" ); break;
    case NAL_SUFFIX_ESEI: return std::string( "NAL_SUFFIX_ESEI" ); break;
    case NAL_SUFFIX_NSEI: return std::string( "NAL_SUFFIX_NSEI" ); break;
    default: return std::string( "others" ); break;
  }
}

static inline std::string toString( V3CUnitType type ) {
  switch ( type ) {
    case V3C_VPS: return std::string( "V3C_VPS" ); break;
    case V3C_AD: return std::string( "V3C_AD" ); break;
    case V3C_OVD: return std::string( "V3C_OVD" ); break;
    case V3C_GVD: return std::string( "V3C_GVD" ); break;
    case V3C_AVD: return std::string( "V3C_AVD" ); break;
    default: return std::string( "reserved?" ); break;
  }
}
static inline std::string toString( PCCVideoType type ) {
  size_t typeIndex = (size_t)type;
  if ( typeIndex == (size_t)VIDEO_OCCUPANCY )
    return std::string( "occupancy map video " );
  else if ( typeIndex == (size_t)VIDEO_GEOMETRY )
    return std::string( "geometry video " );
  else if ( typeIndex >= (size_t)VIDEO_GEOMETRY_D0 && typeIndex <= (size_t)VIDEO_GEOMETRY_D15 )
    return std::string( "geometry D" ) + std::to_string( typeIndex - (size_t)VIDEO_GEOMETRY_D0 ) +
           std::string( " video " );
  else if ( typeIndex == (size_t)VIDEO_GEOMETRY_RAW )
    return std::string( "raw points geometry video " );
  else if ( typeIndex == (size_t)VIDEO_TEXTURE )
    return std::string( "texture video " );
  else if ( typeIndex >= (size_t)VIDEO_TEXTURE_T0 && typeIndex <= (size_t)VIDEO_TEXTURE_T15 )
    return std::string( "texture T" ) + std::to_string( typeIndex - (size_t)VIDEO_TEXTURE_T0 ) +
           std::string( " video " );
  else if ( typeIndex == (size_t)VIDEO_TEXTURE_RAW )
    return std::string( "raw points texture video " );
  else
    return std::string( "not supported" );
}
static bool exist( const std::string& sString ) {
  struct stat buffer;
  return ( stat( sString.c_str(), &buffer ) == 0 );
}

static inline void removeFile( const std::string string ) {
  if ( exist( string ) ) {
    if ( remove( string.c_str() ) != 0 ) { std::cout << "Could not remove the file: " << string << std::endl; }
  }
}

static inline std::string removeFileExtension( const std::string string ) {
  size_t pos = string.find_last_of( "." );
  return ( pos != std::string::npos && pos + 4 == string.length() ) ? string.substr( 0, pos ) : string;
}

static inline std::string addVideoFormat( const std::string filename,
                                          const size_t      width,
                                          const size_t      height,
                                          const bool        isYUV = true,
                                          const bool        is420 = true,
                                          const std::string pixel = "8" ) {
  std::stringstream result;
  result << filename << "_" << width << "x" << height << "_" << pixel << "bit_" << ( isYUV & is420 ? "p420" : "p444" )
         << ( isYUV ? ".yuv" : ".rgb" );
  return result.str();
}

template <typename T>
static inline const T PCCEndianSwap( const T u ) {
  union {
    T       u;
    uint8_t u8[sizeof( T )];
  } source, dest;
  source.u = u;
  for ( size_t k = 0; k < sizeof( T ); k++ ) dest.u8[k] = source.u8[sizeof( T ) - k - 1];
  return dest.u;
}

static inline uint32_t getFixedLengthCodeBitsCount( uint32_t range ) {
  int count = 0;
  if ( range > 0 ) {
    range -= 1;
    while ( range > 0 ) {
      count++;
      range >>= 1;
    }
  }
  return count;
}

static inline int floorLog2( uint32_t x ) {
  if ( x == 0 ) {
    // note: ceilLog2() expects -1 as return value
    return -1;
  }
#ifdef __GNUC__
  return 31 - __builtin_clz( x );
#else
#ifdef _MSC_VER
  unsigned long r = 0;
  _BitScanReverse( &r, x );
  return r;
#else
  int result = 0;
  if ( x & 0xffff0000 ) {
    x >>= 16;
    result += 16;
  }
  if ( x & 0xff00 ) {
    x >>= 8;
    result += 8;
  }
  if ( x & 0xf0 ) {
    x >>= 4;
    result += 4;
  }
  if ( x & 0xc ) {
    x >>= 2;
    result += 2;
  }
  if ( x & 0x2 ) {
    x >>= 1;
    result += 1;
  }
  return result;
#endif
#endif
}

static inline int ceilLog2( uint32_t x ) { return ( x == 0 ) ? -1 : floorLog2( x - 1 ) + 1; }
}  // namespace pcc

#endif /* PCC_BITSTREAM_COMMON_H */
