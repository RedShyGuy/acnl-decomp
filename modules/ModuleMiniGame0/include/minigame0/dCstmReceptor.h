#pragma once

#include "decomp.h"
#include "script/dITalkRecept.h"
#include "sead/seadIDisposer.h"

namespace minigame0 {
// vtable +0xE548C in ModuleMiniGame0.cro, offset_to_top 0, 63 entries
// vtable +0xE5590 in ModuleMiniGame0.cro, offset_to_top -124, 2 entries
class CstmReceptor : public ::script::ITalkRecept, public ::sead::IDisposer
{
public:
    CstmReceptor(); // ctor address unknown
    virtual ~CstmReceptor(); // ModuleMiniGame0.cro +0x0AF0C4 slot 0x00
};
} // namespace minigame0
