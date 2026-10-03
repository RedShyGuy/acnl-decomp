#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/fnd/fnd_ExpHeapTemplate.h"
#include "nn/fnd/fnd_IAllocator.h"
#include "nn/socket/socket_Types.h"

namespace nn {
namespace socket {
namespace detail {
// The functions on the soc:U sessions (socket_IpcWrapper.cpp). Each puts the BSD style return value
// into *result and returns the IPC result. The calls that may block take a session of the pool,
// the others use the control session.

// control session (with the transfer memory of the sockets)
nn::Result InitializeControlSession(uptr address, size_t size); // 0x004879B0 | fefates:bytes [tier B]
void FinalizeControlSession(); // 0x00487960 | fefates:bytes [tier B]
nn::Result Socket(s32* result, s32 domain, s32 type, s32 protocol); // 0x004895CC | tier C (confirmed by the code)
nn::Result Fcntl(s32* result, s32 socket, s32 operation, s32 argument); // 0x004890D0 (name is ours, after User::Fcntl)
nn::Result SetSockOpt(s32* result, s32 socket, s32 level, s32 name, const u8* value, s32 valueSize); // 0x00484E74 | fefates:bytes [tier B]
nn::Result GetSockName(s32* result, s32 socket, u8* address, size_t addressSize); // 0x00485378 (name is ours, after User::GetSockName)
nn::Result GetNetworkOpt(s32* result, s32 level, s32 name, u8* value, s32* valueSize); // 0x00486780 | fefates:bytes [tier B]
nn::Result GetHostId(u32* hostId); // 0x0048A4EC | fefates:bytes [tier B]

// session pool
nn::Result InitializeSessionPool(nn::fnd::IAllocator& allocator, s32 count); // 0x00487528 | fefates:bytes [tier B]
void FinalizeSessionPool(); // 0x00487390 | fefates:bytes [tier B]
size_t GetRequiredMemorySizeForSessionPool(s32 count); // 0x00487A78 | mk7dlp:callgraph [tier A]
nn::Result Bind(s32* result, s32 socket, const u8* address, size_t addressSize); // 0x00487A84 | tier C (confirmed by the code)
nn::Result Connect(s32* result, s32 socket, const u8* address, size_t addressSize); // 0x00489628 (name is ours, after User::Connect)
nn::Result RecvFrom(s32* result, s32 socket, u8* buffer, s32 size, s32 flags, u8* address, size_t addressSize); // 0x00489B18 | tier C (confirmed by the code)
nn::Result RecvFromSmall(s32* result, s32 socket, u8* buffer, s32 size, s32 flags, u8* address, size_t addressSize); // 0x004867E4 | fefates:bytes [tier B]
nn::Result SendTo(s32* result, s32 socket, const u8* buffer, s32 size, s32 flags, const u8* address, size_t addressSize); // 0x0048912C | tier C (confirmed by the code)
nn::Result SendToSmall(s32* result, s32 socket, const u8* buffer, s32 size, s32 flags, const u8* address, size_t addressSize); // 0x004853D4 | tier C (confirmed by the code)
nn::Result SendToSmallMulti(s32* result, s32 socket, const u8* buffer, s32 size, s32 flags, const u8* addresses, size_t addressSize, size_t addressesSize); // 0x00486DCC | fefates:bytes [tier B]
nn::Result Poll(s32* result, nn::socket::PollFd* fds, u32 count, s32 timeout); // 0x00487F48 | fefates:bytes [tier B]
nn::Result Close(s32* result, s32 socket); // 0x00488C24 | fefates:bytes [tier B]
nn::Result Shutdown(s32* result, s32 socket, s32 how); // 0x00489FB8 | fefates:bytes [tier B]
nn::Result GetHostByName(s32* result, const char* name, size_t nameSize, u8* hostEntry, size_t hostEntrySize); // 0x004862F8 | fefates:bytes [tier B]
nn::Result GetAddrInfo(s32* result, const char* node, size_t nodeSize, const char* service, size_t serviceSize, const u8* hints, s32 hintsSize, s32* count, u8* addrInfo, size_t addrInfoSize); // 0x00484EE0 | fefates:bytes [tier B]

// the BSD style error number of an IPC result
s32 ConvertErrorResult(nn::Result result); // 0x00487284 | fefates:bytes-fuzzy [tier B]

// the heap of the library (socket_Api.cpp; names are ours)
typedef nn::fnd::ExpHeapTemplate<nn::os::LockPolicy::Object<nn::os::CriticalSection> > Heap;
extern Heap s_Heap;                         // 0x00AE1FE0
extern Heap::Allocator s_HeapAllocator;     // 0x00AE2044

void* Allocate(size_t size, s32 alignment); // 0x00489AB8 | fefates:bytes [tier B]
void Free(void* p); // 0x00487F14 | fefates:bytes [tier B]
nn::Result InitializeDnsClient(); // 0x0048746C | fefates:bytes [tier B]
} // namespace detail
} // namespace socket
} // namespace nn
