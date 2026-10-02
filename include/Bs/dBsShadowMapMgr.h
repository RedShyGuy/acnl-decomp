#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 14BsShadowMapMgr @ 0x008CBC5C
// vtable 0x008F0160 (vptr 0x008F0168), offset_to_top 0, 16 entries
class BsShadowMapMgr : public ::Base
{
public:
    BsShadowMapMgr(); // ctor candidate(s) 0x00266274 (unverified)
    virtual ~BsShadowMapMgr(); // 0x002662E8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002662BC slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00264CEC slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0026551C slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00264EF0 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00264CD4 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
