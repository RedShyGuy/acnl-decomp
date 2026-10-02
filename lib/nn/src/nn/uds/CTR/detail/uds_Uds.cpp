#include "nn/uds/CTR/detail/uds_Uds.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"
#include "nn/uds/CTR/uds_NetworkDescription.h"

#include <string.h>

namespace nn {
namespace uds {
namespace CTR {
namespace detail {
namespace {

// command headers (id << 16 | normal parameters << 6 | translate parameters), 3dbrew "NWM_UDS"
const bit32 COMMAND_FINALIZE = 0x00030000;
const bit32 COMMAND_EJECT_CLIENT = 0x00050040;
const bit32 COMMAND_UPDATE_NETWORK_ATTRIBUTE = 0x00070080;
const bit32 COMMAND_DESTROY_NETWORK = 0x00080000;
const bit32 COMMAND_DISCONNECT_NETWORK = 0x000A0000;
const bit32 COMMAND_GET_CONNECTION_STATUS = 0x000B0000;
const bit32 COMMAND_GET_NODE_INFORMATION = 0x000D0040;
const bit32 COMMAND_START_SCAN = 0x000F0404;
const bit32 COMMAND_SET_APPLICATION_DATA = 0x00100042;
const bit32 COMMAND_GET_APPLICATION_DATA = 0x00110040;
const bit32 COMMAND_BIND = 0x00120100;
const bit32 COMMAND_UNBIND = 0x00130040;
const bit32 COMMAND_PULL_PACKET = 0x001400C0;
const bit32 COMMAND_SEND_TO = 0x00170182;
const bit32 COMMAND_GET_CHANNEL = 0x001A0000;
const bit32 COMMAND_INITIALIZE_WITH_VERSION = 0x001B0302;
const bit32 COMMAND_CREATE_NETWORK = 0x001D0044;
const bit32 COMMAND_CONNECT_NETWORK = 0x001E0084;
const bit32 COMMAND_DECRYPT_BEACON_DATA = 0x001F0006;
const bit32 COMMAND_SET_PROBE_RESPONSE_PARAM = 0x00210080;
const bit32 COMMAND_SCAN_ON_CONNECTION = 0x00220402;

// translate descriptors
const bit32 IPC_COPY_HANDLE = 0;

inline bit32 StaticBufferDescriptor(size_t size, s32 index)
{
    return (size << 14) | (index << 10) | 2;
}

inline bit32 WriteBufferDescriptor(size_t size)
{
    return (size << 4) | 0xC;
}

// the reply's result, or the error of the request
inline nn::Result Send(nn::Handle session, bit32* command)
{
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsSuccess()) {
        result = nn::Result(command[1]);
    }
    return result;
}

} // namespace

// 0x00469B6C | fefates:bytes (symbols.json picks a conflicting mk7dlp name)
nn::Result nn::uds::CTR::detail::Uds::InitializeWithVersion(nn::Handle* statusEvent, nn::Handle sharedMemory,
                                                           size_t sharedMemorySize, const NodeInformationRaw* node,
                                                           u16 version)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_INITIALIZE_WITH_VERSION;
    command[1] = sharedMemorySize;
    *reinterpret_cast<NodeInformationRaw*>(&command[2]) = *node;
    *reinterpret_cast<u16*>(&command[12]) = version;
    command[14] = sharedMemory.GetPrintableBits();
    command[13] = IPC_COPY_HANDLE;
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    if (result.IsFailure()) {
        return result;
    }
    *statusEvent = nn::Handle(command[3]);
    return nn::Result(command[1]);
}

// 0x00469DE4 | tier C
nn::Result nn::uds::CTR::detail::Uds::Finalize()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_FINALIZE;
    return Send(m_Session, command);
}

// 0x00469870 (name after 3dbrew)
nn::Result nn::uds::CTR::detail::Uds::EjectClient(u16 nodeId)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_EJECT_CLIENT;
    *reinterpret_cast<u16*>(&command[1]) = nodeId;
    return Send(m_Session, command);
}

// 0x00469C28 | fefates:bytes [tier A]
nn::Result nn::uds::CTR::detail::Uds::UpdateNetworkAttribute(u16 bits, bool set)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_UPDATE_NETWORK_ATTRIBUTE;
    *reinterpret_cast<u16*>(&command[1]) = bits;
    *reinterpret_cast<bool*>(&command[2]) = set;
    return Send(m_Session, command);
}

// 0x004698FC (name is ours)
nn::Result nn::uds::CTR::detail::Uds::DestroyNetwork()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_DESTROY_NETWORK;
    return Send(m_Session, command);
}

// 0x00469A00 (name is ours)
nn::Result nn::uds::CTR::detail::Uds::DisconnectNetwork()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_DISCONNECT_NETWORK;
    return Send(m_Session, command);
}

