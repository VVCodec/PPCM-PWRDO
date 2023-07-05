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
#ifndef PCC_BITSTREAM_SEI_H
#define PCC_BITSTREAM_SEI_H

#include "PCCBitstreamCommon.h"

namespace pcc {

// Annex E: Supplemental enhancement information
// F.2.1	General SEI message syntax  <=> 7.3.8	Supplemental enhancement
// information message syntax
class SEI {
 public:
  SEI() { payloadSize_ = 0; }
  virtual ~SEI() {}
  virtual SeiPayloadType getPayloadType() = 0;
  size_t                 getPayloadSize() { return payloadSize_; }
  void                   setPayloadSize( size_t value ) { payloadSize_ = value; }

 private:
  size_t payloadSize_;
};

// F.2.3  User data registered by Recommendation ITU-T T.35 SEI message syntax
class SEIUserDataRegisteredItuTT35 : public SEI {
 public:
  SEIUserDataRegisteredItuTT35() : countryCode_( 0 ), countryCodeExtensionByte_( 0 ) {}

  ~SEIUserDataRegisteredItuTT35() { payloadByte_.clear(); }
  SEIUserDataRegisteredItuTT35& operator=( const SEIUserDataRegisteredItuTT35& ) = default;

  SeiPayloadType getPayloadType() { return USER_DATAREGISTERED_ITUTT35; }

  uint8_t               getCountryCode() { return countryCode_; }
  uint8_t               getCountryCodeExtensionByte() { return countryCodeExtensionByte_; }
  std::vector<uint8_t>& getPayloadByte() { return payloadByte_; }
  void                  setCountryCode( uint8_t value ) { countryCode_ = value; }
  void                  setCountryCodeExtensionByte( uint8_t value ) { countryCodeExtensionByte_ = value; }

 private:
  uint8_t              countryCode_;
  uint8_t              countryCodeExtensionByte_;
  std::vector<uint8_t> payloadByte_;
};

// F.2.4  User data unregistered SEI message syntax
class SEIUserDataUnregistered : public SEI {
 public:
  SEIUserDataUnregistered() {
    for ( size_t i = 0; i < 16; i++ ) { uuidIsoIec11578_[i] = 0; }
  }

  ~SEIUserDataUnregistered() { userDataPayloadByte_.clear(); }
  SEIUserDataUnregistered& operator=( const SEIUserDataUnregistered& ) = default;

  SeiPayloadType getPayloadType() { return USER_DATA_UNREGISTERED; }

  uint8_t*              getUuidIsoIec11578() { return uuidIsoIec11578_; }
  uint8_t               getUuidIsoIec11578( size_t i ) { return uuidIsoIec11578_[i]; }
  std::vector<uint8_t>& getUserDataPayloadByte() { return userDataPayloadByte_; }
  uint8_t&              getUserDataPayloadByte( size_t i ) { return userDataPayloadByte_[i]; }
  void                  setUuidIsoIec11578( size_t i, uint8_t value ) { uuidIsoIec11578_[i] = value; }
  void                  setUserDataPayloadByte( size_t i, uint8_t value ) { userDataPayloadByte_[i] = value; }

 private:
  uint8_t              uuidIsoIec11578_[16];
  std::vector<uint8_t> userDataPayloadByte_;
};

// F.2.5  Recovery point SEI message syntax
class SEIRecoveryPoint : public SEI {
 public:
  SEIRecoveryPoint() : recoveryAfocCnt_( 0 ), exactMatchFlag_( 0 ), brokenLinkFlag_( 0 ) {}

  ~SEIRecoveryPoint() {}
  SEIRecoveryPoint& operator=( const SEIRecoveryPoint& ) = default;

  SeiPayloadType getPayloadType() { return RECOVERY_POINT; }

  uint8_t getRecoveryAfocCnt() { return recoveryAfocCnt_; }
  uint8_t getExactMatchFlag() { return exactMatchFlag_; }
  uint8_t getBrokenLinkFlag() { return brokenLinkFlag_; }
  void    setRecoveryAfocCnt( uint8_t value ) { recoveryAfocCnt_ = value; }
  void    setExactMatchFlag( uint8_t value ) { exactMatchFlag_ = value; }
  void    setBrokenLinkFlag( uint8_t value ) { brokenLinkFlag_ = value; }

 private:
  int32_t recoveryAfocCnt_;
  uint8_t exactMatchFlag_;
  uint8_t brokenLinkFlag_;
};

// F.2.6  No display SEI message syntax
class SEINoDisplay : public SEI {
 public:
  SEINoDisplay() {}
  ~SEINoDisplay() {}
  SEINoDisplay& operator=( const SEINoDisplay& ) = default;

  SeiPayloadType getPayloadType() { return NO_DISPLAY; }

 private:
};

// F.2.7  Reserved SEI message syntax
class SEIReservedSeiMessage : public SEI {
 public:
  SEIReservedSeiMessage() {}
  ~SEIReservedSeiMessage() { payloadByte_.clear(); }
  SEIReservedSeiMessage& operator=( const SEIReservedSeiMessage& ) = default;

  SeiPayloadType getPayloadType() { return RESERVED_SEI_MESSAGE; }

  std::vector<uint8_t>& getPayloadByte() { return payloadByte_; }
  uint8_t               getPayloadByte( size_t i ) { return payloadByte_[i]; }

  void setPayloadByte( size_t i, uint8_t value ) { payloadByte_[i] = value; }

 private:
  std::vector<uint8_t> payloadByte_;
};

// F.2.8  SEI manifest SEI message syntax
class SEIManifest : public SEI {
 public:
  SEIManifest() : numSeiMsgTypes_( 0 ) {}
  ~SEIManifest() {
    seiPayloadType_.clear();
    seiDescription_.clear();
  }
  SEIManifest& operator=( const SEIManifest& ) = default;

  SeiPayloadType getPayloadType() { return SEI_MANIFEST; }

  void allocate() {
    seiPayloadType_.resize( numSeiMsgTypes_, 0 );
    seiDescription_.resize( numSeiMsgTypes_, 0 );
  }
  uint16_t               getNumSeiMsgTypes() { return numSeiMsgTypes_; }
  std::vector<uint16_t>& getSeiPayloadType() { return seiPayloadType_; }
  std::vector<uint8_t>&  getSeiDescription() { return seiDescription_; }
  uint16_t               getSeiPayloadType( size_t i ) { return seiPayloadType_[i]; }
  uint8_t                getSeiDescription( size_t i ) { return seiDescription_[i]; }
  void                   setNumSeiMsgTypes( uint16_t value ) { numSeiMsgTypes_ = value; }
  void                   setSeiPayloadType( size_t i, uint16_t value ) { seiPayloadType_[i] = value; }
  void                   setSeiDescription( size_t i, uint8_t value ) { seiDescription_[i] = value; }

 private:
  uint16_t              numSeiMsgTypes_;
  std::vector<uint16_t> seiPayloadType_;
  std::vector<uint8_t>  seiDescription_;
};

// F.2.9  SEI prefix indication SEI message syntax
class SEIPrefixIndication : public SEI {
 public:
  SEIPrefixIndication() : prefixSeiPayloadType_( 0 ), numSeiPrefixIndicationsMinus1_( 0 ) {}

  ~SEIPrefixIndication() {
    numBitsInPrefixIndicationMinus1_.clear();
    for ( auto& element : seiPrefixDataBit_ ) { element.clear(); }
    seiPrefixDataBit_.clear();
  }
  SEIPrefixIndication& operator=( const SEIPrefixIndication& ) = default;

  SeiPayloadType getPayloadType() { return SEI_PREFIX_INDICATION; }

  uint16_t                        getPrefixSeiPayloadType() { return prefixSeiPayloadType_; }
  uint8_t                         getNumSeiPrefixIndicationsMinus1() { return numSeiPrefixIndicationsMinus1_; }
  std::vector<uint16_t>&          getNumBitsInPrefixIndicationMinus1() { return numBitsInPrefixIndicationMinus1_; }
  std::vector<std::vector<bool>>& getSeiPrefixDataBit() { return seiPrefixDataBit_; }
  uint16_t          getNumBitsInPrefixIndicationMinus1( size_t i ) { return numBitsInPrefixIndicationMinus1_[i]; }
  std::vector<bool> getSeiPrefixDataBit( size_t i ) { return seiPrefixDataBit_[i]; }
  bool              getSeiPrefixDataBit( size_t i, size_t j ) { return seiPrefixDataBit_[i][j]; }

  void setPrefixSeiPayloadType( uint16_t value ) { prefixSeiPayloadType_ = value; }
  void setNumSeiPrefixIndicationsMinus1( uint8_t value ) { numSeiPrefixIndicationsMinus1_ = value; }
  void setNumBitsInPrefixIndicationMinus1( size_t i, uint16_t value ) { numBitsInPrefixIndicationMinus1_[i] = value; }
  void setSeiPrefixDataBit( size_t i, size_t j, bool value ) { seiPrefixDataBit_[i][j] = value; }

