#pragma once

#include "decomp.h"

class Field
{
public:
    void GetFg(long, long, unsigned char); // 0x002FEF9C | libgarden [tier A]

    // the fields by FieldName (name is ours)
    static Field* s_pFields[32];
};
