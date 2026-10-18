#pragma once

#include "decomp.h"
#include "nn/nex/nex_NotificationEventManager.h"
#include "nn/nex/nex_qMap.h"

namespace nn {
namespace nex {
// The notification protocol of the RVClientCore: an event goes to the registered managers (of the
// credentials of the receiving stream). The names of the members and of the functions without a
// reference name are ours.
// RTTI N2nn3nex30GlobalNotificationEventManagerE @ 0x008CF1B8
// vtable 0x008FF3A4 (vptr 0x008FF3AC), offset_to_top 0, 25 entries
class GlobalNotificationEventManager : public ::nn::nex::NotificationEventManager
{
public:
    virtual ~GlobalNotificationEventManager(); // 0x003C28F8 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003C28E8 slot 0x04 (deleting dtor)
    virtual void DispatchProtocolMessage(nn::nex::Message* pRequest, nn::nex::Message* pResponse, bool* pIsHandled, nn::nex::EndPoint* pEndPoint); // 0x003C247C slot 0x50 | mk7dlp:callseq
    virtual void ProcessNotificationEvent(unsigned int principalId, unsigned int connectionId, const nn::nex::NotificationEvent& event); // 0x003C2590 slot 0x60

    bool RegisterGlobalNotificationEventManager(nn::nex::NotificationEventManager* pManager); // 0x003C262C
    bool UnregisterGlobalNotificationEventManager(nn::nex::NotificationEventManager* pManager); // 0x003C281C (name is ours)

    std::set<NotificationEventManager*> m_Managers; // 0x84
};
ASSERT_SIZE(GlobalNotificationEventManager, 0xA0);
} // namespace nex
} // namespace nn
