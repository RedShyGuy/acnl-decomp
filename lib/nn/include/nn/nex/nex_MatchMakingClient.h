#pragma once

#include "decomp.h"
#include "nn/nex/nex_ServiceClient.h"

namespace nn {
namespace nex {
class Gathering;
class ResultRange;
class StationURL;
class String;
template <typename T0> class qList;
template <typename T0, typename T1> class AnyObjectHolder;
// RTTI N2nn3nex17MatchMakingClientE @ 0x008CE6C4
// vtable 0x008FD3E8 (vptr 0x008FD3F0), offset_to_top 0, 10 entries
class MatchMakingClient : public ::nn::nex::ServiceClient
{
public:
    MatchMakingClient(); // ctor address unknown
    virtual ~MatchMakingClient(); // 0x0038735C slot 0x00 | fefates:bytes
    // 0x00387324 slot 0x04 | slot vf_0x04 of nn::nex::ServiceClient (deleting dtor)
    virtual void UpdateProtocolsDefaultCredentials(nn::nex::Credentials*); // 0x00387088 slot 0x20 | fefates:bytes-fuzzy
    bool EndParticipation(nn::nex::ProtocolCallContext*, unsigned, const nn::nex::String&); // 0x00386FD4 | mk7dlp:bytes-fuzzy [tier A]
    bool UnregisterGathering(nn::nex::ProtocolCallContext*, unsigned int); // 0x00387030 | fefates:bytes [tier B]
    // the station URLs of the host of a gathering (name after MatchMakingProtocolClient::
    // ProtoReturn_GetSessionURLs) and the new host (name after pia's CallUpdateSessionHost)
    bool GetSessionURLs(nn::nex::ProtocolCallContext* pContext, u32 gatheringId, nn::nex::qList<nn::nex::StationURL>* pUrls); // 0x00386FBC
    bool UpdateSessionHost(nn::nex::ProtocolCallContext* pContext, u32 gatheringId, bool isMigrateOwner); // 0x00387014
    // (names after the method names of the protocol in the binary, by their ids 22, 44 and 21)
    bool FindByOwner(nn::nex::ProtocolCallContext* pContext, u32 ownerPrincipalId, const nn::nex::ResultRange& range,
                     nn::nex::qList<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String> >* pGatherings); // 0x00386F98
    bool MigrateGatheringOwnership(nn::nex::ProtocolCallContext* pContext, u32 gatheringId, const nn::nex::qList<u32>& candidates); // 0x0038704C
    bool FindBySingleID(nn::nex::ProtocolCallContext* pContext, u32 gatheringId,
                        nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>* pGathering); // 0x003870AC
};
} // namespace nex
} // namespace nn
