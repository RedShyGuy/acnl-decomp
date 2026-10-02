#include "nn/nex/nex_NotificationEventManager.h"
#include "nn/nex/nex_GlobalNotificationEventManager.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::GlobalNotificationEventManager::GlobalNotificationEventManager()
{
}

// 0x003C28F8 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
nn::nex::GlobalNotificationEventManager::~GlobalNotificationEventManager()
{
}

// 0x003C247C slot 0x50 | mk7dlp:callseq
void nn::nex::GlobalNotificationEventManager::DispatchProtocolMessage(nn::nex::Message*, nn::nex::Message*, bool*, nn::nex::EndPoint*)
{
}

// 0x003C2590 slot 0x60 | virtual slot, introduced by nn::nex::NotificationEventManager
void nn::nex::GlobalNotificationEventManager::vf_0x60()
{
}

} // namespace nex
} // namespace nn
