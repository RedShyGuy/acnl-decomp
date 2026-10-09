#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/boss/boss_Types.h"

namespace nn {
namespace boss {
// RTTI N2nn4boss10TaskStatusE @ 0x008D035C
// The status of a task as Task::GetStateDetail reads it (the member names are ours).
class TaskStatus
{
public:
    TaskStatus(); // 0x0046AD08 | fefates:bytes [tier B]
    virtual ~TaskStatus();

    nn::Result GetProperty(nn::boss::PropertyType type, void* pValue, unsigned int size); // 0x0046AA78 | fefates:bytes-fuzzy [tier B]

    u32 m_Unknown04;                   // 0x04
    nn::boss::TaskStatusInfo m_Info;   // 0x08
};
ASSERT_SIZE(TaskStatus, 0x80);
} // namespace boss
} // namespace nn
