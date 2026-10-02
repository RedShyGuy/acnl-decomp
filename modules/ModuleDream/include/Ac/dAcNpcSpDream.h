#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x1336C in ModuleDream.cro, offset_to_top 0, 89 entries
class AcNpcSpDream : public ::AcNpcSp
{
public:
    class AsyncNetAccessHandleHook;
    class TalkRecept;
    AcNpcSpDream(); // ctor address unknown
    virtual ~AcNpcSpDream(); // ModuleDream.cro +0x006B78 slot 0x00
    virtual void Initialize(); // ModuleDream.cro +0x00AA88 slot 0x0C
    virtual void Finalize(); // ModuleDream.cro +0x00AB24 slot 0x18
    virtual void Unk0(); // ModuleDream.cro +0x00A140 slot 0x3C
};
