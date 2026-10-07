#include "nn/pia/session/session_SessionProtocol.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SignatureSetting.h"
#include "nn/pia/session/session_ConfigParticipationJobBase.h"
#include "nn/pia/session/session_JointSessionJob.h"
#include "nn/pia/session/session_Session.h"
#include "nn/pia/session/session_StationIdStatusTable.h"
#include "nn/pia/transport/transport_PacketHandler.h"
#include "nn/pia/transport/transport_ProtocolEvent.h"
#include "nn/pia/transport/transport_ProtocolId.h"
#include "nn/pia/transport/transport_ProtocolMessageReader.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationIdTable.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace session {
namespace {
const u64 TRACE_FLAG = 0x40000000;

// the message types (names are ours)
enum MessageType : u8
{
    MESSAGE_TYPE_SESSION_INFO = 3,
    MESSAGE_TYPE_STATUS = 4,
    MESSAGE_TYPE_STATION_LIST_6 = 6,
    MESSAGE_TYPE_7 = 7,
    MESSAGE_TYPE_STATION_LIST_8 = 8,
    MESSAGE_TYPE_9 = 9,
    MESSAGE_TYPE_STATION_LIST_10 = 10,
    MESSAGE_TYPE_11 = 11,
    MESSAGE_TYPE_12 = 12,
    MESSAGE_TYPE_13 = 13,
    MESSAGE_TYPE_14 = 14,
    MESSAGE_TYPE_15 = 15,
    MESSAGE_TYPE_16 = 16,
    MESSAGE_TYPE_19 = 19,
    MESSAGE_TYPE_STATION_LIST_20 = 20,
    MESSAGE_TYPE_21 = 21,
    MESSAGE_TYPE_22 = 22,
    MESSAGE_TYPE_23 = 23,
    MESSAGE_TYPE_24 = 24,
};

// the header of a message: the type at 0 and the station list from 11 (names are ours)
const u32 STATION_LIST_OFFSET = 11;
const u32 STATION_NUM_MAX = 12;
} // namespace

// (inline)
nn::Result nn::pia::session::SessionProtocol::CheckWindow(transport::ReliableSlidingWindow* pWindow, u32 size)
{
    if (!pWindow->IsInCommunication()) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (pWindow->CanPushData(size)) {
        return nn::Result();
    }
    pWindow->Trace(TRACE_FLAG);
    return common::RESULT_BUFFER_IS_FULL;
}

// (inline)
nn::Result nn::pia::session::SessionProtocol::SendToStations(const StationId* pTargets, u32 targetNum, const StationId& skipStationId, u32 size,
                                                             bool isMarked)
{
    transport::ReliableSlidingWindow* pWindows[STATION_NUM_MAX] = {};
    u32 windowNum = 0;
    transport::StationIdTable* pTable = transport::Transport::s_pInstance->m_pStationIdTable;
    transport::StationIdTable::Entry entry;
    for (u32 i = 0; i < targetNum; i++) {
        const StationId& stationId = pTargets[i];
        if (stationId == skipStationId) {
            continue;
        }
        if (pTable->Find(&entry, stationId).IsFailure() || entry.m_StationIndex > STATION_NUM_MAX) {
            continue;
        }
        transport::ReliableSlidingWindow* pWindow = GetWindow(entry.m_StationIndex);
        pWindows[windowNum] = pWindow;
        nn::Result result = CheckWindow(pWindow, size);
        if (result == common::RESULT_NOT_INITIALIZED) {
            continue;
        }
        if (result == common::RESULT_BUFFER_IS_FULL) {
            return result;
        }
        if (isMarked) {
            Session::s_pInstance->m_pStationIdStatusTable->SetUnknown0x12(stationId, true);
        }
        windowNum++;
    }
    if (windowNum == 0) {
        return common::RESULT_NOT_FOUND;
    }
    u32 sentNum = 0;
    for (u32 i = 0; i < windowNum; i++) {
        if (pWindows[i]->PushData(m_Buffer, size).IsSuccess()) {
            sentNum++;
        }
    }
    if (sentNum != 0) {
        return nn::Result();
    }
    return common::RESULT_NOT_IN_SESSION;
}

// 0x0043425C (name is ours)
nn::Result nn::pia::session::SessionProtocol::Initialize(u32 stationNum)
{
    u32 windowNum = stationNum - 1;
    m_WindowNum = windowNum;
    m_pWindows = common::NewArray<transport::ReliableSlidingWindow>(windowNum);
    for (u32 i = 0; i < m_WindowNum; i++) {
        nn::Result result = m_pWindows[i].Initialize(2, 2);
        if (result.IsFailure()) {
            if (m_pWindows != nullptr) {
                common::DeleteArray(m_pWindows);
                m_pWindows = nullptr;
            }
            m_WindowNum = 0;
            return result;
        }
    }
    return nn::Result();
}

