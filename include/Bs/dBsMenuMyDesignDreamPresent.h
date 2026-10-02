#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 26BsMenuMyDesignDreamPresent @ 0x008CD084
// vtable 0x008F7D90 (vptr 0x008F7D98), offset_to_top 0, 25 entries
// vtable 0x008F7DFC (vptr 0x008F7E04), offset_to_top -40, 3 entries
class BsMenuMyDesignDreamPresent : public ::MenuBase, public ::state::Mode<BsMenuMyDesignDreamPresent>
{
public:
    BsMenuMyDesignDreamPresent(); // ctor address unknown
    virtual ~BsMenuMyDesignDreamPresent(); // 0x00342794 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00342784 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00341744 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x003420F4 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00341F5C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x003416B4 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
