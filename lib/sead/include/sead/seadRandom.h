#pragma once

#include "decomp.h"

namespace sead {
class Random
{
public:
    void init();
    void init(unsigned); // 0x0055D5A4 | nintendogs:bytes [tier A]
};
} // namespace sead
