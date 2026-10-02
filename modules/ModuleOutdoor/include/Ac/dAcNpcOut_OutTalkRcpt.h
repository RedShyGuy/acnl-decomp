#pragma once

#include "decomp.h"
#include "Ac/dAcNpcNml.h"
#include "Ac/dAcNpcNml_TalkRcpt.h"
#include "Ac/dAcNpcOut.h"

// vtable +0x98E0C in ModuleOutdoor.cro, offset_to_top 0, 85 entries
// vtable +0x98F68 in ModuleOutdoor.cro, offset_to_top -124, 14 entries
class AcNpcOut::OutTalkRcpt : public ::AcNpcNml::TalkRcpt
{
public:
    OutTalkRcpt(); // ctor address unknown
    virtual ~OutTalkRcpt(); // ModuleOutdoor.cro +0x0656C0 slot 0x00
};
