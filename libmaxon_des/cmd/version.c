#include "cmd.h"

#include <string.h>
#include <assert.h>

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
