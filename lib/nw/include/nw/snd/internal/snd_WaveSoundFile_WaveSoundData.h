#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_WaveSoundFile.h"

class nw::snd::internal::WaveSoundFile::WaveSoundData
{
public:
    const WaveSoundInfo* GetWaveSoundInfo() const; // 0x0073F480 | nintendogs:callseq [tier C]
    // the note of an index (NULL: none)
    const NoteInfo* GetNoteInfo(u32 index) const; // 0x0013D1FC | nintendogs:bytes [tier A]

    BinaryReference waveSoundInfo; // 0x00
    BinaryReference trackInfoTable; // 0x08 (not read by the library; name is ours)
    BinaryReference noteInfoTable; // 0x10 BinaryReferenceTable
};
