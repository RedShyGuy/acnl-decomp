#pragma once

#include "decomp.h"
#include "script/dITalkRecept.h"

// vtable +0x30DEC in ModuleMiniGame1.cro, offset_to_top 0, 63 entries
class PaneponSvData : public ::script::ITalkRecept
{
public:
    PaneponSvData(); // ctor address unknown
    virtual ~PaneponSvData(); // ModuleMiniGame1.cro +0x010110 slot 0x00
};
