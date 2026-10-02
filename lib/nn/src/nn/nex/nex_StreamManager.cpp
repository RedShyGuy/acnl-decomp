#include "nn/nex/nex_EndPointEventHandler.h"
#include "nn/nex/nex_StreamManager.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x0036E6C4 (unverified)
nn::nex::StreamManager::StreamManager()
{
}

// 0x0036E988 slot 0x00 | fefates:bytes-fuzzy
nn::nex::StreamManager::~StreamManager()
{
}

// 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
void nn::nex::StreamManager::Receive(nn::nex::EndPoint*, nn::nex::Buffer*, unsigned char)
{
}

// 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
void nn::nex::StreamManager::FaultDetection(nn::nex::EndPoint*, unsigned int)
{
}

// 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
void nn::nex::StreamManager::PeerDisconnected(nn::nex::EndPoint*)
{
}

// 0x0036E670 slot 0x14 | slot vf_0x14 of nn::nex::StreamManager
void nn::nex::StreamManager::SetCredentials(nn::nex::Credentials*)
{
}

// 0x0036E478 | fefates:bytes-fuzzy [tier B]
void nn::nex::StreamManager::Initialize(unsigned short, unsigned char, unsigned int)
{
}

// 0x0036E68C | fefates:bytes [tier B]
void nn::nex::StreamManager::AssociateSecureStream(nn::nex::ConnectionOrientedStream*, bool (*)(const nn::nex::UserContext&,nn::nex::Buffer*,nn::nex::Buffer*,nn::nex::EndPoint*))
{
}

} // namespace nex
} // namespace nn