// 0x0043437C (name is ours)
void nn::pia::session::SessionProtocol::ParseMessage(const ReceivedMessage& message)
{
    if (message.m_Size == 0) {
        return;
    }
    switch (message.m_pData[0]) {
    case MESSAGE_TYPE_SESSION_INFO:
        ReceiveSessionInfo(message);
        return;
    case MESSAGE_TYPE_STATUS: {
        StationIndex stationIndex = message.m_StationIndex;
        if (stationIndex >= STATION_NUM_MAX) {
            return;
        }
        const u8* pData = message.m_pData;
        u8 phase = pData[1];
        bool isTwo = pData[2] != 0;
        u32 sessionId = common::deserializeU32(&pData[3]);
        StationId stationId = DeserializeStationId(&pData[7]);
        transport::StationIdTable::Entry entry;
        if (transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, stationIndex).IsFailure() || entry.m_StationId != stationId) {
            return;
        }
        Session* pSession = Session::s_pInstance;
        u32 expectedSessionId;
        if (phase == 9 || phase == 10) {
            expectedSessionId = pSession->m_SessionIds[pSession->m_CurrentIndex];
        } else {
            expectedSessionId = pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0];
        }
        if (sessionId != expectedSessionId) {
            return;
        }
        Session::s_pInstance->m_pStationIdStatusTable->SetStatus(stationId, isTwo);
        return;
    }
    case MESSAGE_TYPE_STATION_LIST_6:
        ReceiveStationList6(message);
        return;
    case MESSAGE_TYPE_STATION_LIST_8:
        ReceiveStationList8(message);
        return;
    case MESSAGE_TYPE_STATION_LIST_10:
        ReceiveStationList10(message);
        return;
    case MESSAGE_TYPE_7:
    case MESSAGE_TYPE_9:
    case MESSAGE_TYPE_11:
    case MESSAGE_TYPE_21: {
        // the answers to the station lists
        StationIndex stationIndex = message.m_StationIndex;
        if (stationIndex >= STATION_NUM_MAX) {
            return;
        }
        transport::StationIdTable::Entry entry;
        if (transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, stationIndex).IsFailure()) {
            return;
        }
        const u8* pData = message.m_pData;
        u8 type = pData[0];
        u8 phase = pData[1];
        bool isOne = pData[2] != 0;
        StationId stationId = DeserializeStationId(&pData[3]);
        if (stationId != entry.m_StationId) {
            return;
        }
        JointSessionJob* pJointSessionJob = Session::s_pInstance->m_pJointSessionJob;
        if (pJointSessionJob->m_Phase != 0 && pJointSessionJob->m_Phase == phase && pJointSessionJob->m_MessageType == type) {
            Session::s_pInstance->m_pStationIdStatusTable->SetUnknown0x1(stationId, isOne);
        }
        return;
    }
    case MESSAGE_TYPE_12: {
        if (message.m_StationIndex >= STATION_NUM_MAX) {
            return;
        }
        const u8* pData = message.m_pData;
        u32 sessionId = common::deserializeU32(&pData[1]);
        u8 value1 = pData[5];
        StationId stationId = DeserializeStationId(&pData[6]);
        u8 value2 = pData[14];
        ConfigParticipationJobBase* pJob = Session::s_pInstance->m_pConfigParticipationJob;
        if (pJob != nullptr) {
            pJob->ReceiveStartRequest(stationId, sessionId, value1, value2);
        }
        return;
    }
    case MESSAGE_TYPE_13: {
        if (message.m_StationIndex >= STATION_NUM_MAX) {
            return;
        }
        StationId stationId = DeserializeStationId(&message.m_pData[2]);
        ConfigParticipationJobBase* pJob = Session::s_pInstance->m_pConfigParticipationJob;
        if (pJob != nullptr) {
            pJob->AddRespondedStation(stationId);
        }
        return;
    }
    case MESSAGE_TYPE_14:
        ReceiveMessage14(message);
        return;
    case MESSAGE_TYPE_15: {
        if (message.m_StationIndex >= STATION_NUM_MAX) {
            return;
        }
        u32 sessionId = common::deserializeU32(&message.m_pData[1]);
        ConfigParticipationJobBase* pJob = Session::s_pInstance->m_pConfigParticipationJob;
        if (pJob != nullptr && pJob->IsRunning() && Session::s_pInstance->GetJoinedSessionId() == sessionId) {
            pJob->SetEndReceived();
        }
        return;
    }
    case MESSAGE_TYPE_16: {
        if (message.m_StationIndex >= STATION_NUM_MAX) {
            return;
        }
        StationId stationId = DeserializeStationId(&message.m_pData[2]);
        ConfigParticipationJobBase* pJob = Session::s_pInstance->m_pConfigParticipationJob;
        if (pJob != nullptr) {
            pJob->AddRespondedStation2(stationId);
        }
        return;
    }
    case MESSAGE_TYPE_19:
        ReceiveMessage19(message);
        return;
    case MESSAGE_TYPE_STATION_LIST_20: {
        StationIndex stationIndex = message.m_StationIndex;
        if (stationIndex >= STATION_NUM_MAX) {
            return;
        }
        transport::StationIdTable::Entry entry;
        if (transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, stationIndex).IsFailure()) {
            return;
        }
        const u8* pData = message.m_pData;
        u8 type = pData[0];
        u8 phase = pData[1];
        StationId stationId = DeserializeStationId(&pData[2]);
        JointSessionJob* pJointSessionJob = Session::s_pInstance->m_pJointSessionJob;
        if (pJointSessionJob->m_IsFailed) {
            return;
        }
        if (pJointSessionJob->m_Phase != 0 && pJointSessionJob->m_Phase == phase && pJointSessionJob->m_StationId == stationId &&
            pJointSessionJob->m_MessageType == type) {
            pJointSessionJob->ReceiveAck20(phase, stationId);
        }
        SendMessage(MESSAGE_TYPE_21, phase, stationId, 1);
        return;
    }
    case MESSAGE_TYPE_22: {
        StationIndex stationIndex = message.m_StationIndex;
        if (stationIndex >= STATION_NUM_MAX) {
            return;
        }
        StationId stationId = DeserializeStationId(&message.m_pData[2]);
        transport::StationIdTable::Entry entry;
        if (transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, stationIndex).IsFailure() || entry.m_StationId != stationId) {
            return;
        }
        // the owner left the joint session
        JointSessionJob* pJointSessionJob = Session::s_pInstance->m_pJointSessionJob;
        if (pJointSessionJob->m_IsFailed || pJointSessionJob->m_Phase == 0) {
            return;
        }
        pJointSessionJob->m_IsFailed = true;
        pJointSessionJob->m_FailureResult = common::RESULT_SESSION_OWNER_LEFT;
        return;
    }
    case MESSAGE_TYPE_23:
        ReceiveMessage23(message);
        return;
    case MESSAGE_TYPE_24: {
        if (message.m_StationIndex >= STATION_NUM_MAX || message.m_pData[2] != 0) {
            return;
        }
        ConfigParticipationJobBase* pJob = Session::s_pInstance->m_pConfigParticipationJob;
        if (pJob != nullptr) {
            pJob->SetEndReceived();
        }
        return;
    }
    default:
        return;
    }
}