// 0x00469B1C | fefates:bytes [tier B]
nn::Result nn::uds::CTR::detail::Uds::GetConnectionStatus(ConnectionStatus* status)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_CONNECTION_STATUS;
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    if (result.IsFailure()) {
        return result;
    }
    *status = *reinterpret_cast<ConnectionStatus*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x00469A88 | fefates:bytes [tier A]
nn::Result nn::uds::CTR::detail::Uds::GetNodeInformation(NodeInformationRaw* node, u16 nodeId)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_NODE_INFORMATION;
    *reinterpret_cast<u16*>(&command[1]) = nodeId;
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    if (result.IsFailure()) {
        return result;
    }
    *node = *reinterpret_cast<NodeInformationRaw*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x00469E0C | fefates:bytes [tier A]
nn::Result nn::uds::CTR::detail::Uds::StartScan(nn::Handle event, u8* buffer, size_t size,
                                               const nn::nwm::CTR::ScanParamIpc& param, u32 localCommunicationId, u8 subId)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_START_SCAN;
    command[1] = size;
    *reinterpret_cast<nn::nwm::CTR::ScanParamIpc*>(&command[2]) = param;
    command[15] = localCommunicationId;
    *reinterpret_cast<u8*>(&command[16]) = subId;
    command[18] = event.GetPrintableBits();
    command[17] = IPC_COPY_HANDLE;
    command[20] = reinterpret_cast<uptr>(buffer);
    command[19] = WriteBufferDescriptor(size);
    return Send(m_Session, command);
}

// 0x00469AD8 (name is ours)
nn::Result nn::uds::CTR::detail::Uds::SetApplicationData(const void* data, size_t size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_APPLICATION_DATA;
    command[1] = size;
    command[3] = reinterpret_cast<uptr>(data);
    command[2] = StaticBufferDescriptor(size, 4);
    return Send(m_Session, command);
}

// 0x00469A28 (name after 3dbrew)
nn::Result nn::uds::CTR::detail::Uds::GetApplicationData(u8* buffer, size_t* actualSize, size_t size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_APPLICATION_DATA;
    command[1] = size;
    bit32* staticBuffers = nn::os::detail::GetIpcStaticBuffers();
    bit32 saved0 = staticBuffers[0];
    bit32 saved1 = staticBuffers[1];
    staticBuffers[0] = StaticBufferDescriptor(size, 0);
    staticBuffers[1] = reinterpret_cast<uptr>(buffer);
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    staticBuffers[0] = saved0;
    staticBuffers[1] = saved1;
    if (result.IsFailure()) {
        return result;
    }
    *actualSize = command[2];
    return nn::Result(command[1]);
}

// 0x00469CDC | fefates:bytes [tier B]
nn::Result nn::uds::CTR::detail::Uds::Bind(nn::Handle* event, EndpointDescriptor endpoint, s32 receiveBufferSize,
                                          u8 dataChannel, u16 nodeId)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_BIND;
    command[1] = endpoint.id;
    command[2] = receiveBufferSize;
    *reinterpret_cast<u8*>(&command[3]) = dataChannel;
    *reinterpret_cast<u16*>(&command[4]) = nodeId;
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    if (result.IsFailure()) {
        return result;
    }
    *event = nn::Handle(command[3]);
    return nn::Result(command[1]);
}

// 0x00469D9C | fefates:bytes [tier A]
nn::Result nn::uds::CTR::detail::Uds::Unbind(EndpointDescriptor endpoint, ReceiveReport* report)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_UNBIND;
    command[1] = endpoint.id;
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    if (result.IsFailure()) {
        return result;
    }
    *report = *reinterpret_cast<ReceiveReport*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x00469804 | fefates:bytes [tier A]
nn::Result nn::uds::CTR::detail::Uds::PullPacket(EndpointDescriptor endpoint, bit32* buffer, size_t sizeInWords,
                                                size_t maxSize, size_t* receivedSize, u16* sourceNodeId)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_PULL_PACKET;
    command[1] = endpoint.id;
    command[2] = sizeInWords;
    command[3] = maxSize;
    bit32* staticBuffers = nn::os::detail::GetIpcStaticBuffers();
    bit32 saved0 = staticBuffers[0];
    bit32 saved1 = staticBuffers[1];
    staticBuffers[0] = StaticBufferDescriptor(sizeInWords * sizeof(bit32), 0);
    staticBuffers[1] = reinterpret_cast<uptr>(buffer);
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    staticBuffers[0] = saved0;
    staticBuffers[1] = saved1;
    if (result.IsFailure()) {
        return result;
    }
    *receivedSize = command[2];
    *sourceNodeId = *reinterpret_cast<u16*>(&command[3]);
    return nn::Result(command[1]);
}

