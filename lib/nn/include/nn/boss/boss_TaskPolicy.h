#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/boss/boss_Types.h"

namespace nn {
namespace boss {
// RTTI N2nn4boss10TaskPolicyE @ 0x008D0354
// When and how often a task runs (the properties 0x00 - 0x05; the member name is ours).
class TaskPolicy
{
public:
    TaskPolicy(); // 0x0046AA4C | tier C
    virtual ~TaskPolicy();

    nn::Result SetProperty(nn::boss::PropertyType type, const void* pValue, unsigned size); // 0x0046A8B8 | nintendogs:bytes-fuzzy [tier A]
    nn::Result InitializeWithSecInterval(unsigned interval, unsigned count); // 0x0046AA14 | nintendogs:bytes [tier A]

    nn::boss::TaskPolicyConfig m_Config; // 0x04
};
ASSERT_SIZE(TaskPolicy, 0x14);

// The options of a task (the properties 0x18 - 0x1C). This program never makes one; the type name
// is from the binary, the layout from VerifyTaskOptionConfig / RegisterTask (the first word is
// probably a vptr; the member names are ours).
class TaskOption
{
public:
    u32 m_Unknown00;                     // 0x00
    nn::boss::TaskOptionConfig m_Config; // 0x04
};
ASSERT_SIZE(TaskOption, 0x10);
} // namespace boss
} // namespace nn
