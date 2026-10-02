#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x669AC in ModuleIndoor.cro, offset_to_top 0, 89 entries
class AcNpcSpSecretaryIn : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpSecretaryIn(); // ctor address unknown
    virtual ~AcNpcSpSecretaryIn(); // ModuleIndoor.cro +0x038610 slot 0x00
};
