#pragma once

#include "decomp.h"
#include "Ac/dAcNpc.h"

// vtable +0x658FC in ModuleIndoor.cro, offset_to_top 0, 89 entries
class AcNpcAutoCamp : public ::AcNpc
{
public:
    class AutoCampTalkRcpt;
    AcNpcAutoCamp(); // ctor address unknown
    virtual ~AcNpcAutoCamp(); // ModuleIndoor.cro +0x012374 slot 0x00
    virtual void Calc(); // ModuleIndoor.cro +0x011FC8 slot 0x24
};
