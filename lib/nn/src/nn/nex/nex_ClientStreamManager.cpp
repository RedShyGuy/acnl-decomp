#include "nn/nex/nex_StreamManager.h"
#include "nn/nex/nex_ClientStreamManager.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::ClientStreamManager::ClientStreamManager()
{
}

// 0x00392B04 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
nn::nex::ClientStreamManager::~ClientStreamManager()
{
}

// 0x003D912C slot 0x08 | fefates:bytes
void nn::nex::ClientStreamManager::Receive(nn::nex::EndPoint*, nn::nex::Buffer*, unsigned char)
{
}

// 0x003929DC slot 0x0C | slot vf_0x0C of nn::nex::StreamManager
void nn::nex::ClientStreamManager::FaultDetection(nn::nex::EndPoint*, unsigned int)
{
}

// 0x00392A78 slot 0x10 | slot vf_0x10 of nn::nex::StreamManager
void nn::nex::ClientStreamManager::PeerDisconnected(nn::nex::EndPoint*)
{
}

// 0x003929EC slot 0x14 | fefates:bytes
void nn::nex::ClientStreamManager::SetCredentials(nn::nex::Credentials*)
{
}

} // namespace nex
} // namespace nn
