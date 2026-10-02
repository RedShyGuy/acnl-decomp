#pragma once

#include "decomp.h"
#include "nn/gr/CTR/gr_RenderState.h"

class nn::gr::CTR::RenderState::StencilTest
{
public:
    void MakeCommand(unsigned int*, bool) const; // 0x00726FE8 | fefates:bytes [tier B]
};
