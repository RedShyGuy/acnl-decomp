#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"
#include "nw/snd/internal/snd_SoundArchiveFile_FileHeader.h"
#include "nw/snd/snd_SoundArchive.h"

namespace nw {
namespace snd {
namespace internal {
// Reads the information of the items of a sound archive file into the structs of SoundArchive
// (the class and function names are from the reference symbols, the members are ours). It keeps a
// copy of the file header and the bodies of the info and string blocks.
class SoundArchiveFileReader
{
public:
    // the versions of the archive the reader knows
    static const u32 VERSION_MIN = 0x02000000;
    static const u32 VERSION_MAX = 0x02030200;
    // the version with the new stream sound info / with the prefetch files and stream buffer times
    static const u32 VERSION_STREAM_SOUND_INFO = 0x02030100;
    static const u32 VERSION_PREFETCH = 0x02030200;

    SoundArchiveFileReader(); // 0x004C9984 | nintendogs:bytes [tier C]

    // the header copied if it is a sound archive of a known version
    void Initialize(const void* pSoundArchiveFile); // 0x004C98F0 | fefates:bytes [tier B]
    void Finalize(); // 0x004C9970 | nintendogs:bytes [tier C]
    // the blocks (with their block headers)
    void SetInfoBlock(const void* pInfoBlock); // 0x004C9958 | nintendogs:bytes [tier C]
    void SetStringBlock(const void* pStringBlock); // 0x004C9964 | nintendogs:bytes [tier C]

    // (inline)
    u32 GetInfoBlockSize() const { return m_Header.GetInfoBlockSize(); }
    u32 GetInfoBlockOffset() const { return m_Header.GetInfoBlockOffset(); }
    u32 GetStringBlockSize() const { return m_Header.GetStringBlockSize(); }
    u32 GetStringBlockOffset() const { return m_Header.GetStringBlockOffset(); }
    u32 GetFileBlockOffset() const { return m_Header.GetFileBlockOffset(); }

    SoundArchive::SoundType GetSoundType(u32 soundId) const; // 0x00139D54 | nintendogs:bytes [tier A]
    bool ReadWaveSoundInfo(u32 soundId, SoundArchive::WaveSoundInfo* pInfo) const; // 0x0013D530 | nintendogs:bytes [tier A]
    bool ReadSoundGroupInfo(u32 soundGroupId, SoundArchive::SoundGroupInfo* pInfo) const; // 0x0013D59C | nintendogs:bytes [tier A]
    bool ReadSequenceSoundInfo(u32 soundId, SoundArchive::SequenceSoundInfo* pInfo) const; // 0x0013D5E8 | nintendogs:bytes [tier A]
    bool ReadBankInfo(u32 bankId, SoundArchive::BankInfo* pInfo) const; // 0x0013FF20 | nintendogs:bytes [tier A]
    bool ReadSoundInfo(u32 soundId, SoundArchive::SoundInfo* pInfo) const; // 0x0013FF48 | nintendogs:bytes [tier A]
    bool ReadWaveArchiveInfo(u32 waveArchiveId, SoundArchive::WaveArchiveInfo* pInfo) const; // 0x0013FFBC | nintendogs:bytes [tier A]
    bool ReadFileInfo(u32 fileId, SoundArchive::FileInfo* pInfo, int index) const; // 0x00141F98 | nintendogs:bytes [tier A]
    // the wave archives of a wave sound (by its sound group), a sound group or a bank (NULL: none)
    const BinaryTable<u32>* GetWaveArchiveIdTable(u32 id) const; // 0x00142014 | nintendogs:bytes [tier A]
    u32 GetItemFileId(u32 id) const; // 0x0013D528 (name after SoundArchive::GetItemFileId)
    u32 GetItemPrefetchFileId(u32 id) const; // 0x0013D5E0 (name after the InfoBlockBody)
    const char* GetItemLabel(u32 id) const; // 0x00740CB0 | nintendogs:bytes [tier A]
    bool ReadGroupInfo(u32 groupId, SoundArchive::GroupInfo* pInfo) const; // 0x00740D24 | fefates:bytes [tier B]
    bool ReadPlayerInfo(u32 playerId, SoundArchive::PlayerInfo* pInfo) const; // 0x00740DCC | nintendogs:bytes [tier A]
    bool ReadSound3DInfo(u32 soundId, SoundArchive::Sound3DInfo* pInfo) const; // 0x00740DFC | nintendogs:bytes [tier A]
    bool ReadSoundUserParam(u32 soundId, int index, u32& value) const; // 0x00740E48 | fefates:bytes [tier B]
    bool ReadStreamSoundInfo(u32 soundId, SoundArchive::StreamSoundInfo* pInfo) const; // 0x00740E94 | fefates:bytes [tier B]
    bool ReadStreamSoundInfo2(u32 soundId, SoundArchive::StreamSoundInfo2* pInfo) const; // 0x00741124 | fefates:bytes [tier B]
    bool ReadSoundArchivePlayerInfo(SoundArchive::SoundArchivePlayerInfo* pInfo) const; // 0x007411E0 | fefates:bytes [tier B]
    u32 GetItemId(const char* pLabel) const; // 0x0074125C | fefates:bytes [tier B]
    // the numbers of items (names are ours)
    u32 GetFileCount() const; // 0x00740C9C (name is ours)
    u32 GetGroupCount() const; // 0x00740CF4 (name is ours)
    u32 GetSoundCount() const; // 0x00740D10 (name is ours)
    u32 GetPlayerCount() const; // 0x00740DB8 (name is ours)
    u32 GetWaveArchiveCount() const; // 0x00740E80 (name is ours)

    SoundArchiveFile::FileHeader m_Header;                      // 0x00
    const SoundArchiveFile::StringBlockBody* m_pStringBlockBody; // 0x38
    const SoundArchiveFile::InfoBlockBody* m_pInfoBlockBody;     // 0x3C
    u32 m_Unknown0x40;                                          // 0x40
};
ASSERT_SIZE(SoundArchiveFileReader, 0x44);
} // namespace internal
} // namespace snd
} // namespace nw
