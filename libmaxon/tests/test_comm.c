#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <memory.h>
#include <stdio.h>
#include <malloc.h>
#include <cmocka.h>

#include "mx_des.h"
#include "mx_des_comm.h"

int __wrap_read(int fn, char *bytes, size_t len)
{
    check_expected(len);

    size_t available = (size_t)mock();
    if (available)
        memcpy(bytes, (char *)mock(), available);

    return (int)mock();
}

int __wrap_write(int fn, char *bytes, size_t len)
{
    check_expected(len);
    check_expected_ptr(bytes);

    return (int)mock();
}

void test_write_byte(void **ctx)
{
    des_context context;
    context.port = 0;

    uint8_t data = 0x56;

    // Normal operation
    {
        expect_value(__wrap_write, len, 1);
        expect_memory(__wrap_write, bytes, &data, 1);
        will_return(__wrap_write, 1);

        des_error err = des_write_byte(&context, data);

        assert_int_equal(err, DES_OK);
    }

    // On DES_COMM_ERROR
    {
        expect_value(__wrap_write, len, 1);
        expect_memory(__wrap_write, bytes, &data, 1);
        will_return(__wrap_write, 0);

        des_error err = des_write_byte(&context, data);

        assert_int_equal(err, DES_COMM_ERROR);
    }
}

void test_write_word(void **ctx)
{
    des_context context;
    context.port = 0;

    uint16_t value = 0x5644;
    uint8_t data[2] = {0x44, 0x56};

    // Normal Operation
    {
        expect_value(__wrap_write, len, 2);
        expect_memory(__wrap_write, bytes, &data, 2);
        will_return(__wrap_write, 2);

        des_error err = des_write_word(&context, value);

        assert_int_equal(err, DES_OK);
    }
    // On DES_COMM_ERROR
    {
        expect_value(__wrap_write, len, 2);
        expect_memory(__wrap_write, bytes, &data, 2);
        will_return(__wrap_write, 0);

        des_error err = des_write_word(&context, value);

        assert_int_equal(err, DES_COMM_ERROR);
    }
}

void test_write_data(void **ctx)
{
    des_context context;
    context.port = 0;

    uint16_t value[3] = {0x4555, 0x3322, 0x6644};
    uint8_t data[6] = {0x55, 0x45, 0x22, 0x33, 0x44, 0x66};

    // Normal Operation
    {
        expect_value(__wrap_write, len, 6);
        expect_memory(__wrap_write, bytes, data, 6);
        will_return(__wrap_write, 6);

        des_error err = des_write_data(&context, value, 3);

        assert_int_equal(err, DES_OK);
    }
    // On DES_COMM_ERROR
    {
        expect_value(__wrap_write, len, 6);
        expect_memory(__wrap_write, bytes, data, 6);
        will_return(__wrap_write, 3);

        des_error err = des_write_data(&context, value, 3);

        assert_int_equal(err, DES_COMM_ERROR);
    }
}

void test_read_byte(void **ctx)
{
    des_context context;
    context.port = 0;

    // Normal operation
    {
        uint8_t data = 0x54;

        expect_value(__wrap_read, len, 1);
        will_return(__wrap_read, 1);
        will_return(__wrap_read, &data);
        will_return(__wrap_read, 1);

        uint8_t received;
        des_error err = des_read_byte(&context, &received);

        assert_int_equal(err, DES_OK);
    }
}

int main()
{
    const struct CMUnitTest tests[] = {
        // write functions
        cmocka_unit_test(test_write_byte),
        cmocka_unit_test(test_write_word),
        cmocka_unit_test(test_write_data),
        // read functions
        cmocka_unit_test(test_read_byte),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}