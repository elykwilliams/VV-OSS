CMAKE_MINIMUM_REQUIRED(VERSION 3.9)

FIND_PACKAGE(deal.II 9.0.0 
			 QUIET 
			 HINTS ${deal.II_DIR} ${DEAL_II_DIR} /usr/local/dealii /storage/kylew/deal.II-9.2
			)

IF(NOT ${deal.II_FOUND})
    MESSAGE(FATAL_ERROR
            "*** Could not locate deal.II. ***
			You may want to either pass a flag -DDEAL_II_DIR=/path/to/deal.II to cmake
			or set an environment variable \"DEAL_II_DIR\" that contains this path."
            )
ENDIF(NOT ${deal.II_FOUND})

IF( NOT DEAL_II_WITH_MPI)
    MESSAGE(FATAL_ERROR
            "Error! The deal.II library found at ${DEAL_II_PATH} was not configured with
            DEAL_II_WITH_MPI = ON"
			)
ELSEIF(NOT DEAL_II_WITH_P4EST)
	MESSAGE(FATAL_ERROR
			"Error! The deal.II library found at ${DEAL_II_PATH} was not configured with
			DEAL_II_WITH_P4EST = ON"
			)
ELSEIF(NOT DEAL_II_WITH_TRILINOS)
	MESSAGE(FATAL_ERROR
			"Error! The deal.II library found at ${DEAL_II_PATH} was not configured with
            DEAL_II_WITH_TRILINOS = ON"
			)
ENDIF()

ADD_CUSTOM_TARGET(debug
        COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
        COMMENT "Switch CMAKE_BUILD_TYPE to Debug"
        )

ADD_CUSTOM_TARGET(release
        COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
        COMMENT "Switch CMAKE_BUILD_TYPE to Release"
        )

DEAL_II_INITIALIZE_CACHED_VARIABLES()

SET(CMAKE_CXX_STANDARD 17)
SET(CLEAN_UP_FILES *.vtu *.pvtu *.visit)

SET(TARGET "NSE")
PROJECT(${TARGET} C CXX)

ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/test)

