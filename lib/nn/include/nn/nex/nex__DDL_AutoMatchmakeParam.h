#pragma once

#include "decomp.h"
#include "nn/nex/nex_MatchmakeSession.h"
#include "nn/nex/nex_MatchmakeSessionSearchCriteria.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex23_DDL_AutoMatchmakeParamE @ 0x008CED24
// vtable 0x008FE66C (vptr 0x008FE674), offset_to_top 0, 2 entries
class _DDL_AutoMatchmakeParam : public ::nn::nex::RootObject
{
public:
    _DDL_AutoMatchmakeParam(); // ctor address unknown
    virtual ~_DDL_AutoMatchmakeParam(); // 0x003B1A2C slot 0x00 | slot vf_0x00 of nn::nex::_DDL_AutoMatchmakeParam
    // 0x003B1A1C slot 0x04 | slot vf_0x04 of nn::nex::_DDL_AutoMatchmakeParam (deleting dtor)

    // (the layout is from AutoMatchmakeParam::AutoMatchmakeParam; the names are ours)
    u8 m_Unknown0x4;                                     // 0x004
    MatchmakeSession m_SourceMatchmakeSession;           // 0x008
    qList<u32> m_AdditionalParticipants;                 // 0x0B8
    u32 m_GatheringIdForParticipationCheck;              // 0x0D0
    u32 m_AutoMatchmakeOption;                           // 0x0D4
    String m_JoinMessage;                                // 0x0D8
    u16 m_ParticipationCount;                            // 0x0E0
    qList<MatchmakeSessionSearchCriteria> m_SearchCriteria; // 0x0E4
    qList<u32> m_TargetGatherings;                       // 0x0FC
};
ASSERT_OFFSET(_DDL_AutoMatchmakeParam, m_AdditionalParticipants, 0xB8);
ASSERT_OFFSET(_DDL_AutoMatchmakeParam, m_SearchCriteria, 0xE4);
ASSERT_SIZE(_DDL_AutoMatchmakeParam, 0x118); // (0x114 and the padding of the 8 byte alignment)
} // namespace nex
} // namespace nn
