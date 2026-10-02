#pragma once

#include "decomp.h"
#include "Ac/dAcFtrMonitor.h"

// vtable +0x2A788 in ModuleFtr.cro, offset_to_top 0, 74 entries
// vtable +0x2A8B8 in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x2A8D0 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x2A9F8 in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrMiniGameWiiU : public ::AcFtrMonitor
{
public:
    AcFtrMiniGameWiiU(); // ctor address unknown
    virtual ~AcFtrMiniGameWiiU(); // ModuleFtr.cro +0x018D74 slot 0x00
};
