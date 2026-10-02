#pragma once

#include "decomp.h"

namespace sead {
class Color4f
{
public:
    void adjustOverflow_(); // 0x0055DFF8 | nintendogs:bytes [tier A]
    void setLerp(const sead::Color4f&, const sead::Color4f&, float); // 0x0055E11C | nintendogs:bytes [tier A]
};
} // namespace sead