// 0x004349A0 (name is ours)
nn::Result nn::pia::session::SessionProtocol::SendSessionInfo(u8 phase, u32 value1, u32 value2, u32 value3, const common::SignatureSetting* pSignatureSetting,
                                                              const StationId* pTargets, u32 targetNum)
{
    transport::StationIdTable* pTable = transport::Transport::s_pInstance->m_pStationIdTable;
    transport::StationIdTable::Entry entry;
    nn::Result result = pTable->Find(&entry, m_LocalStationIndex);
    if (result.IsFailure()) {
        return result;
    }
    StationId localStationId = entry.m_StationId;
    u8* pBuffer = m_Buffer;
    pBuffer[0] = MESSAGE_TYPE_SESSION_INFO;
    pBuffer[1] = phase;
    common::serializeU32(&pBuffer[2], value1);
    common::serializeU32(&pBuffer[6], value2);
    common::serializeU64(&pBuffer[10], localStationId.ToU64());
    // the entry limit of the session the message is about
    Session* pSession = Session::s_pInstance;
    u32 index = pSession->m_CurrentIndex;
    if (phase == 8 || phase == 6 || phase == 7) {
        index = index == 0 ? 1 : 0;
    }
    pBuffer[18] = pSession->m_StationIdEntryNumMax[index];
    common::serializeU32(&pBuffer[19], value3);
    pBuffer[23] = pSignatureSetting->m_Mode;
    pBuffer[24] = pSignatureSetting->m_KeySize;
    const u8* pKey = static_cast<const u8*>(pSignatureSetting->m_pKey);
    for (u32 i = 0; i < 32; i++) {
        pBuffer[25 + i] = pKey[i];
    }
    if (value1 == 0 || value2 == 0) {
        return common::RESULT_INVALID_STATE;
    }
    return SendToStations(pTargets, targetNum, localStationId, 25 + 32, true);
}

// 0x00434CE0 (name is ours)
void nn::pia::session::SessionProtocol::ReceiveSessionInfo(const ReceivedMessage& message)
{
    StationIndex stationIndex = message.m_StationIndex;
    if (stationIndex >= STATION_NUM_MAX) {
        return;
    }
    const u8* pData = message.m_pData;
    u8 type = pData[0];
    u8 phase = pData[1];
    u32 sessionId = common::deserializeU32(&pData[2]);
    u32 value2 = common::deserializeU32(&pData[6]);
    StationId stationId = DeserializeStationId(&pData[10]);
    u8 entryNumMax = pData[18];
    u32 value3 = 0;
    u8 signatureMode = 0xFF;
    u8 signatureKeySize = 0;
    const u8* pSignatureKey = nullptr;
    if (message.m_Size > 18) {
        value3 = common::deserializeU32(&pData[19]);
        signatureMode = pData[23];
        signatureKeySize = pData[24];
        pSignatureKey = &pData[25];
    }
    transport::StationIdTable::Entry entry;
    if (transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, stationIndex).IsFailure() || entry.m_StationId != stationId) {
        return;
    }
    if (entryNumMax == 0) {
        return;
    }
    Session* pSession = Session::s_pInstance;
    if (pSession->m_SessionIds[pSession->m_CurrentIndex] != sessionId) {
        return;
    }
    JointSessionJob* pJointSessionJob = pSession->m_pJointSessionJob;
    if (pJointSessionJob->m_Phase == 0) {
        SendMessage4(phase, stationId, value2, 0);
        return;
    }
    if (pJointSessionJob->m_MessageType != type) {
        return;
    }
    pSession->m_StationIdEntryNumMax[pSession->m_CurrentIndex == 0 ? 1 : 0] = entryNumMax;
    pJointSessionJob->ReceiveSessionInfo(phase, value2, value3, signatureMode, pSignatureKey, signatureKeySize, stationId);
}

