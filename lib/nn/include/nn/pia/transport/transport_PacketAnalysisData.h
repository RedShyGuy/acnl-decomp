#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace transport {
class PacketAnalysisData
{
public:
    void ClearCounters(); // 0x00457B54 | fefates:bytes [tier B]
    void ClearExceptName(); // 0x00457B94 | fefates:bytes [tier B]
    void Print(bool) const; // 0x00735C0C | fefates:bytes [tier B]
    void GetIndex(nn::pia::transport::ProtocolId) const; // 0x00736014 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
