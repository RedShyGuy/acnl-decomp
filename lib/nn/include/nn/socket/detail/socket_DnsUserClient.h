#pragma once

#include "decomp.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/socket/socket_Types.h"

namespace nn {
namespace socket {
namespace detail {
// RTTI N2nn6socket6detail13DnsUserClientE @ 0x008D0500
// vtable 0x00902328 (vptr 0x00902330), offset_to_top 0, 2 entries
//
// Name resolution over the pool sessions: turns the replies of soc:U (GetHostByName,
// GetAddrInfo) into hostent / addrinfo structures. Allocated from the library's heap by
// InitializeDnsClient. The members and the reply layouts (3dbrew "Socket Services") are ours.
class DnsUserClient
{
public:
    // inline (in InitializeDnsClient)
    DnsUserClient() : mCurrentHostEntry(0), mLock(nn::os::CriticalSection::InitializeTag()) {}
    virtual ~DnsUserClient(); // 0x004862E8 slot 0x00, 0x004862D8 slot 0x04 (deleting)

    // 0 or a negative error number; the list must be freed with FreeAddrInfo
    s32 GetAddrInfo(const char* node, const char* service, const nn::socket::AddrInfo* hints, nn::socket::AddrInfo** result); // 0x00485AA4 | fefates:bytes [tier B]
    void FreeAddrInfo(nn::socket::AddrInfo* info); // 0x00485EE4 | fefates:bytes [tier B]
    // valid until the next call; 0 on an error
    nn::socket::HostEnt* GetHostByName(const char* name); // 0x00485F3C | fefates:bytes [tier B]

private:
    static const s32 HOST_ENTRY_COUNT = 1;
    static const s32 MAX_ALIASES = 24;
    static const s32 MAX_ADDRESSES = 24;
    static const s32 MAX_ADDR_INFOS = 24;
    static const size_t NAME_SIZE = 256;

    // the reply of GetHostByName
    struct RawHostEntry
    {
        s16 addrType;                       // 0x0000
        s16 length;                         // 0x0002
        s16 addressCount;                   // 0x0004
        s16 aliasCount;                     // 0x0006
        char name[NAME_SIZE];               // 0x0008
        char aliases[MAX_ALIASES][NAME_SIZE];   // 0x0108
        u8 addresses[MAX_ADDRESSES][16];    // 0x1908
    };

    // one entry of the reply of GetAddrInfo (also the form of the hints)
    struct RawAddrInfo
    {
        s32 flags;                  // 0x000
        s32 family;                 // 0x004
        s32 sockType;               // 0x008
        s32 protocol;               // 0x00C
        s32 addrLength;             // 0x010
        char canonName[NAME_SIZE];  // 0x014
        u8 addr[0x1C];              // 0x114
    };

    // fills mHostEntries[mCurrentHostEntry] from the raw reply
    void MakeHostEntry(); // 0x00486054 | fefates:bytes [tier B]

    nn::socket::HostEnt mHostEntries[HOST_ENTRY_COUNT];                     // 0x0004
    RawHostEntry mRawHostEntries[HOST_ENTRY_COUNT];                         // 0x0014
    char* mHostPointers[HOST_ENTRY_COUNT][MAX_ALIASES + 2];                 // 0x1A9C the addresses, 0, the aliases
    s32 mCurrentHostEntry;                                                  // 0x1B04
    RawAddrInfo mRawAddrInfos[MAX_ADDR_INFOS];                              // 0x1B08
    nn::os::CriticalSection mLock;                                          // 0x3788

    static void CheckLayout()
    {
        ASSERT_SIZE(RawHostEntry, 0x1A88);
        ASSERT_SIZE(RawAddrInfo, 0x130);
        ASSERT_OFFSET(DnsUserClient, mRawHostEntries, 0x14);
        ASSERT_OFFSET(DnsUserClient, mHostPointers, 0x1A9C);
        ASSERT_OFFSET(DnsUserClient, mCurrentHostEntry, 0x1B04);
        ASSERT_OFFSET(DnsUserClient, mRawAddrInfos, 0x1B08);
        ASSERT_OFFSET(DnsUserClient, mLock, 0x3788);
    }
};
ASSERT_SIZE(DnsUserClient, 0x3794);

extern DnsUserClient* s_pDnsClient; // 0x0097F070 (name is ours)
} // namespace detail
} // namespace socket
} // namespace nn
