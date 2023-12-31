CMAKE_MINIMUM_REQUIRED (VERSION 2.8.11)

GET_FILENAME_COMPONENT(MYNAME ${CMAKE_CURRENT_LIST_DIR} NAME)
STRING(REPLACE " " "_" MYNAME ${MYNAME})
PROJECT(${MYNAME} C CXX)

FILE(GLOB SRC  include/*.h 
               source/*.cpp  
               ${HDRTOOLS_DIR}/projects/HDRConvert/inc/ProjectParameters.h
               ${HDRTOOLS_DIR}/projects/HDRConvert/src/ProjectParameters.cpp )

INCLUDE_DIRECTORIES( include 
                     ${CMAKE_SOURCE_DIR}/source/lib/PccLibCommon/include/
                     ${CMAKE_SOURCE_DIR}/source/lib/PccLibBitstreamCommon/include/
                     ${HDRTOOLS_DIR}/common/inc
                     ${HDRTOOLS_DIR}/projects/HDRConvert/inc )

SET( LIBS PccLibCommon )
IF( USE_HDRTOOLS )
SET( LIBS ${LIBS} HDRLib )
ENDIF()
ADD_LIBRARY( ${MYNAME} ${LINKER} ${SRC} )

TARGET_LINK_LIBRARIES( ${MYNAME} ${LIBS} )

SET_TARGET_PROPERTIES( ${MYNAME} PROPERTIES LINKER_LANGUAGE CXX)
