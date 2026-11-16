#pragma once

#include "decomp.h"
#include "Sv/dSvFgName.h"

namespace sead {
class Heap;
} // namespace sead

// the data of the current tour (names are ours; only what the code reads; the class name is from the RTTI of its
// SingletonDisposer_)
class TourData
{
public:
    class SingletonDisposer_;

    // a fossil in a template hut of the Fossil Finder's Tour (an even hut; names are ours)
    struct HutFossil
    {
        s32 x;      // 0x0 the unit
        s32 z;      // 0x4
        u16 item;   // 0x8 SvFgName::ID
        u8 unk_0xA; // 0xA (Func76A388 of the item)
    };
    // the fossils of one hut
    static const u32 HUT_FOSSIL_NUM = 4;

    // the kind of game of the current tour (-1 without a tour; name after the address until it is decompiled)
    s8 Func76F6E8() const; // 0x0076F6E8
    // whether the list of 8 items of the tour type (a table 0x0095D704 of 12 lists) has an item (this is not used)
    bool Func76F678(s8 tourType) const; // 0x0076F678

    // the instance (292 bytes) in heap, with its SingletonDisposer_ (name after the sead singletons)
    static TourData* createInstance(sead::Heap* heap); // 0x006C4634

    // (the SingletonDisposer_ clears it; name is ours)
    static TourData* s_pInstance;

    u8 _unknown0[0x10];
    // the seed of the random numbers of the tour fields (the island huts); the functions of the tour read the part from
    // here on (Func76F7E4)
    u32 m_Seed;             // 0x010
    // the item of the tour (the flower picking: the flower to pick; NAME_EMPTY: any)
    SvFgName m_TourItem;    // 0x014
    // the half of the places for treasures of the treasure hunt (isfactory::TreasureHuntBuilder::Setup1C)
    s32 m_TreasurePairNum;  // 0x018
    s8 m_Tour;              // 0x01C the tour (see GetTourType)
    u8 _unknown1D[7];
    union {
        // the fossils of the even huts of the tour of type 9 (ishutfact2::FossilTemplateBuilder::SetupGrass)
        HutFossil m_HutFossils[4][HUT_FOSSIL_NUM]; // 0x024
        // the items of other tours (the maze reads 12; SvFgName::NAME_EMPTY: none)
        u16 m_Items[0x60]; // 0x024
    };
    u8 _unknownE4[0x28];
    u8 m_State;             // 0x10C
    u8 m_OceanSide;         // 0x10D
};