// 0x00434E6C (name is ours)
nn::Result nn::pia::session::SessionProtocol::SendStationList(u8 type, u8 phase, const StationId* pStationIds, u32 stationNum, const StationId* pStationId,
                                                              const StationId* pTargets, u32 targetNum)
{
    u8* pBuffer = m_Buffer;
    pBuffer[0] = type;
    pBuffer[1] = phase;
    common::serializeU64(&pBuffer[2], pStationId->ToU64());
    pBuffer[10] = stationNum;
    if (stationNum > STATION_NUM_MAX) {
        return common::RESULT_INVALID_STATE;
    }
    u32 offset = STATION_LIST_OFFSET;
    for (u32 i = 0; i < stationNum; i++) {
        if (BUFFER_SIZE - offset < sizeof(u64)) {
            return common::RESULT_INVALID_STATE;
        }
        common::serializeU64(&pBuffer[offset], pStationIds[i].ToU64());
        offset += sizeof(u64);
    }
    // without targets to the listed stations
    if (pTargets == nullptr) {
        targetNum = stationNum;
        pTargets = pStationIds;
    }
    return SendToStations(pTargets, targetNum, *pStationId, offset, true);
}

// 0x004350EC
nn::Result nn::pia::session::SessionProtocol::UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent& event)
{
    if (m_LocalStationIndex > STATION_INDEX_MAX) {
        return common::RESULT_INVALID_STATE;
    }
    StationIndex stationIndex = event.m_StationIndex;
    if (stationIndex >= static_cast<u8>(m_WindowNum + 1) || stationIndex == m_LocalStationIndex) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    transport::ReliableSlidingWindow* pWindow = GetWindow(stationIndex);
    switch (event.m_Type) {
    case transport::ProtocolEvent::TYPE_JOIN: {
        transport::ProtocolId protocolId;
        protocolId.SetType(GetProtocolType());
        protocolId.SetPort(1);
        return pWindow->Startup(m_pPacketHandler, protocolId.m_Id, m_LocalStationIndex, stationIndex);
    }
    case transport::ProtocolEvent::TYPE_LEAVE:
        pWindow->Cleanup();
        return nn::Result();
    default:
        return common::RESULT_INVALID_ARGUMENT;
    }
}

// 0x004351BC (name is ours)
nn::pia::StationId nn::pia::session::SessionProtocol::DeserializeStationId(const u8* pData) const
{
    return StationId::FromU64(common::deserializeU64(pData));
}

// 0x004351D4 (name is ours)
nn::Result nn::pia::session::SessionProtocol::SendStationList8(u8 phase, const StationId* pStationIds, u32 stationNum, const StationId* pStationId,
                                                               const StationId* pTargets, u32 targetNum)
{
    return SendStationList(MESSAGE_TYPE_STATION_LIST_8, phase, pStationIds, stationNum, pStationId, pTargets, targetNum);
}

// 0x00435208 (name is ours)
void nn::pia::session::SessionProtocol::ReceiveStationList8(const ReceivedMessage& message)
{
    StationIndex stationIndex = message.m_StationIndex;
    if (stationIndex >= STATION_NUM_MAX) {
        return;
    }
    transport::StationIdTable::Entry entry;
    if (transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, stationIndex).IsFailure()) {
        return;
    }
    const u8* pData = message.m_pData;
    u8 type = pData[0];
    u8 phase = pData[1];
    StationId stationId = DeserializeStationId(&pData[2]);
    u32 stationNum = pData[10];
    if (phase != 9) {
        return;
    }
    JointSessionJob* pJointSessionJob = Session::s_pInstance->m_pJointSessionJob;
    if (pJointSessionJob->m_IsFailed) {
        return;
    }
    if (pJointSessionJob->m_Phase != 0) {
        if (pJointSessionJob->m_Phase == 9 || pJointSessionJob->m_Phase == 10) {
            // the host of the other session asks again
            StationId jointHostStationId = Session::s_pInstance->GetJointHostStationId();
            if (jointHostStationId == stationId) {
                SendMessage(MESSAGE_TYPE_9, phase, stationId, 1);
            }
            return;
        }
    } else if (stationId == entry.m_StationId) {
        if (stationNum <= STATION_NUM_MAX) {
            StationId stationIds[STATION_NUM_MAX];
            u32 offset = STATION_LIST_OFFSET;
            for (int i = 0; i < static_cast<int>(stationNum); i++) {
                stationIds[i] = DeserializeStationId(&pData[offset]);
                offset += sizeof(u64);
            }
            if (pJointSessionJob->Start(phase, type, stationIds, stationNum, stationId).IsSuccess()) {
                pJointSessionJob->Ready(false);
                return;
            }
            if (pJointSessionJob->m_Phase != 0) {
                pJointSessionJob->m_IsFailed = true;
                pJointSessionJob->m_FailureResult = common::RESULT_SESSION_OWNER_LEFT;
                return;
            }
        }
        if (Session::s_pInstance->IsJoining()) {
            Session::s_pInstance->SetUnknownFlagOfJoiningJobs();
        } else {
            Session::s_pInstance->SetDisconnectedByError();
        }
        return;
    } else {
        stationId = entry.m_StationId;
        if (pJointSessionJob->m_Phase == 0) {
            if (Session::s_pInstance->IsJoining()) {
                Session::s_pInstance->SetUnknownFlagOfJoiningJobs();
            } else {
                Session::s_pInstance->SetDisconnectedByError();
            }
            return;
        }
    }
    pJointSessionJob->m_IsFailed = true;
    pJointSessionJob->m_FailureResult = common::RESULT_SESSION_OWNER_LEFT;
}

// 0x00435420 (name is ours)
nn::Result nn::pia::session::SessionProtocol::SendMessage7(u8 phase, const StationId& stationId, u8 value)
{
    return SendMessage(MESSAGE_TYPE_7, phase, stationId, value);
}