 private:
  uint16_t                       prefixSeiPayloadType_;
  uint8_t                        numSeiPrefixIndicationsMinus1_;
  std::vector<uint16_t>          numBitsInPrefixIndicationMinus1_;
  std::vector<std::vector<bool>> seiPrefixDataBit_;
};

// F.2.10  Attribute transformation parameters SEI message syntax
class SEIAttributeTransformationParams : public SEI {
 public:
  SEIAttributeTransformationParams() : cancelFlag_( false ), numAttributeUpdates_( 0 ), persistenceFlag_( false ) {}

  ~SEIAttributeTransformationParams() {
    attributeIdx_.clear();
    dimensionMinus1_.clear();
    for ( auto& element : scaleParamsEnabledFlag_ ) { element.clear(); }
    for ( auto& element : offsetParamsEnabledFlag_ ) { element.clear(); }
    for ( auto& element : attributeScale_ ) { element.clear(); }
    for ( auto& element : attributeOffset_ ) { element.clear(); }
    scaleParamsEnabledFlag_.clear();
    offsetParamsEnabledFlag_.clear();
    attributeScale_.clear();
    attributeOffset_.clear();
  }
  SEIAttributeTransformationParams& operator=( const SEIAttributeTransformationParams& ) = default;

  SeiPayloadType getPayloadType() { return ATTRIBUTE_TRANSFORMATION_PARAMS; }
  void           allocate() {
    attributeIdx_.resize( numAttributeUpdates_ + 1, 0 );
    dimensionMinus1_.resize( 256 + 1, 0 );
    scaleParamsEnabledFlag_.resize( 256 );
    offsetParamsEnabledFlag_.resize( 256 );
    attributeScale_.resize( 256 );
    attributeOffset_.resize( 256 );
  }
  void allocate( size_t i ) {
    scaleParamsEnabledFlag_[i].resize( dimensionMinus1_[i] + 1, false );
    offsetParamsEnabledFlag_[i].resize( dimensionMinus1_[i] + 1, false );
    attributeScale_[i].resize( dimensionMinus1_[i] + 1, 0 );
    attributeOffset_[i].resize( dimensionMinus1_[i] + 1, 0 );
  }
  bool                                getCancelFlag() { return cancelFlag_; }
  int32_t                             getNumAttributeUpdates() { return numAttributeUpdates_; }
  std::vector<uint8_t>&               AttributeIdx() { return attributeIdx_; }
  std::vector<uint8_t>&               getDimensionMinus() { return dimensionMinus1_; }
  std::vector<std::vector<bool>>&     getScaleParamsEnabledFlag() { return scaleParamsEnabledFlag_; }
  std::vector<std::vector<bool>>&     getOffsetParamsEnabledFlag() { return offsetParamsEnabledFlag_; }
  std::vector<std::vector<uint32_t>>& getAttributeScale() { return attributeScale_; }
  std::vector<std::vector<int32_t>>&  getAttributeOffset() { return attributeOffset_; }
  bool                                getPersistenceFlag() { return persistenceFlag_; }
  uint8_t                             getAttributeIdx( size_t i ) { return attributeIdx_[i]; }
  uint8_t                             getDimensionMinus1( size_t i ) { return dimensionMinus1_[i]; }
  bool     getScaleParamsEnabledFlag( size_t i, size_t j ) { return scaleParamsEnabledFlag_[i][j]; }
  bool     getOffsetParamsEnabledFlag( size_t i, size_t j ) { return offsetParamsEnabledFlag_[i][j]; }
  uint32_t getAttributeScale( size_t i, size_t j ) { return attributeScale_[i][j]; }
  int32_t  getAttributeOffset( size_t i, size_t j ) { return attributeOffset_[i][j]; }

  void setCancelFlag( bool value ) { cancelFlag_ = value; }
  void setNumAttributeUpdates( int32_t value ) { numAttributeUpdates_ = value; }
  void setAttributeIdx( size_t i, uint8_t value ) { attributeIdx_[i] = value; }
  void setDimensionMinus1( size_t i, uint8_t value ) { dimensionMinus1_[i] = value; }
  void setScaleParamsEnabledFlag( size_t i, size_t j, bool value ) { scaleParamsEnabledFlag_[i][j] = value; }
  void setOffsetParamsEnabledFlag( size_t i, size_t j, bool value ) { offsetParamsEnabledFlag_[i][j] = value; }
  void setAttributeScale( size_t i, size_t j, uint32_t value ) { attributeScale_[i][j] = value; }
  void setAttributeOffset( size_t i, size_t j, int32_t value ) { attributeOffset_[i][j] = value; }
  void setPersistenceFlag( bool value ) { persistenceFlag_ = value; }

 private:
  bool                               cancelFlag_;
  int32_t                            numAttributeUpdates_;
  std::vector<uint8_t>               attributeIdx_;
  std::vector<uint8_t>               dimensionMinus1_;
  std::vector<std::vector<bool>>     scaleParamsEnabledFlag_;
  std::vector<std::vector<bool>>     offsetParamsEnabledFlag_;
  std::vector<std::vector<uint32_t>> attributeScale_;
  std::vector<std::vector<int32_t>>  attributeOffset_;
  bool                               persistenceFlag_;
};

// F.2.11  Active substreams SEI message syntax
class SEIActiveSubBitstreams : public SEI {
 public:
  SEIActiveSubBitstreams() :
      activeAttributesChangesFlag_( 0 ),
      activeMapsChangesFlag_( 0 ),
      auxiliaryPointsSubstreamsActiveFlag_( 0 ),
      allAttributesActiveFlag_( 0 ),
      allMapsActiveFlag_( 0 ),
      activeAttributeCountMinus1_( 0 ),
      activeMapCountMinus1_( 0 ) {}
  ~SEIActiveSubBitstreams() {
    activeAttributeIdx_.clear();
    activeMapIdx_.clear();
  }
  SEIActiveSubBitstreams& operator=( const SEIActiveSubBitstreams& ) = default;

  SeiPayloadType        getPayloadType() { return ACTIVE_SUB_BITSTREAMS; }
  bool                  getActiveAttributesChangesFlag() { return activeAttributesChangesFlag_; }
  bool                  getActiveMapsChangesFlag() { return activeMapsChangesFlag_; }
  bool                  getAuxiliaryPointsSubstreamsActiveFlag() { return auxiliaryPointsSubstreamsActiveFlag_; }
  bool                  getAllAttributesActiveFlag() { return allAttributesActiveFlag_; }
  bool                  getAllMapsActiveFlag() { return allMapsActiveFlag_; }
  uint8_t               getActiveAttributeCountMinus1() { return activeAttributeCountMinus1_; }
  uint8_t               getActiveMapCountMinus1() { return activeMapCountMinus1_; }
  std::vector<uint8_t>& getActiveAttributeIdx() { return activeAttributeIdx_; }
  std::vector<uint8_t>& getActiveMapIdx() { return activeMapIdx_; }
  uint8_t               getActiveAttributeIdx( size_t i ) { return activeAttributeIdx_[i]; }
  uint8_t               getActiveMapIdx( size_t i ) { return activeMapIdx_[i]; }

  void setActiveAttributesChangesFlag( bool value ) { activeAttributesChangesFlag_ = value; }
  void setActiveMapsChangesFlag( bool value ) { activeMapsChangesFlag_ = value; }
  void setAuxiliaryPointsSubstreamsActiveFlag( bool value ) { auxiliaryPointsSubstreamsActiveFlag_ = value; }
  void setAllAttributesActiveFlag( bool value ) { allAttributesActiveFlag_ = value; }
  void setAllMapsActiveFlag( bool value ) { allMapsActiveFlag_ = value; }
  void setActiveAttributeCountMinus1( uint8_t value ) { activeAttributeCountMinus1_ = value; }
  void setActiveMapCountMinus1( uint8_t value ) { activeMapCountMinus1_ = value; }
  void setActiveAttributeIdx( size_t i, uint8_t value ) { activeAttributeIdx_[i] = value; }
  void setActiveMapIdx( size_t i, uint8_t value ) { activeMapIdx_[i] = value; }

 private:
  bool                 activeAttributesChangesFlag_;
  bool                 activeMapsChangesFlag_;
  bool                 auxiliaryPointsSubstreamsActiveFlag_;
  bool                 allAttributesActiveFlag_;
  bool                 allMapsActiveFlag_;
  uint8_t              activeAttributeCountMinus1_;
  uint8_t              activeMapCountMinus1_;
  std::vector<uint8_t> activeAttributeIdx_;
  std::vector<uint8_t> activeMapIdx_;
};

// F.2.12  Component codec mapping SEI message syntax
class SEIComponentCodecMapping : public SEI {
 public:
  SEIComponentCodecMapping() : codecMappingsCountMinus1_( 0 ) {}
  ~SEIComponentCodecMapping() {
    codecId_.clear();
    codec4cc_.clear();
  }
  SEIComponentCodecMapping& operator=( const SEIComponentCodecMapping& ) = default;

  SeiPayloadType getPayloadType() { return COMPONENT_CODEC_MAPPING; }

  void allocate() {
    codecId_.resize( codecMappingsCountMinus1_ + 1, 0 );
    codec4cc_.resize( 256 );
  }
  uint8_t     getCodecMappingsCountMinus1() { return codecMappingsCountMinus1_; }
  uint8_t     getCodecId( size_t i ) { return codecId_[i]; }
  std::string getCodec4cc( size_t i ) { return codec4cc_[i]; }

