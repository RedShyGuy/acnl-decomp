#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x127EC in ModuleWinter.cro, offset_to_top 0, 89 entries
class AcNpcSpFrankrin : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpFrankrin(); // ctor address unknown
    virtual ~AcNpcSpFrankrin(); // ModuleWinter.cro +0x00E318 slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleWinter.cro +0x010EEC slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleWinter.cro +0x010F88 slot 0x18
    virtual bool Unk0(); // ModuleWinter.cro +0x0109A0 slot 0x3C
};
