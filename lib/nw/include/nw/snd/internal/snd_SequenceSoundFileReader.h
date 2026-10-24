#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_SequenceSoundFile.h"

namespace nw {
namespace snd {
namespace internal {
// Reads a sequence sound file (the class and function names are from the reference symbols, the
// members are ours).
class SequenceSoundFileReader
{
public:
    SequenceSoundFileReader(const void* pSequenceSoundFile); // 0x004C9998 | fefates:bytes [tier B]

    // the offset in the data of a label
    bool GetOffsetByLabel(const char* pLabel, u32* pOffset) const; // 0x007404D8 (name is ours)

    const SequenceSoundFile::FileHeader* m_pHeader;          // 0x0
    const void* m_pDataBlockBody;                            // 0x4 the sequence data
    const SequenceSoundFile::LabelBlockBody* m_pLabelBlockBody; // 0x8
};
ASSERT_SIZE(SequenceSoundFileReader, 0xC);
} // namespace internal
} // namespace snd
} // namespace nw
