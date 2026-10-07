#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace pia {
namespace inet {
// the argument of Initialize (the type name is from the symbols, the member is ours)
struct Setting
{
    u32 m_MtuSize; // 0x0, at most MTU_SIZE_MAX
};

// the largest MTU Initialize takes, and the IP and UDP headers it subtracts for the payload
const u32 MTU_SIZE_MAX = 1364;
const u32 IP_UDP_HEADER_SIZE = 28;

nn::Result Initialize(const nn::pia::inet::Setting& setting); // 0x003E25E4 | fefates:bytes [tier B]
void Finalize(); // 0x00412C64 | fefates:bytes [tier B]
nn::Result BeginSetup(); // 0x003E2598 | fefates:bytes [tier B]
nn::Result EndSetup(); // 0x00412C08 | fefates:bytes [tier B]
// (names are ours)
bool IsInSetupMode(); // 0x003E6B84
bool IsInitialized(); // 0x003E6B94

// the result of a negative return value of the socket service when sending (the others:
// convertSocketErrorToResult in inet_Socket.cpp)
nn::Result convertSendSocketErrorToResult(int error); // 0x00411504 | fefates:bytes [tier B]
} // namespace inet
} // namespace pia
} // namespace nn
