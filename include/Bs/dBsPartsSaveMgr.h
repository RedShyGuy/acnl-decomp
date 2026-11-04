#pragma once

#include "decomp.h"

// Instantiations found in the binary:
//   BsPartsSaveMgr<SvBestFriendList>  typeinfo 0x008CBC38  vtable 0x008F0088
//   BsPartsSaveMgr<svholder::Mail>  typeinfo 0x008CBC44  vtable 0x008F00D0
//   BsPartsSaveMgr<svholder::MyDesign>  typeinfo 0x008CBC50  vtable 0x008F0118
template <typename T0>
class BsPartsSaveMgr
{
public:
    // the state of the part save of T0 (names are ours, after SvGetBestFriendList: the data is returned while
    // s_HasData is set and s_IsDataBlocked is clear)
    static bool s_HasData;
    static T0* s_pData;
    static bool s_IsDataBlocked;
};
