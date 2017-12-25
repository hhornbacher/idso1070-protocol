add_executable(idso1070a-protocol-test Command_unittest.cxx ../src/Command.cxx)

target_link_libraries (idso1070a-protocol-test ${PTHREAD_LIBRARIES} gtest_main)