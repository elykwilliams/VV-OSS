FILE(GLOB PRM_FILES ${CMAKE_SOURCE_DIR}/test/prm/*.prm)

SET(TARGET update_prm_files)
ADD_EXECUTABLE(${TARGET} update_prm.cpp)
TARGET_INCLUDE_DIRECTORIES(${TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/src/)
DEAL_II_SETUP_TARGET(${TARGET})

ADD_CUSTOM_COMMAND(TARGET ${TARGET} COMMAND ./update-all.sh)