  void setCodecMappingsCountMinus1( uint8_t value ) { codecMappingsCountMinus1_ = value; }
  void setCodecId( size_t i, uint8_t value ) { codecId_[i] = value; }
  void setCodec4cc( size_t i, std::string value ) { codec4cc_[i] = value; }

 private:
  uint8_t                  codecMappingsCountMinus1_;
  std::vector<uint8_t>     codecId_;
  std::vector<std::string> codec4cc_;
};

// F.2.14  Volumetric Tiling SEI message syntax
// m52705
class SEISceneObjectInformation : public SEI {
 public:
  SEISceneObjectInformation() {}
  ~SEISceneObjectInformation() {}
  SEISceneObjectInformation& operator=( const SEISceneObjectInformation& ) = default;
  void                       allocateObjectIdx() { objectIdx_.resize( numObjectUpdates_ ); }
  void                       allocateObjectNumDependencies( size_t i, size_t objectNumDependencies ) {
    objectDependencyIdx_[i].resize( objectNumDependencies );
  }
  void allocate( size_t size ) {
    if ( objectCancelFlag_.size() < size ) {
      objectCancelFlag_.resize( size );
      objectLabelUpdateFlag_.resize( size );
      objectLabelIdx_.resize( size );
      priorityUpdateFlag_.resize( size );
      priorityValue_.resize( size );
      objectHiddenFlag_.resize( size );
      objectDependencyUpdateFlag_.resize( size );
      objectNumDependencies_.resize( size );
      visibilityConesUpdateFlag_.resize( size );
      directionX_.resize( size );
      directionY_.resize( size );
      directionZ_.resize( size );
      angle_.resize( size );
      boundingBoxUpdateFlag_.resize( size );
      boundingBoxX_.resize( size );
      boundingBoxY_.resize( size );
      boundingBoxZ_.resize( size );
      boundingBoxDeltaX_.resize( size );
      boundingBoxDeltaY_.resize( size );
      boundingBoxDeltaZ_.resize( size );
      collisionShapeUpdateFlag_.resize( size );
      collisionShapeId_.resize( size );
      pointStyleUpdateFlag_.resize( size );
      pointShapeId_.resize( size );
      pointSize_.resize( size );
      materialIdUpdateFlag_.resize( size );
      materialId_.resize( size );
      objectDependencyIdx_.resize( size );  // 16
    }
  }

  SeiPayloadType         getPayloadType() { return SCENE_OBJECT_INFORMATION; }
  bool                   getCancelFlag() { return cancelFlag_; }
  uint32_t               getNumObjectUpdates() { return numObjectUpdates_; }
  bool                   getSimpleObjectsFlag() { return soiSimpleObjectsFlag_; }
  bool                   getObjectLabelPresentFlag() { return objectLabelPresentFlag_; }
  bool                   getPriorityPresentFlag() { return priorityPresentFlag_; }
  bool                   getObjectHiddenPresentFlag() { return objectHiddenPresentFlag_; }
  bool                   getObjectDependencyPresentFlag() { return objectDependencyPresentFlag_; }
  bool                   getVisibilityConesPresentFlag() { return visibilityConesPresentFlag_; }
  bool                   get3dBoundingBoxPresentFlag() { return boundingBoxPresentFlag_; }
  bool                   getCollisionShapePresentFlag() { return collisionShapePresentFlag_; }
  bool                   getPointStylePresentFlag() { return pointStylePresentFlag_; }
  bool                   getMaterialIdPresentFlag() { return materialIdPresentFlag_; }
  bool                   getExtensionPresentFlag() { return extensionPresentFlag_; }
  uint8_t                get3dBoundingBoxScaleLog2() { return boundingBoxScaleLog2_; }
  uint8_t                get3dBoundingBoxPrecisionMinus8() { return boundingBoxPrecisionMinus8_; }
  uint8_t                getLog2MaxObjectIdxUpdated() { return log2MaxObjectIdxUpdated_; }
  uint8_t                getLog2MaxObjectDependencyIdx() { return log2MaxObjectDependencyIdx_; }
  std::vector<uint32_t>& getObjectIdx() { return objectIdx_; }
  uint32_t               getObjectIdx( size_t i ) { return objectIdx_[i]; }
  bool                   getObjectCancelFlag( size_t i ) { return objectCancelFlag_[i]; }
  bool                   getObjectLabelUpdateFlag( size_t i ) { return objectLabelUpdateFlag_[i]; }
  uint32_t               getObjectLabelIdx( size_t i ) { return objectLabelIdx_[i]; }
  bool                   getPriorityUpdateFlag( size_t i ) { return priorityUpdateFlag_[i]; }
  uint8_t                getPriorityValue( size_t i ) { return priorityValue_[i]; }
  bool                   getObjectHiddenFlag( size_t i ) { return objectHiddenFlag_[i]; }
  bool                   getObjectDependencyUpdateFlag( size_t i ) { return objectDependencyUpdateFlag_[i]; }
  uint8_t                getObjectNumDependencies( size_t i ) { return objectNumDependencies_[i]; }
  uint32_t               getObjectDependencyIdx( size_t i, size_t objnum ) { return objectDependencyIdx_[i][objnum]; }
  bool                   getVisibilityConesUpdateFlag( size_t i ) { return visibilityConesUpdateFlag_[i]; }
  uint32_t               getDirectionX( size_t i ) { return directionX_[i]; }
  uint32_t               getDirectionY( size_t i ) { return directionY_[i]; }
  uint32_t               getDirectionZ( size_t i ) { return directionZ_[i]; }
  uint16_t               getAngle( size_t i ) { return angle_[i]; }
  bool                   get3dBoundingBoxUpdateFlag( size_t i ) { return boundingBoxUpdateFlag_[i]; }
  uint32_t               get3dBoundingBoxX( size_t i ) { return boundingBoxX_[i]; }
  uint32_t               get3dBoundingBoxY( size_t i ) { return boundingBoxY_[i]; }
  uint32_t               get3dBoundingBoxZ( size_t i ) { return boundingBoxZ_[i]; }
  uint32_t               get3dBoundingBoxDeltaX( size_t i ) { return boundingBoxDeltaX_[i]; }
  uint32_t               get3dBoundingBoxDeltaY( size_t i ) { return boundingBoxDeltaY_[i]; }
  uint32_t               get3dBoundingBoxDeltaZ( size_t i ) { return boundingBoxDeltaZ_[i]; }
  bool                   getCollisionShapeUpdateFlag( size_t i ) { return collisionShapeUpdateFlag_[i]; }
  uint16_t               getCollisionShapeId( size_t i ) { return collisionShapeId_[i]; }
  bool                   getPointStyleUpdateFlag( size_t i ) { return pointStyleUpdateFlag_[i]; }
  uint8_t                getPointShapeId( size_t i ) { return pointShapeId_[i]; }
  uint16_t               getPointSize( size_t i ) { return pointSize_[i]; }
  bool                   getMaterialIdUpdateFlag( size_t i ) { return materialIdUpdateFlag_[i]; }
  uint16_t               getMaterialId( size_t i ) { return materialId_[i]; }

