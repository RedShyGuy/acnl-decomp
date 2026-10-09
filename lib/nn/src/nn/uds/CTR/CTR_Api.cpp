#include "nn/uds/CTR/CTR_Api.h"
#include "nn/applet/CTR/CTR_Api.h"
#include "nn/cfg/CTR/CTR_Api.h"
#include "nn/ndm/CTR/detail/ndm_Interface.h"
#include "nn/ndm/ndm_Api.h"
#include "nn/svc/svc_Api.h"
#include "nn/uds/CTR/detail/detail_Api.h"

#include <string.h>

namespace nn {
namespace uds {
namespace CTR {

using namespace nn::uds::CTR::detail;

namespace {

// the vendor tag of the probe response: Nintendo's OUI
const u8 PROBE_RESPONSE_OUI[3] = { 0x00, 0x1F, 0x32 }; // 0x008B46E0
// any access point
const nn::nwm::Mac BSSID_BROADCAST = { { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } }; // 0x008B46EC

// applet sleep notification states in which uds cannot start (meaning ours: going to sleep)
const u8 SLEEP_NOTIFICATION_STATE_2 = 2;
const u8 SLEEP_NOTIFICATION_STATE_4 = 4;

// the exclusive state of ndm that uds needs (value from the binary, name ours)
const s32 NDM_EXCLUSIVE_STATE_LOCAL_COMMUNICATIONS = 2;

// NetworkDescriptionElement::attribute (host byte order): bit 0 refuses spectators; the
// bits of UpdateNetworkAttribute refuse new clients (2) and spectators (4)
const u16 ATTRIBUTE_NO_SPECTATOR = 1;
const u16 ATTRIBUTE_BITS_CLIENTS = 2;
const u16 ATTRIBUTE_BITS_CLIENTS_AND_SPECTATORS = 6;

const u16 CHANNEL_MASK_DEFAULT = 0x421;     // channels 1, 6 and 11
const u8 CHANNEL_MAX = 13;
const u16 SCAN_TIME_DEFAULT = 110;
const u16 SCAN_TIME_PASSIVE = 20;
const u16 SCAN_TYPE_SCAN = 2;
const u16 SCAN_TYPE_ON_CONNECTION = 3;

const size_t APPLICATION_DATA_SIZE_MAX = 200;
const u32 ENDPOINT_ID_MAX = 0x0FFFFFFF;

// configuration memory, 3dbrew "Configuration Memory": ENVINFO, bit 0 set on retail units
const uptr CONFIG_MEMORY_ENVINFO = 0x1FF80014;

// 1 << (channel - 1), the default channels for channel 0
inline u16 GetChannelMask(u8 channel)
{
    return channel == 0 ? CHANNEL_MASK_DEFAULT : static_cast<u16>(1 << (channel - 1));
}

// the session closed means the service is gone: report "not initialized"
inline nn::Result ConvertSessionClosed(nn::Result result)
{
    if (result == nn::Result(RESULT_SESSION_CLOSED)) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return result;
}

} // namespace

// 0x00467BF8 (name is ours)
nn::Result GetChannel(u8* channel)
{
    nn::os::CriticalSection::ScopedLock lock(s_CriticalSection);
    if (!s_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    nn::Result result = GetUds().GetChannel(channel);
    ThrowIfSessionClosed(result);
    return result;
}

// 0x00467C70 | tier C
nn::Result Initialize(nn::os::Event* statusEvent, void* buffer, size_t bufferSize,
                      const nn::cfg::CTR::UserName* userName)
{
    if (s_IsInitialized) {
        return nn::Result(RESULT_ALREADY_INITIALIZED);
    }
    u8 sleepState = nn::applet::CTR::GetSleepNotificationState();
    if (sleepState == SLEEP_NOTIFICATION_STATE_2 || sleepState == SLEEP_NOTIFICATION_STATE_4) {
        return nn::Result(RESULT_NOT_AUTHORIZED_STATE);
    }
    if (nn::ndm::Initialize().IsFailure()) {
        return nn::Result(RESULT_OUT_OF_MEMORY);
    }
    if (nn::ndm::CTR::detail::Interface::EnterExclusiveStateEntry(NDM_EXCLUSIVE_STATE_LOCAL_COMMUNICATIONS).IsFailure()) {
        nn::ndm::Finalize();
        return nn::Result(RESULT_ALREADY_INITIALIZED);
    }
    nn::Result result = InitializeCore(statusEvent, buffer, bufferSize, userName);
    if (result.IsFailure()) {
        nn::ndm::CTR::detail::Interface::LeaveExclusiveStateEntry();
        nn::ndm::Finalize();
        return result;
    }
    s_ChannelForCreateNetwork = 0;
    s_ChannelForStartScan = 0;
    return result;
}

// 0x00469F3C (name is ours)
void Finalize()
{
    if (!s_IsInitialized) {
        return;
    }
    FinalizeCore();
    nn::ndm::CTR::detail::Interface::LeaveExclusiveStateEntry();
    nn::ndm::Finalize();
}

// 0x00467D38 | tier C
nn::Result EjectClient(u16 nodeId)
{
    nn::os::CriticalSection::ScopedLock lock(s_CriticalSection);
    if (!s_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    if (nodeId == NODE_ID_HOST) {
        return nn::Result(RESULT_NOT_AUTHORIZED);
    }
    nn::Result result = GetUds().EjectClient(nodeId);
    ThrowIfSessionClosed(result);
    return result;
}

// 0x00467DC0 | fefates:bytes-fuzzy [tier B]
nn::Result ReceiveFrom(const EndpointDescriptor& endpoint, void* buffer, size_t* receivedSize, u16* sourceNodeId,
                       size_t bufferSize, u8 option)
{
    if (!s_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    if (endpoint.id == 0) {
        return nn::Result(RESULT_NOT_AUTHORIZED);
    }
    s32 i;
    {
        nn::os::CriticalSection::ScopedLock lock(s_CriticalSection);
        for (i = 0; i < ENDPOINT_MAX; i++) {
            if (s_Endpoints[i].id == endpoint.id && s_Endpoints[i].isAttached) {
                break;
            }
        }
    }
    if (i >= ENDPOINT_MAX) {
        return nn::Result(RESULT_NOT_AUTHORIZED);
    }
    if (bufferSize & 3) {
        return nn::Result(RESULT_MISALIGNED_SIZE);
    }
    if ((reinterpret_cast<uptr>(buffer) & 3) || buffer == NULL) {
        return nn::Result(RESULT_MISALIGNED_ADDRESS);
    }

    EndpointEntry& entry = s_Endpoints[i];
    for (;;) {
        nn::Result result = Uds(s_ReceiveSession.GetHandle())
                                .PullPacket(endpoint, static_cast<bit32*>(buffer), bufferSize / sizeof(bit32),
                                            bufferSize, receivedSize, sourceNodeId);
        if (result.IsFailure() || *receivedSize != 0 || option == 1) {
            return ConvertSessionClosed(result);
        }
        if (entry.id == 0) {
            return nn::Result(RESULT_NOT_AUTHORIZED);
        }
        if (nn::svc::WaitSynchronization1(entry.event, -1).IsFailure()) {
            return nn::Result(RESULT_NOT_AUTHORIZED);
        }
    }
}

// 0x00467F7C | fefates:bytes-fuzzy [tier B]
nn::Result CreateNetwork(u8 subId, u8 nodeCountMax, u32 localCommunicationId, const char* passphrase,
                         size_t passphraseSize, bool probeResponse, u8 channel, const void* applicationData,
                         size_t applicationDataSize)
{
    nn::os::CriticalSection::ScopedLock lock(s_CriticalSection);
    if (!s_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    if (localCommunicationId == 0) {
        return nn::Result(RESULT_NOT_AUTHORIZED);
    }
    if (nodeCountMax <= 1 || nodeCountMax > NODE_MAX || subId == 0xFF || passphrase == NULL ||
        passphraseSize < PASSPHRASE_SIZE_MIN || passphraseSize > PASSPHRASE_SIZE_MAX) {
        return nn::Result(RESULT_OUT_OF_RANGE);
    }
    if (!(channel == 0 || channel == 1 || channel == 6 || channel == 11)) {
        return nn::Result(RESULT_OUT_OF_RANGE);
    }

    // only debug units may choose the channel
    nn::cfg::CTR::Initialize();
    if (!nn::cfg::CTR::IsDebugMode()) {
        channel = 0;
    }
    nn::cfg::CTR::Finalize();
    if (s_ChannelForCreateNetwork != 0) {
        channel = s_ChannelForCreateNetwork;
    }

    nn::Result result = s_NetworkDescription.Initialize(localCommunicationId, subId, nodeCountMax, channel,
                                                        applicationData, applicationDataSize);
    if (result.IsFailure()) {
        return result;
    }
    result = GetUds().SetProbeResponseParam(PROBE_RESPONSE_OUI, probeResponse);
    if (result.IsFailure()) {
        return result;
    }
    result = GetUds().CreateNetwork2(s_NetworkDescription, passphrase, passphraseSize);
    ThrowIfSessionClosed(result);
    return result;
}

// 0x004680F4 | fefates:bytes [tier B]
nn::Result CreateNetwork(u8 subId, u8 nodeCountMax, u32 localCommunicationId, const char* passphrase,
                         size_t passphraseSize, u8 channel, const void* applicationData,
                         size_t applicationDataSize)
{
    return CreateNetwork(subId, nodeCountMax, localCommunicationId, passphrase, passphraseSize, false, channel,
                         applicationData, applicationDataSize);
}

// 0x00468120 | tier C
nn::Result AllowToConnect()
{
    nn::os::CriticalSection::ScopedLock lock(s_CriticalSection);
    if (!s_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    nn::Result result = GetUds().UpdateNetworkAttribute(ATTRIBUTE_BITS_CLIENTS_AND_SPECTATORS, false);
    ThrowIfSessionClosed(result);
    return result;
}

// 0x004685DC | tier C
nn::Result DisallowToConnect(bool spectatorsToo)
{
    nn::os::CriticalSection::ScopedLock lock(s_CriticalSection);
    if (!s_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    nn::Result result = GetUds().UpdateNetworkAttribute(
        spectatorsToo ? ATTRIBUTE_BITS_CLIENTS_AND_SPECTATORS : ATTRIBUTE_BITS_CLIENTS, true);
    ThrowIfSessionClosed(result);
    return result;
}

// 0x00468198 | fefates:bytes-fuzzy [tier B]
nn::Result ConnectNetwork(const NetworkDescription& network, ConnectType type, const char* passphrase,
                          size_t passphraseSize)
{
    nn::os::CriticalSection::ScopedLock lock(s_CriticalSection);
    if (!s_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    nn::cfg::CTR::Initialize();
    nn::cfg::CTR::Finalize();
    if (type == CONNECT_TYPE_SPECTATOR) {
        if (!network.m_IsInitialized || (__builtin_bswap16(network.m_Element.attribute) & ATTRIBUTE_NO_SPECTATOR)) {
            return nn::Result(RESULT_NOT_AUTHORIZED);
        }
    }
    if (passphrase == NULL || passphraseSize < PASSPHRASE_SIZE_MIN || passphraseSize > PASSPHRASE_SIZE_MAX) {
        return nn::Result(RESULT_OUT_OF_RANGE);
    }
    nn::Result result = GetUds().ConnectNetwork2(network, type, passphrase, passphraseSize);
    ThrowIfSessionClosed(result);
    return result;
}

// 0x0046828C | tier C
nn::Result CreateEndpoint(EndpointDescriptor* endpoint)
{
    nn::os::CriticalSection::ScopedLock lock(s_CriticalSection);
    if (!s_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    s32 i;
    for (i = 0; i < ENDPOINT_MAX; i++) {
        if (s_Endpoints[i].id == 0) {
            break;
        }
    }
    if (i >= ENDPOINT_MAX || s_LastEndpointId >= ENDPOINT_ID_MAX) {
        return nn::Result(RESULT_OUT_OF_MEMORY);
    }
    s_LastEndpointId++;
    s_Endpoints[i].id = s_LastEndpointId;
    endpoint->id = s_LastEndpointId;
    return nn::Result();
}

// 0x00468370 | tier C
nn::Result DestroyNetwork()
{
    nn::os::CriticalSection::ScopedLock lock(s_CriticalSection);
    if (!s_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    nn::Result result = GetUds().DestroyNetwork();
    ThrowIfSessionClosed(result);
    return result;
}

// 0x004683E0 | tier C
nn::Result DestroyEndpoint(EndpointDescriptor* endpoint)
{
    if (!s_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    ReceiveReport report;
    return detail::DestroyEndpoint(endpoint, &report);
}

// 0x00468414 | fefates:bytes-fuzzy [tier B]
nn::Result ScanOnConnection(void* buffer, size_t bufferSize, u8 subId, u32 localCommunicationId, u8 channel,
                            bool passive, u16 scanTime)
{
    nn::os::CriticalSection::ScopedLock lock(s_CriticalSection);
    if (!s_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    if (localCommunicationId == 0 || channel > CHANNEL_MAX) {
        return nn::Result(RESULT_OUT_OF_RANGE);
    }
    if (scanTime == 0 && !passive) {
        scanTime = SCAN_TIME_DEFAULT;
    } else if (scanTime == 0 && passive) {
        scanTime = SCAN_TIME_PASSIVE;
    }

    nn::nwm::CTR::ScanParamIpc param;
    param.unknown00 = passive ^ 1;
    param.scanType = SCAN_TYPE_ON_CONNECTION;
    param.channelMask = GetChannelMask(channel);
    nn::nwm::Mac bssid = BSSID_BROADCAST;
    param.bssid = bssid;
    param.unknown30 = 0;
    param.scanTime = scanTime;
    nn::Result result = GetUds().ScanOnConnection(static_cast<u8*>(buffer), bufferSize, param,
                                                  localCommunicationId, subId);
    ThrowIfSessionClosed(result);
    return result;
}

// 0x00468554 | fefates:bytes [tier B]
nn::Result ScanOnConnection(void* buffer, size_t bufferSize, u8 subId, u32 localCommunicationId, u8 channel,
                            u16 scanTime)
{
    return ScanOnConnection(buffer, bufferSize, subId, localCommunicationId, channel, false, scanTime);
}

// 0x00468EB8 (name is ours)
nn::Result StartScan(void* buffer, size_t bufferSize, u8 subId, u32 localCommunicationId, u8 channel,
                     u16 scanTime)
{
    nn::os::CriticalSection::ScopedLock lock(s_CriticalSection);
    if (!s_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    if (localCommunicationId == 0 || channel > CHANNEL_MAX) {
        return nn::Result(RESULT_OUT_OF_RANGE);
    }

    // only debug units may choose the channel
    nn::cfg::CTR::Initialize();
    if (!nn::cfg::CTR::IsDebugMode()) {
        channel = 0;
    }
    nn::cfg::CTR::Finalize();
    if (s_ChannelForStartScan != 0) {
        channel = s_ChannelForStartScan;
    }

    nn::nwm::CTR::ScanParamIpc param;
    param.scanType = SCAN_TYPE_SCAN;
    param.channelMask = GetChannelMask(channel);
    param.unknown00 = 1;
    nn::nwm::Mac bssid = BSSID_BROADCAST;
    param.bssid = bssid;
    if (scanTime == 0) {
        scanTime = SCAN_TIME_DEFAULT;
    }
    param.unknown30 = 0;
    param.scanTime = scanTime;

    nn::os::Event event;
    if (event.TryInitialize(nn::os::RESET_TYPE_ONESHOT).IsFailure()) {
        return nn::Result(RESULT_OUT_OF_MEMORY);
    }
    nn::Result result = GetUds().StartScan(event.GetHandle(), static_cast<u8*>(buffer), bufferSize, param,
                                           localCommunicationId, subId);
    event.Close();
    ThrowIfSessionClosed(result);
    return result;
}

// 0x00468E98 (name is ours)
nn::Result StartScan(void* buffer, size_t bufferSize, u8 subId, u32 localCommunicationId)
{
    return StartScan(buffer, bufferSize, subId, localCommunicationId, 0, 0);
}

// 0x00469080 | fefates:bytes-fuzzy [tier B]
nn::Result Attach(EndpointDescriptor* endpoint, u16 nodeId, u8 dataChannel, size_t receiveBufferSize)
{
    nn::os::CriticalSection::ScopedLock lock(s_CriticalSection);
    if (!s_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    if (receiveBufferSize < RECEIVE_BUFFER_SIZE_MIN || dataChannel == 0) {
        return nn::Result(RESULT_OUT_OF_RANGE);
    }
    if (endpoint->id == 0) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    s32 i;
    for (i = 0; i < ENDPOINT_MAX; i++) {
        if (s_Endpoints[i].id == endpoint->id && !s_Endpoints[i].isAttached) {
            break;
        }
    }
    if (i >= ENDPOINT_MAX) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }

    nn::Handle event;
    nn::Result result = GetUds().Bind(&event, *endpoint, receiveBufferSize, dataChannel, nodeId);
    if (result.IsSuccess()) {
        s_Endpoints[i].event = event;
        s_Endpoints[i].isAttached = true;
    } else {
        ThrowIfSessionClosed(result);
    }
    return result;
}

// 0x004691DC | tier C
nn::Result SendTo(const EndpointDescriptor& endpoint, const void* data, size_t size, u16 destination,
                  u8 dataChannel, u8 option)
{
    if (!s_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    if (size > SEND_SIZE_MAX) {
        return nn::Result(RESULT_TOO_LARGE);
    }
    if (dataChannel == 0) {
        return nn::Result(RESULT_OUT_OF_RANGE);
    }
    if (size == 0) {
        return nn::Result();
    }
    if ((reinterpret_cast<uptr>(data) & 3) || data == NULL) {
        return nn::Result(RESULT_MISALIGNED_ADDRESS);
    }
    return ConvertSessionClosed(GetUds().SendTo(endpoint, destination, dataChannel, static_cast<const bit32*>(data),
                                                (size + 3) / sizeof(bit32), size, option));
}

// 0x00468660 | tier C
nn::Result DisconnectNetwork()
{
    nn::os::CriticalSection::ScopedLock lock(s_CriticalSection);
    if (!s_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    nn::Result result = GetUds().DisconnectNetwork();
    ThrowIfSessionClosed(result);
    return result;
}

// 0x004686D0 | fefates:bytes-fuzzy [tier B]
nn::Result GetNodeInformation(NodeInformation* node, u16 nodeId)
{
    nn::os::CriticalSection::ScopedLock lock(s_CriticalSection);
    if (!s_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    const NodeInformation empty = {};
    *node = empty;

    NodeInformationRaw raw;
    nn::Result result;
    {
        nn::os::CriticalSection::ScopedLock innerLock(s_CriticalSection);
        result = GetUds().GetNodeInformation(&raw, nodeId);
        ThrowIfSessionClosed(result);
    }
    if (result.IsFailure()) {
        return result;
    }
    node->userName = raw.userName;
    node->nodeId = raw.nodeId;
    ScrambleLocalFriendCode(&node->scrambledLocalFriendCode, raw.localFriendCodeSeed, raw.nodeId);
    return result;
}

// 0x00468940 (name is ours)
nn::Result GetConnectionStatus(ConnectionStatus* status)
{
    if (!s_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return ConvertSessionClosed(GetUds().GetConnectionStatus(status));
}

// 0x00468B3C (name is ours)
nn::Result SetApplicationData(const void* data, size_t size)
{
    if (!s_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    if (size > APPLICATION_DATA_SIZE_MAX) {
        return nn::Result(RESULT_TOO_LARGE);
    }
    return ConvertSessionClosed(GetUds().SetApplicationData(data, size));
}

// 0x00468B9C (name is ours)
nn::Result GetApplicationData(u8* buffer, size_t* actualSize, size_t size)
{
    if (!s_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return ConvertSessionClosed(GetUds().GetApplicationData(buffer, actualSize, size));
}

// 0x00468B10 | fefates:bytes [tier B]
u32 CreateLocalCommunicationId(u32 uniqueId, bool isDemo)
{
    u32 flags = (*reinterpret_cast<const u8*>(CONFIG_MEMORY_ENVINFO) & 1) ? 0x10 : 0x90;
    if (isDemo) {
        flags |= 1;
    }
    return flags | ((uniqueId & ~0xF00000) << 8);
}

} // namespace CTR
} // namespace uds
} // namespace nn
