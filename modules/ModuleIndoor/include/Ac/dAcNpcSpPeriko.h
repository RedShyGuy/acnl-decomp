#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpPostOffice.h"

// vtable +0x65A68 in ModuleIndoor.cro, offset_to_top 0, 89 entries
class AcNpcSpPeriko : public ::AcNpcSpPostOffice
{
public:
    class TalkRecept;
    AcNpcSpPeriko(); // ctor address unknown
    virtual ~AcNpcSpPeriko(); // ModuleIndoor.cro +0x0127AC slot 0x00
};
