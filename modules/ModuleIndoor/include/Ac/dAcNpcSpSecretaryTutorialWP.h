#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x6775C in ModuleIndoor.cro, offset_to_top 0, 89 entries
class AcNpcSpSecretaryTutorialWP : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpSecretaryTutorialWP(); // ctor address unknown
    virtual ~AcNpcSpSecretaryTutorialWP(); // ModuleIndoor.cro +0x04201C slot 0x00
};
