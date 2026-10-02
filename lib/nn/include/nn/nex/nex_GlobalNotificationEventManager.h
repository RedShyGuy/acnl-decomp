#pragma once

#include "decomp.h"
#include "nn/nex/nex_NotificationEventManager.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex30GlobalNotificationEventManagerE @ 0x008CF1B8
// vtable 0x008FF3A4 (vptr 0x008FF3AC), offset_to_top 0, 25 entries
class GlobalNotificationEventManager : public ::nn::nex::NotificationEventManager
{
public:
    GlobalNotificationEventManager(); // ctor address unknown
    virtual ~GlobalNotificationEventManager(); // 0x003C28F8 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003C28E8 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void DispatchProtocolMessage(nn::nex::Message*, nn::nex::Message*, bool*, nn::nex::EndPoint*); // 0x003C247C slot 0x50 | mk7dlp:callseq
    virtual void vf_0x60(); // 0x003C2590 slot 0x60 | virtual slot, introduced by nn::nex::NotificationEventManager
};
} // namespace nex
} // namespace nn
