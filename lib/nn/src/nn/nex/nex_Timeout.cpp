#include "nn/nex/nex_Timeout.h"
#include "nn/nex/nex_Core.h"

namespace nn {
namespace nex {
// 0x003D3794 | fefates:bytes [tier B]
void nn::nex::Timeout::SetRelativeExpirationTime(int milliseconds)
{
    m_ExpirationTime = SystemClock::GetTime() + milliseconds;
    m_RelativeTime = milliseconds;
}

// 0x003D3814
// 0x003D37F0 (deleting dtor)
nn::nex::Timeout::~Timeout()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0072E478 | fefates:bytes [tier B]
bool nn::nex::Timeout::IsExpired() const
{
    if (m_ExpirationTime.m_Value != 0 && static_cast<s64>(m_ExpirationTime.m_Value - SystemClock::GetTime().m_Value) < 0) {
        return true;
    }
    if (m_pCore->m_IsInterrupted && static_cast<s64>(m_pCore->m_InterruptTime.m_Value - SystemClock::GetTime().m_Value) < 0) {
        return true;
    }
    return false;
}

} // namespace nex
} // namespace nn
