#include "nn/boss/boss_TaskStatus.h"

namespace nn {
namespace boss {
// 0x0046AD34 slot 0x00 | virtual slot, introduced by nn::boss::TaskStatus
void nn::boss::TaskStatus::vf_0x00()
{
}

// 0x0046AD30 slot 0x04 | virtual slot, introduced by nn::boss::TaskStatus
void nn::boss::TaskStatus::vf_0x04()
{
}

// 0x0046AA78 | fefates:bytes-fuzzy [tier B]
void nn::boss::TaskStatus::GetProperty(nn::boss::PropertyType, void*, unsigned int)
{
}

// 0x0046AD08 | fefates:bytes [tier B]
nn::boss::TaskStatus::TaskStatus()
{
}

} // namespace boss
} // namespace nn
