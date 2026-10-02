#pragma once

#include "decomp.h"
#include "Other/dICameraUpdater.h"
#include "sead/seadIDisposer.h"

namespace paneponbg {
// vtable +0x315E8 in ModuleMiniGame1.cro, offset_to_top 0, 3 entries
// vtable +0x315FC in ModuleMiniGame1.cro, offset_to_top -4, 2 entries
class CstmCamera : public ::ICameraUpdater, public ::sead::IDisposer
{
public:
    CstmCamera(); // ctor address unknown
};
} // namespace paneponbg
