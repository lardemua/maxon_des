add_mocked_test(status_functions
    SOURCES test_status_functions.c
            mock_comm.c
    LINK_LIBRARIES maxon_des
    MOCKS des_send_frame des_receive_frame)