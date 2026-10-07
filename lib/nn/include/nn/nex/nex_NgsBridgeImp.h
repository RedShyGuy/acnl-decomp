#pragma once

#include "decomp.h"
#include "nn/nex/nex_NgsBridgeInterface.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex12NgsBridgeImpE @ 0x008CE108
// vtable 0x008FC56C (vptr 0x008FC574), offset_to_top 0, 9 entries
class NgsBridgeImp : public ::nn::nex::NgsBridgeInterface
{
public:
    NgsBridgeImp(); // ctor address unknown
    virtual ~NgsBridgeImp(); // 0x0035D5C8 slot 0x00
    // 0x0035D5C4 slot 0x04 (deleting dtor)
    virtual nn::nex::Credentials* GetCredentials(); // 0x0035D5A0 slot 0x08 (name is ours)
    virtual u32 vf_0x0C(); // 0x0035D5AC slot 0x0C
    virtual bool RegisterNotificationEventHandler(nn::nex::NotificationEventHandler* pHandler); // 0x00376C18 slot 0x10
    virtual bool UnregisterNotificationEventHandler(nn::nex::NotificationEventHandler* pHandler); // 0x00376C34 slot 0x14
    virtual bool ReplaceURL(nn::nex::ProtocolCallContext* pContext, const nn::nex::StationURL& oldUrl, const nn::nex::StationURL& newUrl); // 0x0035D518 slot 0x18 | fefates:callseq
    virtual bool SendReport(unsigned int type, const void* pData, unsigned int size); // 0x0035D55C slot 0x1C | fefates:callseq
    virtual nn::nex::qResult IsConnected(); // 0x0072B460 slot 0x20
};
} // namespace nex
} // namespace nn
