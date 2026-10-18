#pragma once

#include "decomp.h"
#include "nn/nex/nex_NgsBridgeInterface.h"

namespace nn {
namespace nex {
class NgsFacade;

// The bridge of NgsFacade. The member name is ours.
// RTTI N2nn3nex12NgsBridgeImpE @ 0x008CE108
// vtable 0x008FC56C (vptr 0x008FC574), offset_to_top 0, 9 entries
class NgsBridgeImp : public ::nn::nex::NgsBridgeInterface
{
public:
    // (inline in the constructor of NgsFacade)
    explicit NgsBridgeImp(NgsFacade* pFacade) : m_pFacade(pFacade) {}
    virtual ~NgsBridgeImp(); // 0x0035D5C8 slot 0x00
    // 0x0035D5C4 (deleting dtor)
    virtual nn::nex::Credentials* GetCredentials(); // 0x0035D5A0 slot 0x08 (name is ours)
    virtual u32 GetPrincipalId(); // 0x0035D5AC slot 0x0C (name is ours)
    virtual bool RegisterNotificationEventHandler(nn::nex::NotificationEventHandler* pHandler); // 0x00376C18 slot 0x10
    virtual bool UnregisterNotificationEventHandler(nn::nex::NotificationEventHandler* pHandler); // 0x00376C34 slot 0x14
    virtual bool ReplaceURL(nn::nex::ProtocolCallContext* pContext, const nn::nex::StationURL& oldUrl, const nn::nex::StationURL& newUrl); // 0x0035D518 slot 0x18 | fefates:callseq
    virtual void SendReport(unsigned int type, const void* pData, unsigned int size); // 0x0035D55C slot 0x1C | fefates:callseq
    // (only a nop in the original that falls into BackEndServices::IsConnected)
    virtual nn::nex::qResult IsConnected(); // 0x0072B460 slot 0x20

    NgsFacade* m_pFacade; // 0x04
};
ASSERT_SIZE(NgsBridgeImp, 0x8);
} // namespace nex
} // namespace nn
