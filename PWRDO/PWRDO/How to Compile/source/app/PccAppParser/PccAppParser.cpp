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
#include "PccAppParser.h"

using namespace std;
using namespace pcc;

int main( int argc, char* argv[] ) {
  std::cout << "PccAppParser v" << TMC2_VERSION_MAJOR << "." << TMC2_VERSION_MINOR << std::endl << std::endl;

  if ( argc != 2 ) { usage(); }
  if ( !exist( argv[1] ) ) {
    printf( "File %s not exist \n", argv[1] );
    usage();
  }
  std::string filename = argv[1];
  int         ret      = parserPccBin( filename );

  return ret;
}

void usage() {
  printf( "Usage: ./PccAppParser <filename> \n" );
  printf( "      <filename>: path to the compress PCC bitstream \n" );
  exit( -1 );
}

int parserPccBin( const std::string& filename ) {
  PCCBitstream     bitstream;
  PCCBitstreamStat bitstreamStat;
#ifdef BITSTREAM_TRACE
  bitstream.setTrace( true );
  bitstream.openTrace( removeFileExtension( filename ) + "_samplestream_read.txt" );
  bitstream.setTraceFile( bitstream.getTraceFile() );
#endif
  if ( !bitstream.initialize( filename ) ) { return -1; }
  bitstreamStat.setHeader( bitstream.size() );

  SampleStreamV3CUnit ssvu;
  PCCBitstreamReader  bitstreamReader;
  size_t              headerSize = pcc::PCCBitstreamReader::read( bitstream, ssvu );
  bitstreamStat.incrHeader( headerSize );

  bool    bMoreData = true;
  int32_t index     = 0;
  while ( bMoreData ) {
    PCCHighLevelSyntax syntax;
    syntax.setBitstreamStat( bitstreamStat );
    PCCBitstreamReader gofBitstreamReader;
    int                ret = gofBitstreamReader.decode( ssvu, syntax );
    if ( ret != 0 ) { return ret; }
    bMoreData = ( ssvu.getV3CUnitCount() > 0 );
  }
  bitstreamStat.trace();
  return 0;
}
