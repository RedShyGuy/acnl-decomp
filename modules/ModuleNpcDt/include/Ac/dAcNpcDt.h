#pragma once

#include "decomp.h"
#include "Ac/dAcNpcNml.h"

// vtable +0x873C in ModuleNpcDt.cro, offset_to_top 0, 105 entries
class AcNpcDt : public ::AcNpcNml
{
public:
    class DtTalkRcpt;
    AcNpcDt(); // ctor address unknown
    virtual ~AcNpcDt(); // ModuleNpcDt.cro +0x006940 slot 0x00
    virtual void Initialize(); // ModuleNpcDt.cro +0x007618 slot 0x0C
    virtual void Finalize(); // ModuleNpcDt.cro +0x0076B4 slot 0x18
    virtual void Unk0(); // ModuleNpcDt.cro +0x006A10 slot 0x3C
};
