#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/uds/CTR/uds_NetworkDescription.h"
#include "nn/uds/CTR/uds_Types.h"

namespace nn {
namespace uds {
namespace CTR {
// RTTI N2nn3uds3CTR24NetworkDescriptionReaderE @ 0x008D031C
// vtable 0x009020EC (vptr 0x009020F4), offset_to_top 0, 2 entries
// A network that a scan found (ScanResultReader::GetNextDescription). Slot 0x04 of the vtable is
// the deleting destructor (0x00468AF4). Member names are ours.
class NetworkDescriptionReader
{
public:
    explicit NetworkDescriptionReader(const nn::nwm::BssDescription* bss) : m_pBss(bss) {}
    virtual ~NetworkDescriptionReader(); // 0x00468AF8 slot 0x00

    nn::Result GetNetworkDescription(nn::uds::CTR::NetworkDescription* network); // 0x004689FC | fefates:bytes [tier B]
    // nodes: NODE_MAX entries
    nn::Result GetNodeInformationList(nn::uds::CTR::NodeInformation* nodes); // 0x00468A48 | fefates:bytes [tier B]
    // 0 to 3 after the signal strength of the beacon
    nn::Result GetLinkLevel(u8* level); // 0x0046898C (name is ours)

private:
    const nn::nwm::BssDescription* m_pBss;  // 0x4
};
ASSERT_SIZE(NetworkDescriptionReader, 0x8);

} // namespace CTR
} // namespace uds
} // namespace nn