  void setCancelFlag( bool value ) { cancelFlag_ = value; }
  void setNumObjectUpdates( uint32_t value ) { numObjectUpdates_ = value; }
  void setSimpleObjectsFlag( bool value ) { soiSimpleObjectsFlag_ = value; }
  void setObjectLabelPresentFlag( bool value ) { objectLabelPresentFlag_ = value; }
  void setPriorityPresentFlag( bool value ) { priorityPresentFlag_ = value; }
  void setObjectHiddenPresentFlag( bool value ) { objectHiddenPresentFlag_ = value; }
  void setObjectDependencyPresentFlag( bool value ) { objectDependencyPresentFlag_ = value; }
  void setVisibilityConesPresentFlag( bool value ) { visibilityConesPresentFlag_ = value; }
  void set3dBoundingBoxPresentFlag( bool value ) { boundingBoxPresentFlag_ = value; }
  void setCollisionShapePresentFlag( bool value ) { collisionShapePresentFlag_ = value; }
  void setPointStylePresentFlag( bool value ) { pointStylePresentFlag_ = value; }
  void setMaterialIdPresentFlag( bool value ) { materialIdPresentFlag_ = value; }
  void setExtensionPresentFlag( bool value ) { extensionPresentFlag_ = value; }
  void set3dBoundingBoxScaleLog2( uint8_t value ) { boundingBoxScaleLog2_ = value; }
  void set3dBoundingBoxPrecisionMinus8( uint8_t value ) { boundingBoxPrecisionMinus8_ = value; }
  void setLog2MaxObjectIdxUpdated( uint8_t value ) { log2MaxObjectIdxUpdated_ = value; }
  void setLog2MaxObjectDependencyIdx( uint8_t value ) { log2MaxObjectDependencyIdx_ = value; }
  void setObjectIdx( size_t i, uint32_t value ) { objectIdx_[i] = value; }
  void setObjectCancelFlag( size_t i, bool value ) { objectCancelFlag_[i] = value; }
  void setObjectLabelUpdateFlag( size_t i, bool value ) { objectLabelUpdateFlag_[i] = value; }
  void setObjectLabelIdx( size_t i, uint32_t value ) { objectLabelIdx_[i] = value; }
  void setPriorityUpdateFlag( size_t i, bool value ) { priorityUpdateFlag_[i] = value; }
  void setPriorityValue( size_t i, uint8_t value ) { priorityValue_[i] = value; }
  void setObjectHiddenFlag( size_t i, bool value ) { objectHiddenFlag_[i] = value; }
  void setObjectDependencyUpdateFlag( size_t i, bool value ) { objectDependencyUpdateFlag_[i] = value; }
  void setObjectNumDependencies( size_t i, uint8_t value ) { objectNumDependencies_[i] = value; }
  void setObjectDependencyIdx( size_t i, size_t objnum, uint32_t value ) { objectDependencyIdx_[i][objnum] = value; }
  void setVisibilityConesUpdateFlag( size_t i, bool value ) { visibilityConesUpdateFlag_[i] = value; }
  void setDirectionX( size_t i, uint32_t value ) { directionX_[i] = value; }
  void setDirectionY( size_t i, uint32_t value ) { directionY_[i] = value; }
  void setDirectionZ( size_t i, uint32_t value ) { directionZ_[i] = value; }
  void setAngle( size_t i, uint16_t value ) { angle_[i] = value; }
  void set3dBoundingBoxUpdateFlag( size_t i, bool value ) { boundingBoxUpdateFlag_[i] = value; }
  void set3dBoundingBoxX( size_t i, uint32_t value ) { boundingBoxX_[i] = value; }
  void set3dBoundingBoxY( size_t i, uint32_t value ) { boundingBoxY_[i] = value; }
  void set3dBoundingBoxZ( size_t i, uint32_t value ) { boundingBoxZ_[i] = value; }
  void set3dBoundingBoxDeltaX( size_t i, uint32_t value ) { boundingBoxDeltaX_[i] = value; }
  void set3dBoundingBoxDeltaY( size_t i, uint32_t value ) { boundingBoxDeltaY_[i] = value; }
  void set3dBoundingBoxDeltaZ( size_t i, uint32_t value ) { boundingBoxDeltaZ_[i] = value; }
  void setCollisionShapeUpdateFlag( size_t i, bool value ) { collisionShapeUpdateFlag_[i] = value; }
  void setCollisionShapeId( size_t i, uint16_t value ) { collisionShapeId_[i] = value; }
  void setPointStyleUpdateFlag( size_t i, bool value ) { pointStyleUpdateFlag_[i] = value; }
  void setPointShapeId( size_t i, uint8_t value ) { pointShapeId_[i] = value; }
  void setPointSize( size_t i, uint16_t value ) { pointSize_[i] = value; }
  void setMaterialIdUpdateFlag( size_t i, bool value ) { materialIdUpdateFlag_[i] = value; }
  void setMaterialId( size_t i, uint16_t value ) { materialId_[i] = value; }

 private:
  bool                               cancelFlag_;
  uint32_t                           numObjectUpdates_;
  bool                               soiSimpleObjectsFlag_;
  bool                               objectLabelPresentFlag_;
  bool                               priorityPresentFlag_;
  bool                               objectHiddenPresentFlag_;
  bool                               objectDependencyPresentFlag_;
  bool                               visibilityConesPresentFlag_;
  bool                               boundingBoxPresentFlag_;
  bool                               collisionShapePresentFlag_;
  bool                               pointStylePresentFlag_;
  bool                               materialIdPresentFlag_;
  bool                               extensionPresentFlag_;
  uint8_t                            boundingBoxScaleLog2_;
  uint8_t                            boundingBoxPrecisionMinus8_;
  uint8_t                            log2MaxObjectIdxUpdated_;
  uint8_t                            log2MaxObjectDependencyIdx_;
  std::vector<uint32_t>              objectIdx_;
  std::vector<bool>                  objectCancelFlag_;
  std::vector<bool>                  objectLabelUpdateFlag_;
  std::vector<uint32_t>              objectLabelIdx_;
  std::vector<bool>                  priorityUpdateFlag_;
  std::vector<uint8_t>               priorityValue_;
  std::vector<bool>                  objectHiddenFlag_;
  std::vector<bool>                  objectDependencyUpdateFlag_;
  std::vector<uint8_t>               objectNumDependencies_;
  std::vector<std::vector<uint32_t>> objectDependencyIdx_;  // 16
  std::vector<bool>                  visibilityConesUpdateFlag_;
  std::vector<uint32_t>              directionX_;
  std::vector<uint32_t>              directionY_;
  std::vector<uint32_t>              directionZ_;
  std::vector<uint16_t>              angle_;
  std::vector<bool>                  boundingBoxUpdateFlag_;
  std::vector<uint32_t>              boundingBoxX_;
  std::vector<uint32_t>              boundingBoxY_;
  std::vector<uint32_t>              boundingBoxZ_;
  std::vector<uint32_t>              boundingBoxDeltaX_;
  std::vector<uint32_t>              boundingBoxDeltaY_;
  std::vector<uint32_t>              boundingBoxDeltaZ_;
  std::vector<bool>                  collisionShapeUpdateFlag_;
  std::vector<uint16_t>              collisionShapeId_;
  std::vector<bool>                  pointStyleUpdateFlag_;
  std::vector<uint8_t>               pointShapeId_;
  std::vector<uint16_t>              pointSize_;
  std::vector<bool>                  materialIdUpdateFlag_;
  std::vector<uint16_t>              materialId_;

};  // sceneobjectinformation

class SEIObjectLabelInformation : public SEI {
 public:
  SEIObjectLabelInformation() {}
  ~SEIObjectLabelInformation() {}
  SEIObjectLabelInformation& operator=( const SEIObjectLabelInformation& ) = default;

  SeiPayloadType getPayloadType() { return OBJECT_LABEL_INFORMATION; }
  void           allocate() { labelIdx_.resize( numLabelUpdates_, 0 ); }
  bool           getCancelFlag() { return cancelFlag_; }
  bool           getLabelLanguagePresentFlag() { return labelLanguagePresentFlag_; }
  std::string&   getLabelLanguage() { return labelLanguage_; }
  uint32_t       getNumLabelUpdates() { return numLabelUpdates_; }
  uint32_t       getLabelIdx( size_t i ) { return labelIdx_[i]; }
  bool           getLabelCancelFlag() { return labelCancelFlag_; }
  bool           getBitEqualToZero() { return bitEqualToZero_; }
  std::string&   getLabel( size_t i ) { return label_[i]; }
  bool           getPersistenceFlag() { return persistenceFlag_; }

  void setCancelFlag( bool value ) { cancelFlag_ = value; }
  void setLabelLanguagePresentFlag( bool value ) { labelLanguagePresentFlag_ = value; }
  void setLabelLanguage( std::string value ) { labelLanguage_ = value; }
  void setNumLabelUpdates( uint32_t value ) { numLabelUpdates_ = value; }
  void setLabelIdx( size_t i, uint32_t value ) { labelIdx_[i] = value; }
  void setLabelCancelFlag( bool value ) { labelCancelFlag_ = value; }
  void setBitEqualToZero( bool value ) { bitEqualToZero_ = value; }
  void setLabel( size_t i, std::string value ) {
    if ( label_.size() < i ) { label_.resize( i + 1 ); }
    label_[i] = value;
  }
  void setPersistenceFlag( bool value ) { persistenceFlag_ = value; }

 private:
  bool                     cancelFlag_;
  bool                     labelLanguagePresentFlag_;
  std::string              labelLanguage_;  // st(v)
  uint32_t                 numLabelUpdates_;
  std::vector<uint32_t>    labelIdx_;
  bool                     labelCancelFlag_;
  bool                     bitEqualToZero_;
  std::vector<std::string> label_;  // st(v)
  bool                     persistenceFlag_;
};  // Object label information

class SEIPatchInformation : public SEI {
 public:
  SEIPatchInformation() {}
  ~SEIPatchInformation() {}
  SEIPatchInformation& operator=( const SEIPatchInformation& ) = default;
  SeiPayloadType       getPayloadType() { return PATCH_INFORMATION; }
  bool                 getPersistenceFlag() { return persistenceFlag_; }
  bool                 getResetFlag() { return resetFlag_; }
  uint32_t             getNumTileUpdates() { return numTileUpdates_; }
  uint8_t              getLog2MaxObjectIdxTracked() { return log2MaxObjectIdxTracked_; }
  uint8_t              getLog2MaxPatchIdxUpdated() { return log2MaxPatchIdxUpdated_; }
  uint32_t             getTileAddress( size_t i ) { return tileAddress_[i]; }
  bool                 getTileCancelFlag( size_t i ) { return tileCancelFlag_[i]; }
  uint32_t             getNumPatchUpdates( size_t i ) { return numPatchUpdates_[i]; }
  uint32_t             getPatchIdx( size_t i, size_t j ) { return patchIdx_[i][j]; }
  bool                 getPatchCancelFlag( size_t i, size_t j ) { return patchCancelFlag_[i][j]; }
  uint32_t             getPatchNumberOfObjectsMinus1( size_t i, size_t j ) { return patchNumberOfObjectsMinus1_[i][j]; }
  uint32_t             getPatchObjectIdx( size_t i, size_t j, size_t k ) { return patchObjectIdx_[i][j][k]; }
  void                 setPersistenceFlag( bool value ) { persistenceFlag_ = value; }
  void                 setResetFlag( bool value ) { resetFlag_ = value; }
  void                 setNumTileUpdates( uint32_t value ) { numTileUpdates_ = value; }
  void                 setLog2MaxObjectIdxTracked( uint8_t value ) { log2MaxObjectIdxTracked_ = value; }
  void                 setLog2MaxPatchIdxUpdated( uint8_t value ) { log2MaxPatchIdxUpdated_ = value; }
  void                 setTileAddress( size_t i, uint32_t value ) { tileAddress_[i] = value; }
  void                 setTileCancelFlag( size_t i, bool value ) { tileCancelFlag_[i] = value; }
  void                 setNumPatchUpdates( size_t i, uint32_t value ) { numPatchUpdates_[i] = value; }
  void                 setPatchIdx( size_t i, size_t j, uint32_t value ) { patchIdx_[i][j] = value; }
  void                 setPatchCancelFlag( size_t i, size_t j, bool value ) { patchCancelFlag_[i][j] = value; }
  void                 setPatchNumberOfObjectsMinus1( size_t i, size_t j, uint32_t value ) {
    patchNumberOfObjectsMinus1_[i][j] = value;
  }
  void setPatchObjectIdx( size_t i, size_t j, size_t index3, uint32_t value ) { patchObjectIdx_[i][j][index3] = value; }

