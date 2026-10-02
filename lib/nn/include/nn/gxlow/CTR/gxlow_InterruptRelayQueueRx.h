#pragma once

#include "decomp.h"

namespace nn {
namespace gxlow {
namespace CTR {
class InterruptRelayQueueRx
{
public:
    void SuppressPdcEvents(bool); // 0x00131474 | nintendogs:callgraph [tier A]
};
} // namespace CTR
} // namespace gxlow
} // namespace nn
