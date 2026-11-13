#pragma once

#include "decomp.h"

struct SvFgName;

class Field
{
public:
    void GetFg(long, long, unsigned char); // 0x002FEF9C | libgarden [tier A]
    // (libgarden: stage::Field::PlaceItem(const SvFgName&, math::Vector<int, 2>, u8); the vector is passed as x and z)
    void PlaceItem(const SvFgName& item, s32 x, s32 z, u8 flags); // 0x002FCA24

    // the fields by FieldName (name is ours)
    static Field* s_pFields[32];
};
