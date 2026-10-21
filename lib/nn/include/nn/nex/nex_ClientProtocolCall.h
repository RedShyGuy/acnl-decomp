#pragma once

#include "decomp.h"
#include "nn/nex/_Type_buffer/_Type_buffer_Api.h"
#include "nn/nex/_Type_qresult/_Type_qresult_Api.h"
#include "nn/nex/_Type_stationurl/_Type_stationurl_Api.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_ClientProtocol.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_ProtocolRequestBroker.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// The parts of the calls of the generated protocol clients (inline in every call; the names are
// ours).
namespace ClientProtocolCall {
// the flag of the protocols whose local calls are sent without the lock
const u32 FLAG_SEND_UNLOCKED = 1 << 4;

// the lock of the scheduler
inline CriticalSection& GetLock()
{
    Core* pCore = Core::GetInstance();
    return (pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock;
}

// the lock of the scheduler through the out-of-line Scheduler::GetInstance (the clients of the
// smaller protocols)
inline CriticalSection& GetSchedulerLock()
{
    return Scheduler::GetInstance()->m_Lock;
}

// the lock is entered and the protocol key, call id and method id start the request; false (with
// the lock left) if the call cannot start
inline bool Begin(ClientProtocol* pProtocol, Message* pMessage, CriticalSection& lock, ProtocolCallContext* pContext, u32 methodId)
{
    lock.Enter();
    Protocol::AddProtocolKey(pMessage, Protocol::COMMAND_REQUEST, pProtocol->m_ProtocolId);
    if (!pContext->InitiateCall()) {
        lock.Leave();
        return false;
    }
    u32 callId = pContext->m_Id;
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&callId), sizeof(callId));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&methodId), sizeof(methodId));
    return true;
}

// the request is sent and the lock left (before the sending for the local calls of the protocols
// with FLAG_SEND_UNLOCKED)
inline bool End(ClientProtocol* pProtocol, Message* pMessage, CriticalSection& lock, ProtocolCallContext* pContext)
{
    bool result;
    if (pProtocol->m_IsEndPointKept && (pProtocol->m_Flags & FLAG_SEND_UNLOCKED)) {
        lock.Leave();
        result = pProtocol->SendRequest(pContext, pMessage);
    } else {
        result = pProtocol->SendRequest(pContext, pMessage);
        lock.Leave();
    }
    return result;
}

// the same with the out-of-line helpers of Protocol (the DataStore client): InitMessage,
// RegisterCallContext and AddMethodID start the request
inline bool BeginRegistered(ClientProtocol* pProtocol, Message* pMessage, CriticalSection& lock, ProtocolCallContext* pContext, u32 methodId)
{
    lock.Enter();
    ProtocolRequestBroker::InitMessage(pMessage, pProtocol->m_ProtocolId, static_cast<Protocol::_Type>(Protocol::COMMAND_REQUEST));
    if (!Protocol::RegisterCallContext(pMessage, pContext)) {
        lock.Leave();
        return false;
    }
    Protocol::AddMethodID(pMessage, methodId);
    return true;
}
inline bool EndRegistered(ClientProtocol* pProtocol, Message* pMessage, CriticalSection& lock, ProtocolCallContext* pContext)
{
    bool result;
    if (pProtocol->m_IsEndPointKept && pProtocol->FlagIsSet(FLAG_SEND_UNLOCKED)) {
        lock.Leave();
        result = pProtocol->SendRequest(pContext, pMessage);
    } else {
        result = pProtocol->SendRequest(pContext, pMessage);
        lock.Leave();
    }
    return result;
}

// the results of a call: into the return values, or into temporaries if they are null
inline void ExtractReturn(Message* pMessage, qResult* pResult)
{
    if (pResult != nullptr) {
        _Type_qresult::Extract(pMessage, pResult);
    } else {
        qResult temporary[1];
        _Type_qresult::Extract(pMessage, temporary);
    }
}
inline void ExtractReturn(Message* pMessage, u32* pValue)
{
    if (pValue != nullptr) {
        pMessage->ExtractPrimitiveInline(pValue);
    } else {
        u32 temporary;
        pMessage->ExtractPrimitiveInline(&temporary);
    }
}
inline void ExtractReturn(Message* pMessage, bool* pValue)
{
    if (pValue != nullptr) {
        *pMessage >> *pValue;
    } else {
        bool temporary;
        *pMessage >> temporary;
    }
}
inline void ExtractReturn(Message* pMessage, Buffer* pBuffer)
{
    if (pBuffer != nullptr) {
        _Type_buffer::Extract(pMessage, pBuffer);
    } else {
        Buffer temporary[1];
        _Type_buffer::Extract(pMessage, temporary);
    }
}
inline void ExtractReturn(Message* pMessage, String* pString)
{
    if (pString != nullptr) {
        _Type_string::Extract(pMessage, pString);
    } else {
        String temporary[1];
        _Type_string::Extract(pMessage, temporary);
    }
}
inline void ExtractReturn(Message* pMessage, StationURL* pURL)
{
    if (pURL != nullptr) {
        _Type_stationurl::Extract(pMessage, pURL);
    } else {
        StationURL temporary[1];
        _Type_stationurl::Extract(pMessage, temporary);
    }
}
} // namespace ClientProtocolCall
} // namespace nex
} // namespace nn
