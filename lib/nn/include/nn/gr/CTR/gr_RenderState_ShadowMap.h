#pragma once

#include "decomp.h"
#include "nn/gr/CTR/gr_RenderState.h"

class nn::gr::CTR::RenderState::ShadowMap
{
public:
    void MakeCommand(unsigned int*, bool, bool) const; // 0x0012E1E8 | fefates:bytes [tier B]
};
