#include "maxon_des.h"
#include "maxon_des_functions.h"
#include "maxon_des_comm.h"

DES_FN_ONE_PARAM_NO_RET(des_set_velocity, 0x21, int16_t, velocity, velocity)

DES_FN_ONE_PARAM_NO_RET(des_set_current, 0x22, int16_t, current, current)

DES_FN_NO_PARAM_NO_RET(des_stop_motion, 0x23)