 private:
  bool                                            persistenceFlag_;
  bool                                            resetFlag_;
  uint32_t                                        numTileUpdates_;
  uint8_t                                         log2MaxObjectIdxTracked_;
  uint8_t                                         log2MaxPatchIdxUpdated_;
  std::vector<uint32_t>                           tileAddress_;
  std::vector<bool>                               tileCancelFlag_;
  std::vector<uint32_t>                           numPatchUpdates_;
  std::vector<std::vector<uint32_t>>              patchIdx_;
  std::vector<std::vector<bool>>                  patchCancelFlag_;
  std::vector<std::vector<uint32_t>>              patchNumberOfObjectsMinus1_;
  std::vector<std::vector<std::vector<uint32_t>>> patchObjectIdx_;
};

class SEIVolumetricRectangleInformation : public SEI {
 public:
  SEIVolumetricRectangleInformation() {}
  ~SEIVolumetricRectangleInformation() {}
  SEIVolumetricRectangleInformation& operator=( const SEIVolumetricRectangleInformation& ) = default;
  void                               allocate( size_t size ) {
    boundingBoxUpdateFlag_.resize( size );
    boundingBoxTop_.resize( size );
    boundingBoxLeft_.resize( size );
    boundingBoxWidth_.resize( size );
    boundingBoxHeight_.resize( size );
    rectangleObjectIdx_.resize( size );
  }
  void allocateRectangleObjectIdx( size_t i, size_t size ) { rectangleObjectIdx_[i].resize( size ); }

  SeiPayloadType getPayloadType() { return VOLUMETRIC_RECTANGLE_INFORMATION; }
  bool           getPersistenceFlag() { return persistenceFlag_; }
  bool           getResetFlag() { return resetFlag_; }
  uint32_t       getNumRectanglesUpdates() { return numRectanglesUpdates_; }
  uint8_t        getLog2MaxObjectIdxTracked() { return log2MaxObjectIdxTracked_; }
  uint8_t        getLog2MaxRectangleIdxUpdated() { return log2MaxRectangleIdxUpdated_; }
  uint32_t       getRectangleIdx( size_t i ) { return rectangleIdx_[i]; }
  bool           getRectangleCancelFlag( size_t i ) { return rectangleCancelFlag_[i]; }
  bool           getBoundingBoxUpdateFlag( size_t i ) { return boundingBoxUpdateFlag_[i]; }
  uint32_t       getBoundingBoxTop( size_t i ) { return boundingBoxTop_[i]; }
  uint32_t       getBoundingBoxLeft( size_t i ) { return boundingBoxLeft_[i]; }
  uint32_t       getBoundingBoxWidth( size_t i ) { return boundingBoxWidth_[i]; }
  uint32_t       getBoundingBoxHeight( size_t i ) { return boundingBoxHeight_[i]; }
  uint32_t       getRectangleNumberOfObjectsMinus1( size_t i ) { return rectangleNumberOfObjectsMinus1_[i]; }
  uint32_t       getRectangleObjectIdx( size_t i, size_t j ) { return rectangleObjectIdx_[i][j]; }

  void setPersistenceFlag( bool value ) { persistenceFlag_ = value; }
  void setResetFlag( bool value ) { resetFlag_ = value; }
  void setNumRectanglesUpdates( uint32_t value ) { numRectanglesUpdates_ = value; }
  void setLog2MaxObjectIdxTracked( uint8_t value ) { log2MaxObjectIdxTracked_ = value; }
  void setLog2MaxRectangleIdxUpdated( uint8_t value ) { log2MaxRectangleIdxUpdated_ = value; }
  void setRectangleIdx( size_t i, uint32_t value ) { rectangleIdx_[i] = value; }
  void setRectangleCancelFlag( size_t i, bool value ) { rectangleCancelFlag_[i] = value; }
  void setBoundingBoxUpdateFlag( size_t i, bool value ) { boundingBoxUpdateFlag_[i] = value; }
  void setBoundingBoxTop( size_t i, uint32_t value ) { boundingBoxTop_[i] = value; }
  void setBoundingBoxLeft( size_t i, uint32_t value ) { boundingBoxLeft_[i] = value; }
  void setBoundingBoxWidth( size_t i, uint32_t value ) { boundingBoxWidth_[i] = value; }
  void setBoundingBoxHeight( size_t i, uint32_t value ) { boundingBoxHeight_[i] = value; }
  void setRectangleNumberOfObjectsMinus1( size_t i, uint32_t value ) { rectangleNumberOfObjectsMinus1_[i] = value; }
  void setRectangleObjectIdx( size_t i, size_t j, uint32_t value ) { rectangleObjectIdx_[i][j] = value; }

 private:
  bool                               persistenceFlag_;
  bool                               resetFlag_;
  uint32_t                           numRectanglesUpdates_;
  uint8_t                            log2MaxObjectIdxTracked_;
  uint8_t                            log2MaxRectangleIdxUpdated_;
  std::vector<uint32_t>              rectangleIdx_;
  std::vector<bool>                  rectangleCancelFlag_;
  std::vector<bool>                  boundingBoxUpdateFlag_;
  std::vector<uint32_t>              boundingBoxTop_;
  std::vector<uint32_t>              boundingBoxLeft_;
  std::vector<uint32_t>              boundingBoxWidth_;
  std::vector<uint32_t>              boundingBoxHeight_;
  std::vector<uint32_t>              rectangleNumberOfObjectsMinus1_;
  std::vector<std::vector<uint32_t>> rectangleObjectIdx_;
};

// F.2.15  Buffering period SEI message syntax
class SEIBufferingPeriod : public SEI {
 public:
  SEIBufferingPeriod() :
      irapCabParamsPresentFlag_( false ),
      concatenationFlag_( false ),
      atlasSequenceParameterSetId_( 0 ),
      cabDelayOffset_( 0 ),
      dabDelayOffset_( 0 ),
      atlasCabRemovalDelayDeltaMinus1_( 0 ),
      maxSubLayersMinus1_( 0 ) {}
  ~SEIBufferingPeriod() {
    for ( auto& element : nalInitialCabRemovalDelay_ ) { element.clear(); }
    for ( auto& element : nalInitialCabRemovalOffset_ ) { element.clear(); }
    for ( auto& element : aclInitialCabRemovalDelay_ ) { element.clear(); }
    for ( auto& element : aclInitialCabRemovalOffset_ ) { element.clear(); }
    nalInitialCabRemovalDelay_.clear();
    nalInitialCabRemovalOffset_.clear();
    aclInitialCabRemovalDelay_.clear();
    aclInitialCabRemovalOffset_.clear();
    nalInitialAltCabRemovalDelay_.clear();
    nalInitialAltCabRemovalOffset_.clear();
    aclInitialAltCabRemovalDelay_.clear();
    aclInitialAltCabRemovalOffset_.clear();
  }
  SEIBufferingPeriod& operator=( const SEIBufferingPeriod& ) = default;

