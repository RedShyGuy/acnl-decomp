#include "nn/nex/nex_MatchmakeExtensionProtocolClient.h"
#include "nn/nex/nex_ClientProtocolCall.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_ProtocolRegistry.h"
#include "nn/nex/nex_RVClientCore.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_AnyGatheringAdapter.h"
#include "nn/nex/nex_AutoMatchmakeParam.h"
#include "nn/nex/nex_CreateMatchmakeSessionParam.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_Gathering.h"
#include "nn/nex/nex_JoinMatchmakeSessionParam.h"
#include "nn/nex/nex_MatchmakeSession.h"
#include "nn/nex/nex_MatchmakeSessionSearchCriteria.h"
#include "nn/nex/nex_NotificationEvent.h"
#include "nn/nex/nex_PlayingSession.h"
#include "nn/nex/nex_ResultRange.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_UpdateMatchmakeSessionParam.h"

namespace nn {
namespace nex {
// 0x00ADDA30 (name is ours)
MatchmakeExtensionProtocolClient::ProtoReturnFunction MatchmakeExtensionProtocolClient::s_ProtoReturnFunctions[METHOD_COUNT];

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

// (inline) a result into the return value or into a temporary
inline void ExtractReturnU64(Message* pMessage, u64* pValue)
{
    if (pValue != nullptr) {
        pMessage->ExtractPrimitiveInline(pValue);
    } else {
        u64 temporary;
        pMessage->ExtractPrimitiveInline(&temporary);
    }
}
inline void ExtractReturnSession(Message* pMessage, MatchmakeSession* pSession)
{
    if (pSession != nullptr) {
        _DDL_MatchmakeSession::Extract(pMessage, pSession);
    } else {
        MatchmakeSession temporary[1];
        _DDL_MatchmakeSession::Extract(pMessage, temporary);
    }
}
inline void ExtractReturnGathering(Message* pMessage, GatheringHolder* pGathering)
{
    if (pGathering != nullptr) {
        pGathering->StreamOut(pMessage);
    } else {
        GatheringHolder temporary[1];
        temporary[0].StreamOut(pMessage);
    }
}
} // namespace

// 0x003C6F00 (name is ours)
bool nn::nex::MatchmakeExtensionProtocolClient::CallMethod25(nn::nex::ProtocolCallContext* pContext, const nn::nex::qList<u32>& principalIds)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_25)) {
        return false;
    }
    _Type_qlist<_Type_uint32>::Add(&message, principalIds);
    s_ProtoReturnFunctions[METHOD_25 - 1] = ProtoReturn_Method25;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C7044 (name is ours)
bool nn::nex::MatchmakeExtensionProtocolClient::CallMethod28(nn::nex::ProtocolCallContext* pContext, const unsigned int& value1, const nn::nex::String& text, const unsigned int& value2)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_28)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&value1), sizeof(value1));
    _Type_string::Add(&message, text);
    message.AppendRaw(reinterpret_cast<const u8*>(&value2), sizeof(value2));
    s_ProtoReturnFunctions[METHOD_28 - 1] = ProtoReturn_Method28;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C71B8 (name is ours)
bool nn::nex::MatchmakeExtensionProtocolClient::CallGetPlayingSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::qList<u32>& principalIds,
                                                                     nn::nex::qList<nn::nex::PlayingSession>* pSessions)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_GET_PLAYING_SESSION)) {
        return false;
    }
    _Type_qlist<_Type_uint32>::Add(&message, principalIds);
    if (pContext != nullptr) {
        pContext->AddReturnValuePtr(pSessions);
    }
    s_ProtoReturnFunctions[METHOD_GET_PLAYING_SESSION - 1] = ProtoReturn_GetPlayingSession;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C7318 (name is ours)
bool nn::nex::MatchmakeExtensionProtocolClient::CallOpenParticipation(nn::nex::ProtocolCallContext* pContext, const unsigned int& gatheringId)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_OPEN_PARTICIPATION)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&gatheringId), sizeof(gatheringId));
    s_ProtoReturnFunctions[METHOD_OPEN_PARTICIPATION - 1] = ProtoReturn_OpenParticipation;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C7460 (name is ours)
