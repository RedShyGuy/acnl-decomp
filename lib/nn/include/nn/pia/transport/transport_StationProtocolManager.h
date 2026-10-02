#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace transport {
class StationProtocolManager
{
public:
    void GetRttProtocol(); // 0x0045BFC8 | fefates:bytes [tier B]
    void GetStationProtocol(); // 0x0045C00C | fefates:bytes [tier B]
    void GetStationProtocolReliable(); // 0x0045C028 | fefates:bytes [tier B]
    void Finalize(); // 0x0045C044 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
