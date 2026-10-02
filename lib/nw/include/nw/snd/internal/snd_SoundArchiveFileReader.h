#pragma once

#include "decomp.h"
#include "nw/snd/snd_SoundArchive.h"

namespace nw {
namespace snd {
namespace internal {
class SoundArchiveFileReader
{
public:
    void GetSoundType(unsigned) const; // 0x00139D54 | nintendogs:bytes [tier A]
    void ReadWaveSoundInfo(unsigned, nw::snd::SoundArchive::WaveSoundInfo*) const; // 0x0013D530 | nintendogs:bytes [tier A]
    void ReadSoundGroupInfo(unsigned, nw::snd::SoundArchive::SoundGroupInfo*) const; // 0x0013D59C | nintendogs:bytes [tier A]
    void ReadSequenceSoundInfo(unsigned, nw::snd::SoundArchive::SequenceSoundInfo*) const; // 0x0013D5E8 | nintendogs:bytes [tier A]
    void ReadBankInfo(unsigned, nw::snd::SoundArchive::BankInfo*) const; // 0x0013FF20 | nintendogs:bytes [tier A]
    void ReadSoundInfo(unsigned, nw::snd::SoundArchive::SoundInfo*) const; // 0x0013FF48 | nintendogs:bytes [tier A]
    void ReadWaveArchiveInfo(unsigned, nw::snd::SoundArchive::WaveArchiveInfo*) const; // 0x0013FFBC | nintendogs:bytes [tier A]
    void ReadFileInfo(unsigned, nw::snd::SoundArchive::FileInfo*, int) const; // 0x00141F98 | nintendogs:bytes [tier A]
    void GetWaveArchiveIdTable(unsigned) const; // 0x00142014 | nintendogs:bytes [tier A]
    void Initialize(const void*); // 0x004C98F0 | fefates:bytes [tier B]
    void GetItemLabel(unsigned) const; // 0x00740CB0 | nintendogs:bytes [tier A]
    void ReadGroupInfo(unsigned int, nw::snd::SoundArchive::GroupInfo*) const; // 0x00740D24 | fefates:bytes [tier B]
    void ReadPlayerInfo(unsigned, nw::snd::SoundArchive::PlayerInfo*) const; // 0x00740DCC | nintendogs:bytes [tier A]
    void ReadSound3DInfo(unsigned, nw::snd::SoundArchive::Sound3DInfo*) const; // 0x00740DFC | nintendogs:bytes [tier A]
    void ReadSoundUserParam(unsigned int, int, unsigned int&) const; // 0x00740E48 | fefates:bytes [tier B]
    void ReadStreamSoundInfo(unsigned int, nw::snd::SoundArchive::StreamSoundInfo*) const; // 0x00740E94 | fefates:bytes [tier B]
    void ReadStreamSoundInfo2(unsigned int, nw::snd::SoundArchive::StreamSoundInfo2*) const; // 0x00741124 | fefates:bytes [tier B]
    void ReadSoundArchivePlayerInfo(nw::snd::SoundArchive::SoundArchivePlayerInfo*) const; // 0x007411E0 | fefates:bytes [tier B]
    void GetItemId(const char*) const; // 0x0074125C | fefates:bytes [tier B]
};
} // namespace internal
} // namespace snd
} // namespace nw
