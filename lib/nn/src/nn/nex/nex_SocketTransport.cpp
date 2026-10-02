#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_SocketTransport.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::SocketTransport::SocketTransport()
{
}

// 0x0037D534 slot 0x00 | slot vf_0x00 of nn::nex::RootTransport
nn::nex::SocketTransport::~SocketTransport()
{
}

// 0x0037B374 slot 0x08 | virtual slot, introduced by nn::nex::RootTransport
void nn::nex::SocketTransport::vf_0x08()
{
}

// 0x0037B85C slot 0x0C | fefates:bytes
void nn::nex::SocketTransport::StartListen(unsigned short, unsigned short*, bool, unsigned int, bool)
{
}

// 0x0037BB6C slot 0x10 | virtual slot, introduced by nn::nex::RootTransport
void nn::nex::SocketTransport::vf_0x10()
{
}

// 0x0037B3A0 slot 0x14 | virtual slot, introduced by nn::nex::RootTransport
void nn::nex::SocketTransport::vf_0x14()
{
}

// 0x0037B7D8 slot 0x18 | virtual slot, introduced by nn::nex::RootTransport
void nn::nex::SocketTransport::vf_0x18()
{
}

// 0x0037C498 slot 0x1C | virtual slot, introduced by nn::nex::RootTransport
void nn::nex::SocketTransport::vf_0x1C()
{
}

// 0x0037CE24 slot 0x20 | virtual slot, introduced by nn::nex::RootTransport
void nn::nex::SocketTransport::vf_0x20()
{
}

// 0x0037CF98 slot 0x24 | mk7dlp:bytes
void nn::nex::SocketTransport::Receive(unsigned short, nn::nex::Buffer*, const nn::nex::InetAddress*)
{
}

// 0x0037CFDC slot 0x28 | virtual slot, introduced by nn::nex::RootTransport
void nn::nex::SocketTransport::vf_0x28()
{
}

// 0x0037CFD4 slot 0x2C | virtual slot, introduced by nn::nex::RootTransport
void nn::nex::SocketTransport::vf_0x2C()
{
}

// 0x0072B6D0 slot 0x30 | virtual slot, introduced by nn::nex::RootTransport
void nn::nex::SocketTransport::vf_0x30()
{
}

// 0x0072B6D8 slot 0x34 | virtual slot, introduced by nn::nex::RootTransport
void nn::nex::SocketTransport::vf_0x34()
{
}

// 0x0037CC38 slot 0x38 | virtual slot, introduced by nn::nex::RootTransport
void nn::nex::SocketTransport::vf_0x38()
{
}

// 0x0037CA5C slot 0x3C | virtual slot, introduced by nn::nex::RootTransport
void nn::nex::SocketTransport::vf_0x3C()
{
}

// 0x0037C718 slot 0x40 | virtual slot, introduced by nn::nex::RootTransport
void nn::nex::SocketTransport::vf_0x40()
{
}

// 0x0037C724 | mk7dlp:callgraph [tier A]
void nn::nex::SocketTransport::FillPacketQueueFromBuffer(nn::nex::QueuingSocket*, nn::nex::Buffer*, const nn::nex::InetAddress*)
{
}

// 0x0037CFE4 | mk7dlp:callgraph [tier A]
void nn::nex::SocketTransport::GetSocket(unsigned short)
{
}

} // namespace nex
} // namespace nn
