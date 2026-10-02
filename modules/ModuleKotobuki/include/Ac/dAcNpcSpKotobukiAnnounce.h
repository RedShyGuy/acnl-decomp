#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x4544 in ModuleKotobuki.cro, offset_to_top 0, 89 entries
class AcNpcSpKotobukiAnnounce : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpKotobukiAnnounce(); // ctor address unknown
    virtual ~AcNpcSpKotobukiAnnounce(); // ModuleKotobuki.cro +0x002C64 slot 0x00
    virtual void Initialize(); // ModuleKotobuki.cro +0x0030F0 slot 0x0C
    virtual void Finalize(); // ModuleKotobuki.cro +0x00318C slot 0x18
    virtual void Unk0(); // ModuleKotobuki.cro +0x002DE4 slot 0x3C
};
