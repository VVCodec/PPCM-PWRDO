CMAKE_MINIMUM_REQUIRED (VERSION 2.8.11)

GET_FILENAME_COMPONENT(MYNAME ${CMAKE_CURRENT_LIST_DIR} NAME)
STRING(REPLACE " " "_" MYNAME ${MYNAME})
SET( MYNAME ${MYNAME}${CMAKE_DEBUG_POSTFIX} )
PROJECT(${MYNAME} C CXX)

FILE(GLOB SRC *.h *.cpp *.c 
                ${CMAKE_SOURCE_DIR}/dependencies/program-options-lite/* )
                     
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/source/lib/PccLibCommon/include                    
                     ${CMAKE_SOURCE_DIR}/source/lib/PccLibBitstreamCommon/include      
                     ${CMAKE_SOURCE_DIR}/source/lib/PccLibColorConverter/include 
                     ${CMAKE_SOURCE_DIR}/dependencies/program-options-lite  )

SET( LIBS PccLibCommon PccLibBitstreamCommon PccLibColorConverter ) 

ADD_EXECUTABLE( ${MYNAME} ${SRC} )

TARGET_LINK_LIBRARIES( ${MYNAME} ${LIBS} )

INSTALL( TARGETS ${MYNAME} DESTINATION bin )