// 0x0043543C (name is ours)
nn::Result nn::pia::session::SessionProtocol::SendStationList10(u8 phase, const StationId* pStationIds, u32 stationNum, const StationId* pStationId,
                                                                const StationId* pTargets, u32 targetNum)
{
    return SendStationList(MESSAGE_TYPE_STATION_LIST_10, phase, pStationIds, stationNum, pStationId, pTargets, targetNum);
}

// 0x00435470 (name is ours)
void nn::pia::session::SessionProtocol::ReceiveStationList10(const ReceivedMessage& message)
{
    StationIndex stationIndex = message.m_StationIndex;
    if (stationIndex >= STATION_NUM_MAX) {
        return;
    }
    transport::StationIdTable::Entry entry;
    if (transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, stationIndex).IsFailure()) {
        return;
    }
    const u8* pData = message.m_pData;
    u8 type = pData[0];
    u8 phase = pData[1];
    StationId stationId = DeserializeStationId(&pData[2]);
    u8 stationNum = pData[10];
    StationId stationIds[STATION_NUM_MAX];
    u32 offset = STATION_LIST_OFFSET;
    for (u8 i = 0; i < stationNum; i++) {
        stationIds[i] = DeserializeStationId(&pData[offset]);
        offset += sizeof(u64);
    }
    JointSessionJob* pJointSessionJob = Session::s_pInstance->m_pJointSessionJob;
    if (pJointSessionJob->m_IsFailed) {
        return;
    }
    if (pJointSessionJob->m_Phase == 0 || pJointSessionJob->m_Phase != phase || pJointSessionJob->m_StationId != stationId ||
        pJointSessionJob->m_MessageType != type) {
        return;
    }
    pJointSessionJob->ReceiveAck10(phase, stationId);
    for (u8 i = 0; i < stationNum; i++) {
        Session::s_pInstance->m_pStationIdStatusTable->SetStatus(stationIds[i], true);
    }
    SendMessage(MESSAGE_TYPE_11, phase, stationId, 1);
}

// 0x004355EC (name is ours)
nn::Result nn::pia::session::SessionProtocol::SendStationList20(u8 phase, const StationId* pStationIds, u32 stationNum, const StationId* pStationId,
                                                                const StationId* pTargets, u32 targetNum)
{
    return SendStationList(MESSAGE_TYPE_STATION_LIST_20, phase, pStationIds, stationNum, pStationId, pTargets, targetNum);
}

// 0x00435620 (name is ours)
nn::Result nn::pia::session::SessionProtocol::SendStationList6(u8 phase, const StationId* pStationIds, u32 stationNum, const StationId* pStationId,
                                                               const StationId* pTargets, u32 targetNum)
{
    return SendStationList(MESSAGE_TYPE_STATION_LIST_6, phase, pStationIds, stationNum, pStationId, pTargets, targetNum);
}

// 0x00435654 (name is ours)
void nn::pia::session::SessionProtocol::ReceiveStationList6(const ReceivedMessage& message)
{
    StationIndex stationIndex = message.m_StationIndex;
    if (stationIndex >= STATION_NUM_MAX) {
        return;
    }
    transport::StationIdTable::Entry entry;
    if (transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, stationIndex).IsFailure()) {
        return;
    }
    const u8* pData = message.m_pData;
    u8 type = pData[0];
    u8 phase = pData[1];
    StationId stationId = DeserializeStationId(&pData[2]);
    u32 stationNum = pData[10];
    JointSessionJob* pJointSessionJob = Session::s_pInstance->m_pJointSessionJob;
    if (pJointSessionJob->m_IsFailed) {
        return;
    }
    if (pJointSessionJob->m_Phase == 0) {
        if (stationId == entry.m_StationId) {
            if (stationNum > STATION_NUM_MAX) {
                if (Session::s_pInstance->IsJoining()) {
                    Session::s_pInstance->SetUnknownFlagOfJoiningJobs();
                } else {
                    Session::s_pInstance->SetDisconnectedByError();
                }
                return;
            }
            StationId stationIds[STATION_NUM_MAX];
            u32 offset = STATION_LIST_OFFSET;
            for (int i = 0; i < static_cast<int>(stationNum); i++) {
                stationIds[i] = DeserializeStationId(&pData[offset]);
                offset += sizeof(u64);
            }
            if (pJointSessionJob->Start(phase, type, stationIds, stationNum, stationId).IsSuccess()) {
                pJointSessionJob->Ready(false);
                return;
            }
        } else {
            stationId = entry.m_StationId;
        }
    } else if (pJointSessionJob->m_Phase == phase) {
        if (phase == 9 && pJointSessionJob->m_StationId == GetStationIdOfIndex253()) {
            // the list of a new joint session
            if (stationNum <= STATION_NUM_MAX) {
                StationId stationIds[STATION_NUM_MAX];
                u32 offset = STATION_LIST_OFFSET;
                for (int i = 0; i < static_cast<int>(stationNum); i++) {
                    stationIds[i] = DeserializeStationId(&pData[offset]);
                    offset += sizeof(u64);
                }
                if (pJointSessionJob->Restart(phase, stationIds, stationNum, stationId).IsSuccess()) {
                    return;
                }
            }
        } else if (pJointSessionJob->m_StationId == stationId) {
            SendMessage(MESSAGE_TYPE_7, phase, stationId, 1);
            return;
        }
    }
    if (pJointSessionJob->m_Phase != 0) {
        pJointSessionJob->m_IsFailed = true;
        pJointSessionJob->m_FailureResult = common::RESULT_SESSION_OWNER_LEFT;
        return;
    }
    if (Session::s_pInstance->IsJoining()) {
        Session::s_pInstance->SetUnknownFlagOfJoiningJobs();
    } else {
        Session::s_pInstance->SetDisconnectedByError();
    }
}

