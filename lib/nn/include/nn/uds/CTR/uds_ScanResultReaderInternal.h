#pragma once

#include "decomp.h"
#include "nn/nwm/nwm_ScanResultReaderBase.h"

namespace nn {
namespace uds {
namespace CTR {
// RTTI N2nn3uds3CTR24ScanResultReaderInternalE @ 0x008D0324
// vtable 0x009020FC (vptr 0x00902104), offset_to_top 0, 2 entries
class ScanResultReaderInternal : public ::nn::nwm::ScanResultReaderBase
{
public:
    ScanResultReaderInternal(const void* buffer, u8* current) : ScanResultReaderBase(buffer, current) {}
    virtual ~ScanResultReaderInternal(); // 0x00468B00 slot 0x00
};

} // namespace CTR
} // namespace uds
} // namespace nn
