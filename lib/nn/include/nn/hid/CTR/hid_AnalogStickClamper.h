#pragma once

#include "decomp.h"

namespace nn {
namespace hid {
namespace CTR {
class AnalogStickClamper
{
public:
    void ClampValueOfClamp(); // 0x003538B8 | nintendogs:bytes [tier A]
    void NormalizeStickWithScale(float*, float*, short, short); // 0x00353904 | nintendogs:bytes [tier B]
    void ClampCore(short*, short*, int, int); // 0x00353B9C | nintendogs:bytes [tier A]
    AnalogStickClamper(); // 0x00353C30 | nintendogs:bytes [tier A]
};
} // namespace CTR
} // namespace hid
} // namespace nn
