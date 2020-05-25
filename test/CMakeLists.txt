
ENABLE_TESTING()

ADD_SUBDIRECTORY(unit_tests)
ADD_SUBDIRECTORY(regression_tests)
ADD_SUBDIRECTORY(prm)

ADD_CUSTOM_TARGET(compile_tests
        COMMAND make regression_tests
        COMMAND make unit_tests)
