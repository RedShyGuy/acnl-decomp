#include "nw/snd/internal/driver/snd_SequenceSoundLoader.h"

#include "nw/snd/internal/snd_LoaderManager.h"
#include "nw/snd/snd_SoundPlayer.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004D2660 | fefates:callgraph [tier C]
DECOMP_NOINLINE bool nw::snd::internal::driver::SequenceSoundLoader::IsInUse()
{
    return !m_Task.m_Event.TryWait();
}

// 0x004D267C | fefates:bytes [tier B]
DECOMP_NOINLINE void nw::snd::internal::driver::SequenceSoundLoader::Finalize()
{
    if (m_Task.m_pPlayerHeap != nullptr) {
        m_Task.m_Arg.soundPlayer->detail_FreePlayerHeap(m_Task.m_pPlayerHeap);
    }
}

// 0x004D2694 | fefates:bytes [tier B]
nw::snd::internal::driver::SequenceSoundLoader::LoadInfo::LoadInfo(const SoundArchive* pSoundArchive, const SoundDataManager* pSoundDataManager,
                                                                   LoadItemInfo* pLoadInfoSeq, LoadItemInfo* pLoadInfoBanks, SoundPlayer* pSoundPlayer)
    : soundArchive(pSoundArchive), soundDataManager(pSoundDataManager), loadInfoSeq(pLoadInfoSeq), soundPlayer(pSoundPlayer)
{
    for (int i = 0; i < BANK_COUNT; ++i) {
        loadInfoBanks[i] = &pLoadInfoBanks[i];
    }
}

} // namespace driver

// 0x007E791C (name is ours)
// 0x007E790C (deleting dtor)
template nw::snd::internal::LoaderManager<nw::snd::internal::driver::SequenceSoundLoader>::~LoaderManager();
// 0x007E78A8 slot 0x08 | fefates:bytes [tier B]
template void nw::snd::internal::LoaderManager<nw::snd::internal::driver::SequenceSoundLoader>::OnBeginSoundFrame();

} // namespace internal
} // namespace snd
} // namespace nw
