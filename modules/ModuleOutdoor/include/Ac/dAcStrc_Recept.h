#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "script/dITalkRecept.h"

// vtable +0x98C18 in ModuleOutdoor.cro, offset_to_top 0, 63 entries
class AcStrc::Recept : public ::script::ITalkRecept
{
public:
    Recept(); // ctor address unknown
    virtual ~Recept(); // ModuleOutdoor.cro +0x060000 slot 0x00
};
