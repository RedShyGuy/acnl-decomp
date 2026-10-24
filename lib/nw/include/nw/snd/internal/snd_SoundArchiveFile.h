#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
// The parts of a sound archive file (BCSAR; the class names are from the reference symbols, the
// members are ours). The info block holds tables of the items by kind; the optional values of an
// item are kept in a BitFlag.
class SoundArchiveFile
{
public:
    // the type ids of the references
    enum ReferenceType : u16
    {
        TYPE_STRING_BLOCK = 0x2000,
        TYPE_INFO_BLOCK = 0x2001,
        TYPE_FILE_BLOCK = 0x2002,
        TYPE_STREAM_SOUND_INFO = 0x2201,
        TYPE_WAVE_SOUND_INFO = 0x2202,
        TYPE_SEQUENCE_SOUND_INFO = 0x2203,
        TYPE_WAVE_SOUND_GROUP_INFO = 0x2205,
        TYPE_INTERNAL_FILE_INFO = 0x220C,
        TYPE_EXTERNAL_FILE_INFO = 0x220D,
        TYPE_STREAM_TRACK_INFO = 0x220E,
        TYPE_STREAM_SOUND_EXTENSION = 0x2210,
        TYPE_TABLE_EMBEDDING_TABLE = 0x0101,
    };

    class InfoBlockBody;
    class WaveSoundInfo;
    class SequenceSoundInfo;
    class SoundInfo;
    class WaveArchiveInfo;
    class FileInfo;
    class FileHeader;
    class PlayerInfo;
    class StreamSoundInfo;
    class StringBlockBody;
    class StreamSoundInfoLegacy;
    struct BankInfo;
    struct WaveSoundGroupInfo;
    struct SoundGroupInfo;
    struct GroupInfo;
    struct StreamTrackInfo;
    struct SoundArchivePlayerInfo;
};
} // namespace internal
} // namespace snd
} // namespace nw
