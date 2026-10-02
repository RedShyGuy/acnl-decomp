#include "nn/nex/nex_EndPoint.h"
#include "nn/nex/nex_SecureEndPoint.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::SecureEndPoint::SecureEndPoint()
{
}

// 0x00374220 slot 0x00 | slot vf_0x00 of nn::nex::EndPoint
nn::nex::SecureEndPoint::~SecureEndPoint()
{
}

// 0x00373D8C slot 0x08 | virtual slot, introduced by nn::nex::EndPoint
void nn::nex::SecureEndPoint::vf_0x08()
{
}

// 0x0072B2B8 slot 0x0C | virtual slot, introduced by nn::nex::EndPoint
void nn::nex::SecureEndPoint::vf_0x0C()
{
}

// 0x0072B320 slot 0x10 | virtual slot, introduced by nn::nex::EndPoint
void nn::nex::SecureEndPoint::vf_0x10()
{
}

// 0x0072B328 slot 0x14 | virtual slot, introduced by nn::nex::EndPoint
void nn::nex::SecureEndPoint::vf_0x14()
{
}

// 0x0072B2C0 slot 0x18 | virtual slot, introduced by nn::nex::EndPoint
void nn::nex::SecureEndPoint::vf_0x18()
{
}

// 0x00373CA8 slot 0x1C | slot vf_0x1C of nn::nex::EndPoint
void nn::nex::SecureEndPoint::IsNotConnected()
{
}

// 0x00373C8C slot 0x20 | fefates:bytes
void nn::nex::SecureEndPoint::IsConnecting()
{
}

// 0x00373CC8 slot 0x24 | slot vf_0x24 of nn::nex::EndPoint
void nn::nex::SecureEndPoint::IsDisconnecting()
{
}

// 0x00374010 slot 0x28 | fefates:bytes
void nn::nex::SecureEndPoint::IsFaulty()
{
}

// 0x00373B64 slot 0x2C | fefates:bytes
void nn::nex::SecureEndPoint::IsConnected()
{
}

// 0x00373CE4 slot 0x30 | fefates:bytes
void nn::nex::SecureEndPoint::PeerIsConnected()
{
}

// 0x00373E70 slot 0x34 | virtual slot, introduced by nn::nex::EndPoint
void nn::nex::SecureEndPoint::vf_0x34()
{
}

// 0x00373EC8 slot 0x3C | fefates:bytes
void nn::nex::SecureEndPoint::SetKeepAliveTimeout(unsigned int)
{
}

// 0x00373D90 slot 0x40 | fefates:bytes
void nn::nex::SecureEndPoint::SetMaxSilenceTime(unsigned int)
{
}

// 0x00373EAC slot 0x44 | slot vf_0x44 of nn::nex::EndPoint
void nn::nex::SecureEndPoint::GetKeepAliveTimeout()
{
}

// 0x00373D70 slot 0x48 | slot vf_0x48 of nn::nex::EndPoint
void nn::nex::SecureEndPoint::GetMaxSilenceTime()
{
}

// 0x00373D38 slot 0x4C | slot vf_0x4C of nn::nex::EndPoint
void nn::nex::SecureEndPoint::SetPeerConnected()
{
}

// 0x00373EE4 slot 0x50 | slot vf_0x50 of nn::nex::EndPoint
void nn::nex::SecureEndPoint::SetPeerDisconnected()
{
}

// 0x0072B2C8 slot 0x54 | slot vf_0x54 of nn::nex::EndPoint
void nn::nex::SecureEndPoint::GetConnectionState() const
{
}

// 0x00373E90 slot 0x58 | fefates:bytes
void nn::nex::SecureEndPoint::SetConnectionState(nn::nex::EndPoint::_ConnectionState)
{
}

// 0x00373F1C slot 0x5C | mk7dlp:callseq
void nn::nex::SecureEndPoint::RegisterEventHandler(nn::nex::EndPointEventHandler*)
{
}

// 0x00373F00 slot 0x60 | fefates:bytes
void nn::nex::SecureEndPoint::IsAnyPacketProcessed()
{
}

// 0x0072B2E4 slot 0x64 | fefates:bytes
void nn::nex::SecureEndPoint::GetSignaledFaultError() const
{
}

// 0x0072B32C slot 0x68 | virtual slot, introduced by nn::nex::EndPoint
void nn::nex::SecureEndPoint::vf_0x68()
{
}

// 0x0072B29C slot 0x6C | virtual slot, introduced by nn::nex::EndPoint
void nn::nex::SecureEndPoint::vf_0x6C()
{
}

// 0x0037402C slot 0x70 | fefates:bytes
void nn::nex::SecureEndPoint::_Connect(nn::nex::Buffer*, nn::nex::Buffer*, void (*)(nn::nex::EndPoint*,nn::nex::qResult,const nn::nex::UserContext*), const nn::nex::UserContext&, unsigned int)
{
}

// 0x00373B80 slot 0x74 | fefates:bytes
void nn::nex::SecureEndPoint::_Disconnect(void (*)(nn::nex::EndPoint*,nn::nex::qResult,const nn::nex::UserContext*), const nn::nex::UserContext&, unsigned int)
{
}

// 0x00373F90 slot 0x78 | slot vf_0x78 of nn::nex::EndPoint
void nn::nex::SecureEndPoint::_Send(nn::nex::Buffer*, unsigned int, bool, unsigned char, unsigned int)
{
}

// 0x0072B300 slot 0x7C | fefates:bytes
void nn::nex::SecureEndPoint::GetSupportedFunctionsFlags() const
{
}

// 0x00373D54 slot 0x88 | fefates:bytes
void nn::nex::SecureEndPoint::SignalFaultEvent(unsigned int, bool)
{
}

// 0x00373D1C slot 0x8C | fefates:bytes
void nn::nex::SecureEndPoint::SetEncryptionKey(const nn::nex::Key&)
{
}

// 0x00373D00 slot 0x90 | slot vf_0x90 of nn::nex::EndPoint
void nn::nex::SecureEndPoint::GetEncryptionKey()
{
}

} // namespace nex
} // namespace nn
