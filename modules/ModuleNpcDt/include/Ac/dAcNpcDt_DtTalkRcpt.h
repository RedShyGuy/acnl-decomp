#pragma once

#include "decomp.h"
#include "Ac/dAcNpcDt.h"
#include "Ac/dAcNpcNml.h"
#include "Ac/dAcNpcNml_TalkRcpt.h"

// vtable +0x89F8 in ModuleNpcDt.cro, offset_to_top 0, 85 entries
// vtable +0x8B54 in ModuleNpcDt.cro, offset_to_top -124, 14 entries
class AcNpcDt::DtTalkRcpt : public ::AcNpcNml::TalkRcpt
{
public:
    DtTalkRcpt(); // ctor address unknown
    virtual ~DtTalkRcpt(); // ModuleNpcDt.cro +0x0022C4 slot 0x00
};
