#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/transport/transport_ReliableSlidingWindow.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0045B45C slot 0x00 | fefates:bytes
nn::pia::transport::ReliableSlidingWindow::~ReliableSlidingWindow()
{
}

// 0x00736224 slot 0x08 | virtual slot, introduced by nn::pia::transport::ReliableSlidingWindow
void nn::pia::transport::ReliableSlidingWindow::vf_0x08()
{
}

// 0x0045A540 | fefates:bytes [tier B]
void nn::pia::transport::ReliableSlidingWindow::Initialize(unsigned int, unsigned int)
{
}

// 0x0045A6D8 | fefates:bytes [tier B]
void nn::pia::transport::ReliableSlidingWindow::CanPushData(unsigned int)
{
}

// 0x0045AB34 | fefates:bytes [tier B]
void nn::pia::transport::ReliableSlidingWindow::Cleanup()
{
}

// 0x0045ACF4 | fefates:bytes [tier B]
void nn::pia::transport::ReliableSlidingWindow::Startup(nn::pia::transport::PacketHandler*, unsigned int, nn::pia::StationIndex, nn::pia::StationIndex)
{
}

// 0x0045ADE8 | fefates:bytes-fuzzy [tier B]
void nn::pia::transport::ReliableSlidingWindow::Dispatch(nn::pia::transport::PacketHandler*)
{
}

// 0x0045B14C | fefates:bytes [tier B]
void nn::pia::transport::ReliableSlidingWindow::Finalize()
{
}

// 0x0045B1A8 | fefates:bytes [tier B]
void nn::pia::transport::ReliableSlidingWindow::PushData(const void*, unsigned int)
{
}

// 0x0045B3E0 | fefates:bytes [tier B]
nn::pia::transport::ReliableSlidingWindow::ReliableSlidingWindow()
{
}

// 0x00736204 | fefates:bytes [tier B]
void nn::pia::transport::ReliableSlidingWindow::IsInCommunication() const
{
}

} // namespace transport
} // namespace pia
} // namespace nn
