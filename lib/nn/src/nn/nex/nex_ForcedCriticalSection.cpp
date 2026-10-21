#include "nn/nex/nex_ForcedCriticalSection.h"

namespace nn {
namespace nex {
// 0x00398670 (name is ours)
void nn::nex::ForcedCriticalSection::Enter()
{
    m_Mutex.Enter();
}

// 0x00398678 (name is ours)
void nn::nex::ForcedCriticalSection::Leave()
{
    m_Mutex.Leave();
}

// 0x003986AC
// 0x00398680 (deleting dtor)
nn::nex::ForcedCriticalSection::~ForcedCriticalSection()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0072CE04 (name is ours)
bool nn::nex::ForcedCriticalSection::IsForced()
{
    return true;
}

} // namespace nex
} // namespace nn
