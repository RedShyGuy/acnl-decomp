#pragma once

#include "decomp.h"

namespace nn {
namespace gxlow {
namespace CTR {
namespace detail {
void IsAppletMode(); // 0x0012ACB4 | nintendogs:callgraph [tier A]
void IsInitialized(); // 0x00131194 | nintendogs:callgraph [tier A]
void GetInterruptReceiver(); // 0x001372CC | nintendogs:callgraph [tier A]
void GetGpuIpc(); // 0x001372DC | nintendogs:callgraph [tier A]
} // namespace detail
} // namespace CTR
} // namespace gxlow
} // namespace nn
