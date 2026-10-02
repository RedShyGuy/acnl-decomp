#pragma once

#include "decomp.h"

namespace nn {
namespace applet {
namespace CTR {
class SysSleepAcceptedCallbackInfo
{
public:
    void Unregister(); // 0x0047FD28 | nintendogs:bytes [tier A]
    void Register(); // 0x0047FDA8 | nintendogs:bytes [tier A]
};
} // namespace CTR
} // namespace applet
} // namespace nn
