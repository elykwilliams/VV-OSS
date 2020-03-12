

SET(TEST_TARGET unit_test)

SET(TEST_TARGET_SOURCES 
	${CMAKE_CURRENT_SOURCE_DIR} test_driver.cc
	)

SET(TEST_TARGET_INCLUDE_DIRS
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/src/
   ) 

ADD_EXECUTABLE(${TEST_TARGET} ${TEST_TARGET_SOURCES})

TARGET_INCLUDE_DIRECTORIES(${TEST_TARGET} PRIVATE ${TEST_TARGET_INCLUDE_DIRS})

# Setup DealII
DEAL_II_SETUP_TARGET(${TEST_TARGET})

###############
# Unit Tests
###############

FIND_PACKAGE(Catch2 REQUIRED)
INCLUDE(Catch)

# Run Serial Tests 
CATCH_DISCOVER_TESTS(${TEST_TARGET})

# Run all tests in parallel
ADD_TEST(NAME unit_test_parallel
		 COMMAND mpirun ${TEST_TARGET}
		)

####################
# Regression Tests 
####################

# Use this to run main with given parameter file

# ADD_TEST(NAME test_q2q2
#		 COMMAND mpirun -n 2 ${TARGET} test/prm/q2q2.prm
#	 	 WORKING_DIR ${CMAKE_CURRENT_BUILD_DIR}
#		)