  SeiPayloadType getPayloadType() { return BUFFERING_PERIOD; }
  void           allocate() {
    nalInitialCabRemovalDelay_.resize( maxSubLayersMinus1_ + 1 );
    nalInitialCabRemovalOffset_.resize( maxSubLayersMinus1_ + 1 );
    aclInitialCabRemovalDelay_.resize( maxSubLayersMinus1_ + 1 );
    aclInitialCabRemovalOffset_.resize( maxSubLayersMinus1_ + 1 );
    nalInitialAltCabRemovalDelay_.resize( maxSubLayersMinus1_ + 1 );
    nalInitialAltCabRemovalOffset_.resize( maxSubLayersMinus1_ + 1 );
    aclInitialAltCabRemovalDelay_.resize( maxSubLayersMinus1_ + 1 );
    aclInitialAltCabRemovalOffset_.resize( maxSubLayersMinus1_ + 1 );
  }
  bool                                getIrapCabParamsPresentFlag() { return irapCabParamsPresentFlag_; }
  bool                                getConcatenationFlag() { return concatenationFlag_; }
  uint8_t                             getAtlasSequenceParameterSetId() { return atlasSequenceParameterSetId_; }
  uint32_t                            getCabDelayOffset() { return cabDelayOffset_; }
  uint32_t                            getDabDelayOffset() { return dabDelayOffset_; }
  uint32_t                            getAtlasCabRemovalDelayDeltaMinus1() { return atlasCabRemovalDelayDeltaMinus1_; }
  uint32_t                            getMaxSubLayersMinus1() { return maxSubLayersMinus1_; }
  std::vector<std::vector<uint32_t>>& getNalInitialCabRemovalDelay() { return nalInitialCabRemovalDelay_; }
  std::vector<std::vector<uint32_t>>& getNalInitialCabRemovalOffse() { return nalInitialCabRemovalOffset_; }
  std::vector<uint32_t>&              getNalInitialAltCabRemovalDelay() { return nalInitialAltCabRemovalDelay_; }
  std::vector<uint32_t>&              getNalInitialAltCabRemovalOffset() { return nalInitialAltCabRemovalOffset_; }
  std::vector<std::vector<uint32_t>>& getAclInitialCabRemovalDelay() { return aclInitialCabRemovalDelay_; }
  std::vector<std::vector<uint32_t>>& getAclInitialCabRemovalOffset() { return aclInitialCabRemovalOffset_; }
  std::vector<uint32_t>&              getAclInitialAltCabRemovalDelay() { return aclInitialAltCabRemovalDelay_; }
  std::vector<uint32_t>&              getAclInitialAltCabRemovalOffset() { return aclInitialAltCabRemovalOffset_; }
  std::vector<uint32_t>&              getNalInitialCabRemovalDelay( size_t i ) { return nalInitialCabRemovalDelay_[i]; }
  std::vector<uint32_t>& getNalInitialCabRemovalOffsey( size_t i ) { return nalInitialCabRemovalOffset_[i]; }
  std::vector<uint32_t>& getAclInitialCabRemovalDelay( size_t i ) { return aclInitialCabRemovalDelay_[i]; }
  std::vector<uint32_t>& getAclInitialCabRemovalOffset( size_t i ) { return aclInitialCabRemovalOffset_[i]; }
  uint32_t               getNalInitialAltCabRemovalDelay( size_t i ) { return nalInitialAltCabRemovalDelay_[i]; }
  uint32_t               getNalInitialAltCabRemovalOffset( size_t i ) { return nalInitialAltCabRemovalOffset_[i]; }
  uint32_t               getAclInitialAltCabRemovalDelay( size_t i ) { return aclInitialAltCabRemovalDelay_[i]; }
  uint32_t               getAclInitialAltCabRemovalOffset( size_t i ) { return aclInitialAltCabRemovalOffset_[i]; }
  uint32_t               getNalInitialCabRemovalDelay( size_t i, size_t j ) { return nalInitialCabRemovalDelay_[i][j]; }
  uint32_t getNalInitialCabRemovalOffset( size_t i, size_t j ) { return nalInitialCabRemovalOffset_[i][j]; }
  uint32_t getAclInitialCabRemovalDelay( size_t i, size_t j ) { return aclInitialCabRemovalDelay_[i][j]; }
  uint32_t getAclInitialCabRemovalOffset( size_t i, size_t j ) { return aclInitialCabRemovalOffset_[i][j]; }

  void setIrapCabParamsPresentFlag( bool value ) { irapCabParamsPresentFlag_ = value; }
  void setConcatenationFlag( bool value ) { concatenationFlag_ = value; }
  void setAtlasSequenceParameterSetId( uint8_t value ) { atlasSequenceParameterSetId_ = value; }
  void setCabDelayOffset( uint32_t value ) { cabDelayOffset_ = value; }
  void setDabDelayOffset( uint32_t value ) { dabDelayOffset_ = value; }
  void setAtlasCabRemovalDelayDeltaMinus1( uint32_t value ) { atlasCabRemovalDelayDeltaMinus1_ = value; }
  void setMaxSubLayersMinus1( uint32_t value ) { maxSubLayersMinus1_ = value; }
  void setNalInitialAltCabRemovalDelay( size_t i, uint32_t value ) { nalInitialAltCabRemovalDelay_[i] = value; }
  void setNalInitialAltCabRemovalOffset( size_t i, uint32_t value ) { nalInitialAltCabRemovalOffset_[i] = value; }
  void setAclInitialAltCabRemovalDelay( size_t i, uint32_t value ) { aclInitialAltCabRemovalDelay_[i] = value; }
  void setAclInitialAltCabRemovalOffset( size_t i, uint32_t value ) { aclInitialAltCabRemovalOffset_[i] = value; }
  void setNalInitialCabRemovalDelay( size_t i, size_t j, uint32_t value ) { nalInitialCabRemovalDelay_[i][j] = value; }
  void setNalInitialCabRemovalOffset( size_t i, size_t j, uint32_t value ) {
    nalInitialCabRemovalOffset_[i][j] = value;
  }
  void setAclInitialCabRemovalDelay( size_t i, size_t j, uint32_t value ) { aclInitialCabRemovalDelay_[i][j] = value; }
  void setAclInitialCabRemovalOffset( size_t i, size_t j, uint32_t value ) {
    aclInitialCabRemovalOffset_[i][j] = value;
  }

 private:
  bool                               irapCabParamsPresentFlag_;
  bool                               concatenationFlag_;
  uint8_t                            atlasSequenceParameterSetId_;
  uint32_t                           cabDelayOffset_;
  uint32_t                           dabDelayOffset_;
  uint32_t                           atlasCabRemovalDelayDeltaMinus1_;
  uint32_t                           maxSubLayersMinus1_;
  std::vector<std::vector<uint32_t>> nalInitialCabRemovalDelay_;
  std::vector<std::vector<uint32_t>> nalInitialCabRemovalOffset_;
  std::vector<uint32_t>              nalInitialAltCabRemovalDelay_;
  std::vector<uint32_t>              nalInitialAltCabRemovalOffset_;
  std::vector<std::vector<uint32_t>> aclInitialCabRemovalDelay_;
  std::vector<std::vector<uint32_t>> aclInitialCabRemovalOffset_;
  std::vector<uint32_t>              aclInitialAltCabRemovalDelay_;
  std::vector<uint32_t>              aclInitialAltCabRemovalOffset_;
};

// F.2.16  Atlas frame timing SEI message syntax
class SEIAtlasFrameTiming : public SEI {
 public:
  SEIAtlasFrameTiming() : cabRemovalDelayMinus1_( 0 ), dabOutputDelay_( 0 ) {}
  ~SEIAtlasFrameTiming() {}
  SEIAtlasFrameTiming& operator=( const SEIAtlasFrameTiming& ) = default;

  SeiPayloadType getPayloadType() { return ATLAS_FRAME_TIMING; }

  uint32_t getAftCabRemovalDelayMinus1() { return cabRemovalDelayMinus1_; }
  uint32_t getAftDabOutputDelay() { return dabOutputDelay_; }

  void setAftCabRemovalDelayMinus1( uint32_t value ) { cabRemovalDelayMinus1_ = value; }
  void setAftDabOutputDelay( uint32_t value ) { dabOutputDelay_ = value; }

 private:
  uint32_t cabRemovalDelayMinus1_;
  uint32_t dabOutputDelay_;
};

class SEIAtlasInformation : public SEI {
 public:
  SEIAtlasInformation() :
      persistenceFlag_( false ),
      resetFlag_( false ),
      numAtlasesMinus1_( 0 ),
      numUpdates_( 0 ),
      log2MaxObjectIdxTracked_( 0 ) {}
  ~SEIAtlasInformation() {
    objectIdx_.clear();
    for ( auto& element : objectInAtlasPresentFlag_ ) { element.clear(); }
    objectInAtlasPresentFlag_.clear();
  }
  SEIAtlasInformation& operator=( const SEIAtlasInformation& ) = default;

  SeiPayloadType getPayloadType() { return ATLAS_INFORMATION; }

  void allocate() {
    objectIdx_.resize( numUpdates_, 0 );
    objectInAtlasPresentFlag_.resize( numUpdates_ );
    for ( auto& element : objectInAtlasPresentFlag_ ) { element.resize( numAtlasesMinus1_ + 1, false ); }
  }

  bool     getPersistenceFlag() { return persistenceFlag_; }
  bool     getResetFlag() { return resetFlag_; }
  uint32_t getNumAtlasesMinus1() { return numAtlasesMinus1_; }
  uint32_t getNumUpdates() { return numUpdates_; }
  uint32_t getLog2MaxObjectIdxTracked() { return log2MaxObjectIdxTracked_; }
  uint32_t getObjectIdx( size_t i ) { return objectIdx_[i]; }
  bool     getObjectInAtlasPresentFlag( size_t i, size_t j ) { return objectInAtlasPresentFlag_[i][j]; }

