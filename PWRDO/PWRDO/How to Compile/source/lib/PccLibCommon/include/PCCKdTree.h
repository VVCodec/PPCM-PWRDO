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
#ifndef PCCKdTree_h
#define PCCKdTree_h

#include "PCCCommon.h"
#include "PCCPointSet.h"

namespace pcc {

struct PCCNNQuery3 {
  PCCPoint3D point;
  double     radius;
  size_t     nearestNeighborCount;
};

class PCCNNResult {
 public:
  PCCNNResult() : count_( 0 ) {}
  ~PCCNNResult() {
    indices_.clear();
    dist_.clear();
  }
  inline void resize( const size_t size ) {
    indices_.resize( size );
    dist_.resize( size );
  }
  inline size_t  size() const { return indices_.size(); }
  inline size_t& count() { return count_; }
  inline size_t& indices( size_t index ) { return indices_[index]; }
  inline double& dist( size_t index ) { return dist_[index]; }
  inline size_t* indices() { return indices_.data(); }
  inline double* dist() { return dist_.data(); }
  inline void    pop_dist() { dist_.pop_back(); };
  inline void    pop_indices() { indices_.pop_back(); };
  inline void    dec_count() { --count_; }

 private:
  size_t              count_;
  std::vector<size_t> indices_;
  std::vector<double> dist_;
};

class PCCKdTree {
 public:
  PCCKdTree();
  PCCKdTree( const PCCPointSet3& pointCloud );
  ~PCCKdTree();
  void init( const PCCPointSet3& pointCloud );
  void search( const PCCPoint3D& point, const size_t num_results, PCCNNResult& results ) const;
  void searchRadius( const PCCPoint3D& point,
                     const size_t      num_results,
                     const double      radius,
                     PCCNNResult&      results ) const;

 private:
  void  clear();
  void* kdtree_;
};

}  // namespace pcc
#endif /* PCCKdTree_h */
