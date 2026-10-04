#include "nn/pia/common/common_CriticalSection.h"

namespace nn {
namespace pia {
namespace common {
// 0x004273A4 | fefates:bytes [tier B]
nn::pia::common::CriticalSection::CriticalSection(int)
    : m_CriticalSection(nn::os::CriticalSection::InitializeTag())
{
}

// 0x00427364 (name is ours)
void nn::pia::common::CriticalSection::Lock()
{
    m_CriticalSection.Enter();
}

// 0x004273A0 | fefates:callgraph [tier C]
void nn::pia::common::CriticalSection::Unlock()
{
    m_CriticalSection.Exit();
}

} // namespace common
} // namespace pia
} // namespace nn
