#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex18NgsBridgeInterfaceE @ 0x008CE73C
//
// The bridge from pia to the nex connection of the game (pia::inet::NexFacade::m_pNgsBridge). The
// slot names are the ones of NgsBridgeImp; the return types are ours.
class NgsBridgeInterface : public ::nn::nex::RootObject
{
public:
    NgsBridgeInterface(); // ctor address unknown
    virtual ~NgsBridgeInterface() {} // slot 0x00
    // slot 0x04 (deleting dtor)
    // the credentials of the game (name is ours; NexFacade binds the relay client with them)
    virtual nn::nex::Credentials* GetCredentials() = 0; // slot 0x08
    virtual u32 vf_0x0C() = 0; // slot 0x0C
    virtual bool RegisterNotificationEventHandler(nn::nex::NotificationEventHandler* pHandler) = 0; // slot 0x10
    virtual bool UnregisterNotificationEventHandler(nn::nex::NotificationEventHandler* pHandler) = 0; // slot 0x14
    virtual bool ReplaceURL(nn::nex::ProtocolCallContext* pContext, const nn::nex::StationURL& oldUrl, const nn::nex::StationURL& newUrl) = 0; // slot 0x18
    virtual bool SendReport(unsigned int type, const void* pData, unsigned int size) = 0; // slot 0x1C
    // whether the game is connected (NgsBridgeImp falls into BackEndServices::IsConnected)
    virtual nn::nex::qResult IsConnected() = 0; // slot 0x20
};
} // namespace nex
} // namespace nn
