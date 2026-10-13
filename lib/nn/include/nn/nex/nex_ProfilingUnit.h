#pragma once

#include "decomp.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_qChain.h"

namespace nn {
namespace nex {
// Time statistics of a part of nex (e.g. "JobProcessing" of the Scheduler). All units are in a
// global list. The output of the statistics (Log) is not in this build. The names of the members
// and of the functions without a reference name are ours.
// RTTI N2nn3nex13ProfilingUnitE @ 0x008CE254
// vtable 0x008FC8C4 (vptr 0x008FC8CC), offset_to_top 0, 2 entries
class ProfilingUnit : public ::nn::nex::RootObject
{
public:
    ProfilingUnit(const wchar_t* pName, unsigned int unknown0x44); // 0x003692C8 (name is ours)
    virtual ~ProfilingUnit(); // 0x00369448 slot 0x00 | fefates:callseq [tier C]
    // 0x00369418 slot 0x04 (deleting dtor)
    // (traces the statistics; the trace calls are not in this build)
    void Log(); // 0x00369094 | mk7dlp:bytes-fuzzy [tier A]
    // clears the statistics, the measuring starts at time
    void Reset(nn::nex::Time time); // 0x00369264 | fefates:bytes [tier B]

    ProfilingUnit* m_pChainNext; // 0x04
    ProfilingUnit* m_pChainPrev; // 0x08
    u64 m_Unknown0x10;           // 0x10
    u64 m_TotalTime;             // 0x18 (traced in ms)
    u64 m_MinTime;               // 0x20
    u64 m_MaxTime;               // 0x28
    u64 m_Unknown0x30;           // 0x30
    Time m_StartTime;            // 0x38
    u32 m_Count;                 // 0x40
    u32 m_Unknown0x44;           // 0x44
    String m_Name;               // 0x48
    bool m_IsLoggedAtEnd;        // 0x50 (Log in the destructor)
    u16 m_Unknown0x52;           // 0x52
    u32 m_Id;                    // 0x54
    u32 m_Unknown0x58;           // 0x58
    u32 m_Unknown0x5C;           // 0x5C
    String m_Unknown0x60;        // 0x60
    String m_Unknown0x68;        // 0x68

    static qChain<ProfilingUnit*, DefaultChainPolicy<ProfilingUnit*> > s_Units;
    static CriticalSection s_Lock;
    static u32 s_NextId;
};
ASSERT_OFFSET(ProfilingUnit, m_Name, 0x48);
ASSERT_SIZE(ProfilingUnit, 0x70);
} // namespace nex
} // namespace nn