bool nn::nex::MatchmakeExtensionProtocolClient::CallCloseParticipation(nn::nex::ProtocolCallContext* pContext, const unsigned int& gatheringId)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_CLOSE_PARTICIPATION)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&gatheringId), sizeof(gatheringId));
    s_ProtoReturnFunctions[METHOD_CLOSE_PARTICIPATION - 1] = ProtoReturn_CloseParticipation;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C75A8 (name is ours)
bool nn::nex::MatchmakeExtensionProtocolClient::CallMethod45(nn::nex::ProtocolCallContext* pContext, const nn::nex::AutoMatchmakeParam& param, u64* pResult)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_45)) {
        return false;
    }
    _DDL_AutoMatchmakeParam::Add(&message, param);
    if (pContext != nullptr) {
        pContext->AddReturnValuePtr(pResult);
    }
    s_ProtoReturnFunctions[METHOD_45 - 1] = ProtoReturn_Method45;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C7710 (name is ours)
bool nn::nex::MatchmakeExtensionProtocolClient::CallMethod26(nn::nex::ProtocolCallContext* pContext, const nn::nex::qList<u32>& principalIds)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_26)) {
        return false;
    }
    _Type_qlist<_Type_uint32>::Add(&message, principalIds);
    s_ProtoReturnFunctions[METHOD_26 - 1] = ProtoReturn_Method26;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C7854 (name is ours)
bool nn::nex::MatchmakeExtensionProtocolClient::CallUpdateProgressScore(nn::nex::ProtocolCallContext* pContext, const unsigned int& gatheringId, const u8& score)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_UPDATE_PROGRESS_SCORE)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&gatheringId), sizeof(gatheringId));
    message.AppendRaw(&score, sizeof(score));
    s_ProtoReturnFunctions[METHOD_UPDATE_PROGRESS_SCORE - 1] = ProtoReturn_UpdateProgressScore;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C79B0 (name is ours)
bool nn::nex::MatchmakeExtensionProtocolClient::CallAutoMatchmakePostpone(nn::nex::ProtocolCallContext* pContext, const GatheringHolder& gathering, GatheringHolder* pJoinedGathering,
                                                                         const nn::nex::String& message)
{
    Message request;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &request, lock, pContext, METHOD_AUTO_MATCHMAKE_POSTPONE)) {
        return false;
    }
    gathering.StreamIn(&request);
    _Type_string::Add(&request, message);
    if (pContext != nullptr) {
        pContext->AddReturnValuePtr(pJoinedGathering);
    }
    s_ProtoReturnFunctions[METHOD_AUTO_MATCHMAKE_POSTPONE - 1] = ProtoReturn_AutoMatchmakePostpone;
    return EndRegistered(this, &request, lock, pContext);
}

// 0x003C7B34 (name is ours)
bool nn::nex::MatchmakeExtensionProtocolClient::CallBrowseMatchmakeSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::MatchmakeSessionSearchCriteria& criteria,
                                                                          const nn::nex::ResultRange& range, nn::nex::qList<GatheringHolder>* pGatherings)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_BROWSE_MATCHMAKE_SESSION)) {
        return false;
    }
    _DDL_MatchmakeSessionSearchCriteria::Add(&message, criteria);
    _DDL_ResultRange::Add(&message, range);
    if (pContext != nullptr) {
        pContext->AddReturnValuePtr(pGatherings);
    }
    s_ProtoReturnFunctions[METHOD_BROWSE_MATCHMAKE_SESSION - 1] = ProtoReturn_BrowseMatchmakeSession;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C7CA4 (name is ours)
bool nn::nex::MatchmakeExtensionProtocolClient::CallMethod9(nn::nex::ProtocolCallContext* pContext, const unsigned int& value1, const unsigned int& value2, const unsigned int& value3,
                                                           const nn::nex::String& text)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_9)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&value1), sizeof(value1));
    message.AppendRaw(reinterpret_cast<const u8*>(&value2), sizeof(value2));
    message.AppendRaw(reinterpret_cast<const u8*>(&value3), sizeof(value3));
    _Type_string::Add(&message, text);
    s_ProtoReturnFunctions[METHOD_9 - 1] = ProtoReturn_Method9;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C7EAC (name is ours)
