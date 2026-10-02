#pragma once

#include "decomp.h"
#include "nn/gr/CTR/gr_RenderState.h"

class nn::gr::CTR::RenderState::Blend
{
public:
    void MakeCommand(unsigned int*, bool) const; // 0x0012DF08 | fefates:bytes [tier B]
};
