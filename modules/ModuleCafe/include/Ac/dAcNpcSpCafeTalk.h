#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0xD864 in ModuleCafe.cro, offset_to_top 0, 89 entries
class AcNpcSpCafeTalk : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpCafeTalk(); // ctor address unknown
    virtual ~AcNpcSpCafeTalk(); // ModuleCafe.cro +0x006200 slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleCafe.cro +0x00CAAC slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleCafe.cro +0x00CB48 slot 0x18
    virtual bool Unk0(); // ModuleCafe.cro +0x00AF80 slot 0x3C
};
