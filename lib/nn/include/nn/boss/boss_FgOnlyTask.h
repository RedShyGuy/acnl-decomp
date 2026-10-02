#pragma once

#include "decomp.h"
#include "nn/boss/boss_Task.h"

namespace nn {
namespace boss {
// RTTI N2nn4boss10FgOnlyTaskE @ 0x008D033C
// vtable 0x0090212C (vptr 0x00902134), offset_to_top 0, 2 entries
class FgOnlyTask : public ::nn::boss::Task
{
public:
    FgOnlyTask(); // ctor candidate(s) 0x0046A724 (unverified)
    virtual void vf_0x00(); // 0x0046C84C slot 0x00 | virtual slot, introduced by nn::boss::Task
    virtual void vf_0x04(); // 0x0046A754 slot 0x04 | virtual slot, introduced by nn::boss::Task
};
} // namespace boss
} // namespace nn
