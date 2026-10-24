#include "nw/snd/internal/snd_SoundArchiveFileReader.h"

#include "nw/snd/internal/snd_SoundArchiveFile_FileInfo.h"
#include "nw/snd/internal/snd_SoundArchiveFile_InfoBlockBody.h"
#include "nw/snd/internal/snd_SoundArchiveFile_PlayerInfo.h"
#include "nw/snd/internal/snd_SoundArchiveFile_SequenceSoundInfo.h"
#include "nw/snd/internal/snd_SoundArchiveFile_SoundInfo.h"
#include "nw/snd/internal/snd_SoundArchiveFile_StreamSoundInfo.h"
#include "nw/snd/internal/snd_SoundArchiveFile_StreamSoundInfoLegacy.h"
#include "nw/snd/internal/snd_SoundArchiveFile_StringBlockBody.h"
#include "nw/snd/internal/snd_SoundArchiveFile_WaveArchiveInfo.h"
#include "nw/snd/internal/snd_SoundArchiveFile_WaveSoundInfo.h"

namespace nw {
namespace snd {
namespace internal {
namespace {
// the size of the header of a block
const u32 BLOCK_HEADER_SIZE = 8;

// (inline) the wave archive ids of the wave sound group info of a sound group (NULL: another kind)
inline const BinaryTable<u32>* GetWaveArchiveIdTableOf(const SoundArchiveFile::SoundGroupInfo* pGroupInfo)
{
    const SoundArchiveFile::WaveSoundGroupInfo* pWaveSoundGroupInfo =
        pGroupInfo->detailInfo.typeId == SoundArchiveFile::TYPE_WAVE_SOUND_GROUP_INFO
            ? static_cast<const SoundArchiveFile::WaveSoundGroupInfo*>(GetReferedPointer(pGroupInfo, pGroupInfo->detailInfo))
            : nullptr;
    return static_cast<const BinaryTable<u32>*>(GetReferedPointer(pWaveSoundGroupInfo, pWaveSoundGroupInfo->waveArchiveIdTable));
}
} // namespace

// 0x00139D54 | nintendogs:bytes [tier A]
nw::snd::SoundArchive::SoundType nw::snd::internal::SoundArchiveFileReader::GetSoundType(u32 soundId) const
{
    const SoundArchiveFile::SoundInfo* pInfo = m_pInfoBlockBody->GetSoundInfo(soundId);
    if (pInfo == nullptr) {
        return SoundArchive::SOUND_TYPE_INVALID;
    }
    return static_cast<SoundArchive::SoundType>(pInfo->GetSoundType());
}

// 0x0013D528 (name after SoundArchive::GetItemFileId)
u32 nw::snd::internal::SoundArchiveFileReader::GetItemFileId(u32 id) const
{
    return m_pInfoBlockBody->GetItemFileId(id);
}

// 0x0013D530 | nintendogs:bytes [tier A]
bool nw::snd::internal::SoundArchiveFileReader::ReadWaveSoundInfo(u32 soundId, SoundArchive::WaveSoundInfo* pInfo) const
{
    const SoundArchiveFile::SoundInfo* pSoundInfo = m_pInfoBlockBody->GetSoundInfo(soundId);
    if (pSoundInfo == nullptr) {
        return false;
    }
    if (pSoundInfo->GetSoundType() != SoundArchive::SOUND_TYPE_WAVE) {
        return false;
    }
    const SoundArchiveFile::WaveSoundInfo* pWaveSoundInfo = pSoundInfo->GetWaveSoundInfo();
    pInfo->index = pWaveSoundInfo->index;
    pInfo->allocateTrackCount = pWaveSoundInfo->allocateTrackCount;
    pInfo->channelPriority = pWaveSoundInfo->GetChannelPriority();
    pInfo->isReleasePriorityFix = pWaveSoundInfo->GetIsReleasePriorityFix();
    return true;
}

// 0x0013D59C | nintendogs:bytes [tier A]
bool nw::snd::internal::SoundArchiveFileReader::ReadSoundGroupInfo(u32 soundGroupId, SoundArchive::SoundGroupInfo* pInfo) const
{
    const SoundArchiveFile::SoundGroupInfo* pGroupInfo = m_pInfoBlockBody->GetSoundGroupInfo(soundGroupId);
    if (pGroupInfo == nullptr) {
        return false;
    }
    pInfo->startId = pGroupInfo->startId;
    pInfo->endId = pGroupInfo->endId;
    pInfo->pFileIdTable = static_cast<const BinaryTable<u32>*>(GetReferedPointer(pGroupInfo, pGroupInfo->fileIdTable));
    return true;
}

// 0x0013D5E8 | nintendogs:bytes [tier A]
bool nw::snd::internal::SoundArchiveFileReader::ReadSequenceSoundInfo(u32 soundId, SoundArchive::SequenceSoundInfo* pInfo) const
{
    const SoundArchiveFile::SoundInfo* pSoundInfo = m_pInfoBlockBody->GetSoundInfo(soundId);
    if (pSoundInfo == nullptr) {
        return false;
    }
    if (pSoundInfo->GetSoundType() != SoundArchive::SOUND_TYPE_SEQ) {
        return false;
    }
    const SoundArchiveFile::SequenceSoundInfo* pSequenceSoundInfo = pSoundInfo->GetSequenceSoundInfo();
    pInfo->startOffset = pSequenceSoundInfo->GetStartOffset();
    pInfo->allocateTrackFlags = pSequenceSoundInfo->allocateTrackFlags;
    pInfo->channelPriority = pSequenceSoundInfo->GetChannelPriority();
    pInfo->isReleasePriorityFix = pSequenceSoundInfo->IsReleasePriorityFix();
    pSequenceSoundInfo->GetBankIds(pInfo->bankIds);
    return true;
}

// 0x0013FF20 | nintendogs:bytes [tier A]
bool nw::snd::internal::SoundArchiveFileReader::ReadBankInfo(u32 bankId, SoundArchive::BankInfo* pInfo) const
{
    const SoundArchiveFile::BankInfo* pBankInfo = m_pInfoBlockBody->GetBankInfo(bankId);
    if (pBankInfo == nullptr) {
        return false;
    }
    pInfo->fileId = pBankInfo->fileId;
    return true;
}

// 0x0013FF48 | nintendogs:bytes [tier A]
bool nw::snd::internal::SoundArchiveFileReader::ReadSoundInfo(u32 soundId, SoundArchive::SoundInfo* pInfo) const
{
    const SoundArchiveFile::SoundInfo* pSoundInfo = m_pInfoBlockBody->GetSoundInfo(soundId);
    if (pSoundInfo == nullptr) {
        return false;
    }
    pInfo->fileId = pSoundInfo->fileId;
    pInfo->playerId = pSoundInfo->playerId;
    pInfo->volume = pSoundInfo->volume;
    pInfo->panMode = static_cast<PanMode>(pSoundInfo->GetPanMode());
    pInfo->panCurve = static_cast<PanCurve>(pSoundInfo->GetPanCurve());
    pInfo->playerPriority = pSoundInfo->GetPlayerPriority();
    pInfo->actorPlayerId = pSoundInfo->GetActorPlayerId();
    pInfo->isFrontBypass = pSoundInfo->IsFrontBypass();
    return true;
}

// 0x0013FFBC | nintendogs:bytes [tier A]
bool nw::snd::internal::SoundArchiveFileReader::ReadWaveArchiveInfo(u32 waveArchiveId, SoundArchive::WaveArchiveInfo* pInfo) const
{
    const SoundArchiveFile::WaveArchiveInfo* pArchiveInfo = m_pInfoBlockBody->GetWaveArchiveInfo(waveArchiveId);
    if (pArchiveInfo == nullptr) {
        return false;
    }
    pInfo->fileId = pArchiveInfo->fileId;
    pInfo->waveCount = pArchiveInfo->GetWaveCount();
    pInfo->isLoadIndividual = pArchiveInfo->isLoadIndividual;
    return true;
}

// 0x00141F98 | nintendogs:bytes [tier A]
bool nw::snd::internal::SoundArchiveFileReader::ReadFileInfo(u32 fileId, SoundArchive::FileInfo* pInfo, int /*index*/) const
{
    const SoundArchiveFile::FileInfo* pFileInfo = m_pInfoBlockBody->GetFileInfo(fileId);
    if (pFileInfo == nullptr) {
        return false;
    }
    switch (pFileInfo->GetFileLocationType()) {
    case SoundArchiveFile::FileInfo::FILE_LOCATION_TYPE_INTERNAL: {
        const BinaryReferenceWithSize* pInternal = pFileInfo->GetInternalFileInfo();
        pInfo->fileSize = pInternal->size;
        // the offset counts from the body of the file block
        pInfo->offsetFromFileBlockHead = pInternal->IsValid() ? pInternal->offset + BLOCK_HEADER_SIZE : BinaryReference::INVALID_OFFSET;
        break;
    }
    case SoundArchiveFile::FileInfo::FILE_LOCATION_TYPE_EXTERNAL:
        pInfo->externalFilePath = pFileInfo->GetExternalFileInfo();
        break;
    case SoundArchiveFile::FileInfo::FILE_LOCATION_TYPE_NONE:
        return false;
    }
    return true;
}

// 0x00142014 | nintendogs:bytes [tier A]
const nw::snd::internal::BinaryTable<u32>* nw::snd::internal::SoundArchiveFileReader::GetWaveArchiveIdTable(u32 id) const
{
    switch (SoundArchive::GetItemType(id)) {
    case SoundArchive::ITEM_TYPE_SOUND:
        // the table of the sound group of the wave sound
        if (m_pInfoBlockBody->GetSoundInfo(id)->GetSoundType() != SoundArchive::SOUND_TYPE_WAVE) {
            return nullptr;
        }
        for (u32 i = 0; i < m_pInfoBlockBody->GetSoundGroupInfoReferenceTable()->count; ++i) {
            const SoundArchiveFile::SoundGroupInfo* pGroupInfo =
                m_pInfoBlockBody->GetSoundGroupInfo((SoundArchive::ITEM_TYPE_SOUND_GROUP << 24) | i);
            if (pGroupInfo->startId <= id && id <= pGroupInfo->endId) {
                return GetWaveArchiveIdTableOf(pGroupInfo);
            }
        }
        return nullptr;
    case SoundArchive::ITEM_TYPE_SOUND_GROUP:
        return GetWaveArchiveIdTableOf(m_pInfoBlockBody->GetSoundGroupInfo(id));
    case SoundArchive::ITEM_TYPE_BANK: {
        const SoundArchiveFile::BankInfo* pBankInfo = m_pInfoBlockBody->GetBankInfo(id);
        return static_cast<const BinaryTable<u32>*>(GetReferedPointer(pBankInfo, pBankInfo->waveArchiveIdTable));
    }
    default:
        return nullptr;
    }
}

// 0x004C98F0 | fefates:bytes [tier B]
void nw::snd::internal::SoundArchiveFileReader::Initialize(const void* pSoundArchiveFile)
{
    const SoundArchiveFile::FileHeader* pHeader = static_cast<const SoundArchiveFile::FileHeader*>(pSoundArchiveFile);
    if (pHeader->signature != SoundArchiveFile::FileHeader::SIGNATURE) {
        return;
    }
    if (pHeader->version - VERSION_MIN > VERSION_MAX - VERSION_MIN) {
        return;
    }
    m_Header = *pHeader;
}

// 0x004C9958 | nintendogs:bytes [tier C]
void nw::snd::internal::SoundArchiveFileReader::SetInfoBlock(const void* pInfoBlock)
{
    m_pInfoBlockBody = reinterpret_cast<const SoundArchiveFile::InfoBlockBody*>(static_cast<const u8*>(pInfoBlock) + BLOCK_HEADER_SIZE);
}

// 0x004C9964 | nintendogs:bytes [tier C]
void nw::snd::internal::SoundArchiveFileReader::SetStringBlock(const void* pStringBlock)
{
    m_pStringBlockBody = reinterpret_cast<const SoundArchiveFile::StringBlockBody*>(static_cast<const u8*>(pStringBlock) + BLOCK_HEADER_SIZE);
}

// 0x004C9970 | nintendogs:bytes [tier C]
void nw::snd::internal::SoundArchiveFileReader::Finalize()
{
    m_pStringBlockBody = nullptr;
    m_pInfoBlockBody = nullptr;
    m_Unknown0x40 = 0;
}

// 0x004C9984 | nintendogs:bytes [tier C]
nw::snd::internal::SoundArchiveFileReader::SoundArchiveFileReader()
    : m_pStringBlockBody(nullptr), m_pInfoBlockBody(nullptr), m_Unknown0x40(0)
{
}

// 0x00740C9C (name is ours)
u32 nw::snd::internal::SoundArchiveFileReader::GetFileCount() const
{
    return m_pInfoBlockBody->GetFileInfoReferenceTable()->count;
}

// 0x00740CB0 | nintendogs:bytes [tier A]
const char* nw::snd::internal::SoundArchiveFileReader::GetItemLabel(u32 id) const
{
    if (m_pStringBlockBody == nullptr) {
        return nullptr;
    }
    const u32 strId = m_pInfoBlockBody->GetItemStringId(id);
    if (strId == SoundArchive::INVALID_ID) {
        return nullptr;
    }
    return m_pStringBlockBody->GetString(strId);
}

// 0x00740CF4 (name is ours)
u32 nw::snd::internal::SoundArchiveFileReader::GetGroupCount() const
{
    return m_pInfoBlockBody->GetGroupInfoReferenceTable()->count;
}

// 0x00740D10 (name is ours)
u32 nw::snd::internal::SoundArchiveFileReader::GetSoundCount() const
{
    return m_pInfoBlockBody->GetSoundInfoReferenceTable()->count;
}

// 0x00740D24 | fefates:bytes [tier B]
bool nw::snd::internal::SoundArchiveFileReader::ReadGroupInfo(u32 groupId, SoundArchive::GroupInfo* pInfo) const
{
    const SoundArchiveFile::GroupInfo* pGroupInfo = m_pInfoBlockBody->GetGroupInfo(groupId);
    if (pGroupInfo == nullptr) {
        return false;
    }
    pInfo->fileId = pGroupInfo->fileId;
    u32 fileSize = SoundArchive::INVALID_ID;
    const SoundArchiveFile::FileInfo* pFileInfo = m_pInfoBlockBody->GetFileInfo(pGroupInfo->fileId);
    if (pFileInfo == nullptr) {
        return false;
    }
    switch (pFileInfo->GetFileLocationType()) {
    case SoundArchiveFile::FileInfo::FILE_LOCATION_TYPE_INTERNAL:
        fileSize = pFileInfo->GetInternalFileInfo()->size;
        break;
    case SoundArchiveFile::FileInfo::FILE_LOCATION_TYPE_EXTERNAL:
        pFileInfo->GetExternalFileInfo();
        break;
    case SoundArchiveFile::FileInfo::FILE_LOCATION_TYPE_NONE:
        return false;
    }
    pInfo->fileSize = fileSize;
    return true;
}

// 0x00740DB8 (name is ours)
u32 nw::snd::internal::SoundArchiveFileReader::GetPlayerCount() const
{
    return m_pInfoBlockBody->GetPlayerInfoReferenceTable()->count;
}

// 0x00740DCC | nintendogs:bytes [tier A]
bool nw::snd::internal::SoundArchiveFileReader::ReadPlayerInfo(u32 playerId, SoundArchive::PlayerInfo* pInfo) const
{
    const SoundArchiveFile::PlayerInfo* pPlayerInfo = m_pInfoBlockBody->GetPlayerInfo(playerId);
    if (pPlayerInfo == nullptr) {
        return false;
    }
    pInfo->playableSoundMax = pPlayerInfo->playableSoundMax;
    pInfo->playerHeapSize = pPlayerInfo->GetPlayerHeapSize();
    return true;
}

// 0x00740DFC | nintendogs:bytes [tier A]
bool nw::snd::internal::SoundArchiveFileReader::ReadSound3DInfo(u32 soundId, SoundArchive::Sound3DInfo* pInfo) const
{
    const SoundArchiveFile::SoundInfo* pSoundInfo = m_pInfoBlockBody->GetSoundInfo(soundId);
    if (pSoundInfo == nullptr) {
        return false;
    }
    const SoundArchive::Sound3DInfo* pSound3DInfo = static_cast<const SoundArchive::Sound3DInfo*>(pSoundInfo->GetSound3DInfo());
    if (pSound3DInfo == nullptr) {
        return false;
    }
    pInfo->flags = pSound3DInfo->flags;
    pInfo->decayRatio = pSound3DInfo->decayRatio;
    pInfo->decayCurve = pSound3DInfo->decayCurve;
    pInfo->dopplerFactor = pSound3DInfo->dopplerFactor;
    return true;
}

// 0x00740E48 | fefates:bytes [tier B]
bool nw::snd::internal::SoundArchiveFileReader::ReadSoundUserParam(u32 soundId, int index, u32& value) const
{
    const SoundArchiveFile::SoundInfo* pSoundInfo = m_pInfoBlockBody->GetSoundInfo(soundId);
    if (pSoundInfo == nullptr) {
        return false;
    }
    return pSoundInfo->ReadUserParam(index, value);
}

// 0x00740E80 (name is ours)
u32 nw::snd::internal::SoundArchiveFileReader::GetWaveArchiveCount() const
{
    return m_pInfoBlockBody->GetWaveArchiveInfoReferenceTable()->count;
}

// 0x00740E94 | fefates:bytes [tier B]
bool nw::snd::internal::SoundArchiveFileReader::ReadStreamSoundInfo(u32 soundId, SoundArchive::StreamSoundInfo* pInfo) const
{
    const SoundArchiveFile::SoundInfo* pSoundInfo = m_pInfoBlockBody->GetSoundInfo(soundId);
    if (pSoundInfo == nullptr) {
        return false;
    }
    if (pSoundInfo->GetSoundType() != SoundArchive::SOUND_TYPE_STRM) {
        return false;
    }

    const BinaryReferenceTable* pTrackInfoTable;
    if (m_Header.version >= VERSION_STREAM_SOUND_INFO) {
        const SoundArchiveFile::StreamSoundInfo* pStreamSoundInfo = pSoundInfo->GetStreamSoundInfo();
        pInfo->allocateTrackFlags = pStreamSoundInfo->allocateTrackFlags;
        pInfo->allocateChannelCount = pStreamSoundInfo->allocateChannelCount;
        const SoundArchiveFile::StreamSoundInfo::StreamSoundExtension* pExtension = pStreamSoundInfo->GetStreamSoundExtension();
        pInfo->streamFileType = pExtension != nullptr ? pExtension->streamTypeInfo & 0xFF : 1;
        pInfo->pitch = pStreamSoundInfo->pitch;
        const u32 sendValue = *pStreamSoundInfo->GetSendValue();
        pInfo->mainSend = sendValue;
        pInfo->fxSend[0] = sendValue >> 8;
        pInfo->fxSend[1] = sendValue >> 16;
        if (m_Header.version >= VERSION_PREFETCH) {
            pInfo->prefetchFileId = pStreamSoundInfo->prefetchFileId;
        } else {
            pInfo->prefetchFileId = SoundArchive::INVALID_ID;
        }
        pTrackInfoTable = pStreamSoundInfo->GetTrackInfoTable();
    } else {
        const SoundArchiveFile::StreamSoundInfoLegacy* pStreamSoundInfo = pSoundInfo->GetStreamSoundInfoLegacy();
        pInfo->allocateTrackFlags = pStreamSoundInfo->allocateTrackFlags;
        pInfo->allocateChannelCount = pStreamSoundInfo->allocateChannelCount;
        pInfo->streamFileType = pStreamSoundInfo->GetStreamFileType();
        pInfo->pitch = 1.0f;
        pInfo->mainSend = 127;
        pInfo->fxSend[0] = 0;
        pInfo->fxSend[1] = 0;
        pInfo->prefetchFileId = SoundArchive::INVALID_ID;
        pTrackInfoTable = pStreamSoundInfo->GetTrackInfoTable();
    }

    if (pTrackInfoTable != nullptr) {
        const u32 trackCount = pTrackInfoTable->count;
        for (u32 i = 0; i < trackCount && i < SoundArchive::STREAM_TRACK_MAX; ++i) {
            SoundArchive::StreamTrackInfo& track = pInfo->trackInfo[i];
            const SoundArchiveFile::StreamTrackInfo* pTrackInfo = static_cast<const SoundArchiveFile::StreamTrackInfo*>(
                pTrackInfoTable->GetReferedItem(i, SoundArchiveFile::TYPE_STREAM_TRACK_INFO));
            track.volume = pTrackInfo->volume;
            track.pan = pTrackInfo->pan;
            track.span = pTrackInfo->span;
            track.flags = pTrackInfo->flags;
            const BinaryTable<u8>* pChannelIndexTable =
                static_cast<const BinaryTable<u8>*>(GetReferedPointer(pTrackInfo, pTrackInfo->globalChannelIndexTable));
            track.channelCount = pChannelIndexTable->count;
            if (m_Header.version >= VERSION_STREAM_SOUND_INFO) {
                const u8* pSendValue = static_cast<const u8*>(GetReferedPointer(pTrackInfo, pTrackInfo->sendValue));
                track.mainSend = pSendValue[0];
                for (int j = 0; j < 2; ++j) {
                    track.fxSend[j] = pSendValue[1 + j];
                }
            } else {
                track.mainSend = 127;
                track.fxSend[0] = 0;
                track.fxSend[1] = 0;
            }
            if (m_Header.version >= VERSION_STREAM_SOUND_INFO) {
                track.lpfFreq = pTrackInfo->lpfFreq;
                track.biquadType = pTrackInfo->biquadType;
                track.biquadValue = pTrackInfo->biquadValue;
            } else {
                track.lpfFreq = 64;
                track.biquadType = 0;
                track.biquadValue = 0;
            }
            const u32 channelCount = track.channelCount > 2 ? 2 : track.channelCount;
            for (u32 j = 0; j < channelCount; ++j) {
                track.globalChannelIndex[j] = pChannelIndexTable->items[j];
            }
        }
    }
    return true;
}

// 0x00741124 | fefates:bytes [tier B]
bool nw::snd::internal::SoundArchiveFileReader::ReadStreamSoundInfo2(u32 soundId, SoundArchive::StreamSoundInfo2* pInfo) const
{
    const SoundArchiveFile::SoundInfo* pSoundInfo = m_pInfoBlockBody->GetSoundInfo(soundId);
    if (pSoundInfo == nullptr) {
        return false;
    }
    if (pSoundInfo->GetSoundType() != SoundArchive::SOUND_TYPE_STRM) {
        return false;
    }
    if (m_Header.version >= VERSION_STREAM_SOUND_INFO) {
        const SoundArchiveFile::StreamSoundInfo::StreamSoundExtension* pExtension =
            pSoundInfo->GetStreamSoundInfo()->GetStreamSoundExtension();
        if (pExtension == nullptr) {
            return false;
        }
        pInfo->isLoop = ((pExtension->streamTypeInfo >> 8) & 0xFF) == 1;
        pInfo->loopStartFrame = pExtension->loopStartFrame;
        pInfo->loopEndFrame = pExtension->loopEndFrame;
    } else {
        const SoundArchiveFile::StreamSoundInfoLegacy* pStreamSoundInfo = pSoundInfo->GetStreamSoundInfoLegacy();
        pInfo->isLoop = pStreamSoundInfo->IsLoop();
        pInfo->loopStartFrame = pStreamSoundInfo->GetLoopStartFrame();
        pInfo->loopEndFrame = pStreamSoundInfo->GetLoopEndFrame();
    }
    return true;
}

// 0x007411E0 | fefates:bytes [tier B]
bool nw::snd::internal::SoundArchiveFileReader::ReadSoundArchivePlayerInfo(SoundArchive::SoundArchivePlayerInfo* pInfo) const
{
    const SoundArchiveFile::SoundArchivePlayerInfo* pPlayerInfo =
        static_cast<const SoundArchiveFile::SoundArchivePlayerInfo*>(m_pInfoBlockBody->GetSoundArchivePlayerInfo());
    if (pPlayerInfo == nullptr) {
        return false;
    }
    pInfo->sequenceSoundMax = pPlayerInfo->sequenceSoundMax;
    pInfo->sequenceTrackMax = pPlayerInfo->sequenceTrackMax;
    pInfo->streamSoundMax = pPlayerInfo->streamSoundMax;
    pInfo->streamTrackMax = pPlayerInfo->streamTrackMax;
    pInfo->streamChannelMax = pPlayerInfo->streamChannelMax;
    pInfo->waveSoundMax = pPlayerInfo->waveSoundMax;
    pInfo->waveTrackMax = pPlayerInfo->waveTrackMax;
    if (m_Header.version >= VERSION_PREFETCH) {
        pInfo->streamBufferTimes = pPlayerInfo->streamBufferTimes;
    } else {
        pInfo->streamBufferTimes = 1;
    }
    return true;
}

// 0x0074125C | fefates:bytes [tier B]
u32 nw::snd::internal::SoundArchiveFileReader::GetItemId(const char* pLabel) const
{
    if (m_pStringBlockBody == nullptr) {
        return SoundArchive::INVALID_ID;
    }
    return m_pStringBlockBody->GetItemIdImpl(SoundArchiveFile::StringBlockBody::SECTIONS_PATRICIA_TREE, pLabel);
}

} // namespace internal
} // namespace snd
} // namespace nw
