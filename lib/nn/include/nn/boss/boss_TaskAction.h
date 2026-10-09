#pragma once

#include "decomp.h"
#include "nn/boss/boss_TaskActionBase.h"

namespace nn {
namespace boss {
// RTTI N2nn4boss10TaskActionE @ 0x008D0348
class TaskAction : public ::nn::boss::TaskActionBase
{
public:
    TaskAction(); // 0x0046A86C | nintendogs:bytes [tier A]
    virtual ~TaskAction();
    // a property of the action (the name of the slot is ours)
    virtual nn::Result GetProperty(nn::boss::PropertyType type, void* pValue, unsigned size); // 0x0046A7A0 (name is ours)

    // the properties of the action data (0x08 - 0x0C; the name is ours)
    nn::Result GetActionDataProperty(nn::boss::PropertyType type, void* pValue, unsigned size); // 0x0046B130 (name is ours)
};
} // namespace boss
} // namespace nn
