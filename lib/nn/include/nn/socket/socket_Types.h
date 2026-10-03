#pragma once

#include "decomp.h"

namespace nn {
namespace socket {

// address families (3dbrew "Socket Services")
const s32 AF_INET = 2;
const s32 AF_INET6 = 23;

// one socket of Poll (12 bytes, as the soc:U command passes them; the struct name is from the
// symbols, the members follow the POSIX pollfd and are ours)
struct PollFd
{
    s32 fd;
    s32 events;
    s32 revents;
};
ASSERT_SIZE(PollFd, 0xC);

// an address as the service passes it (name is ours; its size is AddrInfo::addrLength)
struct SockAddr;

// the result of GetAddrInfo, a list allocated from the library's heap (the struct name is from the
// symbols, the members follow the POSIX addrinfo and are ours)
struct AddrInfo
{
    s32 flags;          // 0x00
    s32 family;         // 0x04
    s32 sockType;       // 0x08
    s32 protocol;       // 0x0C
    s32 addrLength;     // 0x10
    char* canonName;    // 0x14
    SockAddr* addr;     // 0x18
    AddrInfo* next;     // 0x1C
};
ASSERT_SIZE(AddrInfo, 0x20);

// the result of GetHostByName (name and members are ours, after the POSIX hostent)
struct HostEnt
{
    char* name;         // 0x0
    char** aliases;     // 0x4
    s16 addrType;       // 0x8
    s16 length;         // 0xA
    char** addrList;    // 0xC
};
ASSERT_SIZE(HostEnt, 0x10);

} // namespace socket
} // namespace nn
