#pragma once

#include "decomp.h"

namespace nn {
namespace mic {
namespace CTR {
namespace detail {
class Mic
{
public:
    void GetPGAB(unsigned char*); // 0x00131084 | nintendogs:bytes [tier B]
    void FreeBuffer(); // 0x00140AF0 | nintendogs:bytes [tier A]
    void IsSampling(bool*); // 0x00140B20 | nintendogs:bytes [tier A]
    void SetMicBias(bool); // 0x00140B60 | nintendogs:bytes [tier A]
    void StopSampling(); // 0x00140BA0 | nintendogs:bytes [tier A]
    void SetPGAB(unsigned char); // 0x00354A90 | nintendogs:bytes [tier A]
};
} // namespace detail
} // namespace CTR
} // namespace mic
} // namespace nn
