#include "nn/nex/nex_ProfilingUnit.h"

namespace nn {
namespace nex {
// 0x00ADE8F8 (name is ours)
qChain<ProfilingUnit*, DefaultChainPolicy<ProfilingUnit*> > ProfilingUnit::s_Units;
// 0x00ADE90C (name is ours)
CriticalSection ProfilingUnit::s_Lock(0x20000000);
// 0x0096C97C (name is ours)
u32 ProfilingUnit::s_NextId;

// 0x00369094 | mk7dlp:bytes-fuzzy [tier A]
void nn::nex::ProfilingUnit::Log()
{
    if (m_Count == 0) {
        return;
    }
    Time now = SystemClock::GetTime();
    s32 elapsed = static_cast<s32>(now.m_Value) - static_cast<s32>(m_StartTime.m_Value);
    if (elapsed <= 0) {
        return;
    }
    // (the trace of the share of the time, the count per second, the minimum, the maximum, the
    // mean and the last time is not in this build)
}

// 0x00369264 | fefates:bytes [tier B]
void nn::nex::ProfilingUnit::Reset(nn::nex::Time time)
{
    m_Count = 0;
    m_Unknown0x10 = 0;
    m_TotalTime = 0;
    m_MaxTime = 0;
    m_Unknown0x30 = 0;
    m_MinTime = static_cast<u64>(-1);
    m_StartTime = time;
    m_Unknown0x58 = 0;
}

// 0x003692C8 (name is ours)
nn::nex::ProfilingUnit::ProfilingUnit(const wchar_t* pName, unsigned int unknown0x44)
    : m_pChainNext(nullptr), m_pChainPrev(nullptr), m_Name(pName), m_Unknown0x60(L""), m_Unknown0x68(L"")
{
    Time now = SystemClock::GetTime();
    m_Count = 0;
    m_Unknown0x10 = 0;
    m_TotalTime = 0;
    m_MinTime = static_cast<u64>(-1);
    m_MaxTime = 0;
    m_Unknown0x30 = 0;
    m_Unknown0x58 = 0;
    m_StartTime = now;
    m_IsLoggedAtEnd = false;
    m_Unknown0x44 = unknown0x44;
    m_Unknown0x52 = 0;
    m_Unknown0x5C = 0;
    ScopedCS lock(s_Lock);
    s_Units.push_back(this);
    m_Id = s_NextId++;
}

// 0x00369448 | fefates:callseq [tier C]
// 0x00369418 (deleting dtor)
nn::nex::ProfilingUnit::~ProfilingUnit()
{
    ScopedCS lock(s_Lock);
    s_Units.remove(this);
    if (m_IsLoggedAtEnd) {
        Log();
    }
}

// 0x007E41C0
// 0x007E4134 (deleting dtor)
template class qChain<ProfilingUnit*, DefaultChainPolicy<ProfilingUnit*> >;

} // namespace nex
} // namespace nn
