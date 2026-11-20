#pragma once

#include "decomp.h"
#include "netgame/netgame_Api.h"
#include "Other/dDemoActor.h"
#include "stage/stage_Api.h"
#include "Utl/dUtlBase.h"

// RTTI 5AcNpc @ 0x008CD28C
// vtable 0x008F8844 (vptr 0x008F884C), offset_to_top 0, 89 entries
class AcNpc : public ::UtlBase<DemoActor>
{
public:
    class WanderSearchCandCB;
    AcNpc(); // ctor address unknown
    virtual ~AcNpc(); // 0x0057C3E0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0057C348 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void HandleInitializationResult(oml::framework::Result); // 0x0010C669 slot 0x10 | slot vf_0x10 of oml::framework::Process
    virtual oml::framework::Result Calc(); // 0x0057AF5C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x0057ADCC slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual bool vf_0x48(DemoActor* actor); // 0x007536AC slot 0x48 | virtual slot, introduced by DemoActor
    virtual void vf_0x50(u8 message, netgame::PlayerNo player); // 0x0057BBBC slot 0x50 | virtual slot, introduced by DemoActor
    virtual void vf_0x58(); // 0x00752DB8 slot 0x58 | virtual slot, introduced by DemoActor
    virtual void vf_0x5C(); // 0x00752DAC slot 0x5C | virtual slot, introduced by DemoActor
    virtual void vf_0x60(); // 0x00753238 slot 0x60 | virtual slot, introduced by DemoActor
    virtual bool vf_0x68(u32 value); // 0x00578A78 slot 0x68 | virtual slot, introduced by DemoActor
    virtual void vf_0x6C(); // 0x00578C24 slot 0x6C | virtual slot, introduced by DemoActor
    virtual bool vf_0x70(DemoActor* actor); // 0x00752DD8 slot 0x70 | virtual slot, introduced by DemoActor
    bool StartInitialize() override; // 0x00306990 slot 0x78
    oml::framework::Result UpdateInitialize() override; // 0x00304175 slot 0x7C
    bool EndInitialize() override; // 0x003064C4 slot 0x80
    bool StartFinalize() override; // 0x0010C6E1 slot 0x84
    oml::framework::Result UpdateFinalize() override; // 0x003041A9 slot 0x88
    bool EndFinalize() override; // 0x0010C6AD slot 0x8C
    virtual void vf_0x90(); // 0x00578480 slot 0x90 | virtual slot, introduced by AcNpc
    virtual void vf_0x94(); // 0x00578A1C slot 0x94 | virtual slot, introduced by AcNpc
    virtual void vf_0x98(); // 0x0075327C slot 0x98 | virtual slot, introduced by AcNpc
    virtual u32 vf_0x9C(); // 0x00579B54 slot 0x9C | virtual slot, introduced by AcNpc
    virtual void vf_0xA0(); // 0x0057A514 slot 0xA0 | virtual slot, introduced by AcNpc
    virtual void vf_0xA4(); // 0x00579EB8 slot 0xA4 | virtual slot, introduced by AcNpc
    virtual void vf_0xA8(); // 0x00752F54 slot 0xA8 | virtual slot, introduced by AcNpc
    virtual void vf_0xAC(); // 0x007531EC slot 0xAC | virtual slot, introduced by AcNpc
    virtual void vf_0xB0(); // 0x007536A4 slot 0xB0 | virtual slot, introduced by AcNpc
    virtual void vf_0xB4(); // 0x007531B8 slot 0xB4 | virtual slot, introduced by AcNpc
    virtual void vf_0xB8(); // 0x0057BA04 slot 0xB8 | virtual slot, introduced by AcNpc
    virtual void OnBuild(s32 x0, s32 z0, s32 x1, s32 z1, u8 flag); // 0x00578534 slot 0xBC
    virtual void OnPreview(s32 x0, s32 z0, s32 x1, s32 z1, u8 flag); // 0x00578DAC slot 0xC0
    virtual void OnPreviewEnd(); // 0x005795EC slot 0xC4
    virtual s32 vf_0xC8(u32 a, u32 b); // 0x00579338 slot 0xC8 | virtual slot, introduced by AcNpc
    virtual void OnFallToPitfall(); // 0x00579D0C slot 0xCC
    virtual void OnStrugglePitfall(); // 0x0057A03C slot 0xD0
    virtual void OnMoveOutPitfall(); // 0x00579EC8 slot 0xD4
    virtual void OnPlayerStartEmoticon(u8 emoticon); // 0x0057A680 slot 0xD8
    virtual void OnPlayerEndEmoticon(u8 emoticon); // 0x0057A64C slot 0xDC
    virtual void vf_0xE0(); // 0x00578B90 slot 0xE0 | virtual slot, introduced by AcNpc
    virtual void vf_0xE4(); // 0x00578DE0 slot 0xE4 | virtual slot, introduced by AcNpc
    virtual s32 vf_0xE8(const void* arg); // 0x0057BF74 slot 0xE8
    virtual bool vf_0xEC(const void* arg); // 0x00578D40 slot 0xEC
    virtual s32 vf_0xF0(); // 0x0057A064 slot 0xF0 | virtual slot, introduced by AcNpc
    virtual void vf_0xF4(); // 0x00579EB0 slot 0xF4 | virtual slot, introduced by AcNpc
    virtual void vf_0xF8(); // 0x0075328C slot 0xF8 | virtual slot, introduced by AcNpc
    virtual void vf_0xFC(); // 0x00579C58 slot 0xFC | virtual slot, introduced by AcNpc
    virtual void vf_0x100(); // 0x00753274 slot 0x100 | virtual slot, introduced by AcNpc
    virtual void vf_0x104(); // 0x00579D04 slot 0x104 | virtual slot, introduced by AcNpc
    virtual void vf_0x108(); // 0x00753284 slot 0x108 | virtual slot, introduced by AcNpc
    virtual void vf_0x10C(); // 0x0057A5F4 slot 0x10C | virtual slot, introduced by AcNpc
    virtual void vf_0x110(); // 0x00753294 slot 0x110 | virtual slot, introduced by AcNpc
    virtual void vf_0x114(); // 0x00579CA4 slot 0x114 | virtual slot, introduced by AcNpc
    virtual void vf_0x118(); // 0x0057BF7C slot 0x118 | virtual slot, introduced by AcNpc
    virtual void vf_0x11C(); // 0x00578EDC slot 0x11C | virtual slot, introduced by AcNpc
    virtual void vf_0x120(); // 0x0057848C slot 0x120 | virtual slot, introduced by AcNpc
    virtual void vf_0x124(); // 0x00578C40 slot 0x124 | virtual slot, introduced by AcNpc
    virtual void vf_0x128(); // 0x00579430 slot 0x128 | virtual slot, introduced by AcNpc
    virtual void vf_0x12C(); // 0x00579CA0 slot 0x12C | virtual slot, introduced by AcNpc
    virtual void vf_0x130(); // 0x00579CA8 slot 0x130 | virtual slot, introduced by AcNpc
    virtual void vf_0x134(); // 0x0057A4BC slot 0x134 | virtual slot, introduced by AcNpc
    virtual void vf_0x138(); // 0x00579334 slot 0x138 | virtual slot, introduced by AcNpc
    virtual void vf_0x13C(); // 0x00578484 slot 0x13C | virtual slot, introduced by AcNpc
    virtual void vf_0x140(); // 0x00578DA8 slot 0x140 | virtual slot, introduced by AcNpc
    virtual void vf_0x144(); // 0x0057850C slot 0x144 | virtual slot, introduced by AcNpc
    virtual void vf_0x148(); // 0x00579754 slot 0x148 | virtual slot, introduced by AcNpc
    virtual void vf_0x14C(); // 0x00579CE4 slot 0x14C | virtual slot, introduced by AcNpc
    virtual void vf_0x150(); // 0x00579340 slot 0x150 | virtual slot, introduced by AcNpc
    virtual void vf_0x154(); // 0x00579C28 slot 0x154 | virtual slot, introduced by AcNpc
    virtual void vf_0x158(); // 0x0057A5FC slot 0x158 | virtual slot, introduced by AcNpc
    virtual void vf_0x15C(); // 0x00578C38 slot 0x15C | virtual slot, introduced by AcNpc
    virtual void vf_0x160(); // 0x00578C3C slot 0x160 | virtual slot, introduced by AcNpc

