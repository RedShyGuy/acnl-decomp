#pragma once

#include "decomp.h"

namespace nn {
namespace boss {
// RTTI N2nn4boss10TaskStatusE @ 0x008D035C
// vtable 0x00902160 (vptr 0x00902168), offset_to_top 0, 2 entries
class TaskStatus
{
public:
    virtual void vf_0x00(); // 0x0046AD34 slot 0x00 | virtual slot, introduced by nn::boss::TaskStatus
    virtual void vf_0x04(); // 0x0046AD30 slot 0x04 | virtual slot, introduced by nn::boss::TaskStatus
    void GetProperty(nn::boss::PropertyType, void*, unsigned int); // 0x0046AA78 | fefates:bytes-fuzzy [tier B]
    TaskStatus(); // 0x0046AD08 | fefates:bytes [tier B]
};
} // namespace boss
} // namespace nn
