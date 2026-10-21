#include "nn/nex/nex_UtilityProtocolClient.h"
#include "nn/nex/nex_ClientProtocolCall.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_ProtocolRegistry.h"
#include "nn/nex/nex_RVClientCore.h"

namespace nn {
namespace nex {
// 0x00ADDB24 (name is ours)
UtilityProtocolClient::ProtoReturnFunction UtilityProtocolClient::s_ProtoReturnFunctions[METHOD_COUNT];

namespace {
// the method is not known
const int RESULT_UNKNOWN_METHOD = static_cast<int>(0x80010002);
typedef _Type_map<_Type_uint16, _Type_uint32> SettingsType;
} // namespace

using namespace ClientProtocolCall;

// 0x0039AE48 (name is ours)
bool nn::nex::UtilityProtocolClient::CallGetIntegerSettings(nn::nex::ProtocolCallContext* pContext, const unsigned int& index, nn::nex::qMap<u16, u32>* pSettings)
{
    Message message;
    CriticalSection& lock = GetSchedulerLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_GET_INTEGER_SETTINGS)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&index), sizeof(index));
    if (pContext != nullptr) {
        pContext->AddReturnValuePtr(pSettings);
    }
    s_ProtoReturnFunctions[METHOD_GET_INTEGER_SETTINGS - 1] = ProtoReturn_GetIntegerSettings;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x0039AFA4 slot 0x50
void nn::nex::UtilityProtocolClient::ExtractCallSpecificResults(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
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

// 0x0039B028 (name is ours)
void nn::nex::UtilityProtocolClient::ProtoReturn_GetIntegerSettings(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    qMap<u16, u32>* pSettings = static_cast<qMap<u16, u32>*>(pContext->GetReturnValuePtr(0));
    if (pSettings != nullptr) {
        SettingsType::Extract(pMessage, pSettings);
    } else {
        qMap<u16, u32> temporary[1];
        SettingsType::Extract(pMessage, temporary);
    }
}

// 0x0039B574
nn::nex::UtilityProtocolClient::UtilityProtocolClient(unsigned short id) : ClientProtocol(FLAG_REGISTER)
{
    if (id != 0) {
        m_ProtocolId = id;
        UtilityProtocolClient* pResponder = new UtilityProtocolClient(0);
        RVClientCore::GetInstanceInline()->m_pProtocolRegistry->RegisterProtocol(id, pResponder);
    }
}

// 0x00370EEC slot 0x00
// 0x0039B65C (deleting dtor)
nn::nex::UtilityProtocolClient::~UtilityProtocolClient()
{
    // nothing to do
}

// 0x0072CE48 slot 0x54
nn::nex::ClientProtocol* nn::nex::UtilityProtocolClient::CreateResponder() const
{
    return new UtilityProtocolClient(0);
}

// 0x003CE88C (name is ours)
template <>
DECOMP_NOINLINE qMap<u16, u32>::qMap()
{
    // nothing to do: the tree is constructed by the compiler
}

// 0x003CE928 (name is ours)
template <>
DECOMP_NOINLINE qMap<u16, u32>::~qMap()
{
    // nothing to do: the tree is destroyed by the compiler
}

} // namespace nex
} // namespace nn

namespace __rw {
// 0x0080C958 (name is ours)
template <>
DECOMP_NOINLINE __rb_tree<u16, std::pair<const u16, u32>, __select1st<std::pair<const u16, u32>, u16>, std::less<u16>, nn::nex::MemAllocator<std::pair<const u16, u32> > >::iterator
__rb_tree<u16, std::pair<const u16, u32>, __select1st<std::pair<const u16, u32>, u16>, std::less<u16>, nn::nex::MemAllocator<std::pair<const u16, u32> > >::erase(iterator first,
                                                                                                                                                               iterator last)
{
    return EraseRangeAllInline(first, last);
}

// 0x0080D100 (name is ours)
template __rb_tree<u16, std::pair<const u16, u32>, __select1st<std::pair<const u16, u32>, u16>, std::less<u16>, nn::nex::MemAllocator<std::pair<const u16, u32> > >::iterator
__rb_tree<u16, std::pair<const u16, u32>, __select1st<std::pair<const u16, u32>, u16>, std::less<u16>, nn::nex::MemAllocator<std::pair<const u16, u32> > >::_C_insert(
    Node* pNode, Node* pParent, const std::pair<const u16, u32>& value);
// 0x0080D0C4 (name is ours)
template void __rb_tree<u16, std::pair<const u16, u32>, __select1st<std::pair<const u16, u32>, u16>, std::less<u16>, nn::nex::MemAllocator<std::pair<const u16, u32> > >::_C_erase(Node* pNode);
} // namespace __rw