bool nn::nex::MatchmakeExtensionProtocolClient::CallUpdateApplicationBuffer(nn::nex::ProtocolCallContext* pContext, const unsigned int& gatheringId, const nn::nex::qVector<u8>& buffer)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_UPDATE_APPLICATION_BUFFER)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&gatheringId), sizeof(gatheringId));
    _Type_qvector<_Type_byte>::AddInline(&message, buffer);
    s_ProtoReturnFunctions[METHOD_UPDATE_APPLICATION_BUFFER - 1] = ProtoReturn_UpdateApplicationBuffer;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C804C (name is ours)
bool nn::nex::MatchmakeExtensionProtocolClient::CallMethod10(nn::nex::ProtocolCallContext* pContext, nn::nex::qList<nn::nex::NotificationEvent>* pEvents, const unsigned int& value)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_10)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&value), sizeof(value));
    if (pContext != nullptr) {
        pContext->AddReturnValuePtr(pEvents);
    }
    s_ProtoReturnFunctions[METHOD_10 - 1] = ProtoReturn_Method10;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C81B0 (name is ours)
void nn::nex::MatchmakeExtensionProtocolClient::ProtoReturn_GetPlayingSession(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    qList<PlayingSession>* pSessions = static_cast<qList<PlayingSession>*>(pContext->GetReturnValuePtr(0));
    if (pSessions != nullptr) {
        _Type_qlist<DDLClassMetaType<PlayingSession> >::Extract(pMessage, pSessions);
    } else {
        qList<PlayingSession> temporary[1];
        _Type_qlist<DDLClassMetaType<PlayingSession> >::Extract(pMessage, temporary);
    }
}

// 0x003C88F4 (name is ours)
bool nn::nex::MatchmakeExtensionProtocolClient::CallModifyCurrentGameAttribute(nn::nex::ProtocolCallContext* pContext, const unsigned int& gatheringId, const unsigned int& index,
                                                                              const unsigned int& value)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_MODIFY_CURRENT_GAME_ATTRIBUTE)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&gatheringId), sizeof(gatheringId));
    message.AppendRaw(reinterpret_cast<const u8*>(&index), sizeof(index));
    message.AppendRaw(reinterpret_cast<const u8*>(&value), sizeof(value));
    s_ProtoReturnFunctions[METHOD_MODIFY_CURRENT_GAME_ATTRIBUTE - 1] = ProtoReturn_ModifyCurrentGameAttribute;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C8A6C (name is ours)
bool nn::nex::MatchmakeExtensionProtocolClient::CallUpdateMatchmakeSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::UpdateMatchmakeSessionParam& param)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_UPDATE_MATCHMAKE_SESSION)) {
        return false;
    }
    _DDL_UpdateMatchmakeSessionParam::Add(&message, param);
    s_ProtoReturnFunctions[METHOD_UPDATE_MATCHMAKE_SESSION - 1] = ProtoReturn_UpdateMatchmakeSession;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C8BB4 (name is ours)
void nn::nex::MatchmakeExtensionProtocolClient::ProtoReturn_Method45(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturnU64(pMessage, static_cast<u64*>(pContext->GetReturnValuePtr(0)));
}

// 0x003C8CC8 (name is ours)
bool nn::nex::MatchmakeExtensionProtocolClient::CallJoinMatchmakeSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::JoinMatchmakeSessionParam& param,
                                                                        nn::nex::MatchmakeSession* pSession)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_JOIN_MATCHMAKE_SESSION)) {
        return false;
    }
    _DDL_JoinMatchmakeSessionParam::Add(&message, param);
    if (pContext != nullptr) {
        pContext->AddReturnValuePtr(pSession);
    }
    s_ProtoReturnFunctions[METHOD_JOIN_MATCHMAKE_SESSION - 1] = ProtoReturn_JoinMatchmakeSession;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C8E28 (name is ours)
void nn::nex::MatchmakeExtensionProtocolClient::ProtoReturn_AutoMatchmakePostpone(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturnGathering(pMessage, static_cast<GatheringHolder*>(pContext->GetReturnValuePtr(0)));
}

// 0x003C8F10 (name is ours)
void nn::nex::MatchmakeExtensionProtocolClient::ProtoReturn_BrowseMatchmakeSession(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    qList<GatheringHolder>* pGatherings = static_cast<qList<GatheringHolder>*>(pContext->GetReturnValuePtr(0));
    if (pGatherings != nullptr) {
        ExtractGatherings(pMessage, pGatherings);
    } else {
        qList<GatheringHolder> temporary[1];
        ExtractGatherings(pMessage, temporary);
    }
}

