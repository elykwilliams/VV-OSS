SET(REGRESSION_TESTS regression_tests)

SET(REGRESSION_TEST_SOURCES
        test_settings.cpp
        )

SET(PROJECT_SRC_FILES
        ../../src/Base/NSESolver.cpp ../../src/Base/NSESolver.h)

SET(TEST_INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/test/
        ${CMAKE_SOURCE_DIR}/src/
        )

SET(PRM_DIR ${CMAKE_SOURCE_DIR}/test/prm)

ADD_EXECUTABLE(${REGRESSION_TESTS} ${REGRESSION_TEST_SOURCES} ${PROJECT_SRC_FILES})

TARGET_INCLUDE_DIRECTORIES(${REGRESSION_TESTS} PRIVATE ${TEST_INCLUDE_DIRS})

# Setup DealII
DEAL_II_SETUP_TARGET(${REGRESSION_TESTS})

ADD_TEST(test_default_prm mpirun -n 2 ${REGRESSION_TESTS} ${PRM_DIR}/default.prm)
ADD_TEST(test_default2_prm mpirun -n 2 ${REGRESSION_TESTS} ${PRM_DIR}/default2.prm)
