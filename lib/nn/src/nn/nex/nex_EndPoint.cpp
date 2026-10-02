#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_EndPointInfoInterface.h"
#include "nn/nex/nex_EndPoint.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x003D4DAC (unverified)
nn::nex::EndPoint::EndPoint()
{
}

// 0x003D4E24 slot 0x00 | mk7dlp:bytes-fuzzy
nn::nex::EndPoint::~EndPoint()
{
}

// 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::vf_0x08()
{
}

// 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::vf_0x0C()
{
}

// 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::vf_0x10()
{
}

// 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::vf_0x14()
{
}

// 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::vf_0x18()
{
}

// 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::IsNotConnected()
{
}

// 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::IsConnecting()
{
}

// 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::IsDisconnecting()
{
}

// 0x0011C12F slot 0x28 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::IsFaulty()
{
}

// 0x0011C12F slot 0x2C | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::IsConnected()
{
}

// 0x0011C12F slot 0x30 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::PeerIsConnected()
{
}

// 0x0011C12F slot 0x34 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::vf_0x34()
{
}

// 0x003D4D28 slot 0x38 | slot vf_0x38 of nn::nex::EndPoint
void nn::nex::EndPoint::DisableKeepAlive()
{
}

// 0x0011C12F slot 0x3C | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::SetKeepAliveTimeout(unsigned int)
{
}

// 0x0011C12F slot 0x40 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::SetMaxSilenceTime(unsigned int)
{
}

// 0x0011C12F slot 0x44 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::GetKeepAliveTimeout()
{
}

// 0x0011C12F slot 0x48 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::GetMaxSilenceTime()
{
}

// 0x0011C12F slot 0x4C | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::SetPeerConnected()
{
}

// 0x0011C12F slot 0x50 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::SetPeerDisconnected()
{
}

// 0x0011C12F slot 0x54 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::GetConnectionState() const
{
}

// 0x0011C12F slot 0x58 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::SetConnectionState(nn::nex::EndPoint::_ConnectionState)
{
}

// 0x003D4D48 slot 0x5C | mk7dlp:callseq-callee
void nn::nex::EndPoint::RegisterEventHandler(nn::nex::EndPointEventHandler*)
{
}

// 0x0011C12F slot 0x60 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::IsAnyPacketProcessed()
{
}

// 0x0011C12F slot 0x64 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::GetSignaledFaultError() const
{
}

// 0x0011C12F slot 0x68 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::vf_0x68()
{
}

// 0x0011C12F slot 0x6C | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::vf_0x6C()
{
}

// 0x0011C12F slot 0x70 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::_Connect(nn::nex::Buffer*, nn::nex::Buffer*, void (*)(nn::nex::EndPoint*,nn::nex::qResult,const nn::nex::UserContext*), const nn::nex::UserContext&, unsigned int)
{
}

// 0x0011C12F slot 0x74 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::_Disconnect(void (*)(nn::nex::EndPoint*,nn::nex::qResult,const nn::nex::UserContext*), const nn::nex::UserContext&, unsigned int)
{
}

// 0x0011C12F slot 0x78 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::_Send(nn::nex::Buffer*, unsigned int, bool, unsigned char, unsigned int)
{
}

// 0x0011C12F slot 0x7C | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::GetSupportedFunctionsFlags() const
{
}

// 0x003D4C68 slot 0x80 | virtual slot, introduced by nn::nex::EndPoint
void nn::nex::EndPoint::vf_0x80()
{
}

// 0x003D4D3C slot 0x84 | virtual slot, introduced by nn::nex::EndPoint
void nn::nex::EndPoint::vf_0x84()
{
}

// 0x003D4D38 slot 0x88 | slot vf_0x88 of nn::nex::EndPoint
void nn::nex::EndPoint::SignalFaultEvent(unsigned int, bool)
{
}

// 0x0011C12F slot 0x8C | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::SetEncryptionKey(const nn::nex::Key&)
{
}

// 0x0011C12F slot 0x90 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EndPoint::GetEncryptionKey()
{
}

// 0x003D4C70 | mk7dlp:bytes-fuzzy [tier A]
void nn::nex::EndPoint::SetPrincipalID(unsigned)
{
}

// 0x003D4CCC | mk7dlp:bytes-fuzzy [tier A]
void nn::nex::EndPoint::SetConnectionID(unsigned)
{
}

// 0x003D4D84 | mk7dlp:bytes [tier A]
void nn::nex::EndPoint::Open()
{
}

// 0x003D4DAC | fefates:bytes [tier B]
nn::nex::EndPoint::EndPoint(nn::nex::ConnectionOrientedStream*, const nn::nex::StationURL*)
{
}

} // namespace nex
} // namespace nn
