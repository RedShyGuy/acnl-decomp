#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 8BsGrowUp @ 0x008CD478
// vtable 0x008F9964 (vptr 0x008F996C), offset_to_top 0, 16 entries
class BsGrowUp : public ::Base
{
public:
    class Recept;
    BsGrowUp(); // ctor address unknown
    virtual ~BsGrowUp(); // 0x00117809 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001177ED slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0011759D slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00117675 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x001175D5 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00117539 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
