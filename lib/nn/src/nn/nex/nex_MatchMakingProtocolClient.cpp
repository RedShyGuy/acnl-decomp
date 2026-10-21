#include "nn/nex/nex_MatchMakingProtocolClient.h"
#include "nn/nex/nex_ClientProtocolCall.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_ProtocolRegistry.h"
#include "nn/nex/nex_RVClientCore.h"
#include "nn/nex/_Type_stationurl/_Type_stationurl_Api.h"
#include "nn/nex/nex_AnyGatheringAdapter.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_Gathering.h"
#include "nn/nex/nex_ResultRange.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex__DDL_ResultRange.h"

namespace nn {
namespace nex {
// 0x00AE6A48 (name is ours)
MatchMakingProtocolClient::ProtoReturnFunction MatchMakingProtocolClient::s_ProtoReturnFunctions[METHOD_COUNT];

namespace {
// the method is not known
const int RESULT_UNKNOWN_METHOD = static_cast<int>(0x80010002);
} // namespace

using namespace ClientProtocolCall;
namespace {
typedef AnyObjectHolder<Gathering, String> GatheringHolder;

// (inline) the gatherings of a result
inline void ExtractGatherings(Message* pMessage, qList<GatheringHolder>* pList)
{
    pList->ClearInline();
    u32 count;
    pMessage->ExtractPrimitiveInline(&count);
    for (u32 i = 0; i < count; i++) {
        GatheringHolder holder;
        holder.StreamOut(pMessage);
        pList->PushBackInline(holder);
    }
}

// (inline) the station URLs of a result
inline void ExtractURLs(Message* pMessage, qList<StationURL>* pList)
{
    pList->ClearInline();
    u32 count;
    pMessage->ExtractPrimitiveInline(&count);
    for (u32 i = 0; i < count; i++) {
        StationURL url;
        _Type_stationurl::Extract(pMessage, &url);
        pList->PushBackInline(url);
    }
}
} // namespace

// 0x003B722C
bool nn::nex::MatchMakingProtocolClient::CallFindByOwner(nn::nex::ProtocolCallContext* pContext, const unsigned int& id, const nn::nex::ResultRange& resultRange,
                                                         nn::nex::qList<GatheringHolder>* plstGathering)
{
    Message message;
    CriticalSection& lock = GetSchedulerLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_FIND_BY_OWNER)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&id), sizeof(id));
    _DDL_ResultRange::Add(&message, resultRange);
    if (pContext != nullptr) {
        pContext->AddReturnValuePtr(plstGathering);
    }
    s_ProtoReturnFunctions[METHOD_FIND_BY_OWNER - 1] = ProtoReturn_FindByOwner;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003B7398
bool nn::nex::MatchMakingProtocolClient::CallFindBySingleID(nn::nex::ProtocolCallContext* pContext, const unsigned int& id, bool* pbResult, GatheringHolder* pGathering)
{
    Message message;
    CriticalSection& lock = GetSchedulerLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_FIND_BY_SINGLE_ID)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&id), sizeof(id));
    if (pContext != nullptr) {
        pContext->AddReturnValuePtr(pbResult);
        pContext->AddReturnValuePtr(pGathering);
    }
    s_ProtoReturnFunctions[METHOD_FIND_BY_SINGLE_ID - 1] = ProtoReturn_FindBySingleID;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003B7504
bool nn::nex::MatchMakingProtocolClient::CallGetSessionURLs(nn::nex::ProtocolCallContext* pContext, const unsigned int& gid, nn::nex::qList<nn::nex::StationURL>* plstURLs)
{
    Message message;
    CriticalSection& lock = GetSchedulerLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_GET_SESSION_URLS)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&gid), sizeof(gid));
    if (pContext != nullptr) {
        pContext->AddReturnValuePtr(plstURLs);
    }
    s_ProtoReturnFunctions[METHOD_GET_SESSION_URLS - 1] = ProtoReturn_GetSessionURLs;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003B7660
bool nn::nex::MatchMakingProtocolClient::CallUpdateSessionHost(nn::nex::ProtocolCallContext* pContext, const unsigned int& gid, const bool& isMigrateOwner)
{
    Message message;
    CriticalSection& lock = GetSchedulerLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_UPDATE_SESSION_HOST)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&gid), sizeof(gid));
    message << isMigrateOwner;
    s_ProtoReturnFunctions[METHOD_UPDATE_SESSION_HOST - 1] = ProtoReturn_UpdateSessionHost;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003B77B0 | fefates
bool nn::nex::MatchMakingProtocolClient::CallUnregisterGathering(nn::nex::ProtocolCallContext* pContext, bool* pRetval, const unsigned int& idGathering)
{
    Message message;
    CriticalSection& lock = GetSchedulerLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_UNREGISTER_GATHERING)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&idGathering), sizeof(idGathering));
    if (pContext != nullptr) {
        pContext->AddReturnValuePtr(pRetval);
    }
    s_ProtoReturnFunctions[METHOD_UNREGISTER_GATHERING - 1] = ProtoReturn_UnregisterGathering;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003B790C | fefates:bytes [tier B]
void nn::nex::MatchMakingProtocolClient::ProtoReturn_FindByOwner(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    qList<GatheringHolder>* plstGathering = static_cast<qList<GatheringHolder>*>(pContext->GetReturnValuePtr(0));
    if (plstGathering != nullptr) {
        ExtractGatherings(pMessage, plstGathering);
    } else {
        qList<GatheringHolder> temporary[1];
        ExtractGatherings(pMessage, temporary);
    }
}