// 0x00435904 (name is ours)
nn::Result nn::pia::session::SessionProtocol::SendMessage(u8 type, u8 phase, const StationId& stationId, u8 value)
{
    transport::StationIdTable::Entry entry;
    nn::Result result = transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, m_LocalStationIndex);
    if (result.IsFailure()) {
        return result;
    }
    StationId localStationId = entry.m_StationId;
    result = transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, stationId);
    if (result.IsFailure()) {
        return result;
    }
    transport::ReliableSlidingWindow* pWindow = GetWindow(entry.m_StationIndex);
    if (!pWindow->IsInCommunication()) {
        return common::RESULT_INVALID_STATE;
    }
    m_Buffer[0] = type;
    m_Buffer[1] = phase;
    m_Buffer[2] = value;
    common::serializeU64(&m_Buffer[3], localStationId.ToU64());
    return pWindow->PushData(m_Buffer, 11);
}

// 0x004359DC (name is ours)
nn::Result nn::pia::session::SessionProtocol::SendMessage15(u32 sessionId, u8 value, const StationId* pTargets, u32 targetNum)
{
    m_Buffer[0] = MESSAGE_TYPE_15;
    common::serializeU32(&m_Buffer[1], sessionId);
    m_Buffer[5] = value;
    StationId localStationId = Session::s_pInstance->m_LocalStationId;
    return SendToStations(pTargets, targetNum, localStationId, 6, false);
}

// 0x00435BE0 (name is ours)
nn::Result nn::pia::session::SessionProtocol::SendMessage12(u32 sessionId, u8 value1, const StationId* pTargets, u32 targetNum, u8 value2)
{
    StationId localStationId = Session::s_pInstance->m_LocalStationId;
    m_Buffer[0] = MESSAGE_TYPE_12;
    common::serializeU32(&m_Buffer[1], sessionId);
    m_Buffer[5] = value1;
    common::serializeU64(&m_Buffer[6], localStationId.ToU64());
    m_Buffer[14] = value2;
    return SendToStations(pTargets, targetNum, localStationId, 15, false);
}

// 0x00435E00 (name is ours)
nn::Result nn::pia::session::SessionProtocol::SendMessage9(u8 phase, const StationId& stationId, u8 value)
{
    return SendMessage(MESSAGE_TYPE_9, phase, stationId, value);
}

// 0x00435E1C (name is ours)
nn::Result nn::pia::session::SessionProtocol::SendMessage22(u8 phase, const StationId& stationId)
{
    transport::StationIdTable::Entry entry;
    nn::Result result = transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, m_LocalStationIndex);
    if (result.IsFailure()) {
        return result;
    }
    StationId localStationId = entry.m_StationId;
    result = transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, stationId);
    if (result.IsFailure()) {
        return result;
    }
    transport::ReliableSlidingWindow* pWindow = GetWindow(entry.m_StationIndex);
    if (!pWindow->IsInCommunication()) {
        return common::RESULT_INVALID_STATE;
    }
    m_Buffer[0] = MESSAGE_TYPE_22;
    m_Buffer[1] = phase;
    common::serializeU64(&m_Buffer[2], localStationId.ToU64());
    return pWindow->PushData(m_Buffer, 10);
}

// 0x00435EEC (name is ours)
nn::Result nn::pia::session::SessionProtocol::SendMessage14(u32 sessionId, u8 value1, u8 value2, const StationId* pTargets, u32 targetNum)
{
    m_Buffer[0] = MESSAGE_TYPE_14;
    common::serializeU32(&m_Buffer[1], sessionId);
    m_Buffer[5] = value1;
    m_Buffer[6] = value2;
    StationId localStationId = Session::s_pInstance->m_LocalStationId;
    return SendToStations(pTargets, targetNum, localStationId, 7, false);
}

// 0x004360FC (name is ours)
nn::Result nn::pia::session::SessionProtocol::SendMessage4(u8 phase, const StationId& stationId, u32 sessionId, u8 value)
{
    transport::StationIdTable::Entry entry;
    nn::Result result = transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, m_LocalStationIndex);
    if (result.IsFailure()) {
        return result;
    }
    StationId localStationId = entry.m_StationId;
    result = transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, stationId);
    if (result.IsFailure()) {
        return result;
    }
    transport::ReliableSlidingWindow* pWindow = GetWindow(entry.m_StationIndex);
    if (!pWindow->IsInCommunication()) {
        return common::RESULT_INVALID_STATE;
    }
    m_Buffer[0] = MESSAGE_TYPE_STATUS;
    m_Buffer[1] = phase;
    m_Buffer[2] = value;
    common::serializeU32(&m_Buffer[3], sessionId);
    common::serializeU64(&m_Buffer[7], localStationId.ToU64());
    if (sessionId == 0) {
        return common::RESULT_INVALID_STATE;
    }
    return pWindow->PushData(m_Buffer, 15);
}

