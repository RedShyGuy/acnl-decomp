#pragma once

#include "decomp.h"
#include "Ac/dAcNpc.h"

// vtable +0x67B8C in ModuleIndoor.cro, offset_to_top 0, 89 entries
class AcNpcCamp : public ::AcNpc
{
public:
    class CampTalkRcpt;
    AcNpcCamp(); // ctor address unknown
    virtual ~AcNpcCamp(); // ModuleIndoor.cro +0x058F90 slot 0x00
};
