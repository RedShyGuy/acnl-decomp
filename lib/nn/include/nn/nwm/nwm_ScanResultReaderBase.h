#pragma once

#include "decomp.h"

namespace nn {
namespace nwm {
// walks the BssDescriptions of a scan result buffer (member names are ours)
class ScanResultReaderBase
{
public:
    ScanResultReaderBase(); // ctor candidate(s) 0x003E22C0 (unverified)
    ScanResultReaderBase(const void* buffer, u8* current); // 0x003E22C0 | fefates:bytes [tier B]
    // 0x003E22F0 slot 0x00, empty (slot 0x04: deleting destructor 0x003E22EC)
    virtual ~ScanResultReaderBase() {}
    // moves to the next description; false at the end (name from the binary)
    bool ProcessBssPointer(); // 0x003E227C | mk7dlp:bytes [tier A]
    u32 GetCount() const; // 0x0072EC5C | tier C
    // the description ProcessBssPointer moves past (name is ours)
    u8* GetCurrentPointer() const { return m_pCurrent; }

protected:
    const void* m_pBuffer;  // 0x4
    u8* m_pCurrent;         // 0x8
};
ASSERT_SIZE(ScanResultReaderBase, 0xC);

} // namespace nwm
} // namespace nn
