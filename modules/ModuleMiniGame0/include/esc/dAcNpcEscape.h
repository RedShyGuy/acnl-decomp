#pragma once

#include "decomp.h"
#include "Ac/dAcNpc.h"

namespace esc {
// vtable +0xDE7F4 in ModuleMiniGame0.cro, offset_to_top 0, 89 entries
class AcNpcEscape : public ::AcNpc
{
public:
    AcNpcEscape(); // ctor address unknown
    virtual ~AcNpcEscape(); // ModuleMiniGame0.cro +0x00EBA8 slot 0x00
    virtual void Initialize(); // ModuleMiniGame0.cro +0x0D7B6C slot 0x0C
    virtual void Finalize(); // ModuleMiniGame0.cro +0x0D7C08 slot 0x18
    virtual void Calc(); // ModuleMiniGame0.cro +0x00E6B8 slot 0x24
    virtual void OnNotify(); // ModuleMiniGame0.cro +0x007D34 slot 0x38
    virtual void Unk0(); // ModuleMiniGame0.cro +0x0B74B0 slot 0x3C
};
} // namespace esc