// 0x003B7CE0
void nn::nex::MatchMakingProtocolClient::ProtoReturn_FindBySingleID(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn(pMessage, static_cast<bool*>(pContext->GetReturnValuePtr(0)));
    if (pMessage->m_IsError) {
        return;
    }
    GatheringHolder* pGathering = static_cast<GatheringHolder*>(pContext->GetReturnValuePtr(1));
    if (pGathering != nullptr) {
        pGathering->StreamOut(pMessage);
    } else {
        GatheringHolder temporary[1];
        temporary[0].StreamOut(pMessage);
    }
}

// 0x003B7E18 | fefates:bytes [tier B]
void nn::nex::MatchMakingProtocolClient::ProtoReturn_GetSessionURLs(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    qList<StationURL>* plstURLs = static_cast<qList<StationURL>*>(pContext->GetReturnValuePtr(0));
    if (plstURLs != nullptr) {
        ExtractURLs(pMessage, plstURLs);
    } else {
        qList<StationURL> temporary[1];
        ExtractURLs(pMessage, temporary);
    }
}

// 0x003B811C
bool nn::nex::MatchMakingProtocolClient::CallMigrateGatheringOwnership(nn::nex::ProtocolCallContext* pContext, const unsigned int& gid, const nn::nex::qList<u32>& lstPotentialNewOwnersID,
                                                                       const bool& participantsOnly)
{
    Message message;
    CriticalSection& lock = GetSchedulerLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_MIGRATE_GATHERING_OWNERSHIP)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&gid), sizeof(gid));
    _Type_qlist<_Type_uint32>::AddInline(&message, lstPotentialNewOwnersID);
    message << participantsOnly;
    s_ProtoReturnFunctions[METHOD_MIGRATE_GATHERING_OWNERSHIP - 1] = ProtoReturn_MigrateGatheringOwnership;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003B82B8
void nn::nex::MatchMakingProtocolClient::ProtoReturn_UpdateSessionHost(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do
}

// 0x003B82BC
void nn::nex::MatchMakingProtocolClient::ProtoReturn_UnregisterGathering(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn(pMessage, static_cast<bool*>(pContext->GetReturnValuePtr(0)));
}

// 0x003B82E8
void nn::nex::MatchMakingProtocolClient::ProtoReturn_MigrateGatheringOwnership(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do
}

// 0x003CF4B8 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE qList<AnyObjectHolder<Gathering, String> >::qList()
{
    // nothing to do: std::list makes the head node
}

// 0x003B7C5C slot 0x50
void nn::nex::MatchMakingProtocolClient::ExtractCallSpecificResults(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    u32 methodId = ExtractMethodID(pMessage) & 0x7FFF;
    if (methodId == 0 || methodId > METHOD_COUNT) {
        SetCallError(qResult(RESULT_UNKNOWN_METHOD));
        return;
    }
    ProtoReturnFunction pFunction = s_ProtoReturnFunctions[methodId - 1];
    if (pFunction != nullptr) {
        pFunction(pMessage, pContext);
    }
}

// 0x003B82EC
nn::nex::MatchMakingProtocolClient::MatchMakingProtocolClient(unsigned short id) : ClientProtocol(FLAG_REGISTER)
{
    if (id != 0) {
        m_ProtocolId = id;
        MatchMakingProtocolClient* pResponder = new MatchMakingProtocolClient(0);
        RVClientCore::GetInstanceInline()->m_pProtocolRegistry->RegisterProtocol(id, pResponder);
    }
}

// 0x003B83E4 slot 0x00
// 0x003B83D4 (deleting dtor)
nn::nex::MatchMakingProtocolClient::~MatchMakingProtocolClient()
{
    // nothing to do
}

// 0x0072D68C slot 0x54
nn::nex::ClientProtocol* nn::nex::MatchMakingProtocolClient::CreateResponder() const
{
    return new MatchMakingProtocolClient(0);
}

// 0x003CF50C (name is ours; it calls the destructor of std::list)
template qList<AnyObjectHolder<Gathering, String> >::~qList();
// 0x00151B80 (an out-of-line copy whose address the original uses; a second one is at 0x00151D64)
template AnyObjectHolder<Gathering, String>::AnyObjectHolder();

} // namespace nex
} // namespace nn

namespace std {
// 0x0082F6AC (name is ours; std::list::~list, the destructor of the qList at 0x003CF50C calls it)
template <>
DECOMP_NOINLINE list<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>, nn::nex::MemAllocator<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String> > >::~list()
{
    DestroyInline();
}

// 0x0082F54C (name is ours)
template <>
DECOMP_NOINLINE void list<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>, nn::nex::MemAllocator<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String> > >::AddBuffer(bool isFirst)
{
    AddBufferInline(isFirst);
}

// 0x0082F5BC (name is ours)
template <>
DECOMP_NOINLINE void list<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>, nn::nex::MemAllocator<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>> >::clear()
{
    ClearEraseInline();
}

// 0x0082F630 (name is ours)
template <>
DECOMP_NOINLINE list<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>, nn::nex::MemAllocator<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String> > >::Node* list<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>, nn::nex::MemAllocator<nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String> > >::erase(Node* pNode)
{
    Node* pNext = pNode;
    if (pNode != m_pHead) {
        pNext = pNode->m_pNext;
        pNode->m_pPrev->m_pNext = pNext;
        pNext->m_pPrev = pNode->m_pPrev;
        m_Size--;
        pNode->m_Value.~AnyObjectHolder();
        pNode->m_pNext = m_pFreeList;
        m_pFreeList = pNode;
    }
    return pNext;
}
} // namespace std
