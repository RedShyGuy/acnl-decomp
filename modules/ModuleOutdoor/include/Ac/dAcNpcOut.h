#pragma once

#include "decomp.h"
#include "Ac/dAcNpcNml.h"

// vtable +0x982B0 in ModuleOutdoor.cro, offset_to_top 0, 106 entries
class AcNpcOut : public ::AcNpcNml
{
public:
    class OutTalkRcpt;
    AcNpcOut(); // ctor address unknown
    virtual ~AcNpcOut(); // ModuleOutdoor.cro +0x079A98 slot 0x00
    virtual void HandleCalcResult(oml::framework::Result); // ModuleOutdoor.cro +0x0658CC slot 0x28
};
