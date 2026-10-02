#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/nwm/nwm_BeaconDescriptionReader.h"
#include "nn/uds/CTR/uds_NetworkDescription.h"
#include "nn/uds/CTR/uds_Types.h"

namespace nn {
namespace uds {
namespace CTR {
// RTTI N2nn3uds3CTR32NetworkDescriptionReaderInternalE @ 0x008D0330
// vtable 0x0090210C (vptr 0x00902114), offset_to_top 0, 6 entries
// Reads the elements of Nintendo's vendor tag in a beacon. Slot 0x04 of the vtable is the
// deleting destructor (0x00468E88).
class NetworkDescriptionReaderInternal : public ::nn::nwm::BeaconDescriptionReader
{
public:
    explicit NetworkDescriptionReaderInternal(const nn::nwm::BssDescription* bss) : BeaconDescriptionReader(bss) {}
    // 0x003E2388 slot 0x00 (only the base destructor; inline here as in the callers)
    virtual ~NetworkDescriptionReaderInternal() {}

    nn::Result GetNetworkDescription(nn::uds::CTR::NetworkDescription* network); // 0x00468BF4 | fefates:bytes [tier B]
    nn::Result GetNodeInformationList(nn::uds::CTR::detail::NodeInformationRaw* nodes); // 0x00468D14 | fefates:bytes [tier B]

private:
    // the data of a vendor tag of Nintendo, NULL without one
    const u8* GetElementData(u8 type);
};

} // namespace CTR
} // namespace uds
} // namespace nn
