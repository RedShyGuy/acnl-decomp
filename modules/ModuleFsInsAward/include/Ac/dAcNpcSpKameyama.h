#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x5470 in ModuleFsInsAward.cro, offset_to_top 0, 89 entries
class AcNpcSpKameyama : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpKameyama(); // ctor address unknown
    virtual ~AcNpcSpKameyama(); // ModuleFsInsAward.cro +0x004920 slot 0x00
    virtual void Initialize(); // ModuleFsInsAward.cro +0x004CE8 slot 0x0C
    virtual void Finalize(); // ModuleFsInsAward.cro +0x004D84 slot 0x18
    virtual void Unk0(); // ModuleFsInsAward.cro +0x004AA4 slot 0x3C
};
