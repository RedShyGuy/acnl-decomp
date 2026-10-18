#include "nn/nex/nex_MatchMakingClient.h"
#include "nn/nex/nex_AnyGatheringAdapter.h"
#include "nn/nex/nex_DynamicGathering.h"
#include "nn/nex/nex_GameSession.h"
#include "nn/nex/nex_Gathering.h"
#include "nn/nex/nex_ProtocolRegistry.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
namespace {
// (inline) the prototype is deleted if the adapter does not take it
inline void RegisterPrototypeInline(Gathering* pPrototype)
{
    if (!AnyGatheringAdapter::s_Instance.RegisterPrototype(pPrototype) && pPrototype != nullptr) {
        delete pPrototype;
    }
}
} // namespace

// 0x00386F98 (name is ours)
bool nn::nex::MatchMakingClient::FindByOwner(nn::nex::ProtocolCallContext* pContext, u32 ownerPrincipalId, const nn::nex::ResultRange& range,
                                             nn::nex::qList<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String> >* pGatherings)
{
    return m_Protocol.CallFindByOwner(pContext, ownerPrincipalId, range, pGatherings);
}

// 0x00386FBC (name is ours)
bool nn::nex::MatchMakingClient::GetSessionURLs(nn::nex::ProtocolCallContext* pContext, u32 gatheringId, nn::nex::qList<nn::nex::StationURL>* pUrls)
{
    return m_Protocol.CallGetSessionURLs(pContext, gatheringId, pUrls);
}

// 0x00386FD4 | mk7dlp:bytes-fuzzy [tier A]
bool nn::nex::MatchMakingClient::EndParticipation(nn::nex::ProtocolCallContext* pContext, unsigned int gatheringId, const nn::nex::String& message)
{
    if (gatheringId == 0) {
        return false;
    }
    return m_ExtProtocol.CallEndParticipation(pContext, nullptr, gatheringId, message);
}

// 0x00387008 (name is ours)
bool nn::nex::MatchMakingClient::RegisterPrototype(nn::nex::Gathering* pPrototype)
{
    return AnyGatheringAdapter::s_Instance.RegisterPrototype(pPrototype);
}

// 0x00387014 (name is ours)
bool nn::nex::MatchMakingClient::UpdateSessionHost(nn::nex::ProtocolCallContext* pContext, u32 gatheringId, bool isMigrateOwner)
{
    return m_Protocol.CallUpdateSessionHost(pContext, gatheringId, isMigrateOwner);
}

// 0x00387030 | fefates:bytes [tier B]
bool nn::nex::MatchMakingClient::UnregisterGathering(nn::nex::ProtocolCallContext* pContext, unsigned int gatheringId)
{
    return m_Protocol.CallUnregisterGathering(pContext, nullptr, gatheringId);
}

// 0x0038704C (name is ours)
bool nn::nex::MatchMakingClient::MigrateGatheringOwnership(nn::nex::ProtocolCallContext* pContext, u32 gatheringId, const nn::nex::qList<u32>& candidates)
{
    bool participantsOnly = true;
    if (gatheringId == 0) {
        return false;
    }
    return m_Protocol.CallMigrateGatheringOwnership(pContext, gatheringId, candidates, participantsOnly);
}

// 0x00387088 slot 0x20 | fefates:bytes-fuzzy
void nn::nex::MatchMakingClient::UpdateProtocolsDefaultCredentials(nn::nex::Credentials* pCredentials)
{
    m_Protocol.SetDefaultCredentials(pCredentials);
    m_ExtProtocol.SetDefaultCredentials(pCredentials);
}

// 0x003870AC (name is ours)
bool nn::nex::MatchMakingClient::FindBySingleID(nn::nex::ProtocolCallContext* pContext, u32 gatheringId,
                                                nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>* pGathering)
{
    return m_Protocol.CallFindBySingleID(pContext, gatheringId, nullptr, pGathering);
}

// 0x003870D0
nn::nex::MatchMakingClient::MatchMakingClient() : ServiceClient(SERVICE_CLIENT_ID), m_Protocol(0), m_ExtProtocol(0)
{
    m_Protocol.m_ProtocolId = PROTOCOL_ID;
    m_ExtProtocol.m_ProtocolId = EXT_PROTOCOL_ID;
    ScopedCS lock(Scheduler::GetInstance()->m_Lock);
    // the first user registers the prototypes
    if (AnyGatheringAdapter::RegisterUse() == 1) {
        RegisterPrototypeInline(new Gathering());
        RegisterPrototypeInline(new DynamicGathering());
        RegisterPrototypeInline(new GameSession());
    }
    GetProtocolRegistry()->RegisterProtocol(PROTOCOL_ID, new MatchMakingProtocolClient(0));
    GetProtocolRegistry()->RegisterProtocol(EXT_PROTOCOL_ID, new MatchMakingProtocolExtClient(0));
}

// 0x0038735C slot 0x00 | fefates:bytes
// 0x00387324 (deleting dtor)
nn::nex::MatchMakingClient::~MatchMakingClient()
{
    AnyGatheringAdapter::UnregisterUse();
}

} // namespace nex
} // namespace nn
