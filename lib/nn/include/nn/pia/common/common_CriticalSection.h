#pragma once

#include "decomp.h"
#include "nn/os/os_CriticalSection.h"

namespace nn {
namespace pia {
namespace common {
// A critical section of the system (nn::os::CriticalSection), initialized by the constructor.
// The member name and Lock are ours.
class CriticalSection
{
public:
    // the parameter (6 in Scheduler::CreateInstance) is not used; its name is ours
    explicit CriticalSection(int spinCount); // 0x004273A4 | fefates:bytes [tier B]

    void Lock(); // 0x00427364
    void Unlock(); // 0x004273A0 | fefates:callgraph [tier C]

    nn::os::CriticalSection m_CriticalSection; // 0x0
};
ASSERT_SIZE(CriticalSection, 0xC);
} // namespace common
} // namespace pia
} // namespace nn