// 0x003C94D0 (name is ours)
bool nn::nex::MatchmakeExtensionProtocolClient::CallAutoMatchmake(nn::nex::ProtocolCallContext* pContext, const nn::nex::AutoMatchmakeParam& param, nn::nex::MatchmakeSession* pSession)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_AUTO_MATCHMAKE)) {
        return false;
    }
    _DDL_AutoMatchmakeParam::Add(&message, param);
    if (pContext != nullptr) {
        pContext->AddReturnValuePtr(pSession);
    }
    s_ProtoReturnFunctions[METHOD_AUTO_MATCHMAKE - 1] = ProtoReturn_AutoMatchmake;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C9638 (name is ours)
bool nn::nex::MatchmakeExtensionProtocolClient::CallCreateMatchmakeSession(nn::nex::ProtocolCallContext* pContext, const nn::nex::CreateMatchmakeSessionParam& param,
                                                                          nn::nex::MatchmakeSession* pSession)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_CREATE_MATCHMAKE_SESSION)) {
        return false;
    }
    _DDL_CreateMatchmakeSessionParam::Add(&message, param);
    if (pContext != nullptr) {
        pContext->AddReturnValuePtr(pSession);
    }
    s_ProtoReturnFunctions[METHOD_CREATE_MATCHMAKE_SESSION - 1] = ProtoReturn_CreateMatchmakeSession;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C9888 (name is ours)
void nn::nex::MatchmakeExtensionProtocolClient::ProtoReturn_Method10(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    qList<NotificationEvent>* pEvents = static_cast<qList<NotificationEvent>*>(pContext->GetReturnValuePtr(0));
    if (pEvents != nullptr) {
        _Type_qlist<DDLClassMetaType<NotificationEvent> >::Extract(pMessage, pEvents);
    } else {
        qList<NotificationEvent> temporary[1];
        _Type_qlist<DDLClassMetaType<NotificationEvent> >::Extract(pMessage, temporary);
    }
}

// 0x003C9E78 (name is ours)
bool nn::nex::MatchmakeExtensionProtocolClient::CallClearMatchmakeSessionSystemPassword(nn::nex::ProtocolCallContext* pContext, const unsigned int& gatheringId)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_CLEAR_MATCHMAKE_SESSION_SYSTEM_PASSWORD)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&gatheringId), sizeof(gatheringId));
    s_ProtoReturnFunctions[METHOD_CLEAR_MATCHMAKE_SESSION_SYSTEM_PASSWORD - 1] = ProtoReturn_ClearMatchmakeSessionSystemPassword;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003C9FC0 (name is ours)
void nn::nex::MatchmakeExtensionProtocolClient::ProtoReturn_JoinMatchmakeSession(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturnSession(pMessage, static_cast<MatchmakeSession*>(pContext->GetReturnValuePtr(0)));
}

// 0x003CA03C (name is ours)
bool nn::nex::MatchmakeExtensionProtocolClient::CallGenerateMatchmakeSessionSystemPassword(nn::nex::ProtocolCallContext* pContext, const unsigned int& gatheringId,
                                                                                          nn::nex::String* pPassword)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_GENERATE_MATCHMAKE_SESSION_SYSTEM_PASSWORD)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&gatheringId), sizeof(gatheringId));
    if (pContext != nullptr) {
        pContext->AddReturnValuePtr(pPassword);
    }
    s_ProtoReturnFunctions[METHOD_GENERATE_MATCHMAKE_SESSION_SYSTEM_PASSWORD - 1] = ProtoReturn_GenerateMatchmakeSessionSystemPassword;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003CA1A0 (name is ours)
bool nn::nex::MatchmakeExtensionProtocolClient::CallGetMatchmakeSession(nn::nex::ProtocolCallContext* pContext, const unsigned int& gatheringId, nn::nex::MatchmakeSession* pSession)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_GET_MATCHMAKE_SESSION)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&gatheringId), sizeof(gatheringId));
    if (pContext != nullptr) {
        pContext->AddReturnValuePtr(pSession);
    }
    s_ProtoReturnFunctions[METHOD_GET_MATCHMAKE_SESSION - 1] = ProtoReturn_GetMatchmakeSession;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003CA304 (name is ours)
