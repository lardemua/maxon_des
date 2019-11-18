add_library(maxon STATIC
        maxon_des.c
        maxon_des_parameter_functions.c
        maxon_des_service_functions.c
        maxon_des_setting_functions.c
        maxon_des_status_functions.c
        maxon_des_monitor_functions.c
        maxon_des_recording_functions.c
        maxon_des_crc.c
        maxon_des_comm.c)

target_include_directories(maxon
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(maxon)

set_target_properties(maxon
    PROPERTIES
        PUBLIC_HEADER "../include/maxon_des.h")

add_library(Maxon::maxon ALIAS maxon)

install(TARGETS maxon
        EXPORT MaxonTargets
        PUBLIC_HEADER DESTINATION include
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(EXPORT MaxonTargets
        FILE MaxonTargets.cmake
        NAMESPACE Maxon::
        DESTINATION lib/cmake/Maxon)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("MaxonConfigVersion.cmake"
        VERSION ${MAXON_VERSION}
        COMPATIBILITY SameMajorVersion)

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/MaxonConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/MaxonConfigVersion.cmake"
        DESTINATION lib/cmake/Maxon)
