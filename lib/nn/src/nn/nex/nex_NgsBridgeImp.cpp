#include "nn/nex/nex_NgsBridgeImp.h"
#include "nn/nex/nex_Credentials.h"
#include "nn/nex/nex_NgsFacade.h"
#include "nn/nex/nex_NotificationEventManager.h"
#include "nn/nex/nex_SecureConnectionClient.h"

namespace nn {
namespace nex {
// 0x0035D518 slot 0x18 | fefates:callseq
bool nn::nex::NgsBridgeImp::ReplaceURL(nn::nex::ProtocolCallContext* pContext, const nn::nex::StationURL& oldUrl, const nn::nex::StationURL& newUrl)
{
    if (m_pFacade->GetSecureConnectionClient() == nullptr) {
        return false;
    }
    return m_pFacade->GetSecureConnectionClient()->ReplaceURL(pContext, oldUrl, newUrl);
}

// 0x0035D55C slot 0x1C | fefates:callseq
void nn::nex::NgsBridgeImp::SendReport(unsigned int type, const void* pData, unsigned int size)
{
    if (m_pFacade->GetSecureConnectionClient() == nullptr) {
        return;
    }
    m_pFacade->GetSecureConnectionClient()->SendReport(type, pData, size);
}

// 0x0035D5A0 slot 0x08 (name is ours)
nn::nex::Credentials* nn::nex::NgsBridgeImp::GetCredentials()
{
    return m_pFacade->m_pCredentials;
}

// 0x0035D5AC slot 0x0C (name is ours)
u32 nn::nex::NgsBridgeImp::GetPrincipalId()
{
    return GetCredentials()->m_PrincipalId;
}

// 0x0035D5C8 slot 0x00
// 0x0035D5C4 (deleting dtor)
nn::nex::NgsBridgeImp::~NgsBridgeImp()
{
    // nothing to do
}

// 0x00376C18 slot 0x10
bool nn::nex::NgsBridgeImp::RegisterNotificationEventHandler(nn::nex::NotificationEventHandler* pHandler)
{
    // (the tail call is a nop in the original: the function falls into the one of the facade)
    return m_pFacade->RegisterNotificationEventHandler(pHandler);
}

// 0x00376C34 slot 0x14
bool nn::nex::NgsBridgeImp::UnregisterNotificationEventHandler(nn::nex::NotificationEventHandler* pHandler)
{
    // (the tail call is a nop in the original)
    return m_pFacade->UnregisterNotificationEventHandler(pHandler);
}

// 0x0072B460 slot 0x20
nn::nex::qResult nn::nex::NgsBridgeImp::IsConnected()
{
    return m_pFacade->IsConnected();
}

} // namespace nex
} // namespace nn
