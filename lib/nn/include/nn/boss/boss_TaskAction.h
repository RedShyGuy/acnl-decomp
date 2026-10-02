#pragma once

#include "decomp.h"
#include "nn/boss/boss_TaskActionBase.h"

namespace nn {
namespace boss {
// RTTI N2nn4boss10TaskActionE @ 0x008D0348
// vtable 0x0090213C (vptr 0x00902144), offset_to_top 0, 3 entries
class TaskAction : public ::nn::boss::TaskActionBase
{
public:
    virtual void vf_0x00(); // 0x0046A8B4 slot 0x00 | virtual slot, introduced by nn::boss::TaskActionBase
    virtual void vf_0x04(); // 0x0046A8AC slot 0x04 | virtual slot, introduced by nn::boss::TaskActionBase
    virtual void vf_0x08(); // 0x0046A7A0 slot 0x08 | virtual slot, introduced by nn::boss::TaskAction
    TaskAction(); // 0x0046A86C | nintendogs:bytes [tier A]
};
} // namespace boss
} // namespace nn
