#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpPerimi.h"
#include "Ac/dAcNpcSpPostOffice.h"
#include "Ac/dAcNpcSpPostOffice_PostOfficeTalkRecept.h"

// vtable +0x683B8 in ModuleIndoor.cro, offset_to_top 0, 92 entries
// vtable +0x68530 in ModuleIndoor.cro, offset_to_top -124, 14 entries
class AcNpcSpPerimi::TalkRecept : public ::AcNpcSpPostOffice::PostOfficeTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleIndoor.cro +0x0129B4 slot 0x00
};
