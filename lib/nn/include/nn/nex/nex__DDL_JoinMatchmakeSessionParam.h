#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex30_DDL_JoinMatchmakeSessionParamE @ 0x008CF20C
// vtable 0x008FF4E8 (vptr 0x008FF4F0), offset_to_top 0, 2 entries
class _DDL_JoinMatchmakeSessionParam : public ::nn::nex::RootObject
{
public:
    _DDL_JoinMatchmakeSessionParam(); // ctor address unknown
    virtual void vf_0x00(); // 0x003C4A48 slot 0x00 | virtual slot, introduced by nn::nex::_DDL_JoinMatchmakeSessionParam
    virtual void vf_0x04(); // 0x003C4A10 slot 0x04 | virtual slot, introduced by nn::nex::_DDL_JoinMatchmakeSessionParam

    // (the layout is from pia::inet::NexMatchmakeSession, which inlines the constructor; the
    // names are ours)
    u8 m_Unknown0x4;                       // 0x04
    u32 m_GatheringId;                     // 0x08
    qList<u32> m_AdditionalParticipants;   // 0x0C
    u32 m_GatheringIdForParticipationCheck; // 0x24
    u32 m_JoinMatchmakeSessionOption;      // 0x28
    u8 m_Unknown0x2C;                      // 0x2C
    String m_Unknown0x30;                  // 0x30 (pia::inet::NexJoinSessionSetting)
    String m_Unknown0x38;                  // 0x38 (pia::inet::NexJoinSessionSetting)
    String m_JoinMessage;                  // 0x40
    u16 m_ParticipationCount;              // 0x48
    u16 m_Unknown0x4A;                     // 0x4A
};
ASSERT_OFFSET(_DDL_JoinMatchmakeSessionParam, m_AdditionalParticipants, 0xC);
ASSERT_OFFSET(_DDL_JoinMatchmakeSessionParam, m_JoinMessage, 0x40);
ASSERT_SIZE(_DDL_JoinMatchmakeSessionParam, 0x4C);
} // namespace nex
} // namespace nn
