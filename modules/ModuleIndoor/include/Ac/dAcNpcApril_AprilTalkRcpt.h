#pragma once

#include "decomp.h"
#include "Ac/dAcNpcApril.h"
#include "Ac/dAcNpcNml.h"
#include "Ac/dAcNpcNml_TalkRcpt.h"

// vtable +0x67CF8 in ModuleIndoor.cro, offset_to_top 0, 85 entries
// vtable +0x67E54 in ModuleIndoor.cro, offset_to_top -124, 14 entries
class AcNpcApril::AprilTalkRcpt : public ::AcNpcNml::TalkRcpt
{
public:
    AprilTalkRcpt(); // ctor address unknown
    virtual ~AprilTalkRcpt(); // ModuleIndoor.cro +0x004ED8 slot 0x00
};
