#pragma once

#include "decomp.h"
#include "ssys/ma/dFileData.h"
#include "ssys/ma/dHeapAllocator.h"

namespace sead {
class Heap;
} // namespace sead

// The icons of the game for StreetPass: the icon of the message box, of the messages and a third
// one, each loaded into an ExpHeap of its own. The game has one instance (address 0x009CAF80, built
// by its static initializer 0x00787A28). (Game code that is not decompiled yet; the names except the
// class are ours.)
// RTTI 8GameIcon @ 0x008CD4F8
// vtable 0x008F9C18 (vptr 0x008F9C20), offset_to_top 0, 2 entries
class GameIcon
{
public:
    GameIcon(); // ctor candidate(s) 0x00787A28 (unverified)
    virtual void vf_0x00(); // 0x006A968C slot 0x00 | virtual slot, introduced by GameIcon
    virtual void vf_0x04(); // 0x006A9644 slot 0x04 | virtual slot, introduced by GameIcon

    // load "CecMessageIcon" / "CecMessageBoxIcon" (true if it is there)
    bool LoadCecMessageIcon(sead::Heap* heap); // 0x006A9460 (name is ours)
    void ReleaseCecMessageIcon(); // 0x006A9568 (name is ours)
    bool LoadCecMessageBoxIcon(sead::Heap* heap); // 0x006A9584 (name is ours)
    void ReleaseCecMessageBoxIcon(); // 0x006A9628 (name is ours)

    ssys::ma::FileData m_BoxIcon;            // 0x04
    ssys::ma::FileData m_MessageIcon;        // 0x14
    ssys::ma::FileData m_Unknown24;          // 0x24
    ssys::ma::HeapAllocator m_BoxIconHeap;   // 0x34
    ssys::ma::HeapAllocator m_MessageIconHeap; // 0x3C
    ssys::ma::HeapAllocator m_Unknown44Heap; // 0x44
};

// (the instance of the game; name is ours)
extern GameIcon g_GameIcon;
