#pragma once

#include "decomp.h"
#include "Ac/dAcNpc.h"

// vtable +0xDD6C in ModuleAutoCamp.cro, offset_to_top 0, 89 entries
class AcNpcAutoCampOut : public ::AcNpc
{
public:
    class AutoCampOutTalkRcpt;
    AcNpcAutoCampOut(); // ctor address unknown
    virtual ~AcNpcAutoCampOut(); // ModuleAutoCamp.cro +0x008EEC slot 0x00
    virtual void Initialize(); // ModuleAutoCamp.cro +0x00C358 slot 0x0C
    virtual void Finalize(); // ModuleAutoCamp.cro +0x00C3F4 slot 0x18
};
