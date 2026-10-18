#include "nn/nex/nex_MatchMakingProtocolExtClient.h"
#include "nn/nex/nex_ClientProtocolCall.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_ProtocolRegistry.h"
#include "nn/nex/nex_RVClientCore.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// 0x00AE6AFC (name is ours)
MatchMakingProtocolExtClient::ProtoReturnFunction MatchMakingProtocolExtClient::s_ProtoReturnFunctions[METHOD_COUNT];

namespace {
// the method is not known
const int RESULT_UNKNOWN_METHOD = static_cast<int>(0x80010002);
} // namespace

using namespace ClientProtocolCall;

// 0x003BDAC8 | fefates
bool nn::nex::MatchMakingProtocolExtClient::CallEndParticipation(nn::nex::ProtocolCallContext* pContext, bool* pRetval, const unsigned int& idGathering, const nn::nex::String& strMessage)
{
    Message message;
    CriticalSection& lock = GetSchedulerLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_END_PARTICIPATION)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&idGathering), sizeof(idGathering));
    _Type_string::Add(&message, strMessage);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pRetval);
    }
    s_ProtoReturnFunctions[METHOD_END_PARTICIPATION - 1] = ProtoReturn_EndParticipation;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003BDCB8 (name is ours)
void nn::nex::MatchMakingProtocolExtClient::ProtoReturn_EndParticipation(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn(pMessage, static_cast<bool*>(pContext->GetReturnValuePtr(0)));
}

// 0x003BDC34 slot 0x50
void nn::nex::MatchMakingProtocolExtClient::ExtractCallSpecificResults(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
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

// 0x003BDCE4
nn::nex::MatchMakingProtocolExtClient::MatchMakingProtocolExtClient(unsigned short id) : ClientProtocol(FLAG_REGISTER)
{
    if (id != 0) {
        m_ProtocolId = id;
        MatchMakingProtocolExtClient* pResponder = new MatchMakingProtocolExtClient(0);
        RVClientCore::GetInstanceInline()->m_pProtocolRegistry->RegisterProtocol(id, pResponder);
    }
}

// 0x003BDDDC slot 0x00
// 0x003BDDCC (deleting dtor)
nn::nex::MatchMakingProtocolExtClient::~MatchMakingProtocolExtClient()
{
    // nothing to do
}

// 0x0072DE20 slot 0x54
nn::nex::ClientProtocol* nn::nex::MatchMakingProtocolExtClient::CreateResponder() const
{
    return new MatchMakingProtocolExtClient(0);
}

} // namespace nex
} // namespace nn
