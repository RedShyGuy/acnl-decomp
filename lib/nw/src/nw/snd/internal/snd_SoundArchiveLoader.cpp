#include "nw/snd/internal/snd_SoundArchiveLoader.h"

namespace nw {
namespace snd {
namespace internal {
// ctor candidate(s) 0x0013C2EC (unverified)
nw::snd::internal::SoundArchiveLoader::SoundArchiveLoader()
{
}

// 0x004C8CF4 slot 0x00 | fefates:callgraph
void nw::snd::internal::SoundArchiveLoader::vf_0x00()
{
}

// 0x004C8CDC slot 0x04 | virtual slot, introduced by nw::snd::internal::SoundArchiveLoader
void nw::snd::internal::SoundArchiveLoader::vf_0x04()
{
}

// 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
void nw::snd::internal::SoundArchiveLoader::vf_0x08()
{
}

// 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
void nw::snd::internal::SoundArchiveLoader::vf_0x0C()
{
}

// 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
void nw::snd::internal::SoundArchiveLoader::vf_0x10()
{
}

// 0x00132564 | nintendogs:callgraph [tier A]
void nw::snd::internal::SoundArchiveLoader::LoadData(unsigned, nw::snd::SoundMemoryAllocatable*, unsigned, unsigned)
{
}

// 0x001347B8 | nintendogs:callgraph [tier A]
void nw::snd::internal::SoundArchiveLoader::IsAvailable() const
{
}

// 0x001348A8 | nintendogs:bytes [tier B]
void nw::snd::internal::SoundArchiveLoader::IsWaveSoundDataLoaded(unsigned, unsigned) const
{
}

// 0x00134AA0 | nintendogs:bytes [tier B]
void nw::snd::internal::SoundArchiveLoader::IsSoundGroupDataLoaded(unsigned, unsigned) const
{
}

// 0x00137D94 | nintendogs:bytes [tier A]
void nw::snd::internal::SoundArchiveLoader::LoadSoundGroup(unsigned, nw::snd::SoundMemoryAllocatable*, unsigned, unsigned)
{
}

// 0x00137F44 | nintendogs:bytes [tier A]
void nw::snd::internal::SoundArchiveLoader::LoadSequenceSound(unsigned, nw::snd::SoundMemoryAllocatable*, unsigned, unsigned)
{
}

// 0x00138028 | nintendogs:callseq [tier A]
void nw::snd::internal::SoundArchiveLoader::PostProcessForLoadedGroupFile(const void*, nw::snd::SoundMemoryAllocatable*, unsigned)
{
}

// 0x0013BC2C | nintendogs:bytes [tier A]
void nw::snd::internal::SoundArchiveLoader::LoadWaveSound(unsigned, nw::snd::SoundMemoryAllocatable*, unsigned, unsigned, unsigned)
{
}

// 0x0013BDC0 | nintendogs:bytes [tier A]
void nw::snd::internal::SoundArchiveLoader::LoadWaveArchiveImpl(unsigned, unsigned, nw::snd::SoundMemoryAllocatable*, unsigned, unsigned)
{
}

// 0x0013BE80 | nintendogs:callseq [tier A]
void nw::snd::internal::SoundArchiveLoader::SetWaveArchiveTableInEmbeddedGroupImpl(unsigned, nw::snd::SoundMemoryAllocatable*)
{
}

// 0x0013BFB8 | nintendogs:callgraph [tier A]
void nw::snd::internal::SoundArchiveLoader::LoadBank(unsigned, nw::snd::SoundMemoryAllocatable*, unsigned, unsigned)
{
}

// 0x0013C1C4 | nintendogs:bytes [tier A]
void nw::snd::internal::SoundArchiveLoader::LoadImpl(unsigned, nw::snd::SoundMemoryAllocatable*, unsigned, bool)
{
}

// 0x0013F19C | nintendogs:callgraph [tier A]
void nw::snd::internal::SoundArchiveLoader::LoadIndividualWave(unsigned, unsigned, nw::snd::SoundMemoryAllocatable*, unsigned)
{
}

// 0x0014123C | nintendogs:callseq [tier A]
void nw::snd::internal::SoundArchiveLoader::LoadWaveArchiveTable(unsigned, nw::snd::SoundMemoryAllocatable*, unsigned)
{
}

// 0x001413C4 | nintendogs:callgraph [tier A]
void nw::snd::internal::SoundArchiveLoader::ReadFile(unsigned, void*, unsigned, int, unsigned)
{
}

// 0x004C8B2C | nintendogs:callgraph [tier A]
void nw::snd::internal::SoundArchiveLoader::SetSoundArchive(const nw::snd::SoundArchive*)
{
}

// 0x004C8B34 | nintendogs:callseq [tier A]
void nw::snd::internal::SoundArchiveLoader::detail_LoadWaveArchiveByBankFile(const void*, nw::snd::SoundMemoryAllocatable*)
{
}

// 0x004C8C54 | nintendogs:bytes [tier A]
void nw::snd::internal::SoundArchiveLoader::detail_LoadWaveArchiveByWaveSoundFile(const void*, int, nw::snd::SoundMemoryAllocatable*)
{
}

// 0x007406F8 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::SoundArchiveLoader::detail_GetFileAddressByItemId(unsigned) const
{
}

// 0x007407DC | nintendogs:bytes [tier A]
void nw::snd::internal::SoundArchiveLoader::GetFileAddressFromSoundArchive(unsigned) const
{
}

} // namespace internal
} // namespace snd
} // namespace nw
