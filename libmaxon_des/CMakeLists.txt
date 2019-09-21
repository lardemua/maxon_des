cmake_minimum_required(VERSION 3.10.2)

project(libmaxon_des
        VERSION 0.1.0
        LANGUAGES C)

option(ENABLE_TESTS "Perform unit tests" OFF)

add_subdirectory(src)

add_subdirectory(cmd)

if (ENABLE_TESTS)
  find_package(CMocka CONFIG REQUIRED)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
  include(AddCMockaTest)
  include(AddMockedTest)
  add_subdirectory(tests)
  enable_testing()
endif(ENABLE_TESTS)
