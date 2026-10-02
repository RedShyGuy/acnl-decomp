#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpPostOffice.h"

// vtable +0x65BD4 in ModuleIndoor.cro, offset_to_top 0, 89 entries
class AcNpcSpPerimi : public ::AcNpcSpPostOffice
{
public:
    class TalkRecept;
    AcNpcSpPerimi(); // ctor address unknown
    virtual ~AcNpcSpPerimi(); // ModuleIndoor.cro +0x012D60 slot 0x00
};
