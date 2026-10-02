#pragma once

#include "decomp.h"

class RandomPlacer
{
public:
    struct Info { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void DoRandom(void (*)(RandomPlacerUsual&, RandomPlacer&, RandomPlacer::Info const&), RandomPlacerUsual*, unsigned int); // 0x002FDBD8 | libgarden [tier A]
};
