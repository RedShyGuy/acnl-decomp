#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"

// RTTI 13BsMenuCatalog @ 0x008CB8CC
// vtable 0x008EED44 (vptr 0x008EED4C), offset_to_top 0, 25 entries
// vtable 0x008EEDB0 (vptr 0x008EEDB8), offset_to_top -40, 3 entries
class BsMenuCatalog : public ::MenuBase, public ::state::Mode<BsMenuCatalog>
{
public:
    BsMenuCatalog(); // ctor address unknown
    virtual ~BsMenuCatalog(); // 0x0021D380 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0021D370 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0021C82C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0021CFE0 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0021CCD0 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0021C764 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void FUN_00767b30(); // 0x00713F88 slot 0x60 | slot vf_0x60 of MenuBase
};
