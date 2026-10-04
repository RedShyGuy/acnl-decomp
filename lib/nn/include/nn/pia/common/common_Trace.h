#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace common {
// The trace output of pia; only the instance pointer is used in common (the class and
// DestroyInstance are from the fefates symbols).
class Trace : public RootObject
{
public:
    static void DestroyInstance(); // 0x00428DD8 | fefates:callgraph [tier C]

    static Trace* s_pInstance; // 0x0097E400
};
} // namespace common
} // namespace pia
} // namespace nn
