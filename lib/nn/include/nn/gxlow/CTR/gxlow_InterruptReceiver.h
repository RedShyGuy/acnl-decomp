#pragma once

#include "decomp.h"

namespace nn {
namespace gxlow {
namespace CTR {
class InterruptReceiver
{
public:
    void Initialize(); // 0x0013123C | nintendogs:callgraph [tier A]
    void WaitAnyHandlerDone(); // 0x0013142C | fefates:bytes [tier B]
    void ReceiverThreadFunc(unsigned int); // 0x00137054 | fefates:bytes [tier B]
};
} // namespace CTR
} // namespace gxlow
} // namespace nn
