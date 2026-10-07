#pragma once

#include "decomp.h"
#include "nn/nex/nex_MatchMakingClient.h"

namespace nn {
namespace nex {
class AutoMatchmakeParam;
class CreateMatchmakeSessionParam;
class JoinMatchmakeSessionParam;
class MatchmakeSession;
class MatchmakeSessionSearchCriteria;
class PlayingSession;
class UpdateMatchmakeSessionParam;
template <typename T0> class qVector;
} // namespace nex
} // namespace nn

namespace nn {
namespace nex {
// RTTI N2nn3nex24MatchmakeExtensionClientE @ 0x008CEDF0
// vtable 0x008FE868 (vptr 0x008FE870), offset_to_top 0, 10 entries
class MatchmakeExtensionClient : public ::nn::nex::MatchMakingClient
{
public:
    virtual ~MatchmakeExtensionClient(); // 0x003B4C1C slot 0x00 | fefates:bytes
    // 0x003B4BE0 slot 0x04 | slot vf_0x04 of nn::nex::ServiceClient (deleting dtor)
    virtual bool Bind(nn::nex::Credentials*); // 0x003B4A88 slot 0x0C | fefates:bytes
    virtual void Unbind(); // 0x003B4B5C slot 0x10 | fefates:bytes
    MatchmakeExtensionClient(); // 0x003B4B94 | fefates:bytes [tier B]

    // The calls of the protocol (it is at +0x9C; the protocol has no method names in the binary,
    // the names are ours after their use in pia::inet::NexMatchmakeSession). armlink merged the
    // tail calls of the last five into the protocol functions.
    bool AutoMatchmakePostpone(nn::nex::ProtocolCallContext* pContext, const nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>& gathering,
                               nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>* pJoinedGathering, const nn::nex::String& message); // 0x003B47FC
    bool OpenParticipation(nn::nex::ProtocolCallContext* pContext, u32 gatheringId); // 0x003B483C
    bool CloseParticipation(nn::nex::ProtocolCallContext* pContext, u32 gatheringId); // 0x003B4854
    // without a call context; at most every 30 seconds (else false)
    bool UpdateProgressScore(u32 gatheringId, u8 score); // 0x003B486C
    bool BrowseMatchmakeSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::MatchmakeSessionSearchCriteria& criteria,
                                const nn::nex::ResultRange& range,
                                nn::nex::qList<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String> >* pGatherings); // 0x003B49A0
    bool UpdateApplicationBuffer(nn::nex::ProtocolCallContext* pContext, u32 gatheringId, const nn::nex::qVector<u8>& buffer); // 0x003B49E4
    // the index of pia counts from 0, the one of the protocol from 1
    bool ModifyCurrentGameAttribute(nn::nex::ProtocolCallContext* pContext, u32 gatheringId, u32 index, u32 value); // 0x003B4A14
    bool ClearMatchmakeSessionSystemPassword(nn::nex::ProtocolCallContext* pContext, u32 gatheringId); // 0x003B4A40
    bool GenerateMatchmakeSessionSystemPassword(nn::nex::ProtocolCallContext* pContext, u32 gatheringId, nn::nex::String* pPassword); // 0x003B4A58
    bool GetMatchmakeSession(nn::nex::ProtocolCallContext* pContext, u32 gatheringId, nn::nex::MatchmakeSession* pSession); // 0x003B4A70
    bool GetPlayingSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::qList<u32>& principalIds,
                           nn::nex::qList<nn::nex::PlayingSession>* pSessions); // 0x003C71B0
    bool UpdateMatchmakeSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::UpdateMatchmakeSessionParam& param); // 0x003C8A64
    bool JoinMatchmakeSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::JoinMatchmakeSessionParam& param,
                              nn::nex::MatchmakeSession* pSession); // 0x003C8CC0
    bool AutoMatchmake(nn::nex::ProtocolCallContext* pContext, const nn::nex::AutoMatchmakeParam& param,
                       nn::nex::MatchmakeSession* pSession); // 0x003C94C8
    bool CreateMatchmakeSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::CreateMatchmakeSessionParam& param,
                                nn::nex::MatchmakeSession* pSession); // 0x003C9630
};
} // namespace nex
} // namespace nn
