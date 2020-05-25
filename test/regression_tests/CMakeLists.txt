SET(REGRESSION_TESTS regression_tests)

SET(REGRESSION_TEST_SOURCES
        test_runner.cpp
        tests.cpp)

SET(PROJECT_SRC_FILES
        ${CMAKE_SOURCE_DIR}/src/Base/NSESolver.cpp
        ${CMAKE_SOURCE_DIR}/src/Geometry/manufactured.cpp
        ${CMAKE_SOURCE_DIR}/src/Geometry/round_cylinder.cpp
        ${CMAKE_SOURCE_DIR}/src/Geometry/channel_flow.cpp)

SET(TEST_INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/test/
        ${CMAKE_SOURCE_DIR}/src/
        )

SET(PRM_DIR ${CMAKE_SOURCE_DIR}/test/prm)

ADD_EXECUTABLE(${REGRESSION_TESTS} ${REGRESSION_TEST_SOURCES} ${PROJECT_SRC_FILES})

TARGET_INCLUDE_DIRECTORIES(${REGRESSION_TESTS} PRIVATE ${TEST_INCLUDE_DIRS})

# Setup DealII
DEAL_II_SETUP_TARGET(${REGRESSION_TESTS})

ADD_TEST(default2D_prm mpirun ${REGRESSION_TESTS} ${PRM_DIR}/default-2D.prm)
ADD_TEST(manufactured2D_prm mpirun ${REGRESSION_TESTS} ${PRM_DIR}/manufactured-2D.prm)
ADD_TEST(roundcylinder2D_prm mpirun ${REGRESSION_TESTS} ${PRM_DIR}/round_cylinder-2D.prm)
ADD_TEST(channelflow2D_prm mpirun ${REGRESSION_TESTS} ${PRM_DIR}/channel_flow-2D.prm)
