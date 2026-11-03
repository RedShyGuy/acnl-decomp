#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 17BsSeadParticleMgr @ 0x008CC4C0
// vtable 0x008F35F0 (vptr 0x008F35F8), offset_to_top 0, 16 entries
class BsSeadParticleMgr : public ::Base
{
public:
    BsSeadParticleMgr(); // ctor candidate(s) 0x002C98B4 (unverified)
    virtual ~BsSeadParticleMgr(); // 0x002C98F8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002C98D4 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Initialize(); // 0x002C96D0 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual oml::framework::Result Finalize(); // 0x002C9870 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual oml::framework::Result Calc(); // 0x002C9834 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x002C96C8 slot 0x30 | slot vf_0x30 of oml::framework::Process

    // (names are ours) the instance (NULL: none)
    static BsSeadParticleMgr* GetInstance(); // 0x002C96B8
    // (empty: armlink removed the call in g3d::Render)
    void DrawScene(u32 /*drawMode*/) {}
};