void nn::nex::MatchmakeExtensionProtocolClient::ProtoReturn_AutoMatchmake(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturnSession(pMessage, static_cast<MatchmakeSession*>(pContext->GetReturnValuePtr(0)));
}

// 0x003CA380 (name is ours)
void nn::nex::MatchmakeExtensionProtocolClient::ProtoReturn_CreateMatchmakeSession(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturnSession(pMessage, static_cast<MatchmakeSession*>(pContext->GetReturnValuePtr(0)));
}

// 0x003CA400 (name is ours)
void nn::nex::MatchmakeExtensionProtocolClient::ProtoReturn_GenerateMatchmakeSessionSystemPassword(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    String* pPassword = static_cast<String*>(pContext->GetReturnValuePtr(0));
    if (pPassword != nullptr) {
        _Type_string::Extract(pMessage, pPassword);
    } else {
        String temporary[1];
        _Type_string::Extract(pMessage, temporary);
    }
}

// 0x003CA47C (name is ours)
void nn::nex::MatchmakeExtensionProtocolClient::ProtoReturn_GetMatchmakeSession(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturnSession(pMessage, static_cast<MatchmakeSession*>(pContext->GetReturnValuePtr(0)));
}

// 0x003C7EA8 (name is ours)
void nn::nex::MatchmakeExtensionProtocolClient::ProtoReturn_Method25(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do
}

// 0x003C8048 (name is ours)
void nn::nex::MatchmakeExtensionProtocolClient::ProtoReturn_Method28(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do
}

// 0x003C88F0 (name is ours)
void nn::nex::MatchmakeExtensionProtocolClient::ProtoReturn_OpenParticipation(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do
}

// 0x003C8BB0 (name is ours)
void nn::nex::MatchmakeExtensionProtocolClient::ProtoReturn_CloseParticipation(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do
}

// 0x003C8CB8 (name is ours)
void nn::nex::MatchmakeExtensionProtocolClient::ProtoReturn_Method26(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do
}

// 0x003C8CBC (name is ours)
void nn::nex::MatchmakeExtensionProtocolClient::ProtoReturn_UpdateProgressScore(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do
}

// 0x003C94C4 (name is ours)
void nn::nex::MatchmakeExtensionProtocolClient::ProtoReturn_Method9(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do
}

// 0x003C9884 (name is ours)
void nn::nex::MatchmakeExtensionProtocolClient::ProtoReturn_UpdateApplicationBuffer(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do
}

// 0x003C9E70 (name is ours)
void nn::nex::MatchmakeExtensionProtocolClient::ProtoReturn_ModifyCurrentGameAttribute(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do
}

// 0x003C9E74 (name is ours)
void nn::nex::MatchmakeExtensionProtocolClient::ProtoReturn_UpdateMatchmakeSession(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do
}

// 0x003CA3FC (name is ours)
void nn::nex::MatchmakeExtensionProtocolClient::ProtoReturn_ClearMatchmakeSessionSystemPassword(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do
}

// 0x003C7E24 slot 0x50
void nn::nex::MatchmakeExtensionProtocolClient::ExtractCallSpecificResults(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
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

// 0x003CA4F8
nn::nex::MatchmakeExtensionProtocolClient::MatchmakeExtensionProtocolClient(unsigned short id) : ClientProtocol(FLAG_REGISTER)
{
    if (id != 0) {
        m_ProtocolId = id;
        MatchmakeExtensionProtocolClient* pResponder = new MatchmakeExtensionProtocolClient(0);
        RVClientCore::GetInstanceInline()->m_pProtocolRegistry->RegisterProtocol(id, pResponder);
    }
}

// 0x003CA5F0 slot 0x00
// 0x003CA5E0 (deleting dtor)
nn::nex::MatchmakeExtensionProtocolClient::~MatchmakeExtensionProtocolClient()
{
    // nothing to do
}

// 0x0072DEDC slot 0x54
nn::nex::ClientProtocol* nn::nex::MatchmakeExtensionProtocolClient::CreateResponder() const
{
    return new MatchmakeExtensionProtocolClient(0);
}

} // namespace nex
} // namespace nn
