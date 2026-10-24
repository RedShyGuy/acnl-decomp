#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BinaryTypes.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"
#include "nw/snd/internal/Util/snd_BitFlag.h"

// the items without functions of their own (names are ours)
struct nw::snd::internal::SoundArchiveFile::BankInfo
{
    u32 fileId;                         // 0x00
    BinaryReference waveArchiveIdTable; // 0x04 BinaryTable<u32>
    Util::BitFlag optionParams;         // 0x0C bit 0 the string id
};
struct nw::snd::internal::SoundArchiveFile::WaveSoundGroupInfo
{
    u32 m_Unknown0x0;                   // 0x0
    BinaryReference waveArchiveIdTable; // 0x4 BinaryTable<u32>
};
struct nw::snd::internal::SoundArchiveFile::SoundGroupInfo
{
    u32 startId;                 // 0x00
    u32 endId;                   // 0x04
    BinaryReference fileIdTable; // 0x08 BinaryTable<u32>
    BinaryReference detailInfo;  // 0x10 WaveSoundGroupInfo
    Util::BitFlag optionParams;  // 0x18 bit 0 the string id
};
struct nw::snd::internal::SoundArchiveFile::GroupInfo
{
    u32 fileId;                 // 0x0
    Util::BitFlag optionParams; // 0x4 bit 0 the string id
};

class nw::snd::internal::SoundArchiveFile::InfoBlockBody
{
public:
    const SoundInfo* GetSoundInfo(u32 soundId) const; // 0x00141D94 | nintendogs:bytes [tier A]
    const SoundGroupInfo* GetSoundGroupInfo(u32 soundGroupId) const; // 0x00141DD0 | nintendogs:bytes [tier A]
    const BankInfo* GetBankInfo(u32 bankId) const; // 0x00141D58 | nintendogs:bytes [tier A]
    const WaveArchiveInfo* GetWaveArchiveInfo(u32 waveArchiveId) const; // 0x00141E0C | nintendogs:bytes [tier A]
    const GroupInfo* GetGroupInfo(u32 groupId) const; // 0x0073FE84 | nintendogs:bytes [tier A]
    const PlayerInfo* GetPlayerInfo(u32 playerId) const; // 0x0073FEC0 | nintendogs:bytes [tier A]
    const FileInfo* GetFileInfo(u32 fileId) const; // 0x00143AB8 | nintendogs:bytes [tier A]
    const BinaryReferenceTable* GetSoundGroupInfoReferenceTable() const; // 0x00143AE4 | nintendogs:bytes [tier A]
    // the string id of an item (-1: none)
    u32 GetItemStringId(u32 id) const; // 0x0073FEFC | nintendogs:bytes [tier A]
    // the file of an item (a sound group: the one of its first sound; -1: none)
    u32 GetItemFileId(u32 id) const; // 0x0013FB24 (name after GetItemPrefetchFileId)
    // the prefetch file of a stream sound (-1: none)
    u32 GetItemPrefetchFileId(u32 id) const; // 0x0013FCB0 | nintendogs:bytes [tier B]
    const void* GetSoundArchivePlayerInfo() const; // 0x00740148 | nintendogs:bytes [tier C]
    // the tables (names are ours)
    const BinaryReferenceTable* GetFileInfoReferenceTable() const; // 0x0074013C (name is ours)
    const BinaryReferenceTable* GetGroupInfoReferenceTable() const; // 0x00740154 (name is ours)
    const BinaryReferenceTable* GetSoundInfoReferenceTable() const; // 0x00740160 (name is ours)
    const BinaryReferenceTable* GetPlayerInfoReferenceTable() const; // 0x0074016C (name is ours)
    const BinaryReferenceTable* GetWaveArchiveInfoReferenceTable() const; // 0x00740178 (name is ours)

    BinaryReference soundInfoTable;           // 0x00
    BinaryReference soundGroupInfoTable;      // 0x08
    BinaryReference bankInfoTable;            // 0x10
    BinaryReference waveArchiveInfoTable;     // 0x18
    BinaryReference groupInfoTable;           // 0x20
    BinaryReference playerInfoTable;          // 0x28
    BinaryReference fileInfoTable;            // 0x30
    BinaryReference soundArchivePlayerInfo;   // 0x38
};
