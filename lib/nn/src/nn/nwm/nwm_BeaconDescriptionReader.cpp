#include "nn/nwm/nwm_BeaconDescriptionReader.h"

namespace nn {
namespace nwm {
// 0x003E22F4 | fefates:bytes [tier B]
nn::nwm::BeaconDescriptionReader::BeaconDescriptionReader(const nn::nwm::BssDescription* bss) : BssDescriptionReaderBase(bss)
{
    if (bss != NULL) {
        m_Reader.SetData(reinterpret_cast<const u8*>(bss) + bss->ieOffset, bss->ieSize);
    }
}

// 0x003E238C slot 0x00 | fefates:bytes
// 0x003E2358 (deleting dtor)
nn::nwm::BeaconDescriptionReader::~BeaconDescriptionReader()
{
    m_Reader.SetData(NULL, 0);
}

// 0x0072ECEC slot 0x08 (name is ours)
u8 nn::nwm::BeaconDescriptionReader::GetIeNum() const
{
    return m_Reader.GetIeNum();
}

// 0x0072EEB0 slot 0x0C (name is ours)
nn::nwm::Ssid nn::nwm::BeaconDescriptionReader::GetSsid() const
{
    return m_Reader.GetSsid();
}

// 0x0072ED4C slot 0x10 (name is ours)
const u8* nn::nwm::BeaconDescriptionReader::GetVendorIe(const u8* oui) const
{
    return m_Reader.GetVendorIe(oui);
}

// 0x0072EDF8 slot 0x14
const u8* nn::nwm::BeaconDescriptionReader::GetVendorSpecificElement(const u8* oui, u8 type) const
{
    return m_Reader.GetVendorIe(oui, type);
}

} // namespace nwm
} // namespace nn
