#pragma once

#include "decomp.h"
#include "Ac/dAcNpc.h"

// vtable +0x3098 in ModulePlayerGhost.cro, offset_to_top 0, 89 entries
class AcNpcPlayerGhost : public ::AcNpc
{
public:
    class TalkRecept;
    AcNpcPlayerGhost(); // ctor address unknown
    virtual ~AcNpcPlayerGhost(); // ModulePlayerGhost.cro +0x001A58 slot 0x00
    virtual void Initialize(); // ModulePlayerGhost.cro +0x002034 slot 0x0C
    virtual void Finalize(); // ModulePlayerGhost.cro +0x0020D0 slot 0x18
    virtual void Unk0(); // ModulePlayerGhost.cro +0x001D18 slot 0x3C
};
