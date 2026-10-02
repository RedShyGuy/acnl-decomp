#pragma once

#include "decomp.h"
#include "Ac/dAcInsectCommon.h"

// vtable +0x15200 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x1559C in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x15A68 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x15F34 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x16400 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x168CC in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x16D98 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x17264 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x17694 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x17B60 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x1802C in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x184F8 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x189C4 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x18E90 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x1935C in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x19828 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x19CF4 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x1A1C0 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x1A68C in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x1AB58 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x1AF88 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x1B454 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x1B920 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x1BDEC in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x1C2B8 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x1C784 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x1CC50 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x1D11C in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x1D5E8 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x220B8 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x2219C in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x17078 in ModuleMusIns.cro, offset_to_top -720, 11 entries
// vtable +0x1BC00 in ModuleMusIns.cro, offset_to_top -720, 11 entries
// vtable +0x16214 in ModuleMusIns.cro, offset_to_top -724, 11 entries
// vtable +0x154E0 in ModuleMusIns.cro, offset_to_top -728, 11 entries
// vtable +0x166E0 in ModuleMusIns.cro, offset_to_top -732, 11 entries
// vtable +0x1C0CC in ModuleMusIns.cro, offset_to_top -732, 11 entries
// vtable +0x187D8 in ModuleMusIns.cro, offset_to_top -744, 11 entries
// vtable +0x1963C in ModuleMusIns.cro, offset_to_top -744, 11 entries
// vtable +0x1C598 in ModuleMusIns.cro, offset_to_top -744, 11 entries
// vtable +0x17E40 in ModuleMusIns.cro, offset_to_top -748, 11 entries
// vtable +0x1B734 in ModuleMusIns.cro, offset_to_top -748, 11 entries
// vtable +0x1D8C8 in ModuleMusIns.cro, offset_to_top -748, 11 entries
// vtable +0x1D3FC in ModuleMusIns.cro, offset_to_top -756, 11 entries
// vtable +0x17974 in ModuleMusIns.cro, offset_to_top -760, 11 entries
// vtable +0x1A4A0 in ModuleMusIns.cro, offset_to_top -760, 11 entries
// vtable +0x1A96C in ModuleMusIns.cro, offset_to_top -760, 11 entries
// vtable +0x19B08 in ModuleMusIns.cro, offset_to_top -764, 11 entries
// vtable +0x19FD4 in ModuleMusIns.cro, offset_to_top -764, 11 entries
// vtable +0x1CA64 in ModuleMusIns.cro, offset_to_top -768, 11 entries
// vtable +0x1B268 in ModuleMusIns.cro, offset_to_top -772, 11 entries
// vtable +0x19170 in ModuleMusIns.cro, offset_to_top -776, 11 entries
// vtable +0x1830C in ModuleMusIns.cro, offset_to_top -784, 11 entries
// vtable +0x1AE38 in ModuleMusIns.cro, offset_to_top -788, 11 entries
// vtable +0x1587C in ModuleMusIns.cro, offset_to_top -796, 11 entries
// vtable +0x18CA4 in ModuleMusIns.cro, offset_to_top -796, 11 entries
// vtable +0x15D48 in ModuleMusIns.cro, offset_to_top -804, 11 entries
// vtable +0x17544 in ModuleMusIns.cro, offset_to_top -804, 11 entries
// vtable +0x16BAC in ModuleMusIns.cro, offset_to_top -824, 11 entries
// vtable +0x1CF30 in ModuleMusIns.cro, offset_to_top -1180, 11 entries
class AcInsectMuseumBase : public ::AcInsectCommon
{
public:
    AcInsectMuseumBase(); // ctor address unknown
    virtual ~AcInsectMuseumBase(); // ModuleMusIns.cro +0x0104D4 slot 0x00
    virtual void Unk0(); // ModuleMusIns.cro +0x011F30 slot 0x3C
};
