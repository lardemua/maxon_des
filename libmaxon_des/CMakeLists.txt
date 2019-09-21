cmake_minimum_required(VERSION 3.10.2)

project(libmaxon_des
        VERSION 0.1.0
        LANGUAGES C)

option(ENABLE_TESTS "Perform unit tests" OFF)

add_library(maxon_des
        src/mx_des.c
        src/mx_des_functions.c
        src/mx_des_status_functions.c
        src/mx_des_parameter_functions.c
        src/mx_des_service_functions.c
        src/mx_des_crc.c
        src/mx_des_comm.c)

target_include_directories(maxon_des
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

target_link_libraries(maxon_des)

if (ENABLE_TESTS)
  find_package(CMocka CONFIG REQUIRED)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
  include(AddCMockaTest)
  include(AddMockedTest)
  add_subdirectory(tests)
  enable_testing()
endif(ENABLE_TESTS)
