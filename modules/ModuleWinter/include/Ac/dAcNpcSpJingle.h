#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x12618 in ModuleWinter.cro, offset_to_top 0, 89 entries
class AcNpcSpJingle : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpJingle(); // ctor address unknown
    virtual ~AcNpcSpJingle(); // ModuleWinter.cro +0x00C410 slot 0x00
};
