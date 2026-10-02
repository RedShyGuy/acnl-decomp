#pragma once

#include "decomp.h"

class FieldRect
{
public:
    struct iterator { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void end() const; // 0x00748F04 | libgarden [tier A]
    void begin() const; // 0x00748F3C | libgarden [tier A]
};
