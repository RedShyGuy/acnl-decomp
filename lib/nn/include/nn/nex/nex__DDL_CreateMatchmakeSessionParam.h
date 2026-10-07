#pragma once

#include "decomp.h"
#include "nn/nex/nex_MatchmakeSession.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex32_DDL_CreateMatchmakeSessionParamE @ 0x008CF2B0
// vtable 0x008FF648 (vptr 0x008FF650), offset_to_top 0, 2 entries
class _DDL_CreateMatchmakeSessionParam : public ::nn::nex::RootObject
{
public:
    _DDL_CreateMatchmakeSessionParam(); // ctor address unknown
    virtual ~_DDL_CreateMatchmakeSessionParam(); // 0x003CA8B0 slot 0x00 | slot vf_0x00 of nn::nex::_DDL_CreateMatchmakeSessionParam
    // 0x003CA8A0 slot 0x04 | slot vf_0x04 of nn::nex::_DDL_CreateMatchmakeSessionParam (deleting dtor)

    // (the layout is from pia::inet::NexMatchmakeSession; the names are ours)
    u8 m_Unknown0x4;                         // 0x04
    MatchmakeSession m_SourceMatchmakeSession; // 0x08
    qList<u32> m_AdditionalParticipants;     // 0xB8
    u32 m_GatheringIdForParticipationCheck;  // 0xD0
    u32 m_CreateMatchmakeSessionOption;      // 0xD4
    String m_JoinMessage;                    // 0xD8
    u16 m_ParticipationCount;                // 0xE0
};
ASSERT_OFFSET(_DDL_CreateMatchmakeSessionParam, m_AdditionalParticipants, 0xB8);
ASSERT_OFFSET(_DDL_CreateMatchmakeSessionParam, m_JoinMessage, 0xD8);
ASSERT_SIZE(_DDL_CreateMatchmakeSessionParam, 0xE8); // (with the padding of the 8 byte alignment)
} // namespace nex
} // namespace nn
