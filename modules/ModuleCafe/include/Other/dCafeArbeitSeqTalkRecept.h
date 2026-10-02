#pragma once

#include "decomp.h"
#include "Npc/dNpcDemoDollTalkRecept.h"

// vtable +0xD9D0 in ModuleCafe.cro, offset_to_top 0, 88 entries
// vtable +0xDB38 in ModuleCafe.cro, offset_to_top -124, 14 entries
class CafeArbeitSeqTalkRecept : public ::NpcDemoDollTalkRecept
{
public:
    CafeArbeitSeqTalkRecept(); // ctor address unknown
    virtual ~CafeArbeitSeqTalkRecept(); // ModuleCafe.cro +0x00A034 slot 0x00
};
