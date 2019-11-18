cmake_minimum_required(VERSION 3.10.2)

project(libmaxon
        VERSION 0.2.0
        LANGUAGES C)

option(BUILD_CMD    "Build the cli command" OFF)
option(ENABLE_TESTS "Perform unit tests" OFF)
option(BUILD_DOCS   "Build the documentation" OFF)

add_subdirectory(src)

if(BUILD_CMD)
  add_subdirectory(cmd)
endif(BUILD_CMD)

if (ENABLE_TESTS)
  find_package(CMocka CONFIG REQUIRED)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
  include(AddCMockaTest)
  include(AddMockedTest)
  add_subdirectory(tests)
  enable_testing()
endif(ENABLE_TESTS)

if (BUILD_DOCS)
  add_subdirectory(docs)
endif(BUILD_DOCS)
