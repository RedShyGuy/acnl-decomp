#pragma once

#include "decomp.h"
#include "nn/gr/CTR/gr_FragmentLight.h"

class nn::gr::CTR::FragmentLight::Source
{
public:
    Source(); // 0x00349A80 | nintendogs:bytes [tier A]
    void MakeAllCommand(unsigned*) const; // 0x007275FC | nintendogs:bytes [tier A]
};
