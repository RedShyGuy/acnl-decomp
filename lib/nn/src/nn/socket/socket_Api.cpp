#include "nn/socket/socket_Api.h"
#include "nn/fnd/fnd_IAllocator.h"
#include "nn/nstd/nstd_String.h"
#include "nn/socket/detail/detail_Api.h"
#include "nn/socket/detail/socket_DnsUserClient.h"
#include "nn/socket/detail/socket_SessionPool.h"
#include "nn/srv/srv_Api.h"

#include <ctype.h>

namespace nn {
namespace socket {

namespace detail {
// the heap of the library (made by the static initializer at 0x0079CAB0)
// 0x00AE1FE0
Heap s_Heap;
// 0x00AE2044
Heap::Allocator s_HeapAllocator;
} // namespace detail

namespace {
// GetNetworkOpt: the configuration level and its IP information option (3dbrew "Socket Services")
const s32 SOL_CONFIG = 0xFFFE;
const s32 NETOPT_IP_INFO = 0x4003;

// the reply of NETOPT_IP_INFO (names are ours)
struct IpInfo
{
    u32 address;
    u32 netmask;
    u32 broadcast;
};

// what the heap needs besides the DnsUserClient and the session items (name is ours)
const size_t HEAP_OVERHEAD = 0x800;

// the error number GetAddrInfo returns without the DNS client (value from the binary)
const s32 ERROR_NOT_INITIALIZED = -308;
} // namespace

// the globals of this file (names are ours)
// 0x00975FB0
bool s_IsInitialized;
// memory to free in Finalize (never set in this program)
// 0x00975FB4
void* s_pMemory;
// 0x00975FB8
nn::fnd::IAllocator* s_pMemoryAllocator;

// 0x00484A18 | mk7dlp:callseq [tier A]
nn::Result Initialize(uptr memory, size_t memorySize, size_t sharedMemorySize, s32 sessionCount)
{
    if (s_IsInitialized) {
        return nn::Result(detail::RESULT_ALREADY_INITIALIZED);
    }
    nn::srv::Initialize();
    detail::s_Heap.Initialize(memory + sharedMemorySize, memorySize - sharedMemorySize, 0);
    detail::s_HeapAllocator.Initialize(&detail::s_Heap);
    nn::Result result = detail::InitializeDnsClient();
    if (result.IsSuccess()) {
        result = detail::InitializeControlSession(memory, sharedMemorySize);
        if (result.IsSuccess()) {
            result = detail::InitializeSessionPool(detail::s_HeapAllocator, sessionCount);
            if (result.IsSuccess()) {
                s_IsInitialized = true;
                return nn::Result();
            }
            detail::FinalizeControlSession();
        }
    }
    if (detail::s_pDnsClient != 0) {
        detail::s_pDnsClient->~DnsUserClient();
        detail::s_Heap.Free(detail::s_pDnsClient);
        detail::s_pDnsClient = 0;
    }
    detail::s_HeapAllocator.Finalize();
    detail::s_Heap.Finalize();
    return result;
}

// 0x00484BA8 | fefates:bytes [tier B]
s32 GetAddrInfo(const char* node, const char* service, const nn::socket::AddrInfo* hints, nn::socket::AddrInfo** result)
{
    if (detail::s_pDnsClient == 0) {
        return ERROR_NOT_INITIALIZED;
    }
    return detail::s_pDnsClient->GetAddrInfo(node, service, hints, result);
}

// 0x00484BE4 | tier C (confirmed by the code)
void FreeAddrInfo(nn::socket::AddrInfo* info)
{
    if (detail::s_pDnsClient != 0) {
        detail::s_pDnsClient->FreeAddrInfo(info);
    }
}

// 0x00484C04 (name is ours, after DnsUserClient::GetHostByName)
nn::socket::HostEnt* GetHostByName(const char* name)
{
    if (detail::s_pDnsClient == 0) {
        return 0;
    }
    return detail::s_pDnsClient->GetHostByName(name);
}

// 0x00484C24 | fefates:bytes [tier B]
s32 GetPrimaryAddress(u8* address, u8* netmask)
{
    IpInfo info;
    s32 size = sizeof(info);
    s32 result = 0;
    nn::Result ipcResult = detail::GetNetworkOpt(&result, SOL_CONFIG, NETOPT_IP_INFO, reinterpret_cast<u8*>(&info), &size);
    s32 error = ipcResult.IsSuccess() ? result : detail::ConvertErrorResult(ipcResult);
    if (error < 0) {
        return error;
    }
    if (address != 0) {
        *reinterpret_cast<u32*>(address) = info.address;
    }
    if (netmask != 0) {
        *reinterpret_cast<u32*>(netmask) = info.netmask;
    }
    return 0;
}

// 0x00484CA0 | mk7dlp:bytes [tier A]
size_t GetRequiredMemorySize(size_t sharedMemorySize, s32 sessionCount)
{
    return detail::GetRequiredMemorySizeForSessionPool(sessionCount) + sharedMemorySize +
           sizeof(detail::DnsUserClient) + HEAP_OVERHEAD;
}

// 0x00484CC0 | fefates:bytes [tier B]
const char* IPAtoN(const char* text, u8* address)
{
    u32 parts[4];
    s32 count = 0;
    u32 value;
    char c = *text;
    for (;;) {
        if (!isdigit(c)) {
            return 0;
        }
        u32 base = 10;
        if (c == '0') {
            c = *++text;
            if (c == 'x' || c == 'X') {
                base = 16;
                c = *++text;
            } else {
                base = 8;
            }
        }
        value = 0;
        for (;;) {
            if (isdigit(c)) {
                value = value * base + (c - '0');
            } else if (base == 16 && isxdigit(c)) {
                value = (value << 4) + (c - (islower(c) ? 'a' : 'A')) + 10;
            } else {
                break;
            }
            c = *++text;
        }
        parts[count++] = value;
        if (c != '.') {
            break;
        }
        if (count >= 4 || value > 0xFF) {
            return 0;
        }
        c = *++text;
    }
    if (c > 0x7F) {
        return 0;
    }
    switch (count) {
    case 1:
        address[0] = value >> 24;
        address[1] = value >> 16;
        address[2] = value >> 8;
        address[3] = value;
        break;
    case 2:
        if (value > 0xFFFFFF) {
            return 0;
        }
        address[0] = parts[0];
        address[1] = value >> 16;
        address[2] = value >> 8;
        address[3] = value;
        break;
    case 3:
        if (value > 0xFFFF) {
            return 0;
        }
        address[0] = parts[0];
        address[1] = parts[1];
        address[2] = value >> 8;
        address[3] = value;
        break;
    case 4:
        if (value > 0xFF) {
            return 0;
        }
        address[0] = parts[0];
        address[1] = parts[1];
        address[2] = parts[2];
        address[3] = value;
        break;
    default:
        return 0;
    }
    return text;
}

// 0x0048A550 | fefates:bytes [tier B]
nn::Result Finalize()
{
    if (!s_IsInitialized) {
        return nn::Result(detail::RESULT_NOT_INITIALIZED);
    }
    detail::FinalizeSessionPool();
    detail::FinalizeControlSession();
    detail::s_pDnsClient->~DnsUserClient();
    detail::s_Heap.Free(detail::s_pDnsClient);
    detail::s_pDnsClient = 0;
    detail::s_HeapAllocator.Finalize();
    detail::s_Heap.Finalize();
    if (s_pMemory != 0) {
        s_pMemoryAllocator->Free(s_pMemory);
        s_pMemory = 0;
        s_pMemoryAllocator = 0;
    }
    s_IsInitialized = false;
    return nn::Result();
}

// 0x0048A628 | fefates:bytes [tier B]
const char* InetNtoP(s32 family, const void* source, char* destination, size_t size)
{
    if (source == 0 || destination == 0) {
        return 0;
    }
    if (family != AF_INET) {
        return 0;
    }
    if (size < 16) {
        return 0;
    }
    const u8* address = static_cast<const u8*>(source);
    nnnstdTSNPrintf(destination, size, "%d.%d.%d.%d", address[0], address[1], address[2], address[3]);
    return destination;
}

// 0x0048A6A0 | fefates:bytes [tier B]
// 1 for an address, 0 for none, -5 for another family
s32 InetPtoN(s32 family, const char* source, void* destination)
{
    if (source == 0 || destination == 0) {
        return 0;
    }
    if (family != AF_INET) {
        return -5;
    }
    const char* end = IPAtoN(source, static_cast<u8*>(destination));
    if (end == 0 || isalnum(*end)) {
        return 0;
    }
    return 1;
}

} // namespace socket
} // namespace nn
