#include "nw/snd/internal/snd_WaveSoundFileReader.h"

#include <cstring>

#include "nw/snd/internal/snd_WaveSoundFile_FileHeader.h"
#include "nw/snd/internal/snd_WaveSoundFile_InfoBlockBody.h"
#include "nw/snd/internal/snd_WaveSoundFile_NoteInfo.h"
#include "nw/snd/internal/snd_WaveSoundFile_WaveSoundData.h"
#include "nw/snd/internal/snd_WaveSoundFile_WaveSoundInfo.h"

namespace nw {
namespace snd {
namespace internal {
namespace {
// the size of the header of a block
const u32 BLOCK_HEADER_SIZE = 8;
} // namespace

// 0x0013C310 | nintendogs:callgraph [tier A]
nw::snd::internal::WaveSoundFileReader::WaveSoundFileReader(const void* pWaveSoundFile) : m_pHeader(nullptr), m_pInfoBlockBody(nullptr)
{
    const WaveSoundFile::FileHeader* pHeader = static_cast<const WaveSoundFile::FileHeader*>(pWaveSoundFile);
    if (pHeader->signature != WaveSoundFile::SIGNATURE_FILE) {
        return;
    }
    if (pHeader->version - WaveSoundFile::VERSION_MIN > WaveSoundFile::VERSION_MAX - WaveSoundFile::VERSION_MIN) {
        return;
    }
    m_pHeader = pHeader;
    const void* pInfoBlock = pHeader->GetInfoBlock();
    if (pInfoBlock == nullptr) {
        return;
    }
    m_pInfoBlockBody = reinterpret_cast<const WaveSoundFile::InfoBlockBody*>(static_cast<const u8*>(pInfoBlock) + BLOCK_HEADER_SIZE);
}

// 0x0013D44C | nintendogs:bytes [tier A]
bool nw::snd::internal::WaveSoundFileReader::ReadNoteInfo(WaveSoundNoteInfo* pInfo, u32 index, u32 noteIndex) const
{
    const WaveSoundFile::NoteInfo* pNoteInfo = m_pInfoBlockBody->GetWaveSoundData(index)->GetNoteInfo(noteIndex);
    const BinaryTable<WaveId>* pWaveIdTable = m_pInfoBlockBody->GetWaveIdTable();
    if (pNoteInfo->waveIdTableIndex >= pWaveIdTable->count) {
        return false;
    }
    const WaveId* pWaveId = &pWaveIdTable->items[pNoteInfo->waveIdTableIndex];
    if (pWaveId == nullptr) {
        return false;
    }
    pInfo->waveArchiveId = pWaveId->waveArchiveId;
    pInfo->waveIndex = pWaveId->waveIndex;
    pInfo->pitch = pNoteInfo->GetPitch();
    std::memcpy(&pInfo->adshrCurve, pNoteInfo->GetAdshrCurve(), sizeof(AdshrCurve));
    pInfo->originalKey = pNoteInfo->GetOriginalKey();
    pInfo->pan = pNoteInfo->GetPan();
    pInfo->surroundPan = pNoteInfo->GetSurroundPan();
    pInfo->volume = pNoteInfo->GetVolume();
    return true;
}

// 0x007408C0 | nintendogs:callseq [tier A]
bool nw::snd::internal::WaveSoundFileReader::ReadWaveSoundInfo(WaveSoundInfo* pInfo, u32 index) const
{
    const WaveSoundFile::WaveSoundInfo* pWaveSoundInfo = m_pInfoBlockBody->GetWaveSoundData(index)->GetWaveSoundInfo();
    pInfo->pitch = pWaveSoundInfo->GetPitch();
    pInfo->pan = pWaveSoundInfo->GetPan();
    pInfo->surroundPan = pWaveSoundInfo->GetSurroundPan();
    pWaveSoundInfo->GetSendValue(&pInfo->mainSend, pInfo->fxSend, WaveSoundFile::WaveSoundInfo::FX_SEND_COUNT_MAX);
    std::memcpy(&pInfo->adshrCurve, pWaveSoundInfo->GetAdshrCurve(), sizeof(AdshrCurve));
    if (m_pHeader->version >= WaveSoundFile::VERSION_FILTER) {
        pInfo->lpfFreq = pWaveSoundInfo->GetLpfFreq();
        pInfo->biquadType = pWaveSoundInfo->GetBiquadType();
        pInfo->biquadValue = pWaveSoundInfo->GetBiquadValue();
    } else {
        pInfo->lpfFreq = 64;
        pInfo->biquadType = 0;
        pInfo->biquadValue = 0;
    }
    return true;
}

} // namespace internal
} // namespace snd
} // namespace nw
