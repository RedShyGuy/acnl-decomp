#pragma once

#include "decomp.h"

namespace sead {
// The common values of the audio classes (the namespace and AuxBus are from the reference symbols, the
// values and the other names are ours).
namespace AudioGlobal {
// the effect buses (-1: none)
enum AuxBus : s8
{
    AUX_BUS_NONE = -1,
    AUX_BUS_A = 0,
    AUX_BUS_B = 1,
    AUX_BUS_NUM = 2,
};
// the output of the sound (AudioSystem::getOutputMode/setOutputMode)
enum OutputMode : s32
{
    OUTPUT_MODE_STEREO = 0,
    OUTPUT_MODE_MONO = 1,
    OUTPUT_MODE_SURROUND = 2,
    OUTPUT_MODE_INVALID = 4,
};
} // namespace AudioGlobal
} // namespace sead
