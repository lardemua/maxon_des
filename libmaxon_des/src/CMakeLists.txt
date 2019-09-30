add_library(maxon_des
    mx_des.c
    mx_des_parameter_functions.c
    mx_des_service_functions.c
    mx_des_setting_functions.c
    mx_des_status_functions.c
    mx_des_monitor_functions.c
    mx_des_recording_functions.c
    mx_des_crc.c
    mx_des_comm.c)

target_include_directories(maxon_des
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src)

set_target_properties(maxon_des
    PROPERTIES
        PUBLIC_HEADER "../include/mx_des.h")

target_link_libraries(maxon_des)

install(TARGETS maxon_des
        PUBLIC_HEADER DESTINATION include
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)