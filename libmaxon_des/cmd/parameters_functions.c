#include "cmd.h"

#include <string.h>
#include <assert.h>

typedef struct
{
    const char *identifier;
    const char *name;
} parameter_detail;

void cmd_version(cmd_context *context)
{
    assert(context != NULL);

    des_version_info version;
    des_error err = des_read_version(context->des, &version);
    if (err != 0)
    {
        log_error("error reading version number: %d", err);

        return;
    }

    printf("version:\n");
    printf("  hardware version: 0x%x\n", version.hardware_version);
    printf("  software version: 0x%x\n", version.software_version);
}

static char *parameter_indexes[1] = {
    "baudrate",
};
size_t num_parameters = 1;

uint16_t parameters_mapping(const char *name)
{
    for (int i = 0; i < num_parameters; i++)
    {
        if (strcmp(parameter_indexes[i], name) == 0)
        {
            return i;
        }
    }
    return -1;
}

void cmd_read_tmp_param(cmd_context *context, char *param)
{
    assert(context != NULL);
    des_error err;
    des_sys_params parameters;

    uint16_t param_idx = parameters_mapping(param);

    if (param_idx == -1)
    {
        printf("parameter does not exist.\n");

        return;
    }

    uint16_t value;

    err = des_read_temp_param(context->des, param_idx, &value);
    if (err)
    {
        printf("error getting temporary parameter: %s\n", des_strerror(err));
        exit(-1);
    }
    printf("temporary parameter:\n");
    printf("  parameter value: %d\n", value);
}
