#include "nw/snd/internal/driver/snd_WaveSoundLoader_DataLoadTask.h"

#include "nw/snd/internal/Util/Util_Api.h"
#include "nw/snd/internal/snd_PlayerHeap.h"
#include "nw/snd/internal/snd_PlayerHeapDataManager.h"
#include "nw/snd/snd_SoundDataManager.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004C9BB8 slot 0x00
// 0x004CD9A8 (deleting dtor)
nw::snd::internal::driver::WaveSoundLoader::DataLoadTask::~DataLoadTask()
{
    // (only the base is destroyed)
}

// 0x004CD88C slot 0x08 | fefates:bytes
void nw::snd::internal::driver::WaveSoundLoader::DataLoadTask::Execute()
{
    PlayerHeapDataManager dataManager;
    dataManager.Initialize(m_Arg.soundArchive);

    // the wave sound file
    if (m_Arg.loadInfoWsd.address == nullptr && m_Arg.loadInfoWsd.itemId != 0xFFFFFFFF) {
        const u32 soundId = m_Arg.loadInfoWsd.itemId;
        if (!dataManager.LoadData(soundId, m_pPlayerHeap, SoundArchiveLoader::LOAD_FLAG_WAVE_SOUND, 0)) {
            m_pPlayerHeap->m_State = PlayerHeap::STATE_TASK_FINISHED;
            m_IsSucceeded = false;
            return;
        }
        m_Arg.loadInfoWsd.address = dataManager.detail_GetFileAddressByItemId(soundId);
    }

    // the wave: loaded by the game or into the heap
    const void* pWaveFile = Util::GetWaveFileOfWaveSound(m_Arg.loadInfoWsd.address, m_Arg.index, *m_Arg.soundArchive, *static_cast<const SoundArchiveLoader*>(m_Arg.soundDataManager));
    if (pWaveFile == nullptr) {
        if (!dataManager.detail_LoadWaveArchiveByWaveSoundFile(m_Arg.loadInfoWsd.address, m_Arg.index, m_pPlayerHeap)) {
            m_pPlayerHeap->m_State = PlayerHeap::STATE_TASK_FINISHED;
            m_IsSucceeded = false;
            return;
        }
        pWaveFile = Util::GetWaveFileOfWaveSound(m_Arg.loadInfoWsd.address, m_Arg.index, *m_Arg.soundArchive, dataManager);
    }
    m_pWaveFile = pWaveFile;
    m_pWaveSoundFile = m_Arg.loadInfoWsd.address;
    m_pPlayerHeap->m_State = PlayerHeap::STATE_TASK_FINISHED;
    dataManager.Finalize();
    m_IsSucceeded = true;
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