  void setPersistenceFlag( bool value ) { persistenceFlag_ = value; }
  void setResetFlag( bool value ) { resetFlag_ = value; }
  void setNumAtlasesMinus1( uint32_t value ) { numAtlasesMinus1_ = value; }
  void setNumUpdates( uint32_t value ) { numUpdates_ = value; }
  void setLog2MaxObjectIdxTracked( uint32_t value ) { log2MaxObjectIdxTracked_ = value; }
  void setObjectIdx( size_t i, uint32_t value ) { objectIdx_[i] = value; }
  void setObjectInAtlasPresentFlag( size_t i, size_t j, bool value ) { objectInAtlasPresentFlag_[i][j] = value; }

 private:
  bool                           persistenceFlag_;
  bool                           resetFlag_;
  uint32_t                       numAtlasesMinus1_;
  uint32_t                       numUpdates_;
  uint32_t                       log2MaxObjectIdxTracked_;
  std::vector<uint32_t>          objectIdx_;
  std::vector<std::vector<bool>> objectInAtlasPresentFlag_;
};

class SEIViewportCameraParameters : public SEI {
 public:
  SEIViewportCameraParameters() :
      cameraId_( 0 ),
      cancelFlag_( false ),
      persistenceFlag_( false ),
      cameraType_( 0.f ),
      erpHorizontalFov_( 0.f ),
      erpVerticalFov_( 0.f ),
      perspectiveAspectRatio_( 0.f ),
      perspectiveHorizontalFov_( 0.f ),
      orthoAspectRatio_( 0.f ),
      orthoHorizontalSize_( 0.f ),
      clippingNearPlane_( 0.f ),
      clippingFarPlane_( 0.f ) {}
  ~SEIViewportCameraParameters() {}
  SEIViewportCameraParameters& operator=( const SEIViewportCameraParameters& ) = default;

  SeiPayloadType getPayloadType() { return VIEWPORT_CAMERA_PARAMETERS; }

  uint32_t getCameraId() { return cameraId_; }
  bool     getCancelFlag() { return cancelFlag_; }
  bool     getPersistenceFlag() { return persistenceFlag_; }
  uint8_t  getCameraType() { return cameraType_; }
  float    getErpHorizontalFov() { return erpHorizontalFov_; }
  float    getErpVerticalFov() { return erpVerticalFov_; }
  float    getPerspectiveAspectRatio() { return perspectiveAspectRatio_; }
  float    getPerspectiveHorizontalFov() { return perspectiveHorizontalFov_; }
  float    getOrthoAspectRatio() { return orthoAspectRatio_; }
  float    getOrthoHorizontalSize() { return orthoHorizontalSize_; }
  float    getClippingNearPlane() { return clippingNearPlane_; }
  float    getClippingFarPlane() { return clippingFarPlane_; }

  void setCameraId( uint32_t value ) { cameraId_ = value; }
  void setCancelFlag( bool value ) { cancelFlag_ = value; }
  void setPersistenceFlag( bool value ) { persistenceFlag_ = value; }
  void setCameraType( uint8_t value ) { cameraType_ = value; }
  void setErpHorizontalFov( float value ) { erpHorizontalFov_ = value; }
  void setErpVerticalFov( float value ) { erpVerticalFov_ = value; }
  void setPerspectiveAspectRatio( float value ) { perspectiveAspectRatio_ = value; }
  void setPerspectiveHorizontalFov( float value ) { perspectiveHorizontalFov_ = value; }
  void setOrthoAspectRatio( float value ) { orthoAspectRatio_ = value; }
  void setOrthoHorizontalSize( float value ) { orthoHorizontalSize_ = value; }
  void setClippingNearPlane( float value ) { clippingNearPlane_ = value; }
  void setClippingFarPlane( float value ) { clippingFarPlane_ = value; }

 private:
  uint32_t cameraId_;
  bool     cancelFlag_;
  bool     persistenceFlag_;
  uint8_t  cameraType_;
  float    erpHorizontalFov_;
  float    erpVerticalFov_;
  float    perspectiveAspectRatio_;
  float    perspectiveHorizontalFov_;
  float    orthoAspectRatio_;
  float    orthoHorizontalSize_;
  float    clippingNearPlane_;
  float    clippingFarPlane_;
};

class SEIViewportPosition : public SEI {
 public:
  SEIViewportPosition() :
      viewportId_( 0 ),
      cameraParametersPresentFlag_( false ),
      CameraId_( 0 ),
      cancelFlag_( false ),
      persistenceFlag_( false ),
      position_{0.0f, 0.0f, 0.0f},
      quaternionX_( 0.0f ),
      quaternionY_( 0.0f ),
      quaternionZ_( 0.0f ),
      centerViewFlag_( false ),
      leftViewFlag_( false ) {}
  ~SEIViewportPosition() {}
  SEIViewportPosition& operator=( const SEIViewportPosition& ) = default;

  SeiPayloadType getPayloadType() { return VIEWPORT_POSITION; }

  uint32_t getViewportId() { return viewportId_; }
  bool     getCameraParametersPresentFlag() { return cameraParametersPresentFlag_; }
  uint8_t  getVcpCameraId() { return CameraId_; }
  bool     getCancelFlag() { return cancelFlag_; }
  bool     getPersistenceFlag() { return persistenceFlag_; }
  float    getPosition( size_t i ) { return position_[i]; }
  float    getQuaternionX() { return quaternionX_; }
  float    getQuaternionY() { return quaternionY_; }
  float    getQuaternionZ() { return quaternionZ_; }
  bool     getCenterViewFlag() { return centerViewFlag_; }
  bool     getLeftViewFlag() { return leftViewFlag_; }

  void setViewportId( uint32_t value ) { viewportId_ = value; }
  void setCameraParametersPresentFlag( bool value ) { cameraParametersPresentFlag_ = value; }
  void setVcpCameraId( uint8_t value ) { CameraId_ = value; }
  void setCancelFlag( bool value ) { cancelFlag_ = value; }
  void setPersistenceFlag( bool value ) { persistenceFlag_ = value; }
  void setPosition( size_t i, float value ) { position_[i] = value; }
  void setQuaternionX( float value ) { quaternionX_ = value; }
  void setQuaternionY( float value ) { quaternionY_ = value; }
  void setQuaternionZ( float value ) { quaternionZ_ = value; }
  void setCenterViewFlag( bool value ) { centerViewFlag_ = value; }
  void setLeftViewFlag( bool value ) { leftViewFlag_ = value; }

 private:
  uint32_t viewportId_;
  bool     cameraParametersPresentFlag_;
  uint8_t  CameraId_;
  bool     cancelFlag_;
  bool     persistenceFlag_;
  float    position_[3];
  float    quaternionX_;
  float    quaternionY_;
  float    quaternionZ_;
  bool     centerViewFlag_;
  bool     leftViewFlag_;
};

class SEIGeometrySmoothing : public SEI {
 public:
  SEIGeometrySmoothing() :
      smoothingPersistenceFlag_( false ),
      smoothingResetFlag_( false ),
      smoothingInstancesUpdated_( 0 ) {}
  ~SEIGeometrySmoothing() {
    smoothingInstanceIndex_.clear();
    smoothingInstanceCancelFlag_.clear();
    smoothingMethodType_.clear();
    smoothingGridSizeMinus2_.clear();
    smoothingThreshold_.clear();
    pbfLog2ThresholdMinus1_.clear();
    pbfPassesCountMinus1_.clear();
    pbfFilterSizeMinus1_.clear();
  }
  SEIGeometrySmoothing& operator=( const SEIGeometrySmoothing& ) = default;

  SeiPayloadType getPayloadType() { return GEOMETRY_SMOOTHING; }
  void           allocate() {
    smoothingInstanceIndex_.resize( smoothingInstancesUpdated_, 0 );
    smoothingInstanceCancelFlag_.resize( smoothingInstancesUpdated_, 0 );
    smoothingMethodType_.resize( smoothingInstancesUpdated_, 0 );
    smoothingGridSizeMinus2_.resize( smoothingInstancesUpdated_, 0 );
    smoothingThreshold_.resize( smoothingInstancesUpdated_, 0 );
    pbfLog2ThresholdMinus1_.resize( smoothingInstancesUpdated_, 0 );
    pbfPassesCountMinus1_.resize( smoothingInstancesUpdated_, 0 );
    pbfFilterSizeMinus1_.resize( smoothingInstancesUpdated_, 0 );
  }

  bool    getSmoothingPersistenceFlag() { return smoothingPersistenceFlag_; }
  bool    getSmoothingResetFlag() { return smoothingResetFlag_; }
  uint8_t getSmoothingInstancesUpdated() { return smoothingInstancesUpdated_; }
  uint8_t getSmoothingInstanceIndex( size_t i ) { return smoothingInstanceIndex_[i]; }
  bool    getSmoothingInstanceCancelFlag( size_t i ) { return smoothingInstanceCancelFlag_[i]; }
  uint8_t getSmoothingMethodType( size_t i ) { return smoothingMethodType_[i]; }
  uint8_t getSmoothingGridSizeMinus2( size_t i ) { return smoothingGridSizeMinus2_[i]; }
  uint8_t getSmoothingThreshold( size_t i ) { return smoothingThreshold_[i]; }
  uint8_t getPbfLog2ThresholdMinus1( size_t i ) { return pbfLog2ThresholdMinus1_[i]; }
  uint8_t getPbfPassesCountMinus1( size_t i ) { return pbfPassesCountMinus1_[i]; }
  uint8_t getPbfFilterSizeMinus1( size_t i ) { return pbfFilterSizeMinus1_[i]; }

