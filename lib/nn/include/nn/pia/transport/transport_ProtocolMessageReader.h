#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace transport {
class ProtocolMessageReader
{
public:
    void Clear(); // 0x0045A1A8 | fefates:bytes [tier B]
    void Attach(const nn::pia::common::Packet&, unsigned int); // 0x0045A1CC | fefates:bytes [tier B]
    ProtocolMessageReader(); // 0x0045A298 | fefates:bytes [tier B]
    ~ProtocolMessageReader(); // 0x0045A2B8 | fefates:bytes [tier B]
    void GetDestination() const; // 0x007361B0 | fefates:bytes [tier B]
    void GetReservedData() const; // 0x007361C4 | fefates:bytes [tier B]
    void GetProtocolIdPort() const; // 0x007361D8 | fefates:bytes [tier B]
    void GetSourceStationKey() const; // 0x007361F0 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
