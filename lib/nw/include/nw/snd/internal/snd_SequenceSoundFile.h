#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BinaryTypes.h"

namespace nw {
namespace snd {
namespace internal {
// The parts of a sequence sound file (BCSEQ; the class names are from the reference symbols, the
// members are ours): the sequence data and the labels of its start positions.
class SequenceSoundFile
{
public:
    static const u32 SIGNATURE_FILE = 0x51455343; // 'CSEQ'
    static const u32 VERSION_MIN = 0x01000000;
    static const u32 VERSION_MAX = 0x01010000;
    static const u16 TYPE_DATA_BLOCK = 0x5000;
    static const u16 TYPE_LABEL_BLOCK = 0x5001;

    class FileHeader;
    class LabelBlockBody;

    // a label: the position in the data block and the name (names are ours)
    struct LabelInfo
    {
        BinaryReference reference; // 0x0 the offset in the data
        u32 nameLength;            // 0x8
        char name[1];              // 0xC
    };
};
} // namespace internal
} // namespace snd
} // namespace nw
