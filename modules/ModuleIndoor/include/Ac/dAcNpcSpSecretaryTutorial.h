#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x673C0 in ModuleIndoor.cro, offset_to_top 0, 89 entries
class AcNpcSpSecretaryTutorial : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpSecretaryTutorial(); // ctor address unknown
    virtual ~AcNpcSpSecretaryTutorial(); // ModuleIndoor.cro +0x040348 slot 0x00
};
