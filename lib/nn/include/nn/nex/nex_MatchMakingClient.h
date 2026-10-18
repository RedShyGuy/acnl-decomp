#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyObjectHolder.h"
#include "nn/nex/nex_MatchMakingProtocolClient.h"
#include "nn/nex/nex_MatchMakingProtocolExtClient.h"
#include "nn/nex/nex_ServiceClient.h"

namespace nn {
namespace nex {
class Gathering;
class ProtocolCallContext;
class ResultRange;
class StationURL;
class String;
template <typename T0>
class qList;

// The client of the match making service: the calls of the match making protocols and the
// prototypes of the gatherings. The names of the members are ours.
// RTTI N2nn3nex17MatchMakingClientE @ 0x008CE6C4
// vtable 0x008FD3E8 (vptr 0x008FD3F0), offset_to_top 0, 10 entries
class MatchMakingClient : public ::nn::nex::ServiceClient
{
public:
    // the id of the service client
    static const u32 SERVICE_CLIENT_ID = 12;
    // the protocol ids
    static const u16 PROTOCOL_ID = 21;
    static const u16 EXT_PROTOCOL_ID = 50;

    MatchMakingClient(); // 0x003870D0
    virtual ~MatchMakingClient(); // 0x0038735C slot 0x00 | fefates:bytes
    // 0x00387324 slot 0x04 (deleting dtor)
    virtual void UpdateProtocolsDefaultCredentials(nn::nex::Credentials* pCredentials); // 0x00387088 slot 0x20 | fefates:bytes-fuzzy
    // false for the gathering id 0
    bool EndParticipation(nn::nex::ProtocolCallContext* pContext, unsigned int gatheringId, const nn::nex::String& message); // 0x00386FD4 | mk7dlp:bytes-fuzzy [tier A]
    bool UnregisterGathering(nn::nex::ProtocolCallContext* pContext, unsigned int gatheringId); // 0x00387030 | fefates:bytes [tier B]
    // the station URLs of the host of a gathering (name after MatchMakingProtocolClient::
    // ProtoReturn_GetSessionURLs) and the new host (name after pia's CallUpdateSessionHost)
    bool GetSessionURLs(nn::nex::ProtocolCallContext* pContext, u32 gatheringId, nn::nex::qList<nn::nex::StationURL>* pUrls); // 0x00386FBC
    bool UpdateSessionHost(nn::nex::ProtocolCallContext* pContext, u32 gatheringId, bool isMigrateOwner); // 0x00387014
    // (names after the method names of the protocol in the binary, by their ids 22, 44 and 21)
    bool FindByOwner(nn::nex::ProtocolCallContext* pContext, u32 ownerPrincipalId, const nn::nex::ResultRange& range,
                     nn::nex::qList<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String> >* pGatherings); // 0x00386F98
    // false for the gathering id 0 (only to participants)
    bool MigrateGatheringOwnership(nn::nex::ProtocolCallContext* pContext, u32 gatheringId, const nn::nex::qList<u32>& candidates); // 0x0038704C
    bool FindBySingleID(nn::nex::ProtocolCallContext* pContext, u32 gatheringId,
                        nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>* pGathering); // 0x003870AC
    // a prototype of a gathering type for the any adapter
    static bool RegisterPrototype(nn::nex::Gathering* pPrototype); // 0x00387008

    MatchMakingProtocolClient m_Protocol;       // 0x10
    MatchMakingProtocolExtClient m_ExtProtocol; // 0x54
};
ASSERT_OFFSET(MatchMakingClient, m_ExtProtocol, 0x54);
ASSERT_SIZE(MatchMakingClient, 0x98);
} // namespace nex
} // namespace nn
