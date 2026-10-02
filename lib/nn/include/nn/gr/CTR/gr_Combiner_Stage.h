#pragma once

#include "decomp.h"
#include "nn/gr/CTR/gr_Combiner.h"

class nn::gr::CTR::Combiner::Stage
{
public:
    Stage(int); // 0x0034B398 | fefates:bytes-fuzzy [tier B]
    Stage(); // 0x0034B4E4 | fefates:bytes [tier B]
    void MakeCommand(unsigned int*) const; // 0x00728A80 | fefates:bytes [tier B]
};
