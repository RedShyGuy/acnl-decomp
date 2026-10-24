#include "nw/snd/internal/snd_WaveSoundFile_WaveSoundData.h"

namespace nw {
namespace snd {
namespace internal {
// 0x0013D1FC | nintendogs:bytes [tier A]
const nw::snd::internal::WaveSoundFile::NoteInfo* nw::snd::internal::WaveSoundFile::WaveSoundData::GetNoteInfo(u32 index) const
{
    const BinaryReferenceTable* pTable = static_cast<const BinaryReferenceTable*>(GetReferedPointer(this, noteInfoTable));
    return static_cast<const NoteInfo*>(pTable->GetReferedItem(index, TYPE_NOTE_INFO));
}

// 0x0073F480 | nintendogs:callseq [tier C]
const nw::snd::internal::WaveSoundFile::WaveSoundInfo* nw::snd::internal::WaveSoundFile::WaveSoundData::GetWaveSoundInfo() const
{
    return static_cast<const WaveSoundInfo*>(GetReferedPointer(this, waveSoundInfo));
}

} // namespace internal
} // namespace snd
} // namespace nw
