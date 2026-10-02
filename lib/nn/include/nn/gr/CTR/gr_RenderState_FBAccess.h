#pragma once

#include "decomp.h"
#include "nn/gr/CTR/gr_RenderState.h"

class nn::gr::CTR::RenderState::FBAccess
{
public:
    void MakeCommand(unsigned int*, bool) const; // 0x00134564 | fefates:bytes [tier B]
};
