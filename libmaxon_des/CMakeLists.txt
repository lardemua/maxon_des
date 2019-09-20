cmake_minimum_required(VERSION 3.10.2)

project(libmaxon_des
        VERSION 0.1
        LANGUAGES C)

add_library(libmaxon_des
    src/mx_des.c
    src/mx_des_functions.c
    src/mx_des_status_functions.c
    src/mx_des_crc.c
    src/mx_des_comm.c)

target_include_directories(libmaxon_des PUBLIC include)

target_link_libraries(libmaxon_des)
