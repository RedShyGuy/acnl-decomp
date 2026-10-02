#include "nn/nex/nex__Proto_NintendoNotificationEventProtocolServer.h"
#include "nn/nex/nex_NintendoNotificationEventManager.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x003CA6A0 (unverified)
nn::nex::NintendoNotificationEventManager::NintendoNotificationEventManager()
{
}

// 0x003CA7E0 slot 0x00 | fefates:bytes
nn::nex::NintendoNotificationEventManager::~NintendoNotificationEventManager()
{
}

// 0x0072DF00 slot 0x4C | virtual slot, introduced by nn::nex::ServerProtocol
void nn::nex::NintendoNotificationEventManager::vf_0x4C()
{
}

// 0x003CD708 slot 0x50 | fefates:bytes
void nn::nex::NintendoNotificationEventManager::DispatchProtocolMessage(nn::nex::Message*, nn::nex::Message*, bool*, nn::nex::EndPoint*)
{
}

// 0x003CA5F4 slot 0x5C | fefates:bytes
void nn::nex::NintendoNotificationEventManager::ProcessNintendoNotificationEvent(const nn::nex::NintendoNotificationEvent&)
{
}

// 0x003CA6A0 | fefates:bytes [tier B]
nn::nex::NintendoNotificationEventManager::NintendoNotificationEventManager(nn::nex::NintendoNotificationEventManager*)
{
}

} // namespace nex
} // namespace nn
