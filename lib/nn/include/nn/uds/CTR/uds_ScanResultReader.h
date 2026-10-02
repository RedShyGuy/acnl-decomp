#pragma once

#include "decomp.h"
#include "nn/uds/CTR/uds_NetworkDescriptionReader.h"

namespace nn {
namespace uds {
namespace CTR {
// RTTI N2nn3uds3CTR16ScanResultReaderE @ 0x008D0314
// vtable 0x009020DC (vptr 0x009020E4), offset_to_top 0, 2 entries
// Walks the networks of a scan buffer. Slot 0x04 of the vtable is the deleting destructor
// (0x004685D4). Member names are ours.
class ScanResultReader
{
public:
    ScanResultReader(); // ctor candidate(s) 0x0041EAA4, 0x00425994 (unverified)
    virtual ~ScanResultReader(); // 0x004685D8 slot 0x00

    // the next network; its reader has no description at the end
    NetworkDescriptionReader GetNextDescription(); // 0x00468578 | fefates:bytes [tier B]
    u32 GetCount() const; // 0x007370FC | fefates:bytes [tier B]

private:
    const void* m_pBuffer;  // 0x4
    u8* m_pCurrent;         // 0x8
};
ASSERT_SIZE(ScanResultReader, 0xC);

} // namespace CTR
} // namespace uds
} // namespace nn
