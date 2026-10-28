#include "nw/snd/internal/driver/snd_SequenceSoundLoader.h"

#include "nw/snd/internal/Util/Util_Api.h"
#include "nw/snd/internal/snd_PlayerHeap.h"
#include "nw/snd/internal/snd_PlayerHeapDataManager.h"
#include "nw/snd/snd_SoundDataManager.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004D265C slot 0x00
// 0x004D264C (deleting dtor)
nw::snd::internal::driver::SequenceSoundLoader::DataLoadTask::~DataLoadTask()
{
    // empty (in the original too)
}

// 0x004D240C slot 0x08 | fefates:bytes
void nw::snd::internal::driver::SequenceSoundLoader::DataLoadTask::Execute()
{
    PlayerHeapDataManager dataManager;
    dataManager.Initialize(m_Arg.soundArchive);

    // the sequence file
    if (m_Arg.loadInfoSeq.address == nullptr && m_Arg.loadInfoSeq.itemId != 0xFFFFFFFF) {
        const u32 soundId = m_Arg.loadInfoSeq.itemId;
        if (!dataManager.LoadData(soundId, m_pPlayerHeap, SoundArchiveLoader::LOAD_FLAG_SEQUENCE, 0)) {
            m_pPlayerHeap->m_State = PlayerHeap::STATE_TASK_FINISHED;
            m_IsSucceeded = false;
            return;
        }
        m_Arg.loadInfoSeq.address = dataManager.detail_GetFileAddressByItemId(soundId);
    }

    // the banks and the wave archives of their first waves
    LoadItemInfo warcInfos[BANK_COUNT];
    bool warcIsIndividual[BANK_COUNT] = {false, false, false, false};
    const SoundArchiveLoader& loader = dataManager;
    for (int i = 0; i < BANK_COUNT; ++i) {
        LoadItemInfo& bankInfo = m_Arg.loadInfoBanks[i];
        if (bankInfo.address == nullptr) {
            const u32 bankId = bankInfo.itemId;
            if (bankId == 0xFFFFFFFF) {
                continue;
            }
            if (!dataManager.LoadData(bankId, m_pPlayerHeap, SoundArchiveLoader::LOAD_FLAG_BANK, 0)) {
                m_pPlayerHeap->m_State = PlayerHeap::STATE_TASK_FINISHED;
                m_IsSucceeded = false;
                return;
            }
            bankInfo.address = dataManager.detail_GetFileAddressByItemId(bankId);
        }
        if (bankInfo.itemId == 0xFFFFFFFF || bankInfo.address == nullptr) {
            continue;
        }
        switch (Util::GetWaveArchiveOfBank(warcInfos[i], warcIsIndividual[i], bankInfo.address, *m_Arg.soundArchive,
                                           *static_cast<const SoundArchiveLoader*>(m_Arg.soundDataManager))) {
        case Util::WAVE_ARCHIVE_OF_BANK_NOT_LOADED:
        case Util::WAVE_ARCHIVE_OF_BANK_WAVE_MISSING:
            if (!dataManager.detail_LoadWaveArchiveByBankFile(bankInfo.address, m_pPlayerHeap)) {
                m_pPlayerHeap->m_State = PlayerHeap::STATE_TASK_FINISHED;
                m_IsSucceeded = false;
                return;
            }
            Util::GetWaveArchiveOfBank(warcInfos[i], warcIsIndividual[i], bankInfo.address, *m_Arg.soundArchive, loader);
            break;
        default:
            break;
        }
    }

    m_Result.seqFile = m_Arg.loadInfoSeq.address;
    for (int i = 0; i < BANK_COUNT; ++i) {
        m_Result.bankFiles[i] = m_Arg.loadInfoBanks[i].address;
        m_Result.warcFiles[i] = warcInfos[i].address;
        m_Result.warcIsIndividual[i] = warcIsIndividual[i];
    }
    m_pPlayerHeap->m_State = PlayerHeap::STATE_TASK_FINISHED;
    dataManager.Finalize();
    m_IsSucceeded = true;
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
