#include "nn/socket/detail/socket_DnsUserClient.h"
#include "nn/socket/detail/detail_Api.h"

#include <algorithm>
#include <string.h>

namespace nn {
namespace socket {
namespace detail {

namespace {
// the library's error numbers (values from the binary; names are ours)
const s32 ERROR_NO_MEMORY = -304;
const s32 ERROR_NO_ADDRESS = -305;
} // namespace

// 0x00485AA4 | fefates:bytes [tier B]
s32 nn::socket::detail::DnsUserClient::GetAddrInfo(const char* node, const char* service, const nn::socket::AddrInfo* hints, nn::socket::AddrInfo** result)
{
    nn::os::CriticalSection::ScopedLock lock(mLock);
    char empty = '\0';
    size_t nodeSize;
    if (node == 0) {
        node = &empty;
        nodeSize = 0;
    } else {
        nodeSize = std::min(strlen(node) + 1, NAME_SIZE);
    }
    size_t serviceSize;
    if (service == 0) {
        service = &empty;
        serviceSize = 0;
    } else {
        serviceSize = strlen(service) + 1;
    }
    RawAddrInfo rawHints;
    memset(&rawHints, 0, sizeof(rawHints));
    s32 hintsSize = 0;
    if (hints != 0) {
        rawHints.flags = hints->flags;
        rawHints.family = hints->family;
        rawHints.sockType = hints->sockType;
        rawHints.protocol = hints->protocol;
        hintsSize = sizeof(rawHints);
    }
    memset(mRawAddrInfos, 0, sizeof(mRawAddrInfos));
    s32 count = 0;
    s32 error = 0;
    nn::Result ipcResult = detail::GetAddrInfo(&error, node, nodeSize, service, serviceSize,
                                               reinterpret_cast<const u8*>(&rawHints), hintsSize, &count,
                                               reinterpret_cast<u8*>(mRawAddrInfos), sizeof(mRawAddrInfos));
    if (ipcResult.IsFailure()) {
        return ConvertErrorResult(ipcResult);
    }
    if (error != 0) {
        return error;
    }

    nn::socket::AddrInfo* first = 0;
    nn::socket::AddrInfo* last = 0;
    for (s32 i = 0; i < count; i++) {
        const RawAddrInfo& raw = mRawAddrInfos[i];
        if (raw.family != AF_INET && raw.family != AF_INET6) {
            continue;
        }
        nn::socket::AddrInfo* info = static_cast<nn::socket::AddrInfo*>(Allocate(sizeof(nn::socket::AddrInfo), 4));
        if (info == 0) {
            FreeAddrInfo(first);
            return ERROR_NO_MEMORY;
        }
        *info = nn::socket::AddrInfo();
        info->flags = raw.flags;
        info->family = raw.family;
        info->sockType = raw.sockType;
        info->protocol = raw.protocol;
        info->addrLength = raw.addrLength;
        if (strlen(raw.canonName) != 0) {
            size_t size = strlen(raw.canonName) + 1;
            info->canonName = static_cast<char*>(Allocate(size, 4));
            if (info->canonName == 0) {
                FreeAddrInfo(first);
                FreeAddrInfo(info);
                return ERROR_NO_MEMORY;
            }
            strlcpy(info->canonName, raw.canonName, size);
        }
        info->addr = static_cast<nn::socket::SockAddr*>(Allocate(info->addrLength, 4));
        if (info->addr == 0) {
            FreeAddrInfo(first);
            FreeAddrInfo(info);
            return ERROR_NO_MEMORY;
        }
        memcpy(info->addr, raw.addr, info->addrLength);
        info->next = 0;
        if (last != 0) {
            last->next = info;
        }
        if (i == 0) {
            first = info;
        }
        last = info;
    }
    if (first == 0) {
        return ERROR_NO_ADDRESS;
    }
    *result = first;
    return error;
}

// 0x00485EE4 | fefates:bytes [tier B]
void nn::socket::detail::DnsUserClient::FreeAddrInfo(nn::socket::AddrInfo* info)
{
    while (info != 0) {
        if (info->canonName != 0) {
            Free(info->canonName);
            info->canonName = 0;
        }
        if (info->addr != 0) {
            Free(info->addr);
            info->addr = 0;
        }
        nn::socket::AddrInfo* next = info->next;
        Free(info);
        info = next;
    }
}

// 0x00485F3C | fefates:bytes [tier B]
nn::socket::HostEnt* nn::socket::detail::DnsUserClient::GetHostByName(const char* name)
{
    if (name == 0) {
        return 0;
    }
    nn::os::CriticalSection::ScopedLock lock(mLock);
    if (++mCurrentHostEntry >= HOST_ENTRY_COUNT) {
        mCurrentHostEntry = 0;
    }
    s32 error = 0;
    size_t nameSize = std::min(strlen(name) + 1, NAME_SIZE);
    nn::Result ipcResult = detail::GetHostByName(&error, name, nameSize,
                                                 reinterpret_cast<u8*>(&mRawHostEntries[mCurrentHostEntry]),
                                                 sizeof(RawHostEntry));
    if (ipcResult.IsFailure() || error != 0) {
        if (mCurrentHostEntry != 0) {
            mCurrentHostEntry--;
        }
        return 0;
    }
    MakeHostEntry();
    return &mHostEntries[mCurrentHostEntry];
}

// 0x00486054 | fefates:bytes [tier B]
void nn::socket::detail::DnsUserClient::MakeHostEntry()
{
    RawHostEntry& raw = mRawHostEntries[mCurrentHostEntry];
    char** table = mHostPointers[mCurrentHostEntry];
    s32 count = 0;
    char** addrList = 0;
    for (s32 i = 0; i < raw.addressCount; i++) {
        table[count++] = reinterpret_cast<char*>(raw.addresses[i]);
    }
    if (count != 0) {
        addrList = table;
    }
    table[count++] = 0;
    s32 firstAlias = count;
    for (s32 i = 0; i < raw.aliasCount; i++) {
        table[count++] = raw.aliases[i];
    }
    char** aliases = (count != firstAlias) ? &table[firstAlias] : 0;

    nn::socket::HostEnt& entry = mHostEntries[mCurrentHostEntry];
    entry.name = raw.name;
    entry.aliases = aliases;
    entry.addrType = raw.addrType;
    entry.length = raw.length;
    entry.addrList = addrList;
}

// 0x004862E8 slot 0x00
// 0x004862D8 slot 0x04 (deleting dtor)
nn::socket::detail::DnsUserClient::~DnsUserClient()
{
    // nothing to do
}

} // namespace detail
} // namespace socket
} // namespace nn
