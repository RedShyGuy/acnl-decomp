#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_WaveSoundFile.h"

namespace nw {
namespace snd {
namespace internal {
// Reads the wave sounds of a wave sound file (the class and function names are from the reference
// symbols, the members are ours).
class WaveSoundFileReader
{
public:
    WaveSoundFileReader(const void* pWaveSoundFile); // 0x0013C310 | nintendogs:callgraph [tier A]

    // the note noteIndex of the wave sound index; false if its wave is beyond the wave table
    bool ReadNoteInfo(WaveSoundNoteInfo* pInfo, u32 index, u32 noteIndex) const; // 0x0013D44C | nintendogs:bytes [tier A]
    bool ReadWaveSoundInfo(WaveSoundInfo* pInfo, u32 index) const; // 0x007408C0 | nintendogs:callseq [tier A]

    const WaveSoundFile::FileHeader* m_pHeader;          // 0x0
    const WaveSoundFile::InfoBlockBody* m_pInfoBlockBody; // 0x4
};
ASSERT_SIZE(WaveSoundFileReader, 0x8);
} // namespace internal
} // namespace snd
} // namespace nw
