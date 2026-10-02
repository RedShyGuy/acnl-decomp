#include "nw/snd/snd_SoundArchive.h"
#include "nw/snd/internal/snd_SoundArchiveFileReader.h"

namespace nw {
namespace snd {
namespace internal {
// 0x00139D54 | nintendogs:bytes [tier A]
void nw::snd::internal::SoundArchiveFileReader::GetSoundType(unsigned) const
{
}

// 0x0013D530 | nintendogs:bytes [tier A]
void nw::snd::internal::SoundArchiveFileReader::ReadWaveSoundInfo(unsigned, nw::snd::SoundArchive::WaveSoundInfo*) const
{
}

// 0x0013D59C | nintendogs:bytes [tier A]
void nw::snd::internal::SoundArchiveFileReader::ReadSoundGroupInfo(unsigned, nw::snd::SoundArchive::SoundGroupInfo*) const
{
}

// 0x0013D5E8 | nintendogs:bytes [tier A]
void nw::snd::internal::SoundArchiveFileReader::ReadSequenceSoundInfo(unsigned, nw::snd::SoundArchive::SequenceSoundInfo*) const
{
}

// 0x0013FF20 | nintendogs:bytes [tier A]
void nw::snd::internal::SoundArchiveFileReader::ReadBankInfo(unsigned, nw::snd::SoundArchive::BankInfo*) const
{
}

// 0x0013FF48 | nintendogs:bytes [tier A]
void nw::snd::internal::SoundArchiveFileReader::ReadSoundInfo(unsigned, nw::snd::SoundArchive::SoundInfo*) const
{
}

// 0x0013FFBC | nintendogs:bytes [tier A]
void nw::snd::internal::SoundArchiveFileReader::ReadWaveArchiveInfo(unsigned, nw::snd::SoundArchive::WaveArchiveInfo*) const
{
}

// 0x00141F98 | nintendogs:bytes [tier A]
void nw::snd::internal::SoundArchiveFileReader::ReadFileInfo(unsigned, nw::snd::SoundArchive::FileInfo*, int) const
{
}

// 0x00142014 | nintendogs:bytes [tier A]
void nw::snd::internal::SoundArchiveFileReader::GetWaveArchiveIdTable(unsigned) const
{
}

// 0x004C98F0 | fefates:bytes [tier B]
void nw::snd::internal::SoundArchiveFileReader::Initialize(const void*)
{
}

// 0x00740CB0 | nintendogs:bytes [tier A]
void nw::snd::internal::SoundArchiveFileReader::GetItemLabel(unsigned) const
{
}

// 0x00740D24 | fefates:bytes [tier B]
void nw::snd::internal::SoundArchiveFileReader::ReadGroupInfo(unsigned int, nw::snd::SoundArchive::GroupInfo*) const
{
}

// 0x00740DCC | nintendogs:bytes [tier A]
void nw::snd::internal::SoundArchiveFileReader::ReadPlayerInfo(unsigned, nw::snd::SoundArchive::PlayerInfo*) const
{
}

// 0x00740DFC | nintendogs:bytes [tier A]
void nw::snd::internal::SoundArchiveFileReader::ReadSound3DInfo(unsigned, nw::snd::SoundArchive::Sound3DInfo*) const
{
}

// 0x00740E48 | fefates:bytes [tier B]
void nw::snd::internal::SoundArchiveFileReader::ReadSoundUserParam(unsigned int, int, unsigned int&) const
{
}

// 0x00740E94 | fefates:bytes [tier B]
void nw::snd::internal::SoundArchiveFileReader::ReadStreamSoundInfo(unsigned int, nw::snd::SoundArchive::StreamSoundInfo*) const
{
}

// 0x00741124 | fefates:bytes [tier B]
void nw::snd::internal::SoundArchiveFileReader::ReadStreamSoundInfo2(unsigned int, nw::snd::SoundArchive::StreamSoundInfo2*) const
{
}

// 0x007411E0 | fefates:bytes [tier B]
void nw::snd::internal::SoundArchiveFileReader::ReadSoundArchivePlayerInfo(nw::snd::SoundArchive::SoundArchivePlayerInfo*) const
{
}

// 0x0074125C | fefates:bytes [tier B]
void nw::snd::internal::SoundArchiveFileReader::GetItemId(const char*) const
{
}

} // namespace internal
} // namespace snd
} // namespace nw
