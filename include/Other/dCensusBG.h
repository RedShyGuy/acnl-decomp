#pragma once

#include "decomp.h"
#include "Other/dInOutWindow.h"

// RTTI 8CensusBG @ 0x008CD4D8
// vtable 0x008F9BB8 (vptr 0x008F9BC0), offset_to_top 0, 13 entries
class CensusBG : public ::InOutWindow
{
public:
    CensusBG(); // ctor address unknown
    virtual ~CensusBG(); // 0x006A375C slot 0x00 | slot vf_0x00 of InOutWindow
    // 0x006A3738 slot 0x04 | slot vf_0x04 of InOutWindow (deleting dtor)
    virtual void vf_0x28(); // 0x002B8030 slot 0x28 | virtual slot, introduced by InOutWindow
};
