#pragma once

#include "decomp.h"
#include "nn/gr/CTR/gr_RenderState.h"

class nn::gr::CTR::RenderState::Culling
{
public:
    void MakeCommand(unsigned int*, bool) const; // 0x00727074 | fefates:bytes [tier B]
};
