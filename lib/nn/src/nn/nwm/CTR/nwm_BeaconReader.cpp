#include "nn/nwm/CTR/nwm_BeaconReader.h"

namespace nn {
namespace nwm {
namespace CTR {
namespace {
// the fixed part of a beacon frame (timestamp, interval, capabilities)
const u32 BEACON_FIXED_SIZE = 12;
} // namespace

// 0x003E23BC slot 0x00
// 0x003E23B8 (deleting dtor)
nn::nwm::CTR::BeaconReader::~BeaconReader()
{
}

// 0x0072ECCC slot 0x08 (name is ours)
const u8* nn::nwm::CTR::BeaconReader::GetIePointer() const
{
    if (m_pData == NULL) {
        return NULL;
    }
    return m_pData + BEACON_FIXED_SIZE;
}

// 0x0072ECDC slot 0x0C (name is ours)
u32 nn::nwm::CTR::BeaconReader::GetIeSize() const
{
    if (m_Size > BEACON_FIXED_SIZE) {
        return m_Size - BEACON_FIXED_SIZE;
    }
    return m_Size;
}

} // namespace CTR
} // namespace nwm
} // namespace nn
