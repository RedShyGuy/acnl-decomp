#pragma once

#include "decomp.h"
#include "nn/gr/CTR/gr_RenderState.h"

class nn::gr::CTR::RenderState::AlphaTest
{
public:
    void MakeCommand(unsigned int*, bool) const; // 0x007270F0 | fefates:bytes [tier B]
};