// 0x004361F8 (name is ours)
void nn::pia::session::SessionProtocol::ReceiveMessage14(const ReceivedMessage& message)
{
    StationIndex stationIndex = message.m_StationIndex;
    if (stationIndex >= STATION_NUM_MAX) {
        return;
    }
    const u8* pData = message.m_pData;
    u32 sessionId = common::deserializeU32(&pData[1]);
    u8 value = pData[5];
    u8 phase = pData[6];
    ConfigParticipationJobBase* pJob = Session::s_pInstance->m_pConfigParticipationJob;
    Session::s_pInstance->GetJoinedSessionId();
    transport::StationIdTable::Entry entry;
    if (transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, stationIndex).IsFailure()) {
        return;
    }
    // the answer: message 16
    m_Buffer[0] = MESSAGE_TYPE_16;
    m_Buffer[1] = phase;
    common::serializeU64(&m_Buffer[2], Session::s_pInstance->m_LocalStationId.ToU64());
    transport::StationIdTable::Entry entry2;
    transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry2, entry.m_StationId);
    transport::ReliableSlidingWindow* pWindow = GetWindow(entry2.m_StationIndex);
    if (pWindow->IsInCommunication()) {
        pWindow->PushData(m_Buffer, 10);
    }
    if (pJob != nullptr && pJob->IsRunning() && Session::s_pInstance->GetJoinedSessionId() == sessionId) {
        pJob->ReceiveFinish(value);
    }
}

// 0x0043633C (name is ours)
nn::Result nn::pia::session::SessionProtocol::SendMessage19(u8 phase, const StationId& stationId, u8 value)
{
    transport::StationIdTable::Entry entry;
    nn::Result result = transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, m_LocalStationIndex);
    if (result.IsFailure()) {
        return result;
    }
    StationId localStationId = entry.m_StationId;
    result = transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, stationId);
    if (result.IsFailure()) {
        return result;
    }
    transport::ReliableSlidingWindow* pWindow = GetWindow(entry.m_StationIndex);
    if (!pWindow->IsInCommunication()) {
        return common::RESULT_INVALID_STATE;
    }
    m_Buffer[0] = MESSAGE_TYPE_19;
    m_Buffer[1] = phase;
    m_Buffer[2] = value;
    common::serializeU64(&m_Buffer[3], localStationId.ToU64());
    return pWindow->PushData(m_Buffer, 11);
}

// 0x00436414 (name is ours)
void nn::pia::session::SessionProtocol::ReceiveMessage19(const ReceivedMessage& message)
{
    StationIndex stationIndex = message.m_StationIndex;
    if (stationIndex >= STATION_NUM_MAX) {
        return;
    }
    const u8* pData = message.m_pData;
    u8 phase = pData[1];
    bool value = pData[2] != 0;
    StationId stationId = DeserializeStationId(&pData[3]);
    transport::StationIdTable::Entry entry;
    if (transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, stationIndex).IsFailure() || entry.m_StationId != stationId) {
        return;
    }
    Session::s_pInstance->m_pStationIdStatusTable->SetUnknown0x13(stationId, value);
    Session* pSession = Session::s_pInstance;
    JointSessionJob* pJointSessionJob = pSession->m_pJointSessionJob;
    if (pJointSessionJob->m_IsFailed) {
        return;
    }
    if (pJointSessionJob->m_Phase != 0) {
        if (!pJointSessionJob->m_Unknown0x9C) {
            return;
        }
        StationId localStationId = pSession->m_LocalStationId;
        SendStationList(MESSAGE_TYPE_STATION_LIST_20, phase, &stationId, 1, &localStationId, nullptr, 0);
        return;
    }
    if (pSession->GetStatus() == Session::STATUS_JOINT) {
        StationId localStationId = Session::s_pInstance->m_LocalStationId;
        SendStationList(MESSAGE_TYPE_STATION_LIST_20, phase, &stationId, 1, &localStationId, nullptr, 0);
        return;
    }
    SendMessage22(phase, stationId);
}

// 0x004365A8 (name is ours)
nn::Result nn::pia::session::SessionProtocol::SendMessage13(const StationId& stationId, u8 value)
{
    m_Buffer[0] = MESSAGE_TYPE_13;
    m_Buffer[1] = value;
    common::serializeU64(&m_Buffer[2], Session::s_pInstance->m_LocalStationId.ToU64());
    transport::StationIdTable::Entry entry;
    transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, stationId);
    transport::ReliableSlidingWindow* pWindow = GetWindow(entry.m_StationIndex);
    if (!pWindow->IsInCommunication()) {
        return common::RESULT_INVALID_STATE;
    }
    return pWindow->PushData(m_Buffer, 10);
}

// 0x00436668 (name is ours)
nn::Result nn::pia::session::SessionProtocol::SendMessage23(const StationId& stationId, u8 value)
{
    m_Buffer[0] = MESSAGE_TYPE_23;
    m_Buffer[1] = value;
    common::serializeU64(&m_Buffer[2], Session::s_pInstance->m_LocalStationId.ToU64());
    transport::StationIdTable::Entry entry;
    nn::Result result = transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, stationId);
    if (result.IsFailure()) {
        return result;
    }
    transport::ReliableSlidingWindow* pWindow = GetWindow(entry.m_StationIndex);
    if (!pWindow->IsInCommunication()) {
        return common::RESULT_INVALID_STATE;
    }
    return pWindow->PushData(m_Buffer, 10);
}

