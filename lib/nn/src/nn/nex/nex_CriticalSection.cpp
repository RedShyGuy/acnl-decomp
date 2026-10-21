#include "nn/nex/nex_CriticalSection.h"

namespace nn {
namespace nex {
// 0x0096C914 (name is ours)
bool CriticalSection::s_IsLockDisabled;

// 0x00377540 (name is ours)
void nn::nex::CriticalSection::Enter()
{
    if (!s_IsLockDisabled) {
        m_Mutex.Enter();
    }
}

// 0x00377560 (name is ours)
void nn::nex::CriticalSection::Leave()
{
    if (!s_IsLockDisabled) {
        m_Mutex.Leave();
    }
}

// 0x00377580 | fefates:callgraph [tier C]
nn::nex::CriticalSection::CriticalSection(unsigned int level) : m_LockChecker(level)
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x00377608
// 0x003775DC (deleting dtor)
nn::nex::CriticalSection::~CriticalSection()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0072B6C8 (name is ours)
bool nn::nex::CriticalSection::IsForced()
{
    return false;
}

} // namespace nex
} // namespace nn
