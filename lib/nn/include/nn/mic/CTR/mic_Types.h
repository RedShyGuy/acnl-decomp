#pragma once

#include "decomp.h"

namespace nn {
namespace mic {
namespace CTR {
// the format of the samples (type name from the symbols, values after 3dbrew "MIC:StartSampling",
// names are ours)
enum SamplingType : u8 {
    SAMPLING_TYPE_8BIT = 0,
    SAMPLING_TYPE_16BIT = 1,
    SAMPLING_TYPE_SIGNED_8BIT = 2,
    SAMPLING_TYPE_SIGNED_16BIT = 3,
};

// the sample rate (32730, 16360, 10910 and 8180 Hz)
enum SamplingRate : u8 {
    SAMPLING_RATE_32730 = 0,
    SAMPLING_RATE_16360 = 1,
    SAMPLING_RATE_10910 = 2,
    SAMPLING_RATE_8180 = 3,
    SAMPLING_RATE_NUM = 4,
};
} // namespace CTR
} // namespace mic
} // namespace nn
