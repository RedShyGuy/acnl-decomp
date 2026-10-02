#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x31CC in ModuleEventNpc.cro, offset_to_top 0, 89 entries
class AcNpcSpBerlina : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpBerlina(); // ctor address unknown
    virtual ~AcNpcSpBerlina(); // ModuleEventNpc.cro +0x001DD4 slot 0x00
    virtual void Initialize(); // ModuleEventNpc.cro +0x0020C0 slot 0x0C
    virtual void Finalize(); // ModuleEventNpc.cro +0x00215C slot 0x18
    virtual void Calc(); // ModuleEventNpc.cro +0x001A30 slot 0x24
    virtual void Unk0(); // ModuleEventNpc.cro +0x001F5C slot 0x3C
};
