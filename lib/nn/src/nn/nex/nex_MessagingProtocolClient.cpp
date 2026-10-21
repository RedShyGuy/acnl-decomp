#include "nn/nex/nex_MessagingProtocolClient.h"
#include "nn/nex/nex_ClientProtocolCall.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_ProtocolRegistry.h"
#include "nn/nex/nex_RVClientCore.h"

namespace nn {
namespace nex {
// 0x00ADDB7C (name is ours)
MessagingProtocolClient::ProtoReturnFunction MessagingProtocolClient::s_ProtoReturnFunctions[METHOD_COUNT];

namespace {
// the method is not known
const int RESULT_UNKNOWN_METHOD = static_cast<int>(0x80010002);
} // namespace

using namespace ClientProtocolCall;

// 0x003B1174 slot 0x50
void nn::nex::MessagingProtocolClient::ExtractCallSpecificResults(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
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

// 0x003B11F8
nn::nex::MessagingProtocolClient::MessagingProtocolClient(unsigned short id) : ClientProtocol(FLAG_REGISTER)
{
    if (id != 0) {
        m_ProtocolId = id;
        MessagingProtocolClient* pResponder = new MessagingProtocolClient(0);
        RVClientCore::GetInstanceInline()->m_pProtocolRegistry->RegisterProtocol(id, pResponder);
    }
}

// 0x003B12F0 slot 0x00
// 0x003B12E0 (deleting dtor)
nn::nex::MessagingProtocolClient::~MessagingProtocolClient()
{
    // nothing to do
}

// 0x0072D25C slot 0x54
nn::nex::ClientProtocol* nn::nex::MessagingProtocolClient::CreateResponder() const
{
    return new MessagingProtocolClient(0);
}

} // namespace nex
} // namespace nn
