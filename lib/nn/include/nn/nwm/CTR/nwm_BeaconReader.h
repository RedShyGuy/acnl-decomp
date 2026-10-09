#pragma once

#include "decomp.h"
#include "nn/nwm/CTR/nwm_BssReader.h"

namespace nn {
namespace nwm {
namespace CTR {
// RTTI N2nn3nwm3CTR12BeaconReaderE @ 0x008CF7E0
// vtable 0x008FFE14 (vptr 0x008FFE1C), offset_to_top 0, 4 entries
// The elements of a beacon frame: they follow its fixed part of 12 bytes (member names are ours).
class BeaconReader : public ::nn::nwm::CTR::BssReader
{
public:
    BeaconReader() : m_pData(NULL), m_Size(0) {}
    // 0x003E23BC slot 0x00 (deleting dtor 0x003E23B8)
    virtual ~BeaconReader();
    virtual const u8* GetIePointer() const; // 0x0072ECCC slot 0x08 (name is ours)
    virtual u32 GetIeSize() const; // 0x0072ECDC slot 0x0C (name is ours)

    // (inline, name is ours)
    void SetData(const u8* data, u32 size)
    {
        m_pData = data;
        m_Size = size;
    }

private:
    const u8* m_pData; // 0x4
    u32 m_Size;        // 0x8
};
ASSERT_SIZE(BeaconReader, 0xC);
} // namespace CTR
} // namespace nwm
} // namespace nn
