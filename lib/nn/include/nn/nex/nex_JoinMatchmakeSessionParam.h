#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_JoinMatchmakeSessionParam.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex25JoinMatchmakeSessionParamE @ 0x008CEEAC
// vtable 0x008FEA20 (vptr 0x008FEA28), offset_to_top 0, 2 entries
class JoinMatchmakeSessionParam : public ::nn::nex::_DDL_JoinMatchmakeSessionParam
{
public:
    // (inline in pia::inet::NexMatchmakeSession; not decompiled yet)
    JoinMatchmakeSessionParam();
    // the values of a new parameter (inline; name is ours)
    void Reset()
    {
        m_GatheringId = 0;
        m_AdditionalParticipants = qList<u32>();
        m_GatheringIdForParticipationCheck = 0;
        m_JoinMatchmakeSessionOption = 0;
        m_Unknown0x2C = 0;
        m_Unknown0x30 = String(L"");
        m_Unknown0x38 = String(L"");
        m_JoinMessage = String();
        m_ParticipationCount = 1;
        m_Unknown0x4A = 0;
    }
    virtual void vf_0x00(); // 0x003B71F8 slot 0x00 | virtual slot, introduced by nn::nex::_DDL_JoinMatchmakeSessionParam
    virtual void vf_0x04(); // 0x003B71C0 slot 0x04 | virtual slot, introduced by nn::nex::_DDL_JoinMatchmakeSessionParam
};
} // namespace nex
} // namespace nn
