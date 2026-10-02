#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace transport {
class PacketAnalyzer
{
public:
    PacketAnalyzer(); // TODO: default ctor added so derived stubs compile - may not exist
    void ClearPacketAnalysisData(); // 0x0044EF0C | fefates:bytes [tier B]
    void Cleanup(); // 0x0044F100 | fefates:bytes [tier B]
    PacketAnalyzer(const char*, unsigned int); // 0x0044F1B0 | fefates:bytes [tier B]
    ~PacketAnalyzer(); // 0x0044F228 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
