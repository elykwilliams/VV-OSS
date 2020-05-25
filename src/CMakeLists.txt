
SET(TARGET_SRC_FILES
		main.cpp
		Base/NSESolver.cpp
		Geometry/manufactured.cpp
		Geometry/round_cylinder.cpp Geometry/channel_flow.cpp)

SET(TARGET_INC_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR}/.
   )


ADD_EXECUTABLE(${TARGET} ${TARGET_SRC_FILES})
TARGET_INCLUDE_DIRECTORIES(${TARGET} PRIVATE ${TARGET_INC_DIRS})

DEAL_II_SETUP_TARGET(${TARGET})

