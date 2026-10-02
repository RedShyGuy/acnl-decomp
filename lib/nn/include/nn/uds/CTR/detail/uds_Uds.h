#pragma once

// nn::uds::CTR::detail::Uds - the IPC commands of the service "nwm::UDS" (3dbrew "NWM_UDS").
// The class holds only the session; uds_Api copies its session handle into a temporary Uds for
// every command. Names of methods without an address comment note are from the binary; the
// others are ours, after the 3dbrew command names ("NWM Services").

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/uds/CTR/uds_Types.h"

namespace nn {
namespace uds {
namespace CTR {
class NetworkDescription;
namespace detail {

class Uds
{
public:
    explicit Uds(nn::Handle session) : m_Session(session) {}

    nn::Result InitializeWithVersion(nn::Handle* statusEvent, nn::Handle sharedMemory, size_t sharedMemorySize,
                                     const NodeInformationRaw* node, u16 version); // 0x00469B6C | fefates:bytes (symbols.json picks a conflicting mk7dlp name)
    nn::Result Finalize(); // 0x00469DE4 | tier C
    nn::Result EjectClient(u16 nodeId); // 0x00469870 (name after 3dbrew)
    nn::Result UpdateNetworkAttribute(u16 bits, bool set); // 0x00469C28 | fefates:bytes [tier A]
    nn::Result DestroyNetwork(); // 0x004698FC (name is ours)
    nn::Result DisconnectNetwork(); // 0x00469A00 (name is ours)
    nn::Result GetConnectionStatus(ConnectionStatus* status); // 0x00469B1C | fefates:bytes [tier B]
    nn::Result GetNodeInformation(NodeInformationRaw* node, u16 nodeId); // 0x00469A88 | fefates:bytes [tier A]
    nn::Result StartScan(nn::Handle event, u8* buffer, size_t size, const nn::nwm::CTR::ScanParamIpc& param,
                         u32 localCommunicationId, u8 subId); // 0x00469E0C | fefates:bytes [tier A]
    nn::Result SetApplicationData(const void* data, size_t size); // 0x00469AD8 (name is ours)
    nn::Result GetApplicationData(u8* buffer, size_t* actualSize, size_t size); // 0x00469A28 (name after 3dbrew)
    nn::Result Bind(nn::Handle* event, EndpointDescriptor endpoint, s32 receiveBufferSize, u8 dataChannel,
                    u16 nodeId); // 0x00469CDC | fefates:bytes [tier B]
    nn::Result Unbind(EndpointDescriptor endpoint, ReceiveReport* report); // 0x00469D9C | fefates:bytes [tier A]
    // buffer: sizeInWords words; maxSize: the most bytes to take
    nn::Result PullPacket(EndpointDescriptor endpoint, bit32* buffer, size_t sizeInWords, size_t maxSize,
                          size_t* receivedSize, u16* sourceNodeId); // 0x00469804 | fefates:bytes [tier A]
    nn::Result SendTo(EndpointDescriptor endpoint, u16 destination, u8 dataChannel, const bit32* data,
                      size_t sizeInWords, size_t size, u8 option); // 0x00469D28 | fefates:bytes [tier A]
    nn::Result GetChannel(u8* channel); // 0x004697D0 (name is ours)
    nn::Result CreateNetwork2(const NetworkDescription& network, const char* passphrase,
                              s32 passphraseSize); // 0x004698A8 | fefates:bytes [tier B]
    nn::Result ConnectNetwork2(const NetworkDescription& network, ConnectType type, const char* passphrase,
                               s32 passphraseSize); // 0x00469924 | fefates:bytes [tier B]
    // decrypts the node lists of a beacon (command DecryptBeaconData)
    nn::Result GetNodeInformationList2(NodeInformationList* list, const NetworkDescription& network,
                                       const NodeInformationElement& element1,
                                       const NodeInformationElement& element2); // 0x00469C68 | fefates:bytes [tier B]
    nn::Result SetProbeResponseParam(const u8* oui, bool enable); // 0x00469BE0 | fefates:bytes [tier B]
    nn::Result ScanOnConnection(u8* buffer, size_t size, const nn::nwm::CTR::ScanParamIpc& param, u32 localCommunicationId,
                                u8 subId); // 0x00469988 | fefates:bytes [tier A]

private:
    nn::Handle m_Session;
};
ASSERT_SIZE(Uds, 0x4);

} // namespace detail
} // namespace CTR
} // namespace uds
} // namespace nn
