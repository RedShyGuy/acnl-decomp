#include "nw/snd/internal/snd_WaveSoundFile_InfoBlockBody.h"

namespace nw {
namespace snd {
namespace internal {
// 0x0013D1BC | nintendogs:callgraph [tier A]
const nw::snd::internal::BinaryTable<nw::snd::internal::WaveId>* nw::snd::internal::WaveSoundFile::InfoBlockBody::GetWaveIdTable() const
{
    return static_cast<const BinaryTable<WaveId>*>(GetReferedPointer(this, waveIdTable));
}

// 0x0013D1C8 | nintendogs:bytes [tier A]
const nw::snd::internal::WaveSoundFile::WaveSoundData* nw::snd::internal::WaveSoundFile::InfoBlockBody::GetWaveSoundData(u32 index) const
{
    const BinaryReferenceTable* pTable = static_cast<const BinaryReferenceTable*>(GetReferedPointer(this, waveSoundDataTable));
    return static_cast<const WaveSoundData*>(pTable->GetReferedItem(index, TYPE_WAVE_SOUND_DATA));
}

} // namespace internal
} // namespace snd
} // namespace nw
