#pragma once

#include "decomp.h"
#include "Ac/dAcNpcNml.h"

// vtable +0x65218 in ModuleIndoor.cro, offset_to_top 0, 105 entries
class AcNpcApril : public ::AcNpcNml
{
public:
    class AprilTalkRcpt;
    AcNpcApril(); // ctor address unknown
    virtual ~AcNpcApril(); // ModuleIndoor.cro +0x00552C slot 0x00
    virtual void Initialize(); // ModuleIndoor.cro +0x05FBF4 slot 0x0C
    virtual void Finalize(); // ModuleIndoor.cro +0x05FC90 slot 0x18
};
