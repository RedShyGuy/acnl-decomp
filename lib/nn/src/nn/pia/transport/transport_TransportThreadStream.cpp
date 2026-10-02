#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/transport/transport_TransportThreadStream.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0045BD00 slot 0x00 | virtual slot, introduced by nn::pia::transport::TransportThreadStream
void nn::pia::transport::TransportThreadStream::vf_0x00()
{
}

// 0x0011C12F slot 0x04 | slot vf_0x00 of ChangeRentalBase
void nn::pia::transport::TransportThreadStream::ProcessOne()
{
}

// 0x0045B9D4 | fefates:bytes-fuzzy [tier B]
void nn::pia::transport::TransportThreadStream::ChangeState(nn::pia::transport::TransportThreadStream::ThreadState)
{
}

// 0x0045BA6C | fefates:bytes [tier B]
void nn::pia::transport::TransportThreadStream::FinalizeCore()
{
}

// 0x0045BB14 | fefates:bytes [tier B]
void nn::pia::transport::TransportThreadStream::isDropPacket()
{
}

// 0x0045BB50 | fefates:bytes [tier B]
void nn::pia::transport::TransportThreadStream::InitializeCore(const char*, int, unsigned int, unsigned int, bool, unsigned int)
{
}

// 0x0045BC4C | fefates:bytes [tier B]
void nn::pia::transport::TransportThreadStream::Cleanup()
{
}

// 0x0045BC98 | fefates:bytes [tier B]
void nn::pia::transport::TransportThreadStream::Startup()
{
}

// 0x0045BD04 | fefates:bytes [tier B]
nn::pia::transport::TransportThreadStream::TransportThreadStream()
{
}

// 0x0045BD90 | fefates:bytes [tier B]
nn::pia::transport::TransportThreadStream::~TransportThreadStream()
{
}

// 0x00736548 | fefates:bytes [tier B]
void nn::pia::transport::TransportThreadStream::GetLastResult() const
{
}

} // namespace transport
} // namespace pia
} // namespace nn
