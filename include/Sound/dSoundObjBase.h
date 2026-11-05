#pragma once

#include "decomp.h"

// RTTI 12SoundObjBase @ 0x008CB818
// vtable 0x008EE838 (vptr 0x008EE840), offset_to_top 0, 7 entries
class SoundObjBase
{
public:
    SoundObjBase(); // ctor candidate(s) 0x0020B4D4 (unverified)
    virtual ~SoundObjBase(); // 0x0020B570 slot 0x00 | slot vf_0x00 of SoundObjBase
    // 0x0020B56C slot 0x04 | slot vf_0x04 of SoundObjBase (deleting dtor)
    virtual void vf_0x08(); // 0x0020AA88 slot 0x08 | virtual slot, introduced by SoundObjBase
    virtual void vf_0x0C(); // 0x0020B498 slot 0x0C | virtual slot, introduced by SoundObjBase
    virtual void vf_0x10(); // 0x0020B3E8 slot 0x10 | virtual slot, introduced by SoundObjBase
    virtual void vf_0x14(); // 0x0020B264 slot 0x14 | virtual slot, introduced by SoundObjBase
    // the volume, pan and surround pan from the position relative to the listener
    virtual void Calc(); // 0x0020AC30 slot 0x18

    // (the members are being written; names are ours)
    s32 m_Priority;      // 0x04
    f32 m_PosX;          // 0x08 relative to the listener
    f32 m_PosZ;          // 0x0C
    f32 _10;             // 0x10
    f32 m_VolumeScale;   // 0x14
    f32 m_DistanceVolume; // 0x18
    f32 m_Volume;        // 0x1C
    f32 m_Pan;           // 0x20
    f32 m_SurroundPan;   // 0x24
    s32 m_PlayerPriority; // 0x28
    f32 m_Distance;      // 0x2C
    f32 _30;             // 0x30
    u8 m_IsActive;       // 0x34
    s8 _35;              // 0x35
    s8 _36;              // 0x36
    s8 _37;              // 0x37
    u8 _38;              // 0x38
    s8 _39;              // 0x39
    s32 m_Type;          // 0x3C
    s32 m_Counter;       // 0x40
};
ASSERT_SIZE(SoundObjBase, 0x44);
