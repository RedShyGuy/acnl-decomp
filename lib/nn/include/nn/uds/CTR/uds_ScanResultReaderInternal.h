#pragma once

#include "decomp.h"
#include "nn/nwm/nwm_ScanResultReaderBase.h"

namespace nn {
namespace uds {
namespace CTR {
// RTTI N2nn3uds3CTR24ScanResultReaderInternalE @ 0x008D0324
// vtable 0x009020FC (vptr 0x00902104), offset_to_top 0, 2 entries
// Slot 0x04 of the vtable is the deleting destructor (0x00468AFC).
class ScanResultReaderInternal : public ::nn::nwm::ScanResultReaderBase
{
public:
    ScanResultReaderInternal(const void* buffer, u8* current) : ScanResultReaderBase(buffer, current) {}
    // 0x00468B00 slot 0x00 (empty; inline here as in the callers)
    virtual ~ScanResultReaderInternal() {}
};

} // namespace CTR
} // namespace uds
} // namespace nn
