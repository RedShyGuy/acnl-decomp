#pragma once

#include "decomp.h"

namespace sead {
// A DSP effect with the pointer to its parameters (they are given with setParam and needed for the
// memory size; name of the class from the reference symbols, the member name is ours).
template <typename T, typename TParam>
class AudioDspFxCtr : public T
{
public:
    AudioDspFxCtr() : T(), mParam(nullptr) {}

    const TParam* mParam; // 0x10
};
} // namespace sead
