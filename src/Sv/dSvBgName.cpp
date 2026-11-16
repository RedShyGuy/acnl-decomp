#include "Sv/dSvBgName.h"

// 0x006060C0 (name is ours)
bool IsOutdoorBgName(u32 name)
{
    return name <= BG_NAME_GRD_CAMP_1;
}

// 0x006060D0 (name is ours)
bool IsIndoorBgName(u32 name)
{
    return name - BG_NAME_IDR_ROOM_1_T_4_S < BG_NAME_NUM - BG_NAME_IDR_ROOM_1_T_4_S;
}
