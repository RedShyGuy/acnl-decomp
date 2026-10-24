#include "nw/snd/internal/snd_SoundArchiveFile_InfoBlockBody.h"
#include "nw/snd/internal/snd_SoundArchiveFile_SoundInfo.h"
#include "nw/snd/internal/snd_SoundArchiveFile_StreamSoundInfo.h"
#include "nw/snd/internal/snd_SoundArchiveFile_WaveArchiveInfo.h"
#include "nw/snd/internal/snd_SoundArchiveFile_PlayerInfo.h"
#include "nw/snd/snd_SoundArchive.h"

namespace nw {
namespace snd {
namespace internal {
namespace {
// (inline) the item of an id in a table of the block, if the id is of the kind (NULL: not)
inline const void* GetItem(const SoundArchiveFile::InfoBlockBody* pBody, const BinaryReference& table, u32 id, u32 itemType)
{
    if (SoundArchive::GetItemType(id) != itemType) {
        return nullptr;
    }
    return static_cast<const BinaryReferenceTable*>(GetReferedPointer(pBody, table))->GetReferedItem(SoundArchive::GetItemIndex(id));
}

// (inline) a value of the optional values (-1: not there)
inline u32 GetOptionValue(const Util::BitFlag& optionParams, u32 bit)
{
    u32 value;
    if (!optionParams.GetValue(&value, bit)) {
        return SoundArchive::INVALID_ID;
    }
    return value;
}
} // namespace

// 0x0013FB24 (name after GetItemPrefetchFileId)
u32 nw::snd::internal::SoundArchiveFile::InfoBlockBody::GetItemFileId(u32 id) const
{
    switch (SoundArchive::GetItemType(id)) {
    case SoundArchive::ITEM_TYPE_SOUND: {
        const SoundInfo* pInfo = static_cast<const SoundInfo*>(GetItem(this, soundInfoTable, id, SoundArchive::ITEM_TYPE_SOUND));
        if (pInfo != nullptr) {
            return pInfo->fileId;
        }
        break;
    }
    case SoundArchive::ITEM_TYPE_SOUND_GROUP: {
        // the file of the first sound
        const SoundGroupInfo* pGroup = static_cast<const SoundGroupInfo*>(GetItem(this, soundGroupInfoTable, id, SoundArchive::ITEM_TYPE_SOUND_GROUP));
        if (pGroup != nullptr) {
            const SoundInfo* pInfo = static_cast<const SoundInfo*>(GetItem(this, soundInfoTable, pGroup->startId, SoundArchive::ITEM_TYPE_SOUND));
            if (pInfo != nullptr) {
                return pInfo->fileId;
            }
        }
        break;
    }
    case SoundArchive::ITEM_TYPE_BANK: {
        const BankInfo* pInfo = static_cast<const BankInfo*>(GetItem(this, bankInfoTable, id, SoundArchive::ITEM_TYPE_BANK));
        if (pInfo != nullptr) {
            return pInfo->fileId;
        }
        break;
    }
    case SoundArchive::ITEM_TYPE_WAVE_ARCHIVE: {
        const WaveArchiveInfo* pInfo = static_cast<const WaveArchiveInfo*>(GetItem(this, waveArchiveInfoTable, id, SoundArchive::ITEM_TYPE_WAVE_ARCHIVE));
        if (pInfo != nullptr) {
            return pInfo->fileId;
        }
        break;
    }
    case SoundArchive::ITEM_TYPE_GROUP: {
        const GroupInfo* pInfo = static_cast<const GroupInfo*>(GetItem(this, groupInfoTable, id, SoundArchive::ITEM_TYPE_GROUP));
        if (pInfo != nullptr) {
            return pInfo->fileId;
        }
        break;
    }
    }
    return SoundArchive::INVALID_ID;
}

// 0x0013FCB0 | nintendogs:bytes [tier B]
u32 nw::snd::internal::SoundArchiveFile::InfoBlockBody::GetItemPrefetchFileId(u32 id) const
{
    if (SoundArchive::GetItemType(id) != SoundArchive::ITEM_TYPE_SOUND) {
        return SoundArchive::INVALID_ID;
    }
    const SoundInfo* pInfo = static_cast<const SoundInfo*>(GetItem(this, soundInfoTable, id, SoundArchive::ITEM_TYPE_SOUND));
    if (pInfo == nullptr || pInfo->detailInfo.typeId != TYPE_STREAM_SOUND_INFO) {
        return SoundArchive::INVALID_ID;
    }
    return pInfo->GetStreamSoundInfo()->prefetchFileId;
}

// 0x00141D58 | nintendogs:bytes [tier A]
const nw::snd::internal::SoundArchiveFile::BankInfo* nw::snd::internal::SoundArchiveFile::InfoBlockBody::GetBankInfo(u32 bankId) const
{
    return static_cast<const BankInfo*>(GetItem(this, bankInfoTable, bankId, SoundArchive::ITEM_TYPE_BANK));
}

// 0x00141D94 | nintendogs:bytes [tier A]
const nw::snd::internal::SoundArchiveFile::SoundInfo* nw::snd::internal::SoundArchiveFile::InfoBlockBody::GetSoundInfo(u32 soundId) const
{
    return static_cast<const SoundInfo*>(GetItem(this, soundInfoTable, soundId, SoundArchive::ITEM_TYPE_SOUND));
}

// 0x00141DD0 | nintendogs:bytes [tier A]
const nw::snd::internal::SoundArchiveFile::SoundGroupInfo* nw::snd::internal::SoundArchiveFile::InfoBlockBody::GetSoundGroupInfo(u32 soundGroupId) const
{
    return static_cast<const SoundGroupInfo*>(GetItem(this, soundGroupInfoTable, soundGroupId, SoundArchive::ITEM_TYPE_SOUND_GROUP));
}

// 0x00141E0C | nintendogs:bytes [tier A]
const nw::snd::internal::SoundArchiveFile::WaveArchiveInfo* nw::snd::internal::SoundArchiveFile::InfoBlockBody::GetWaveArchiveInfo(u32 waveArchiveId) const
{
    return static_cast<const WaveArchiveInfo*>(GetItem(this, waveArchiveInfoTable, waveArchiveId, SoundArchive::ITEM_TYPE_WAVE_ARCHIVE));
}

// 0x00143AB8 | nintendogs:bytes [tier A]
const nw::snd::internal::SoundArchiveFile::FileInfo* nw::snd::internal::SoundArchiveFile::InfoBlockBody::GetFileInfo(u32 fileId) const
{
    return static_cast<const FileInfo*>(
        static_cast<const BinaryReferenceTable*>(GetReferedPointer(this, fileInfoTable))->GetReferedItem(SoundArchive::GetItemIndex(fileId)));
}

// 0x00143AE4 | nintendogs:bytes [tier A]
const nw::snd::internal::BinaryReferenceTable* nw::snd::internal::SoundArchiveFile::InfoBlockBody::GetSoundGroupInfoReferenceTable() const
{
    return static_cast<const BinaryReferenceTable*>(GetReferedPointer(this, soundGroupInfoTable));
}

// 0x0073FE84 | nintendogs:bytes [tier A]
const nw::snd::internal::SoundArchiveFile::GroupInfo* nw::snd::internal::SoundArchiveFile::InfoBlockBody::GetGroupInfo(u32 groupId) const
{
    return static_cast<const GroupInfo*>(GetItem(this, groupInfoTable, groupId, SoundArchive::ITEM_TYPE_GROUP));
}

// 0x0073FEC0 | nintendogs:bytes [tier A]
const nw::snd::internal::SoundArchiveFile::PlayerInfo* nw::snd::internal::SoundArchiveFile::InfoBlockBody::GetPlayerInfo(u32 playerId) const
{
    return static_cast<const PlayerInfo*>(GetItem(this, playerInfoTable, playerId, SoundArchive::ITEM_TYPE_PLAYER));
}

// 0x0073FEFC | nintendogs:bytes [tier A]
u32 nw::snd::internal::SoundArchiveFile::InfoBlockBody::GetItemStringId(u32 id) const
{
    switch (SoundArchive::GetItemType(id)) {
    case SoundArchive::ITEM_TYPE_SOUND: {
        const SoundInfo* pInfo = GetSoundInfo(id);
        if (pInfo != nullptr) {
            return GetOptionValue(pInfo->optionParams, SoundInfo::OPTION_STRING_ID);
        }
        break;
    }
    case SoundArchive::ITEM_TYPE_BANK: {
        const BankInfo* pInfo = GetBankInfo(id);
        if (pInfo != nullptr) {
            return GetOptionValue(pInfo->optionParams, 0);
        }
        break;
    }
    case SoundArchive::ITEM_TYPE_WAVE_ARCHIVE: {
        const WaveArchiveInfo* pInfo = GetWaveArchiveInfo(id);
        if (pInfo != nullptr) {
            return GetOptionValue(pInfo->optionParams, 0);
        }
        break;
    }
    case SoundArchive::ITEM_TYPE_SOUND_GROUP: {
        const SoundGroupInfo* pInfo = GetSoundGroupInfo(id);
        if (pInfo != nullptr) {
            return GetOptionValue(pInfo->optionParams, 0);
        }
        break;
    }
    case SoundArchive::ITEM_TYPE_GROUP: {
        const GroupInfo* pInfo = GetGroupInfo(id);
        if (pInfo != nullptr) {
            return GetOptionValue(pInfo->optionParams, 0);
        }
        break;
    }
    case SoundArchive::ITEM_TYPE_PLAYER: {
        const PlayerInfo* pInfo = GetPlayerInfo(id);
        if (pInfo != nullptr) {
            return GetOptionValue(pInfo->optionParams, 0);
        }
        break;
    }
    }
    return SoundArchive::INVALID_ID;
}

// 0x0074013C (name is ours)
const nw::snd::internal::BinaryReferenceTable* nw::snd::internal::SoundArchiveFile::InfoBlockBody::GetFileInfoReferenceTable() const
{
    return static_cast<const BinaryReferenceTable*>(GetReferedPointer(this, fileInfoTable));
}

// 0x00740148 | nintendogs:bytes [tier C]
const void* nw::snd::internal::SoundArchiveFile::InfoBlockBody::GetSoundArchivePlayerInfo() const
{
    return GetReferedPointer(this, soundArchivePlayerInfo);
}

// 0x00740154 (name is ours)
const nw::snd::internal::BinaryReferenceTable* nw::snd::internal::SoundArchiveFile::InfoBlockBody::GetGroupInfoReferenceTable() const
{
    return static_cast<const BinaryReferenceTable*>(GetReferedPointer(this, groupInfoTable));
}

// 0x00740160 (name is ours)
const nw::snd::internal::BinaryReferenceTable* nw::snd::internal::SoundArchiveFile::InfoBlockBody::GetSoundInfoReferenceTable() const
{
    return static_cast<const BinaryReferenceTable*>(GetReferedPointer(this, soundInfoTable));
}

// 0x0074016C (name is ours)
const nw::snd::internal::BinaryReferenceTable* nw::snd::internal::SoundArchiveFile::InfoBlockBody::GetPlayerInfoReferenceTable() const
{
    return static_cast<const BinaryReferenceTable*>(GetReferedPointer(this, playerInfoTable));
}

// 0x00740178 (name is ours)
const nw::snd::internal::BinaryReferenceTable* nw::snd::internal::SoundArchiveFile::InfoBlockBody::GetWaveArchiveInfoReferenceTable() const
{
    return static_cast<const BinaryReferenceTable*>(GetReferedPointer(this, waveArchiveInfoTable));
}

} // namespace internal
} // namespace snd
} // namespace nw
