#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BinaryTypes.h"
#include "nw/ut/ut_Api.h"

namespace nw {
namespace snd {
namespace internal {
// The parts of a wave file (BCWAV, 3dbrew "BCWAV"; the class names are from the reference symbols,
// the members are ours).
class WaveFile
{
public:
    static const u32 SIGNATURE_FILE = 0x56415743; // 'CWAV'
    static const u32 SIGNATURE_INFO_BLOCK = 0x4F464E49; // 'INFO'
    static const u32 SIGNATURE_DATA_BLOCK = 0x41544144; // 'DATA'
    static const u16 TYPE_INFO_BLOCK = 0x7000;
    static const u16 TYPE_DATA_BLOCK = 0x7001;
    // the version from which the info has the original loop start
    static const u32 VERSION_ORIGINAL_LOOP_START = 0x02010100;

    class FileHeader;
    class InfoBlockBody;
    class ChannelInfo;

    // the encoding of the samples
    enum Encoding : u8
    {
        ENCODING_PCM8 = 0,
        ENCODING_PCM16 = 1,
        ENCODING_DSP_ADPCM = 2,
    };
};
} // namespace internal
} // namespace snd
} // namespace nw
