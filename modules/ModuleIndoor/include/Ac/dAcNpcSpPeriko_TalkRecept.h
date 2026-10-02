#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpPeriko.h"
#include "Ac/dAcNpcSpPostOffice.h"
#include "Ac/dAcNpcSpPostOffice_PostOfficeTalkRecept.h"

// vtable +0x68204 in ModuleIndoor.cro, offset_to_top 0, 91 entries
// vtable +0x68378 in ModuleIndoor.cro, offset_to_top -124, 14 entries
class AcNpcSpPeriko::TalkRecept : public ::AcNpcSpPostOffice::PostOfficeTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleIndoor.cro +0x012518 slot 0x00
};
