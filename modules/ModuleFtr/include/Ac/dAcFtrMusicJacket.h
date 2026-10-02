#pragma once

#include "decomp.h"
#include "Ac/dAcFtr.h"

// vtable +0x29D38 in ModuleFtr.cro, offset_to_top 0, 67 entries
// vtable +0x29E4C in ModuleFtr.cro, offset_to_top -104, 4 entries
// vtable +0x29E64 in ModuleFtr.cro, offset_to_top -176, 72 entries
// vtable +0x29F8C in ModuleFtr.cro, offset_to_top -300, 14 entries
class AcFtrMusicJacket : public ::AcFtr
{
public:
    AcFtrMusicJacket(); // ctor address unknown
    virtual ~AcFtrMusicJacket(); // ModuleFtr.cro +0x017158 slot 0x00
};