    // (game code of AcNpc, not decompiled yet; meaning unknown)
    s32 Func57A600(u32 arg); // 0x0057A600
    bool Func57A8FC(); // 0x0057A8FC

    // (the members that BsNpcMgr reads; the layout of AcNpc follows with its own code, names are ours)
    u32 _unk68[(0xA8 - 0x68) / 4]; // (words: GCC would put bytes into the tail padding of the base, ARMCC does not)
    u8 m_UnknownA8;              // 0xA8 (BsNpcMgr::SetupAsrWords leaves out a special character with 27)
    u8 _unkA9[3];
    u32 _unkAC[(0x938 - 0xAC) / 4];
    u32 m_Unknown938;            // 0x938 (BsNpcMgr::SetValueOfKind)
    u32 _unk93C[(0x95C - 0x93C) / 4];
    u8 _unk95C[2];
    u8 m_Kind;                   // 0x95E BsNpcMgr finds an NPC of a kind (meaning of the values unknown)
    u8 _unk95F[0x9A8 - 0x95F];
    stage::NpcInfo m_Info;       // 0x9A8 how the NPC is identified (BsNpcMgr::FindNpc)
    u32 _unk9B0[(0xB1C - 0x9B0) / 4];
    u8 _unkB1C[2];
    bool m_UnknownB1E;           // 0xB1E (set for a villager of BsNpcMgr::CreateAnniversaryNpcs)
    u8 _unkB1F;
    u32 _unkB20[(0xDA4 - 0xB20) / 4];
    u8 _unkDA4;
    u8 m_UnknownDA5;             // 0xDA5 (BsNpcMgr::CreateAnniversaryNpcs sets the bit 4)
    u8 _unkDA6[2];
    u32 _unkDA8[(0x1030 - 0xDA8) / 4];
    u32 m_Unknown1030;           // 0x1030 (BsNpcMgr::FindNpcByName)
};
ASSERT_OFFSET(AcNpc, m_Kind, 0x95E);
ASSERT_OFFSET(AcNpc, m_Info, 0x9A8);