  void setSmoothingPersistenceFlag( bool value ) { smoothingPersistenceFlag_ = value; }
  void setSmoothingResetFlag( bool value ) { smoothingResetFlag_ = value; }
  void setSmoothingInstancesUpdated( uint8_t value ) { smoothingInstancesUpdated_ = value; }
  void setSmoothingInstanceIndex( size_t i, uint8_t value ) { smoothingInstanceIndex_[i] = value; }
  void setSmoothingInstanceCancelFlag( size_t i, bool value ) { smoothingInstanceCancelFlag_[i] = value; }
  void setSmoothingMethodType( size_t i, uint8_t value ) { smoothingMethodType_[i] = value; }
  void setSmoothingGridSizeMinus2( size_t i, uint8_t value ) { smoothingGridSizeMinus2_[i] = value; }
  void setSmoothingThreshold( size_t i, uint8_t value ) { smoothingThreshold_[i] = value; }
  void setPbfLog2ThresholdMinus1( size_t i, uint8_t value ) { pbfLog2ThresholdMinus1_[i] = value; }
  void setPbfPassesCountMinus1( size_t i, uint8_t value ) { pbfPassesCountMinus1_[i] = value; }
  void setPbfFilterSizeMinus1( size_t i, uint8_t value ) { pbfFilterSizeMinus1_[i] = value; }

 private:
  bool                 smoothingPersistenceFlag_;
  bool                 smoothingResetFlag_;
  uint8_t              smoothingInstancesUpdated_;
  std::vector<uint8_t> smoothingInstanceIndex_;
  std::vector<bool>    smoothingInstanceCancelFlag_;
  std::vector<uint8_t> smoothingMethodType_;
  std::vector<uint8_t> smoothingGridSizeMinus2_;
  std::vector<uint8_t> smoothingThreshold_;
  std::vector<uint8_t> pbfLog2ThresholdMinus1_;
  std::vector<uint8_t> pbfPassesCountMinus1_;
  std::vector<uint8_t> pbfFilterSizeMinus1_;
};

class SEIAttributeSmoothing : public SEI {
 public:
  SEIAttributeSmoothing() :
      smoothingPersistenceFlag_( false ),
      smoothingResetFlag_( false ),
      numAttributesUpdated_( 0 ) {}
  ~SEIAttributeSmoothing() {
    attributeIdx_.clear();
    attributeSmoothingCancelFlag_.clear();
    smoothingInstancesUpdated_.clear();
    for ( auto& element : smoothingInstanceIndex_ ) { element.clear(); }
    for ( auto& element : smoothingInstanceCancelFlag_ ) { element.clear(); }
    for ( auto& element : smoothingMethodType_ ) { element.clear(); }
    for ( auto& element : smoothingGridSizeMinus2_ ) { element.clear(); }
    for ( auto& element : smoothingThreshold_ ) { element.clear(); }
    for ( auto& element : smoothingThresholdVariation_ ) { element.clear(); }
    for ( auto& element : smoothingThresholdDifference_ ) { element.clear(); }
    smoothingInstanceIndex_.clear();
    smoothingInstanceCancelFlag_.clear();
    smoothingMethodType_.clear();
    smoothingGridSizeMinus2_.clear();
    smoothingThreshold_.clear();
    smoothingThresholdVariation_.clear();
    smoothingThresholdDifference_.clear();
  }
  SEIAttributeSmoothing& operator=( const SEIAttributeSmoothing& ) = default;

  SeiPayloadType getPayloadType() { return ATTRIBUTE_SMOOTHING; }

  void allocate() {
    attributeIdx_.resize( numAttributesUpdated_, 0 );
    attributeSmoothingCancelFlag_.resize( numAttributesUpdated_, false );
    smoothingInstancesUpdated_.resize( numAttributesUpdated_, 0 );
  }
  void allocate( size_t size, size_t dimension ) {
    if ( smoothingInstanceIndex_.size() < size ) {
      smoothingInstanceIndex_.resize( size );
      smoothingInstanceCancelFlag_.resize( size );
      smoothingMethodType_.resize( size );
      smoothingGridSizeMinus2_.resize( size );
      smoothingThreshold_.resize( size );
      smoothingThresholdVariation_.resize( size );
      smoothingThresholdDifference_.resize( size );
    }
    smoothingInstanceIndex_[size - 1].resize( dimension );
    smoothingInstanceCancelFlag_[size - 1].resize( dimension );
    smoothingMethodType_[size - 1].resize( dimension );
    smoothingGridSizeMinus2_[size - 1].resize( dimension );
    smoothingThreshold_[size - 1].resize( dimension );
    smoothingThresholdVariation_[size - 1].resize( dimension );
    smoothingThresholdDifference_[size - 1].resize( dimension );
  }
  uint8_t getSmoothingPersistenceFlag() { return smoothingPersistenceFlag_; }
  uint8_t getSmoothingResetFlag() { return smoothingResetFlag_; }
  uint8_t getNumAttributesUpdated() { return numAttributesUpdated_; }
  uint8_t getAttributeIdx( size_t i ) { return attributeIdx_[i]; }
  uint8_t getAttributeSmoothingCancelFlag( size_t i ) { return attributeSmoothingCancelFlag_[i]; }
  uint8_t getSmoothingInstancesUpdated( size_t i ) { return smoothingInstancesUpdated_[i]; }
  uint8_t getSmoothingInstanceIndex( size_t i, size_t j ) { return smoothingInstanceIndex_[i][j]; }
  uint8_t getSmoothingInstanceCancelFlag( size_t i, size_t j ) { return smoothingInstanceCancelFlag_[i][j]; }
  uint8_t getSmoothingMethodType( size_t i, size_t j ) { return smoothingMethodType_[i][j]; }
  uint8_t getSmoothingGridSizeMinus2( size_t i, size_t j ) { return smoothingGridSizeMinus2_[i][j]; }
  uint8_t getSmoothingThreshold( size_t i, size_t j ) { return smoothingThreshold_[i][j]; }
  uint8_t getSmoothingThresholdVariation( size_t i, size_t j ) { return smoothingThresholdVariation_[i][j]; }
  uint8_t getSmoothingThresholdDifference( size_t i, size_t j ) { return smoothingThresholdDifference_[i][j]; }

  void setSmoothingPersistenceFlag( uint8_t value ) { smoothingPersistenceFlag_ = value; }
  void setSmoothingResetFlag( uint8_t value ) { smoothingResetFlag_ = value; }
  void setNumAttributesUpdated( uint8_t value ) { numAttributesUpdated_ = value; }
  void setAttributeIdx( size_t i, uint8_t value ) { attributeIdx_[i] = value; }
  void setAttributeSmoothingCancelFlag( size_t i, uint8_t value ) { attributeSmoothingCancelFlag_[i] = value; }
  void setSmoothingInstancesUpdated( size_t i, uint8_t value ) { smoothingInstancesUpdated_[i] = value; }
  void setSmoothingInstanceIndex( size_t i, size_t j, uint8_t value ) { smoothingInstanceIndex_[i][j] = value; }
  void setSmoothingInstanceCancelFlag( size_t i, size_t j, uint8_t value ) {
    smoothingInstanceCancelFlag_[i][j] = value;
  }
  void setSmoothingMethodType( size_t i, size_t j, uint8_t value ) { smoothingMethodType_[i][j] = value; }
  void setSmoothingGridSizeMinus2( size_t i, size_t j, uint8_t value ) { smoothingGridSizeMinus2_[i][j] = value; }
  void setSmoothingThreshold( size_t i, size_t j, uint8_t value ) { smoothingThreshold_[i][j] = value; }
  void setSmoothingThresholdVariation( size_t i, size_t j, uint8_t value ) {
    smoothingThresholdVariation_[i][j] = value;
  }
  void setSmoothingThresholdDifference( size_t i, size_t j, uint8_t value ) {
    smoothingThresholdDifference_[i][j] = value;
  }

 private:
  bool                              smoothingPersistenceFlag_;
  bool                              smoothingResetFlag_;
  uint8_t                           numAttributesUpdated_;
  std::vector<uint8_t>              attributeIdx_;
  std::vector<bool>                 attributeSmoothingCancelFlag_;
  std::vector<uint8_t>              smoothingInstancesUpdated_;
  std::vector<std::vector<uint8_t>> smoothingInstanceIndex_;
  std::vector<std::vector<uint8_t>> smoothingInstanceCancelFlag_;
  std::vector<std::vector<uint8_t>> smoothingMethodType_;
  std::vector<std::vector<uint8_t>> smoothingGridSizeMinus2_;
  std::vector<std::vector<uint8_t>> smoothingThreshold_;
  std::vector<std::vector<uint8_t>> smoothingThresholdVariation_;
  std::vector<std::vector<uint8_t>> smoothingThresholdDifference_;
};

};  // namespace pcc

#endif  //~PCC_BITSTREAM_SEI_H
