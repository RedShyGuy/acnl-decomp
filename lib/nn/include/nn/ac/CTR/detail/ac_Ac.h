#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/ac/CTR/CTR_Api.h"

namespace nn {
namespace ac {
namespace CTR {
namespace detail {
// The commands of the service ac:u (3dbrew "AC Services"); all static, on s_Session. The function
// names are from the symbols (or after 3dbrew).
class Ac
{
public:
    static nn::Result CreateDefaultConfig(nnacConfig* pConfig); // 0x00345C30 | fefates:bytes [tier B]
    static nn::Result ConnectAsync(const nnacConfig& config, nn::Handle event); // 0x00345A34 | fefates:bytes [tier B]
    static nn::Result GetConnectResult(); // 0x00345B28 | fefates:callgraph [tier C]
    static nn::Result CloseAsync(nn::Handle event); // 0x003459A0 | fefates:callgraph [tier C]
    static nn::Result GetCloseResult(); // 0x00345AEC | fefates:callgraph [tier C]
    static nn::Result GetLastErrorCode(unsigned int* pCode); // 0x00345BB4 | fefates:bytes [tier B]
    static nn::Result GetLastDetailErrorCode(unsigned int* pCode); // 0x00345D50 (name after 3dbrew)
    static nn::Result GetConnectingLocation(unsigned char* pLocation); // 0x00345C84 | fefates:bytes [tier B]
    static nn::Result AddDenyApType(const nnacConfig& config, nnacConfig* pConfig, nn::ac::CTR::ApType type); // 0x00345A8C | fefates:bytes [tier B]
    static nn::Result GetInfraPriority(const nnacConfig& config, nn::ac::CTR::InfraPriority* pPriority); // 0x00345B64 | fefates:bytes [tier B]
    static nn::Result SetRequestEulaVersion(const nnacConfig& config, nnacConfig* pConfig, unsigned char major, unsigned char minor); // 0x00345CE0 | fefates:bytes [tier B]
    static nn::Result RegisterDisconnectEvent(nn::Handle event); // 0x00345D8C | fefates:callgraph [tier C]
    static nn::Result IsConnected(u32 unknown, bool* pIsConnected); // 0x003459E8 | fefates:callgraph [tier C]
    static nn::Result SetClientVersion(u32 version); // 0x00345BF0 | fefates:callgraph [tier C]

    // right in front of GetConnectingLocation, only a nop left (a removed tail call; name is ours)
    static nn::Result GetConnectingLocationEntry(unsigned char* pLocation); // 0x00345C80 (name is ours)
};

// the session of ac:u (name is ours)
extern nn::Handle s_Session; // 0x0097E7DC
} // namespace detail
} // namespace CTR
} // namespace ac
} // namespace nn
