#include "nn/boss/boss_FgOnlyTask.h"
#include <string.h>

namespace nn {
namespace boss {
namespace {
const char TASK_ID[] = "FGONLYT";
} // namespace

// 0x0046A724 | tier C
nn::boss::FgOnlyTask::FgOnlyTask()
{
    strcpy(m_TaskId, TASK_ID);
}

// 0x0046C84C
// 0x0046A754 (deleting dtor)
nn::boss::FgOnlyTask::~FgOnlyTask()
{
}

} // namespace boss
} // namespace nn
