#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x65548 in ModuleIndoor.cro, offset_to_top 0, 89 entries
class AcNpcSpFuta : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpFuta(); // ctor address unknown
    virtual ~AcNpcSpFuta(); // ModuleIndoor.cro +0x00944C slot 0x00
};
