#pragma once

#include "decomp.h"
#include "Other/dICameraUpdater.h"

// vtable +0xEFFC in ModuleTrain.cro, offset_to_top 0, 1 entries
class TrainCamera : public ::ICameraUpdater
{
public:
    TrainCamera(); // ctor address unknown
};
