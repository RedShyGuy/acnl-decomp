#include "nn/nex/nex_Protocol.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_CallProtocolMethodOperation.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_ProtocolRequestBroker.h"
#include "nn/nex/nex_ProtocolRequestBrokerInterface.h"

namespace nn {
namespace nex {
namespace {
const int RESULT_SUCCESS = qResult::SUCCESS;
const int RESULT_FAILURE = qResult::FAILURE;
// the outcome byte of a response
const u8 OUTCOME_FAILURE = 0;
const u8 OUTCOME_SUCCESS = 1;
const u8 REQUEST_BIT = 0x80;
} // namespace

// 0x003D5E10 (name is ours)
void nn::nex::Protocol::SetFlag(u32 flag)
{
    m_Flags |= flag;
}

// 0x00391B54 | fefates:bytes [tier B]
nn::nex::CallContext* nn::nex::Protocol::RetrieveCallContext(nn::nex::Message* pMessage)
{
    u32 callId;
    pMessage->ExtractPrimitive(&callId);
    return Core::GetInstance()->m_pCallContextRegister->GetCallContext(callId);
}

// 0x003D5A30 | mk7dlp:bytes [tier A]
void nn::nex::Protocol::AddMethodID(nn::nex::Message* pMessage, unsigned int methodId)
{
    pMessage->AppendRaw(reinterpret_cast<const unsigned char*>(&methodId), sizeof(methodId));
}

// 0x003D5A4C | fefates:callgraph [tier C]
void nn::nex::ProtocolRequestBroker::InitMessage(nn::nex::Message* pMessage, unsigned short protocolId, nn::nex::Protocol::_Type type)
{
    Protocol::AddProtocolKey(pMessage, static_cast<Protocol::_Command>(type), protocolId);
}

// 0x003D5A5C | fefates:bytes [tier B]
void nn::nex::Protocol::AddProtocolKey(nn::nex::Message* pMessage, nn::nex::Protocol::_Command command, unsigned short protocolId)
{
    u8 key;
    if (protocolId < SHORT_PROTOCOL_ID) {
        key = ((command != COMMAND_RESPONSE ? REQUEST_BIT : 0) & REQUEST_BIT) | (protocolId & SHORT_PROTOCOL_ID);
        pMessage->AppendRaw(&key, sizeof(key));
        return;
    }
    key = ((command != COMMAND_RESPONSE ? REQUEST_BIT : 0) & REQUEST_BIT) | SHORT_PROTOCOL_ID;
    pMessage->AppendRaw(&key, sizeof(key));
    pMessage->AppendRaw(reinterpret_cast<const unsigned char*>(&protocolId), sizeof(protocolId));
}

// 0x003D5AE8 | fefates:bytes [tier B]
void nn::nex::Protocol::AddCallResponse(nn::nex::Message* pMessage, nn::nex::CallProtocolMethodOperation* pOperation, unsigned int callId, const nn::nex::Message& results)
{
    qResult result = pOperation->m_Result;
    if (result) {
        u8 outcome = OUTCOME_SUCCESS;
        pMessage->AppendRaw(&outcome, sizeof(outcome));
    } else {
        u8 outcome = OUTCOME_FAILURE;
        pMessage->AppendRaw(&outcome, sizeof(outcome));
        s32 code = result.m_Code;
        pMessage->AppendRaw(reinterpret_cast<const unsigned char*>(&code), sizeof(code));
    }
    pMessage->AppendRaw(reinterpret_cast<const unsigned char*>(&callId), sizeof(callId));
    if (result) {
        pMessage->AddDataFromMessage(results);
    }
}

// 0x003D5BBC | mk7dlp:callseq-callee [tier A]
u32 nn::nex::Protocol::ExtractMethodID(nn::nex::Message* pMessage)
{
    u32 methodId;
    pMessage->ExtractPrimitive(&methodId);
    return methodId;
}

// 0x003D5BD0 | fefates:bytes [tier B]
bool nn::nex::Protocol::BeginTermination()
{
    if (m_Flags & FLAG_REGISTER) {
        m_pBroker->UnregisterProtocol(this);
    }
    return true;
}

// 0x003D5C00 | fefates:bytes [tier B]
void nn::nex::Protocol::ExtractCallOutcome(nn::nex::Message* pMessage, nn::nex::qResult* pResult)
{
    u8 outcome = 0xFF;
    pMessage->ExtractPrimitive(&outcome);
    if (outcome == OUTCOME_FAILURE) {
        s32 code;
        pMessage->ExtractPrimitive(&code);
        *pResult = qResult(code);
    } else if (outcome == OUTCOME_SUCCESS) {
        *pResult = qResult(RESULT_SUCCESS);
    } else {
        *pResult = qResult(RESULT_FAILURE);
    }
}

// 0x003D5D08 | fefates:bytes [tier B]
void nn::nex::Protocol::ExtractProtocolKey(nn::nex::Message* pMessage, nn::nex::Protocol::_Command& command, unsigned short& protocolId)
{
    u8 key;
    pMessage->ExtractPrimitive(&key);
    command = static_cast<_Command>(key >> 7);
    protocolId = key & SHORT_PROTOCOL_ID;
    if (protocolId == SHORT_PROTOCOL_ID) {
        pMessage->ExtractPrimitive(&protocolId);
    }
}

// 0x003D5D8C | fefates:bytes [tier B]
bool nn::nex::Protocol::BeginInitialization()
{
    if (m_Flags & FLAG_REGISTER) {
        m_pBroker->RegisterProtocol(this);
    }
    return true;
}

// 0x003D5DBC | mk7dlp:callseq-callee [tier A]
bool nn::nex::Protocol::RegisterCallContext(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    if (!pContext->InitiateCall()) {
        return false;
    }
    u32 callId = pContext->m_Id;
    pMessage->AppendRaw(reinterpret_cast<const unsigned char*>(&callId), sizeof(callId));
    return true;
}

// 0x003D5DFC (name is ours)
u32 nn::nex::Protocol::ExtractCallID(nn::nex::Message* pMessage)
{
    u32 callId;
    pMessage->ExtractPrimitive(&callId);
    return callId;
}

// 0x003D5DF8
void nn::nex::Protocol::vf_0x44()
{
    // nothing to do in this build
}

// 0x003D5A48
void nn::nex::Protocol::vf_0x48()
{
    // nothing to do in this build
}

// 0x0072E9E8
nn::nex::Protocol* nn::nex::Protocol::Clone()
{
    return nullptr;
}

// 0x0072E9F0 | fefates:bytes [tier B]
bool nn::nex::Protocol::FlagIsSet(unsigned int flags) const
{
    return (flags & ~m_Flags) == 0;
}

} // namespace nex
} // namespace nn
