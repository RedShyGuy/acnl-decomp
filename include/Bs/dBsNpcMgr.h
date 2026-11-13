#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "stage/stage_Api.h"

// RTTI 8BsNpcMgr @ 0x008CD4A4
// vtable 0x008F9A08 (vptr 0x008F9A10), offset_to_top 0, 16 entries
class BsNpcMgr : public ::Base
{
public:
    class NpcForeachFunction;
    class NpcPosGetCB;
    class SearchFunction;
    class NpcForeachWithIndexFunction;
    BsNpcMgr(); // ctor candidate(s) 0x006A27F0 (unverified)
    virtual ~BsNpcMgr(); // 0x0011817D slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00118171 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Initialize(); // 0x00117C3D slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual oml::framework::Result Finalize(); // 0x00100685 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual oml::framework::Result Calc(); // 0x006A26E8 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x006A266C slot 0x30 | slot vf_0x30 of oml::framework::Process

    // (names are ours; only what the stage code and the Bug-Off use)
    static BsNpcMgr* s_pInstance;
    // the number of NPCs of the list
    u32 GetNpcNum() const { return m_pNpcsEnd - m_pNpcsBegin; }
    // creates the special NPCs of a stage (command 3 of a stage definition)
    bool CreateSpecialNpcs(const stage::Command* command); // 0x006967BC

    u8 unk_0x14[0xEB54 - 0x14];
    AcNpc** m_pNpcsBegin; // 0xEB54 the NPCs (a vector)
    AcNpc** m_pNpcsEnd;   // 0xEB58
};
