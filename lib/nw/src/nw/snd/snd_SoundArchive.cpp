#include "nw/snd/snd_SoundArchive.h"

namespace nw {
namespace snd {
// 0x004C0348 slot 0x00 | fefates:callgraph
nw::snd::SoundArchive::~SoundArchive()
{
}

// 0x004C0338 slot 0x04 | virtual slot, introduced by nw::snd::SoundArchive
void nw::snd::SoundArchive::vf_0x04()
{
}

// 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
void nw::snd::SoundArchive::vf_0x08()
{
}

// 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
void nw::snd::SoundArchive::vf_0x0C()
{
}

// 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
void nw::snd::SoundArchive::OpenStream(void*, int, unsigned int, unsigned int)
{
}

// 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
void nw::snd::SoundArchive::OpenExtStream(void*, int, const char*, void*, unsigned int, nw::snd::SoundArchive::FileStreamPriority) const
{
}

// 0x00139ACC | nintendogs:callseq-callee [tier A]
void nw::snd::SoundArchive::IsAvailable() const
{
}

// 0x00139D4C | nintendogs:callgraph [tier A]
void nw::snd::SoundArchive::GetSoundType(unsigned) const
{
}

// 0x0013CF84 | nintendogs:callseq-callee [tier A]
void nw::snd::SoundArchive::ReadBankInfo(unsigned, nw::snd::SoundArchive::BankInfo*) const
{
}

// 0x0013CFF0 | nintendogs:callseq-callee [tier A]
void nw::snd::SoundArchive::ReadSoundInfo(unsigned, nw::snd::SoundArchive::SoundInfo*) const
{
}

// 0x0013D048 | nintendogs:callgraph [tier A]
void nw::snd::SoundArchive::ReadSequenceSoundInfo(unsigned, nw::snd::SoundArchive::SequenceSoundInfo*) const
{
}

// 0x0013D0A0 | nintendogs:callgraph [tier A]
void nw::snd::SoundArchive::detail_ReadWaveSoundInfo(unsigned, nw::snd::SoundArchive::WaveSoundInfo*) const
{
}

// 0x0013D0F8 | nintendogs:callgraph [tier A]
void nw::snd::SoundArchive::detail_ReadSoundGroupInfo(unsigned, nw::snd::SoundArchive::SoundGroupInfo*) const
{
}

// 0x0013D520 | nintendogs:callgraph [tier A]
void nw::snd::SoundArchive::GetItemFileId(unsigned) const
{
}

// 0x0013FA10 | nintendogs:callgraph [tier A]
void nw::snd::SoundArchive::ReadWaveArchiveInfo(unsigned, nw::snd::SoundArchive::WaveArchiveInfo*) const
{
}

// 0x0013FA68 | nintendogs:callgraph [tier A]
void nw::snd::SoundArchive::detail_ReadFileInfo(unsigned, nw::snd::SoundArchive::FileInfo*) const
{
}

// 0x001410F4 | fefates:bytes [tier B]
void nw::snd::SoundArchive::detail_OpenFileStream(unsigned int, void*, int, void*, unsigned int, const char*, nw::snd::SoundArchive::FileStreamPriority)
{
}

// 0x0014200C | nintendogs:callgraph [tier A]
void nw::snd::SoundArchive::detail_GetWaveArchiveIdTable(unsigned) const
{
}

// 0x00143980 | nintendogs:bytes-fuzzy [tier A]
void nw::snd::SoundArchive::OpenExtStreamImpl(void*, int, const char*, unsigned, unsigned, nw::snd::SoundArchive::FileStreamPriority) const
{
}

// 0x004C01B4 | fefates:bytes [tier B]
void nw::snd::SoundArchive::Initialize(nw::snd::internal::SoundArchiveFileReader*)
{
}

// 0x004C022C | nintendogs:bytes-fuzzy [tier A]
void nw::snd::SoundArchive::SetExternalFileRoot(const char*)
{
}

// 0x004C02EC | fefates:bytes [tier B]
void nw::snd::SoundArchive::Finalize()
{
}

// 0x004C0304 | fefates:bytes [tier B]
nw::snd::SoundArchive::SoundArchive()
{
}

// 0x0073EAF4 | fefates:bytes [tier B]
void nw::snd::SoundArchive::GetItemLabel(unsigned int) const
{
}

// 0x0073EB94 | fefates:bytes [tier B]
void nw::snd::SoundArchive::ReadPlayerInfo(unsigned int, nw::snd::SoundArchive::PlayerInfo*) const
{
}

// 0x0073EBEC | fefates:bytes [tier B]
void nw::snd::SoundArchive::ReadSound3DInfo(unsigned int, nw::snd::SoundArchive::Sound3DInfo*) const
{
}

// 0x0073EC44 | fefates:bytes [tier B]
void nw::snd::SoundArchive::ReadSoundUserParam(unsigned int, int, unsigned int&) const
{
}

// 0x0073ECA4 | fefates:bytes [tier B]
void nw::snd::SoundArchive::ReadStreamSoundInfo(unsigned int, nw::snd::SoundArchive::StreamSoundInfo*) const
{
}

// 0x0073ECFC | nintendogs:callgraph [tier A]
void nw::snd::SoundArchive::ReadSoundArchivePlayerInfo(nw::snd::SoundArchive::SoundArchivePlayerInfo*) const
{
}

// 0x0073ED4C | fefates:bytes [tier B]
void nw::snd::SoundArchive::detail_ReadStreamSoundInfo2(unsigned int, nw::snd::SoundArchive::StreamSoundInfo2*) const
{
}

// 0x0073EDAC | fefates:bytes [tier B]
void nw::snd::SoundArchive::GetItemId(const char*) const
{
}

// 0x00740C94 | nintendogs:callgraph [tier A]
void nw::snd::SoundArchive::detail_GetFileCount() const
{
}

// 0x00740E78 | nintendogs:callseq-callee [tier A]
void nw::snd::SoundArchive::GetWaveArchiveCount() const
{
}

} // namespace snd
} // namespace nw
