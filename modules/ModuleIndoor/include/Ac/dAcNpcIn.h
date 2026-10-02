#pragma once

#include "decomp.h"
#include "Ac/dAcNpcNml.h"

// vtable +0x67934 in ModuleIndoor.cro, offset_to_top 0, 105 entries
class AcNpcIn : public ::AcNpcNml
{
public:
    class InTalkRcpt;
    AcNpcIn(); // ctor address unknown
    virtual ~AcNpcIn(); // ModuleIndoor.cro +0x0511AC slot 0x00
};
