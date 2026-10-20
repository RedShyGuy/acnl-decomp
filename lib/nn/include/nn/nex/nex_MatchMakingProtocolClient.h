#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyObjectHolder.h"
#include "nn/nex/nex_ClientProtocol.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
class Gathering;
class ProtocolCallContext;
class ResultRange;
class StationURL;
class String;

// The client of the match making protocol (protocol id 21). The method and parameter names are
// from the DDL strings, the names of the members are ours.
// RTTI N2nn3nex25MatchMakingProtocolClientE @ 0x008CEEB8
// vtable 0x008FEA30 (vptr 0x008FEA38), offset_to_top 0, 23 entries
class MatchMakingProtocolClient : public ::nn::nex::ClientProtocol
{
public:
    // the methods of the protocol that are called
    enum Method
    {
        METHOD_UNREGISTER_GATHERING = 2,
        METHOD_FIND_BY_SINGLE_ID = 21,
        METHOD_FIND_BY_OWNER = 22,
        METHOD_GET_SESSION_URLS = 41,
        METHOD_UPDATE_SESSION_HOST = 42,
        METHOD_MIGRATE_GATHERING_OWNERSHIP = 44,
        METHOD_COUNT = 44
    };
    // the extraction of the results of a method
    typedef void (*ProtoReturnFunction)(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext);
    typedef AnyObjectHolder<Gathering, String> GatheringHolder;

    // id: the protocol id (0: the responder, not registered)
    explicit DECOMP_NOINLINE MatchMakingProtocolClient(unsigned short id); // 0x003B82EC
    virtual ~MatchMakingProtocolClient(); // 0x003B83E4 slot 0x00
    // 0x003B83D4 slot 0x04 (deleting dtor)
    // (symbols.json names it the one of DataStoreProtocolClient)
    virtual void ExtractCallSpecificResults(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003B7C5C slot 0x50
    virtual nn::nex::ClientProtocol* CreateResponder() const; // 0x0072D68C slot 0x54

    // (the return types are ours)
    bool CallUnregisterGathering(nn::nex::ProtocolCallContext* pContext, bool* pRetval, const unsigned int& idGathering); // 0x003B77B0 | fefates
    bool CallFindBySingleID(nn::nex::ProtocolCallContext* pContext, const unsigned int& id, bool* pbResult, GatheringHolder* pGathering); // 0x003B7398
    bool CallFindByOwner(nn::nex::ProtocolCallContext* pContext, const unsigned int& id, const nn::nex::ResultRange& resultRange, nn::nex::qList<GatheringHolder>* plstGathering); // 0x003B722C
    bool CallGetSessionURLs(nn::nex::ProtocolCallContext* pContext, const unsigned int& gid, nn::nex::qList<nn::nex::StationURL>* plstURLs); // 0x003B7504
    bool CallUpdateSessionHost(nn::nex::ProtocolCallContext* pContext, const unsigned int& gid, const bool& isMigrateOwner); // 0x003B7660
    bool CallMigrateGatheringOwnership(nn::nex::ProtocolCallContext* pContext, const unsigned int& gid, const nn::nex::qList<u32>& lstPotentialNewOwnersID,
                                       const bool& participantsOnly); // 0x003B811C
    static void ProtoReturn_UnregisterGathering(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003B82BC
    static void ProtoReturn_FindBySingleID(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003B7CE0
    static void ProtoReturn_FindByOwner(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003B790C | fefates:bytes [tier B]
    static void ProtoReturn_GetSessionURLs(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003B7E18 | fefates:bytes [tier B]
    // (no results)
    static void ProtoReturn_UpdateSessionHost(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003B82B8
    static void ProtoReturn_MigrateGatheringOwnership(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003B82E8

    // the extraction of the results of the methods (by the method id - 1; set by the calls)
    static ProtoReturnFunction s_ProtoReturnFunctions[METHOD_COUNT];
};
ASSERT_SIZE(MatchMakingProtocolClient, 0x44);

// (out of line in the original; nex_MatchMakingProtocolClient.cpp)
template <>
DECOMP_NOINLINE qList<AnyObjectHolder<Gathering, String> >::qList();
} // namespace nex
} // namespace nn

namespace std {
template <>
DECOMP_NOINLINE list<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>, nn::nex::MemAllocator<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String> > >::~list();

template <>
DECOMP_NOINLINE list<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>, nn::nex::MemAllocator<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String> > >::Node* list<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>, nn::nex::MemAllocator<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String> > >::erase(Node* pNode);

template <>
DECOMP_NOINLINE void list<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>, nn::nex::MemAllocator<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String> > >::AddBuffer(bool isFirst);
} // namespace std
