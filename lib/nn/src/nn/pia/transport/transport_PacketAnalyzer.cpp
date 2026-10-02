#include "nn/pia/transport/transport_PacketAnalyzer.h"

namespace nn {
namespace pia {
namespace transport {
// TODO: default ctor added so derived stubs compile - may not exist
nn::pia::transport::PacketAnalyzer::PacketAnalyzer()
{
}

// 0x0044EF0C | fefates:bytes [tier B]
void nn::pia::transport::PacketAnalyzer::ClearPacketAnalysisData()
{
}

// 0x0044F100 | fefates:bytes [tier B]
void nn::pia::transport::PacketAnalyzer::Cleanup()
{
}

// 0x0044F1B0 | fefates:bytes [tier B]
nn::pia::transport::PacketAnalyzer::PacketAnalyzer(const char*, unsigned int)
{
}

// 0x0044F228 | fefates:bytes [tier B]
nn::pia::transport::PacketAnalyzer::~PacketAnalyzer()
{
}

} // namespace transport
} // namespace pia
} // namespace nn
