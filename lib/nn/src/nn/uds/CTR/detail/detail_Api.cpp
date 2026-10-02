#include "nn/uds/CTR/detail/detail_Api.h"
#include "nn/cfg/CTR/CTR_Api.h"
#include "nn/nwm/CTR/CTR_Api.h"
#include "nn/os/os_Api.h"
#include "nn/srv/srv_Api.h"

#include <string.h>

namespace nn {
namespace uds {
namespace CTR {
namespace detail {

// 0x0097F090
u8 s_ChannelForCreateNetwork;
// 0x0097F091
u8 s_ChannelForStartScan;
// 0x0097F092
bool s_IsInitialized;
// 0x0097F094
u32 s_LastEndpointId;
// 0x0097F098
nn::os::ipc::Session s_ReceiveSession;
// 0x0097F09C
nn::os::ipc::Session s_Session;
// 0x0097F0A0
nn::os::Event* s_pStatusEvent;
// 0x00AEEA74
NetworkDescription s_NetworkDescription;
// 0x00AEEB7C
nn::os::CriticalSection s_CriticalSection;
// 0x00AEEB88
EndpointEntry s_Endpoints[ENDPOINT_MAX];
// 0x00AEEC48
nn::os::TransferMemoryBlock s_TransferMemory;
// 0x00AEEC68
NodeInformationList s_NodeInformationList;
// 0x00AEEEE8
NetworkDescription s_ReaderNetworkDescription;
// 0x00AEEFF0
NodeInformationElement s_ReaderNodeInformationElements[2];

namespace {

const char SERVICE_NAME[] = "nwm::UDS";

// what the transfer memory allows the service (read and write)
const u32 TRANSFER_MEMORY_PERMISSION_OTHER = 3;

} // namespace

// 0x00469EA0
nn::uds::CTR::detail::EndpointEntry::EndpointEntry()
{
    event = nn::Handle();
}

// 0x00469EAC
nn::uds::CTR::detail::EndpointEntry::~EndpointEntry()
{
    if (event.IsValid()) {
        nn::svc::CloseHandle(event);
        event = nn::Handle();
    }
}

// 0x004692A8 | tier C
void FinalizeCore()
{
    nn::os::CriticalSection::ScopedLock lock(s_CriticalSection);
    if (!s_IsInitialized) {
        return;
    }
    s_IsInitialized = false;

    for (s32 i = 0; i < ENDPOINT_MAX; i++) {
        EndpointEntry& entry = s_Endpoints[i];
        entry.isAttached = false;
        entry.id = 0;
        if (entry.event.IsValid()) {
            nn::svc::CloseHandle(entry.event);
            entry.event = nn::Handle();
        }
    }
    s_LastEndpointId = 0;
    s_pStatusEvent->Close();

    GetUds().Finalize();
    s_ReceiveSession.Close();
    s_Session.Close();
    s_TransferMemory.Finalize();
    nn::srv::Finalize();
}

// 0x004693A8 | fefates:bytes [tier B]
nn::Result GetMacAddress(u8* mac)
{
    nn::nwm::Mac address = {};
    if (nn::nwm::CTR::GetMacAddress(address).IsFailure()) {
        return nn::Result(RESULT_NOT_AUTHORIZED);
    }
    memcpy(mac, &address, sizeof(address));
    return nn::Result();
}

// 0x004693F0 | mk7dlp:callseq [tier A]
nn::Result InitializeCore(nn::os::Event* statusEvent, void* buffer, size_t bufferSize,
                          const nn::cfg::CTR::UserName* userName)
{
    nn::os::CriticalSection::ScopedLock lock(s_CriticalSection);
    if (s_IsInitialized) {
        return nn::Result(RESULT_ALREADY_INITIALIZED);
    }

    s_TransferMemory.Initialize(buffer, bufferSize, 0, TRANSFER_MEMORY_PERMISSION_OTHER);
    nn::Result result = nn::Result(RESULT_OUT_OF_MEMORY);
    if (nn::srv::Initialize().IsFailure()) {
        return result;
    }
    // every failure below goes back through the same steps
    if (nn::srv::GetServiceHandle(&s_Session, SERVICE_NAME).IsSuccess()) {
        if (nn::srv::GetServiceHandle(&s_ReceiveSession, SERVICE_NAME).IsSuccess()) {
            nn::Handle event;
            NodeInformationRaw node;
            nn::cfg::CTR::Initialize();
            if (userName != NULL) {
                node.userName = *userName;
            } else {
                nn::cfg::CTR::UserName systemUserName = {};
                nn::cfg::CTR::GetUserName(&systemUserName);
                node.userName = systemUserName;
            }
            node.localFriendCodeSeed = nn::cfg::CTR::GetLocalFriendCodeSeed();
            nn::cfg::CTR::Finalize();

            result = GetUds().InitializeWithVersion(&event, s_TransferMemory.GetHandle(), bufferSize, &node,
                                                    UDS_VERSION);
            if (result.IsSuccess()) {
                // the handle is stored without closing the event's old one
                *reinterpret_cast<nn::Handle*>(statusEvent) = event;
                s_pStatusEvent = statusEvent;
                s_IsInitialized = true;
                s_LastEndpointId = 0;
                for (s32 i = 0; i < ENDPOINT_MAX; i++) {
                    s_Endpoints[i].isAttached = false;
                    s_Endpoints[i].id = 0;
                }
                return result;
            }
            s_ReceiveSession.Close();
        }
        s_Session.Close();
    }
    s_TransferMemory.Finalize();
    nn::srv::Finalize();
    return result;
}

// 0x0046962C | tier C
nn::Result DestroyEndpoint(EndpointDescriptor* endpoint, ReceiveReport* report)
{
    nn::os::CriticalSection::ScopedLock lock(s_CriticalSection);
    if (!s_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    if (endpoint->id == 0) {
        return nn::Result(RESULT_NOT_AUTHORIZED);
    }
    s32 i;
    for (i = 0; i < ENDPOINT_MAX; i++) {
        if (s_Endpoints[i].id == endpoint->id) {
            break;
        }
    }
    if (i >= ENDPOINT_MAX) {
        return nn::Result(RESULT_NOT_AUTHORIZED);
    }

    nn::Result result = GetUds().Unbind(*endpoint, report);
    if (result.IsSuccess()) {
        EndpointEntry& entry = s_Endpoints[i];
        entry.isAttached = false;
        entry.id = 0;
        if (entry.event.IsValid()) {
            nn::svc::CloseHandle(entry.event);
            entry.event = nn::Handle();
        }
        endpoint->id = 0;
    } else {
        ThrowIfSessionClosed(result);
    }
    return result;
}

// 0x00469764 | fefates:bytes [tier B]
void ScrambleLocalFriendCode(ScrambledLocalFriendCode* scrambled, u64 localFriendCodeSeed, u16 nodeId)
{
    u32 key = static_cast<u32>(static_cast<s64>(nn::os::GetCreationTime()));
    memcpy(scrambled, &localFriendCodeSeed, sizeof(localFriendCodeSeed));
    scrambled->nodeId = nodeId ^ key;
    scrambled->key = key;
    for (s32 i = 0; i < 4; i++) {
        scrambled->code[i] ^= key;
    }
}

} // namespace detail
} // namespace CTR
} // namespace uds
} // namespace nn
