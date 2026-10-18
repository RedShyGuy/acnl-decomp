#include "nn/nex/nex_Chrono.h"
#include "nn/nex/nex_SystemError.h"

namespace nn {
namespace nex {
// 0x003D01E4 (name is ours)
u32 nn::nex::Chrono::Stop()
{
    if (m_State != STATE_RUNNING) {
        SystemError::SignalError(const_cast<wchar_t*>(FILE_NAME), 97, SystemError::ERROR_INVALID_STATE, 0);
        return static_cast<u32>(-1);
    }
    Time now = SystemClock::GetTime();
    m_Elapsed = static_cast<u32>(now.m_Value) - static_cast<u32>(m_Clock.m_StartTime.m_Value) - static_cast<u32>(m_StartOffset.m_Value) + m_Elapsed;
    m_State = STATE_STOPPED;
    return m_Elapsed;
}

// 0x003D0254 | mk7dlp:bytes [tier B]
u32 nn::nex::Chrono::Check()
{
    return CheckInline();
}

// 0x003D029C (name is ours)
bool nn::nex::Chrono::Start()
{
    return StartInline();
}

// 0x003D0310
nn::nex::Chrono::Chrono() : m_StartOffset(0), m_Elapsed(0), m_State(STATE_STOPPED)
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003D03B0
// 0x003D0380 (deleting dtor)
nn::nex::Chrono::~Chrono()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
