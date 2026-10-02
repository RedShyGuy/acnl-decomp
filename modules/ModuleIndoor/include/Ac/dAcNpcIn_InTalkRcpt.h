#pragma once

#include "decomp.h"
#include "Ac/dAcNpcIn.h"
#include "Ac/dAcNpcNml.h"
#include "Ac/dAcNpcNml_TalkRcpt.h"

// vtable +0x69A60 in ModuleIndoor.cro, offset_to_top 0, 85 entries
// vtable +0x69BBC in ModuleIndoor.cro, offset_to_top -124, 14 entries
class AcNpcIn::InTalkRcpt : public ::AcNpcNml::TalkRcpt
{
public:
    InTalkRcpt(); // ctor address unknown
    virtual ~InTalkRcpt(); // ModuleIndoor.cro +0x04AE08 slot 0x00
};
