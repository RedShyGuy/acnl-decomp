#include "nw/snd/internal/snd_BankFileReader.h"

#include <cstring>

#include "nw/snd/internal/snd_BankFile_Instrument.h"
#include "nw/snd/internal/snd_BankFile_KeyRegion.h"
#include "nw/snd/internal/snd_BankFile_VelocityRegion.h"

namespace nw {
namespace snd {
namespace internal {
namespace {
// the size of the header of a block
const u32 BLOCK_HEADER_SIZE = 8;
} // namespace

// 0x0013BB38 | nintendogs:bytes [tier A]
nw::snd::internal::BankFileReader::BankFileReader(const void* pBankFile)
    : m_pHeader(nullptr), m_pInfoBlockBody(nullptr), m_IsInitialized(false)
{
    Initialize(pBankFile);
}

// 0x0013D38C | nintendogs:bytes [tier A]
const nw::snd::internal::BinaryTable<nw::snd::internal::WaveId>* nw::snd::internal::BankFileReader::GetWaveIdTable() const
{
    if (!m_IsInitialized) {
        return nullptr;
    }
    return m_pInfoBlockBody->GetWaveIdTable();
}

// 0x004C8164 | fefates:bytes [tier B]
void nw::snd::internal::BankFileReader::Initialize(const void* pBankFile)
{
    if (pBankFile == nullptr) {
        return;
    }
    const BankFile::FileHeader* pHeader = static_cast<const BankFile::FileHeader*>(pBankFile);
    if (pHeader->signature != BankFile::SIGNATURE_FILE) {
        return;
    }
    if (pHeader->version - BankFile::VERSION_MIN > BankFile::VERSION_MAX - BankFile::VERSION_MIN) {
        return;
    }
    m_pHeader = pHeader;
    m_pInfoBlockBody = reinterpret_cast<const BankFile::InfoBlockBody*>(static_cast<const u8*>(pHeader->GetInfoBlock()) + BLOCK_HEADER_SIZE);
    m_IsInitialized = true;
}

// 0x004C81B8 | fefates:bytes [tier B]
void nw::snd::internal::BankFileReader::Finalize()
{
    if (!m_IsInitialized) {
        return;
    }
    m_pHeader = nullptr;
    m_pInfoBlockBody = nullptr;
    m_IsInitialized = false;
}

// 0x004C81D8 (name after the other constructor)
nw::snd::internal::BankFileReader::BankFileReader() : m_pHeader(nullptr), m_pInfoBlockBody(nullptr), m_IsInitialized(false)
{
}

// 0x0073F6E0 | nintendogs:bytes [tier A]
bool nw::snd::internal::BankFileReader::ReadVelocityRegionInfo(VelocityRegionInfo* pInfo, int programNo, int key, int velocity) const
{
    if (!m_IsInitialized) {
        return false;
    }
    if (programNo < 0 || programNo >= static_cast<int>(m_pInfoBlockBody->GetInstrumentReferenceTable()->count)) {
        return false;
    }
    const BankFile::Instrument* pInstrument = m_pInfoBlockBody->GetInstrument(programNo);
    if (pInstrument == nullptr) {
        return false;
    }
    const BankFile::KeyRegion* pKeyRegion = pInstrument->GetKeyRegion(key);
    if (pKeyRegion == nullptr) {
        return false;
    }
    const BankFile::VelocityRegion* pVelocityRegion = pKeyRegion->GetVelocityRegion(velocity);
    if (pVelocityRegion == nullptr) {
        return false;
    }

    // the wave
    const BinaryTable<WaveId>* pWaveIdTable = m_pInfoBlockBody->GetWaveIdTable();
    const u32 waveIdIndex = pVelocityRegion->waveIdTableIndex;
    if (waveIdIndex >= pWaveIdTable->count) {
        return false;
    }
    const WaveId* pWaveId = &pWaveIdTable->items[waveIdIndex];
    if (pWaveId == nullptr || pWaveId->waveIndex == 0xFFFFFFFF) {
        return false;
    }
    pInfo->waveArchiveId = pWaveId->waveArchiveId;
    pInfo->waveIndex = pWaveId->waveIndex;

    // the parameters: at fixed places if all are set
    const BankFile::VelocityRegion::RegionParameter* pParameter = pVelocityRegion->GetRegionParameter();
    if (pParameter != nullptr) {
        pInfo->originalKey = pParameter->originalKey;
        pInfo->volume = pParameter->volume;
        pInfo->pan = pParameter->pan;
        pInfo->pitch = pParameter->pitch;
        pInfo->isIgnoreNoteOff = pParameter->isIgnoreNoteOff;
        pInfo->keyGroup = pParameter->keyGroup;
        pInfo->interpolationType = pParameter->interpolationType;
        std::memcpy(&pInfo->adshrCurve, &pParameter->adshrCurve, sizeof(AdshrCurve));
    } else {
        pInfo->originalKey = pVelocityRegion->GetOriginalKey();
        pInfo->volume = pVelocityRegion->GetVolume();
        pInfo->pan = pVelocityRegion->GetPan();
        pInfo->pitch = pVelocityRegion->GetPitch();
        pInfo->isIgnoreNoteOff = pVelocityRegion->IsIgnoreNoteOff();
        pInfo->keyGroup = pVelocityRegion->GetKeyGroup();
        pInfo->interpolationType = pVelocityRegion->GetInterpolationType();
        std::memcpy(&pInfo->adshrCurve, pVelocityRegion->GetAdshrCurve(), sizeof(AdshrCurve));
    }
    return true;
}

} // namespace internal
} // namespace snd
} // namespace nw
