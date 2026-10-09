#pragma once

#include <string.h>
#include "decomp.h"
#include "nn/Result.h"
#include "nn/boss/boss_Types.h"

namespace nn {
namespace boss {
// RTTI N2nn4boss14TaskActionBaseE @ 0x008D036C
// What a task does (the action configuration; the member names are ours).
class TaskActionBase
{
public:
    TaskActionBase() { memset(&m_Config, 0, sizeof(m_Config)); }
    virtual ~TaskActionBase();

    // adds a root certificate (at most 3)
    nn::Result SetRootCa(unsigned certificateId); // 0x0046B6F4 | nintendogs:bytes [tier A]
    // the properties all actions have (url, headers, certificates, ...; the names are ours)
    nn::Result GetCommonProperty(nn::boss::PropertyType type, void* pValue, unsigned size); // 0x0046B214 (name is ours)
    nn::Result SetCommonProperty(nn::boss::PropertyType type, const void* pValue, unsigned size); // 0x0046B43C (name is ours)
    nn::Result SetApInfoType(u8 type); // 0x0046B6E8 (name is ours)

    nn::boss::TaskActionConfig m_Config; // 0x04
};
ASSERT_SIZE(TaskActionBase, 0x7D8);
} // namespace boss
} // namespace nn
