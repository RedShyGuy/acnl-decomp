#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/socket/socket_Types.h"

namespace nn {
namespace socket {
// memory: sharedMemorySize bytes for the service (transfer memory), the rest is the library's heap
nn::Result Initialize(uptr memory, size_t memorySize, size_t sharedMemorySize, s32 sessionCount); // 0x00484A18 | mk7dlp:callseq [tier A]
nn::Result Finalize(); // 0x0048A550 | fefates:bytes [tier B]
size_t GetRequiredMemorySize(size_t sharedMemorySize, s32 sessionCount); // 0x00484CA0 | mk7dlp:bytes [tier A]

// name resolution (DnsUserClient)
s32 GetAddrInfo(const char* node, const char* service, const nn::socket::AddrInfo* hints, nn::socket::AddrInfo** result); // 0x00484BA8 | fefates:bytes [tier B]
void FreeAddrInfo(nn::socket::AddrInfo* info); // 0x00484BE4 | tier C (confirmed by the code)
nn::socket::HostEnt* GetHostByName(const char* name); // 0x00484C04 (name is ours, after DnsUserClient::GetHostByName)

// the console's address and net mask; 0 or a negative error number
s32 GetPrimaryAddress(u8* address, u8* netmask); // 0x00484C24 | fefates:bytes [tier B]

// "a.b.c.d" (also a, a.b, a.b.c; decimal, octal or hexadecimal parts) to 4 bytes; the end of the
// parsed text, 0 if it is no address
const char* IPAtoN(const char* text, u8* address); // 0x00484CC0 | fefates:bytes [tier B]
const char* InetNtoP(s32 family, const void* source, char* destination, size_t size); // 0x0048A628 | fefates:bytes [tier B]
s32 InetPtoN(s32 family, const char* source, void* destination); // 0x0048A6A0 | fefates:bytes [tier B]
} // namespace socket
} // namespace nn
