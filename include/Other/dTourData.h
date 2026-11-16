#pragma once

#include "decomp.h"

namespace sead {
class Heap;
} // namespace sead

// the data of the current tour (names are ours; only what the code reads; the class name is from the RTTI of its
// SingletonDisposer_)
class TourData
{
public:
    class SingletonDisposer_;

    // the kind of game of the current tour (-1 without a tour; name after the address until it is decompiled)
    s8 Func76F6E8() const; // 0x0076F6E8

    // the instance (292 bytes) in heap, with its SingletonDisposer_ (name after the sead singletons)
    static TourData* createInstance(sead::Heap* heap); // 0x006C4634

    // (the SingletonDisposer_ clears it; name is ours)
    static TourData* s_pInstance;

    u8 _unknown0[0x1C];
    s8 m_Tour;              // 0x01C the tour (see GetTourType)
    u8 _unknown1D[0xEF];
    u8 m_State;             // 0x10C
    u8 m_OceanSide;         // 0x10D
};
