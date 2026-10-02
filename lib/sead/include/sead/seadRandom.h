#pragma once

#include "decomp.h"

namespace sead {
class Random
{
public:
    void init(); // 0x0053BEF0 | nintendogs:bytes [tier A]
    void init(unsigned); // 0x0055D5A4 | nintendogs:bytes [tier A]
};
} // namespace sead
