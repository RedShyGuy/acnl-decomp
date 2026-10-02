#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 10BsEventMgr @ 0x008CAF60
// vtable 0x008EBCE4 (vptr 0x008EBCEC), offset_to_top 0, 16 entries
class BsEventMgr : public ::Base
{
public:
    class Recept;
    BsEventMgr(); // ctor address unknown
    virtual ~BsEventMgr(); // 0x0018FBE4 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0018FBC0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0018FA1C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0018FB20 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0018FA84 slot 0x24 | slot vf_0x24 of oml::framework::Process
};
