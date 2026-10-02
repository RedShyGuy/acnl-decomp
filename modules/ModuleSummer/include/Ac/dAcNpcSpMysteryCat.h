#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x4590 in ModuleSummer.cro, offset_to_top 0, 89 entries
class AcNpcSpMysteryCat : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpMysteryCat(); // ctor address unknown
    virtual ~AcNpcSpMysteryCat(); // ModuleSummer.cro +0x003084 slot 0x00
    virtual void Initialize(); // ModuleSummer.cro +0x003BE8 slot 0x0C
    virtual void Finalize(); // ModuleSummer.cro +0x003C84 slot 0x18
    virtual void Unk0(); // ModuleSummer.cro +0x003940 slot 0x3C
};