// 0x00469D28 | fefates:bytes [tier A]
nn::Result nn::uds::CTR::detail::Uds::SendTo(EndpointDescriptor endpoint, u16 destination, u8 dataChannel,
                                            const bit32* data, size_t sizeInWords, size_t size, u8 option)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SEND_TO;
    command[1] = endpoint.id;
    *reinterpret_cast<u16*>(&command[2]) = destination;
    *reinterpret_cast<u8*>(&command[3]) = dataChannel;
    command[4] = sizeInWords;
    command[5] = size;
    *reinterpret_cast<u8*>(&command[6]) = option;
    command[7] = StaticBufferDescriptor(sizeInWords * sizeof(bit32), 5);
    command[8] = reinterpret_cast<uptr>(data);
    return Send(m_Session, command);
}

// 0x004697D0 (name is ours)
nn::Result nn::uds::CTR::detail::Uds::GetChannel(u8* channel)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_CHANNEL;
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    if (result.IsFailure()) {
        return result;
    }
    *channel = *reinterpret_cast<u8*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x004698A8 | fefates:bytes [tier B]
nn::Result nn::uds::CTR::detail::Uds::CreateNetwork2(const NetworkDescription& network, const char* passphrase,
                                                    s32 passphraseSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_CREATE_NETWORK;
    command[1] = passphraseSize;
    command[2] = StaticBufferDescriptor(sizeof(NetworkDescription), 1);
    command[3] = reinterpret_cast<uptr>(&network);
    command[4] = StaticBufferDescriptor(passphraseSize, 0);
    command[5] = reinterpret_cast<uptr>(passphrase);
    return Send(m_Session, command);
}

// 0x00469924 | fefates:bytes [tier B]
nn::Result nn::uds::CTR::detail::Uds::ConnectNetwork2(const NetworkDescription& network, ConnectType type,
                                                     const char* passphrase, s32 passphraseSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_CONNECT_NETWORK;
    *reinterpret_cast<u8*>(&command[1]) = type;
    command[2] = passphraseSize;
    command[3] = StaticBufferDescriptor(sizeof(NetworkDescription), 1);
    command[4] = reinterpret_cast<uptr>(&network);
    command[5] = StaticBufferDescriptor(passphraseSize, 0);
    command[6] = reinterpret_cast<uptr>(passphrase);
    return Send(m_Session, command);
}

// 0x00469C68 | fefates:bytes [tier B]
nn::Result nn::uds::CTR::detail::Uds::GetNodeInformationList2(NodeInformationList* list,
                                                             const NetworkDescription& network,
                                                             const NodeInformationElement& element1,
                                                             const NodeInformationElement& element2)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_DECRYPT_BEACON_DATA;
    command[1] = StaticBufferDescriptor(sizeof(NetworkDescription), 1);
    command[2] = reinterpret_cast<uptr>(&network);
    command[3] = StaticBufferDescriptor(sizeof(NodeInformationElement), 2);
    command[4] = reinterpret_cast<uptr>(&element1);
    command[5] = StaticBufferDescriptor(sizeof(NodeInformationElement), 3);
    command[6] = reinterpret_cast<uptr>(&element2);
    bit32* staticBuffers = nn::os::detail::GetIpcStaticBuffers();
    bit32 saved0 = staticBuffers[0];
    bit32 saved1 = staticBuffers[1];
    staticBuffers[0] = StaticBufferDescriptor(sizeof(NodeInformationList), 0);
    staticBuffers[1] = reinterpret_cast<uptr>(list);
    nn::Result result = Send(m_Session, command);
    staticBuffers[0] = saved0;
    staticBuffers[1] = saved1;
    return result;
}

// 0x00469BE0 | fefates:bytes [tier B]
nn::Result nn::uds::CTR::detail::Uds::SetProbeResponseParam(const u8* oui, bool enable)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_PROBE_RESPONSE_PARAM;
    memcpy(&command[1], oui, 3);
    *reinterpret_cast<bool*>(&command[2]) = enable;
    return Send(m_Session, command);
}

// 0x00469988 | fefates:bytes [tier A]
nn::Result nn::uds::CTR::detail::Uds::ScanOnConnection(u8* buffer, size_t size, const nn::nwm::CTR::ScanParamIpc& param,
                                                      u32 localCommunicationId, u8 subId)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SCAN_ON_CONNECTION;
    command[1] = size;
    *reinterpret_cast<nn::nwm::CTR::ScanParamIpc*>(&command[2]) = param;
    command[15] = localCommunicationId;
    *reinterpret_cast<u8*>(&command[16]) = subId;
    command[18] = reinterpret_cast<uptr>(buffer);
    command[17] = WriteBufferDescriptor(size);
    return Send(m_Session, command);
}

} // namespace detail
} // namespace CTR
} // namespace uds
} // namespace nn
