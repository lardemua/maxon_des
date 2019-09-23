add_executable(maxon_des_cmd
    cmd.h
    log.c
    version.c
    errors.c
    setting_functions.c
    status_functions.c
    main.c)

target_link_libraries(maxon_des_cmd
    maxon_des)

set_target_properties(maxon_des_cmd
    PROPERTIES OUTPUT_NAME maxon_des)