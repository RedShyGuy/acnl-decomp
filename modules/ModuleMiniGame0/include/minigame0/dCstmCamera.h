#pragma once

#include "decomp.h"
#include "Other/dICameraUpdater.h"
#include "sead/seadIDisposer.h"

namespace minigame0 {
// vtable +0xE542C in ModuleMiniGame0.cro, offset_to_top 0, 6 entries
// vtable +0xE544C in ModuleMiniGame0.cro, offset_to_top -4, 2 entries
class CstmCamera : public ::ICameraUpdater, public ::sead::IDisposer
{
public:
    CstmCamera(); // ctor address unknown
};
} // namespace minigame0
