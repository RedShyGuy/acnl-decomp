#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x10CB4 in ModuleTour.cro, offset_to_top 0, 89 entries
class AcNpcSpKappeisKidOne : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpKappeisKidOne(); // ctor address unknown
    virtual ~AcNpcSpKappeisKidOne(); // ModuleTour.cro +0x0090D4 slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleTour.cro +0x00EB90 slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleTour.cro +0x00EC2C slot 0x18
    virtual bool Unk0(); // ModuleTour.cro +0x00D224 slot 0x3C
};
