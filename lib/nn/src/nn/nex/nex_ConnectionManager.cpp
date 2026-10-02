#include "nn/nex/nex_TransportEventHandler.h"
#include "nn/nex/nex_Job.h"
#include "nn/nex/nex_ConnectionManager.h"

namespace nn {
namespace nex {
// TODO: default ctor added so derived stubs compile - may not exist
nn::nex::ConnectionManager::ConnectionManager()
{
}

// 0x00386A98 slot 0x00 | virtual slot, introduced by nn::nex::ConnectionManager
void nn::nex::ConnectionManager::vf_0x00()
{
}

// 0x00386A2C slot 0x04 | fefates:bytes
nn::nex::ConnectionManager::~ConnectionManager()
{
}

// 0x0039AE44 slot 0x08 | slot vf_0x08 of nn::nex::ConnectionManager
void nn::nex::ConnectionManager::Receive(nn::nex::ConnectionOrientedStream*, nn::nex::Buffer*, const nn::nex::StationURL*)
{
}

// 0x0039AE38 slot 0x0C | virtual slot, introduced by nn::nex::ConnectionManager
void nn::nex::ConnectionManager::vf_0x0C()
{
}

// 0x0039AE3C slot 0x10 | slot vf_0x10 of nn::nex::ConnectionManager
void nn::nex::ConnectionManager::SwitchToRoutingConnection(nn::nex::EndPoint*)
{
}

// 0x0039AE34 slot 0x14 | virtual slot, introduced by nn::nex::ConnectionManager
void nn::nex::ConnectionManager::vf_0x14()
{
}

// 0x00386518 slot 0x18 | slot vf_0x18 of nn::nex::ConnectionManager
void nn::nex::ConnectionManager::ConnectionRequest(nn::nex::ConnectionOrientedStream*, const nn::nex::StationURL*, nn::nex::Buffer*, nn::nex::EndPoint*)
{
}

// 0x003865A4 slot 0x1C | mk7dlp:callseq
void nn::nex::ConnectionManager::FilterConnectionRequest(nn::nex::ConnectionOrientedStream*, const nn::nex::StationURL*, nn::nex::Buffer*, nn::nex::EndPoint*)
{
}

// 0x00386630 | fefates:bytes [tier B]
void nn::nex::ConnectionManager::Connect(nn::nex::CallContext*, const nn::nex::StationURL&, nn::nex::EndPoint**, unsigned int, nn::nex::JobConnectEndPoint**)
{
}

// 0x0038678C | fefates:bytes-fuzzy [tier B]
nn::nex::ConnectionManager::ConnectionManager(nn::nex::EndPointEventHandler*, nn::nex::Job::JobType, bool, bool)
{
}

// 0x0072C1E4 | fefates:bytes [tier B]
void nn::nex::ConnectionManager::IsTerminated() const
{
}

} // namespace nex
} // namespace nn
