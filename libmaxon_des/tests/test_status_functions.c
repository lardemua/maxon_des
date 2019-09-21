#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <malloc.h>
#include <cmocka.h>

#include <mx_des.h>
#include <mx_des_comm.h>

typedef struct
{
    des_context *context;
} state_t;

static int setup(void **ctx)
{
    state_t **state = (state_t **)ctx;

    *state = malloc(sizeof(state_t));
    (*state)->context = malloc(sizeof(des_context));

    return 0;
}

static int teardown(void **ctx)
{
    state_t **state = (state_t **)ctx;

    free((*state)->context);
    free((*state));

    return 0;
}

static void test_des_read_sys_status(void **ctx)
{
    state_t **state = (state_t **)ctx;
    des_context *context = (*state)->context;

    uint16_t status_value = 0b0001100010100100;

    des_frame frame;
    frame.opcode = 0x00;
    frame.len = 1;
    frame.data = (uint16_t[]){status_value};

    will_return(__wrap_des_receive_frame, &frame);
    will_return(__wrap_des_receive_frame, DES_OK);

    expect_value(__wrap_des_send_frame, opcode, 0x01);
    expect_value(__wrap_des_send_frame, len, 0);
    expect_value(__wrap_des_send_frame, data, NULL);
    will_return(__wrap_des_send_frame, DES_OK);

    des_sys_status status;

    des_error err = des_read_sys_status(context, &status);

    assert_int_equal(status.value, status_value);

    assert_false(status.flags.encoder_index_status);
    assert_true(status.flags.rotor_position_status);
    assert_true(status.flags.enable_input_debuncing_enable);

    assert_int_equal(err, DES_OK);
}

static void test_des_clear_errors(void **ctx)
{
    state_t **state = (state_t **)ctx;
    des_context *context = (*state)->context;

    expect_value(__wrap_des_send_frame, opcode, 0x03);
    expect_value(__wrap_des_send_frame, len, 0);
    expect_value(__wrap_des_send_frame, data, NULL);

    will_return(__wrap_des_send_frame, DES_OK);

    des_error err = des_clear_errors(context);

    assert_int_equal(err, DES_OK);
}

static void test_des_reset(void **ctx)
{
    state_t **state = (state_t **)ctx;
    des_context *context = (*state)->context;

    expect_value(__wrap_des_send_frame, opcode, 0x04);
    expect_value(__wrap_des_send_frame, len, 0);
    expect_value(__wrap_des_send_frame, data, NULL);

    will_return(__wrap_des_send_frame, DES_OK);

    des_error err = des_reset(context);

    assert_int_equal(err, DES_OK);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_des_read_sys_status),
        cmocka_unit_test(test_des_clear_errors),
        cmocka_unit_test(test_des_reset),
    };
    return cmocka_run_group_tests(tests, setup, teardown);
}