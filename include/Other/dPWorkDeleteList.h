#pragma once

#include "decomp.h"
#include "Bs/dBsMenuMapBase.h"
#include "Other/dLayout.h"
#include "Other/dPWorkList.h"
#include "ssys/ma/lyt/dArcResAccReader.h"

struct SvStrc;
struct SvStrcData;

// (game code, not decompiled; names are ours) a structure of the town (SvStrcData::strcs, index below 56), whether it
// is none (id 252 or more) or can be demolished, the low byte of its id, the public works project of an id; the acre
// of a map position (x, y) of the town map from SvTownData::oceanSide on (the acres follow at +4)
SvStrc* Func75A28C(SvStrcData* strcData, s32 index); // 0x0075A28C
bool Func718EC8(const SvStrc* strc); // 0x00718EC8
bool Func6CC848(const SvStrc* strc); // 0x006CC848
u8 Func302720(const SvStrc* strc); // 0x00302720
u8 Func5CA270(u8 id); // 0x005CA270
u16* Func75EBB4(u8* townMap, s32 x, s32 y); // 0x0075EBB4

// The list of the public works projects of the town that can be demolished, next to a map of the town that marks the
// selected one. The items are collected over four frames. Only the class name is from the RTTI; all other names are
// ours.
// RTTI 15PWorkDeleteList @ 0x008CC104
// vtable 0x008F1DDC (vptr 0x008F1DE4), offset_to_top 0, 26 entries
class PWorkDeleteList : public ::PWorkList
{
public:
    // the map panes P_mp_NN of the acres, by row and column
    static const s32 MAP_SIZE = 5;

    // (inline, in CatalogBase::Create<PWorkDeleteList>)
    PWorkDeleteList() : m_SetupStep(0) {}
    virtual ~PWorkDeleteList(); // 0x0029EEFC slot 0x00
    // 0x0029EE90 slot 0x04 (deleting dtor)
    void Finalize() override; // 0x0022344C slot 0x0C (falls into BsMenuMapBase::Func223478)
    void Update() override; // 0x00222E74 slot 0x10 (falls into BsMenuMapBase::Func222E90)
    void Draw() override; // 0x00222B20 slot 0x14 (falls into BsMenuMapBase::Func222B3C)
    // (the index of the structure of the selected project, not its ID)
    s32 GetSelectedId() const override { return m_Selected != -1 ? m_StrcIndices[m_Selected] : -1; } // 0x0071CA24 slot 0x20
    bool vf_0x28() override { return false; } // 0x0071CA1C slot 0x28
    // the cost of the demolition of the selected project
    u32 vf_0x2C() override; // 0x0071CA40 slot 0x2C
    bool SetupItems() override; // 0x0029EA84 slot 0x34
    void OnScroll() override; // 0x0029E7D0 slot 0x3C
    void OnSelect(s32 selected, s32 prev) override; // 0x0029E754 slot 0x40
    void OnEnterIn() override; // 0x0029E918 slot 0x4C
    void UpdateIn() override; // 0x0029E864 slot 0x50
    void OnEnterOut() override; // 0x0029EA28 slot 0x54
    void UpdateOut() override; // 0x0029E96C slot 0x58
    void UpdateWait() override; // 0x0029EDFC slot 0x60

    // the map marks the structure of an item
    DECOMP_ALWAYS_INLINE void MarkStructure(s32 item);

    static const s32 s_MapPaneIndices[MAP_SIZE][MAP_SIZE];

    ssys::ma::lyt::ArcResAccReader m_MapArcReader;  // 0x3B70 Layout/Menu/map_village.arc
    ::Layout m_MapLayout;                           // 0x3DB8
    BsMenuMapBase m_Map;                            // 0x3F04
    s32 m_StrcIndices[PROJECT_NUM];                 // 0xF2F0 the structure of each item
    s32 m_SetupStep;                                // 0xF48C of SetupItems
};
ASSERT_OFFSET(PWorkDeleteList, m_MapArcReader, 0x3B70);
ASSERT_OFFSET(PWorkDeleteList, m_Map, 0x3F04);
ASSERT_OFFSET(PWorkDeleteList, m_StrcIndices, 0xF2F0);
ASSERT_SIZE(PWorkDeleteList, 0xF490);
