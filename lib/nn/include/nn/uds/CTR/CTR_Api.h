#pragma once

// nn::uds::CTR - local wireless communication. Parameter names and the names marked "ours" are
// ours; the others are from the binary.

#include "decomp.h"
#include "nn/Result.h"
#include "nn/cfg/CTR/cfg_Types.h"
#include "nn/os/os_Event.h"
#include "nn/uds/CTR/uds_NetworkDescription.h"
#include "nn/uds/CTR/uds_Types.h"

namespace nn {
namespace uds {
namespace CTR {

const size_t PASSPHRASE_SIZE_MIN = 8;
const size_t PASSPHRASE_SIZE_MAX = 255;
const u8 NODE_ID_HOST = 1;
const size_t SEND_SIZE_MAX = 0x5C6;
const size_t RECEIVE_BUFFER_SIZE_MIN = 0x646;

// statusEvent is signalled when the connection status changes; buffer (bufferSize bytes) is
// handed to the service; userName NULL: the one of the system settings
nn::Result Initialize(nn::os::Event* statusEvent, void* buffer, size_t bufferSize,
                      const nn::cfg::CTR::UserName* userName); // 0x00467C70 | tier C
void Finalize(); // 0x00469F3C (name is ours)
nn::Result GetChannel(u8* channel); // 0x00467BF8 (name is ours)
nn::Result EjectClient(u16 nodeId); // 0x00467D38 | tier C
// option 1: return at once when nothing was received
nn::Result ReceiveFrom(const EndpointDescriptor& endpoint, void* buffer, size_t* receivedSize, u16* sourceNodeId,
                       size_t bufferSize, u8 option); // 0x00467DC0 | fefates:bytes-fuzzy [tier B]
nn::Result CreateNetwork(u8 subId, u8 nodeCountMax, u32 localCommunicationId, const char* passphrase,
                         size_t passphraseSize, bool probeResponse, u8 channel, const void* applicationData,
                         size_t applicationDataSize); // 0x00467F7C | fefates:bytes-fuzzy [tier B]
nn::Result CreateNetwork(u8 subId, u8 nodeCountMax, u32 localCommunicationId, const char* passphrase,
                         size_t passphraseSize, u8 channel, const void* applicationData,
                         size_t applicationDataSize); // 0x004680F4 | fefates:bytes [tier B]
nn::Result AllowToConnect(); // 0x00468120 | tier C
nn::Result DisallowToConnect(bool spectatorsToo); // 0x004685DC | tier C
nn::Result ConnectNetwork(const NetworkDescription& network, ConnectType type, const char* passphrase,
                          size_t passphraseSize); // 0x00468198 | fefates:bytes-fuzzy [tier B]
nn::Result CreateEndpoint(EndpointDescriptor* endpoint); // 0x0046828C | tier C
nn::Result DestroyEndpoint(EndpointDescriptor* endpoint); // 0x004683E0 | tier C
nn::Result Attach(EndpointDescriptor* endpoint, u16 nodeId, u8 dataChannel,
                  size_t receiveBufferSize); // 0x00469080 | fefates:bytes-fuzzy [tier B]
nn::Result SendTo(const EndpointDescriptor& endpoint, const void* data, size_t size, u16 destination,
                  u8 dataChannel, u8 option); // 0x004691DC | tier C
nn::Result DestroyNetwork(); // 0x00468370 | tier C
nn::Result DisconnectNetwork(); // 0x00468660 | tier C
// channel 0: channels 1, 6 and 11; scanTime 0: a default
nn::Result ScanOnConnection(void* buffer, size_t bufferSize, u8 subId, u32 localCommunicationId, u8 channel,
                            bool passive, u16 scanTime); // 0x00468414 | fefates:bytes-fuzzy [tier B]
nn::Result ScanOnConnection(void* buffer, size_t bufferSize, u8 subId, u32 localCommunicationId, u8 channel,
                            u16 scanTime); // 0x00468554 | fefates:bytes [tier B]
nn::Result StartScan(void* buffer, size_t bufferSize, u8 subId, u32 localCommunicationId, u8 channel,
                     u16 scanTime); // 0x00468EB8 (name is ours)
nn::Result StartScan(void* buffer, size_t bufferSize, u8 subId,
                     u32 localCommunicationId); // 0x00468E98 (name is ours)
nn::Result GetNodeInformation(NodeInformation* node, u16 nodeId); // 0x004686D0 | fefates:bytes-fuzzy [tier B]
nn::Result GetConnectionStatus(ConnectionStatus* status); // 0x00468940 (name is ours)
nn::Result SetApplicationData(const void* data, size_t size); // 0x00468B3C (name is ours)
nn::Result GetApplicationData(u8* buffer, size_t* actualSize, size_t size); // 0x00468B9C (name is ours)
// uniqueId: the program's unique id; the result also says retail / development unit and demo
u32 CreateLocalCommunicationId(u32 uniqueId, bool isDemo); // 0x00468B10 | fefates:bytes [tier B]

} // namespace CTR
} // namespace uds
} // namespace nn
