#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpStationMaster.h"
#include "sead/seadIDelegateR.h"

// vtable +0xD644 in ModuleStation.cro, offset_to_top 0, 2 entries
class AcNpcSpStationMaster::AsyncAction : public ::sead::IDelegateR<bool>
{
public:
    AsyncAction(); // ctor address unknown
};