// 0x00436730 (name is ours)
void nn::pia::session::SessionProtocol::ReceiveMessage23(const ReceivedMessage& message)
{
    if (message.m_StationIndex >= STATION_NUM_MAX) {
        return;
    }
    const u8* pData = message.m_pData;
    u8 phase = pData[1];
    StationId stationId = DeserializeStationId(&pData[2]);
    ConfigParticipationJobBase* pJob = Session::s_pInstance->m_pConfigParticipationJob;
    if (pJob == nullptr) {
        return;
    }
    // the answer: message 24 with whether the job runs
    m_Buffer[0] = MESSAGE_TYPE_24;
    m_Buffer[1] = phase;
    m_Buffer[2] = pJob->IsRunning();
    transport::StationIdTable::Entry entry;
    transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, stationId);
    transport::ReliableSlidingWindow* pWindow = GetWindow(entry.m_StationIndex);
    if (pWindow->IsInCommunication()) {
        pWindow->PushData(m_Buffer, 3);
    }
}

// 0x00436810
void nn::pia::session::SessionProtocol::Cleanup()
{
    if (m_LocalStationIndex == STATION_INDEX_UNIDENTIFIED) {
        return;
    }
    m_LocalStationIndex = STATION_INDEX_UNIDENTIFIED;
    for (u32 i = 0; i < m_WindowNum; i++) {
        if (m_pWindows[i].IsInCommunication()) {
            m_pWindows[i].Cleanup();
        }
    }
}

// 0x0043687C
nn::Result nn::pia::session::SessionProtocol::Startup(nn::pia::StationIndex localStationIndex)
{
    if (m_pWindows == nullptr) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (m_LocalStationIndex != STATION_INDEX_UNIDENTIFIED) {
        return common::RESULT_INVALID_STATE;
    }
    if (static_cast<u8>(m_WindowNum) < localStationIndex) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_LocalStationIndex = localStationIndex;
    m_DispatchIndex = 0;
    return nn::Result();
}

// 0x004368CC
nn::Result nn::pia::session::SessionProtocol::Dispatch()
{
    if (m_LocalStationIndex == STATION_INDEX_UNIDENTIFIED) {
        return nn::Result();
    }
    ReceivedMessage message;
    transport::ProtocolId protocolId(GetProtocolType(), 1);
    transport::PacketHandler::Iterator* pIterator = m_pPacketHandler->GetIterator(protocolId);
    pIterator->m_pPacketHandler->BeginIteration();
    while (!pIterator->m_pPacketHandler->IsEndIteration()) {
        const transport::ProtocolMessageReader* pReader = pIterator->GetMessageReader();
        StationIndex sourceStationIndex = pReader->GetSourceStationIndex();
        if (sourceStationIndex <= STATION_INDEX_MAX && m_LocalStationIndex != sourceStationIndex) {
            transport::ReliableSlidingWindow* pWindow = GetWindow(sourceStationIndex);
            if (pWindow->IsInCommunication()) {
                pWindow->AnalyzeProtocolMessage(*pReader);
            }
        }
        pIterator->m_pPacketHandler->NextIteration();
    }
    // the messages of the connected stations
    for (u32 i = 0; i < m_WindowNum; i++) {
        if (!m_pWindows[i].IsInCommunication()) {
            continue;
        }
        transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(m_pWindows[i].m_PeerStationIndex);
        if (pStation == nullptr || pStation->m_State != transport::Station::STATION_STATE_CONNECTED) {
            continue;
        }
        u32 size;
        while (m_pWindows[i].PopData(m_Buffer, &size, BUFFER_SIZE).IsSuccess()) {
            StationIndex stationIndex = m_pWindows[i].m_PeerStationIndex;
            common::StationAddress address(pStation->m_StationAddress);
            message.m_pData = m_Buffer;
            message.m_Size = size;
            message.m_StationIndex = stationIndex;
            message.m_Address = address;
            message.m_Unknown0x1C = 0;
            message.m_Unknown0x20 = 0;
            ParseMessage(message);
        }
    }
    // the windows send in turns, each dispatch beginning with the next one
    m_DispatchIndex++;
    if (m_WindowNum <= m_DispatchIndex) {
        m_DispatchIndex = 0;
    }
    for (u32 i = m_DispatchIndex; i < m_WindowNum; i++) {
        if (m_pWindows[i].IsInCommunication()) {
            m_pWindows[i].Dispatch(m_pPacketHandler);
        }
    }
    for (u32 i = 0; i < m_DispatchIndex; i++) {
        if (m_pWindows[i].IsInCommunication()) {
            m_pWindows[i].Dispatch(m_pPacketHandler);
        }
    }
    // a stream error of the transport ends a session that is not joint
    if (Session::s_pInstance->IsUsingStationIdTable() && transport::Transport::s_pInstance->m_StreamResult.IsFailure()) {
        Session* pSession = Session::s_pInstance;
        if (pSession->m_State != 3 && pSession->m_State != 0) {
            pSession->m_State = 0;
        }
    }
    return nn::Result();
}

// 0x00436BF0 (name is ours)
void nn::pia::session::SessionProtocol::Finalize()
{
    if (m_pWindows != nullptr) {
        common::DeleteArray(m_pWindows);
        m_pWindows = nullptr;
    }
    m_WindowNum = 0;
}

// 0x00436C64
nn::pia::session::SessionProtocol::SessionProtocol()
    : m_LocalStationIndex(STATION_INDEX_UNIDENTIFIED), m_WindowNum(0), m_pWindows(nullptr), m_DispatchIndex(0)
{
}

// 0x00436CA4
// 0x00436C94 (deleting dtor)
nn::pia::session::SessionProtocol::~SessionProtocol()
{
    // empty (in the original too)
}

// 0x007338DC
u16 nn::pia::session::SessionProtocol::GetProtocolType() const
{
    return transport::PROTOCOL_TYPE_SESSION;
}

// 0x007338E4
void nn::pia::session::SessionProtocol::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
