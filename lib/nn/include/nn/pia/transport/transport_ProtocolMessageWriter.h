#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace transport {
class ProtocolMessageWriter
{
public:
    void SetPayload(const void*, unsigned int, unsigned int); // 0x0045A2E8 | fefates:bytes [tier B]
    void AddMessageBuffer(nn::pia::common::Packet*, void*, unsigned int, bool, bool); // 0x0045A2FC | fefates:bytes [tier B]
    void Reset(const nn::pia::transport::ProtocolId&, unsigned int, bool, bool); // 0x0045A330 | fefates:bytes [tier B]
    void Commit(); // 0x0045A35C | fefates:bytes [tier B]
    ProtocolMessageWriter(); // 0x0045A51C | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
