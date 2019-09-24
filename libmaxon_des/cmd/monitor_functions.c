#include "cmd.h"

#include <assert.h>

void cmd_get_speed(cmd_context * context) {
    assert(context != NULL);

    des_reading reading;
    des_error err = des_read_velocity(context->des, false, &reading);

    if(err) {
        printf("error getting speed: %s\n", des_strerror(err));
        return;
    }

    printf("velocity:\n");
    printf("  real: %d\n", reading.real);
    printf("  requested: %d\n", reading.requested);
}