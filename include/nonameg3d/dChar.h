#pragma once

#include "decomp.h"

namespace nonameg3d {
// a string key of a tree map (the lookup tables of g3d by name; names are ours)
// RTTI N9nonameg3d4CharE @ 0x008D4244
// vtable 0x0090C674 (vptr 0x0090C67C), offset_to_top 0, 1 entries
class Char
{
public:
    // (inline)
    explicit Char(const char* name = NULL) : m_pName(name) {}
    // strcmp of the names
    virtual s32 Compare(const Char& other) const; // 0x002FA8C8 slot 0x00

    const char* m_pName; // 0x4
};
ASSERT_SIZE(Char, 0x8);
} // namespace nonameg3d
