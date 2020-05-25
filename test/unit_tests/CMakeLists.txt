SET(UNIT_TESTS unit_tests)

SET(PROJECT_SRC_FILES ../../src/Base/NSESolver.cpp ../../src/Base/NSESolver.h ../../src/Geometry/manufactured.cpp ../../src/Geometry/round_cylinder.cpp ../../src/Geometry/channel_flow.cpp)

SET(UNIT_TEST_SOURCES
        unit_test_driver.cc
        )

SET(TEST_INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/test/
        ${CMAKE_SOURCE_DIR}/src/
        )

ADD_EXECUTABLE(${UNIT_TESTS} ${UNIT_TEST_SOURCES} ${PROJECT_SRC_FILES})

TARGET_INCLUDE_DIRECTORIES(${UNIT_TESTS} PRIVATE ${TEST_INCLUDE_DIRS})

DEAL_II_SETUP_TARGET(${UNIT_TESTS})

SET(CTEST_OUTPUT_ON_FAILURE ON)

ADD_TEST(NAME unit_tests_seriel
        COMMAND ${UNIT_TESTS})

# Run all tests in parallel
ADD_TEST(NAME unit_tests_parallel
        COMMAND mpirun ${UNIT_TESTS}
        )
