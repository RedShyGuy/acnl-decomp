#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/socket/socket_Types.h"

namespace nn {
namespace socket {
namespace detail {
// The commands of the soc:U service on one session (3dbrew "Socket Services"). Every command puts
// the BSD style return value into *result (negative: an error number) and returns the IPC result.
// The member and the parameter names are ours.
class User
{
public:
    explicit User(nn::Handle session) : mSession(session) {}

    nn::Result AttachProcess(nn::Handle sharedMemory, size_t size); // 0x004885FC | fefates:callgraph [tier A]
    nn::Result DetachProcess(); // 0x00488640 | tier C (confirmed by the code)
    nn::Result Socket(s32* result, s32 domain, s32 type, s32 protocol); // 0x00488A7C | fefates:bytes [tier A]
    nn::Result Bind(s32* result, s32 socket, const u8* address, size_t addressSize); // 0x0048887C | fefates:bytes [tier A]
    nn::Result Connect(s32* result, s32 socket, const u8* address, size_t addressSize); // 0x00488AC8 | tier C (confirmed by the code)
    nn::Result RecvFrom(s32* result, s32 socket, u8* buffer, s32 size, s32 flags, u8* address, size_t addressSize); // 0x00488B28 | fefates:bytes [tier A]
    nn::Result RecvFromSmall(s32* result, s32 socket, u8* buffer, s32* size, s32 flags, u8* address, size_t addressSize); // 0x00488760 | fefates:bytes [tier B]
    nn::Result SendTo(s32* result, s32 socket, const u8* buffer, s32 size, s32 flags, const u8* address, size_t addressSize); // 0x004889F8 | fefates:bytes [tier A]
    nn::Result SendToSmall(s32* result, s32 socket, const u8* buffer, s32 size, s32 flags, const u8* address, size_t addressSize); // 0x00488578 | fefates:bytes [tier A]
    nn::Result SendToSmallMulti(s32* result, s32 socket, const u8* buffer, s32 size, s32 flags, const u8* addresses, size_t addressSize, size_t addressesSize); // 0x00488804 | fefates:bytes [tier A]
    nn::Result Close(s32* result, s32 socket); // 0x00488968 | fefates:bytes [tier B]
    nn::Result Shutdown(s32* result, s32 socket, s32 how); // 0x00488BAC | fefates:bytes [tier B]
    nn::Result Fcntl(s32* result, s32 socket, s32 operation, s32 argument); // 0x004889AC | fefates:bytes [tier A]
    nn::Result SetSockOpt(s32* result, s32 socket, s32 level, s32 name, const u8* value, s32 valueSize); // 0x004883DC | fefates:bytes [tier A]
    nn::Result GetSockName(s32* result, s32 socket, u8* address, size_t addressSize); // 0x0048850C | fefates:bytes [tier A]
    nn::Result Poll(s32* result, const nn::socket::PollFd* fds, nn::socket::PollFd* resultFds, u32 count, s32 timeout); // 0x004888DC | fefates:bytes [tier B]
    nn::Result GetHostId(u32* hostId); // 0x00488BF0 | fefates:bytes [tier B]
    nn::Result GetHostByName(s32* result, const char* name, size_t nameSize, u8* hostEntry, size_t hostEntrySize); // 0x00488668 | fefates:bytes [tier A]
    nn::Result GetAddrInfo(s32* result, const char* node, size_t nodeSize, const char* service, size_t serviceSize, const u8* hints, s32 hintsSize, s32* count, u8* addrInfo, size_t addrInfoSize); // 0x00488444 | fefates:bytes [tier A]
    nn::Result GetNetworkOpt(s32* result, s32 level, s32 name, u8* value, s32* valueSize); // 0x004886E4 | fefates:bytes [tier B]

private:
    nn::Handle mSession;    // 0x0
};
ASSERT_SIZE(User, 0x4);
} // namespace detail
} // namespace socket
} // namespace nn
