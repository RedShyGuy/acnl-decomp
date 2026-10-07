#include "nn/nex/nex_NgsBridgeInterface.h"
#include "nn/nex/nex_NgsBridgeImp.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::NgsBridgeImp::NgsBridgeImp()
{
}

// 0x0035D5C8
// 0x0035D5C4 (deleting dtor)
nn::nex::NgsBridgeImp::~NgsBridgeImp()
{
}

// 0x0035D5A0 slot 0x08 (name is ours)
nn::nex::Credentials* nn::nex::NgsBridgeImp::GetCredentials()
{
}

// 0x0035D5AC slot 0x0C
u32 nn::nex::NgsBridgeImp::vf_0x0C()
{
}

// 0x00376C18 slot 0x10
bool nn::nex::NgsBridgeImp::RegisterNotificationEventHandler(nn::nex::NotificationEventHandler*)
{
}

// 0x00376C34 slot 0x14
bool nn::nex::NgsBridgeImp::UnregisterNotificationEventHandler(nn::nex::NotificationEventHandler*)
{
}

// 0x0035D518 slot 0x18 | fefates:callseq
bool nn::nex::NgsBridgeImp::ReplaceURL(nn::nex::ProtocolCallContext*, const nn::nex::StationURL&, const nn::nex::StationURL&)
{
}

// 0x0035D55C slot 0x1C | fefates:callseq
bool nn::nex::NgsBridgeImp::SendReport(unsigned int, const void*, unsigned int)
{
}

// 0x0072B460 slot 0x20 | virtual slot, introduced by nn::nex::NgsBridgeImp
nn::nex::qResult nn::nex::NgsBridgeImp::IsConnected()
{
}

} // namespace nex
} // namespace nn
