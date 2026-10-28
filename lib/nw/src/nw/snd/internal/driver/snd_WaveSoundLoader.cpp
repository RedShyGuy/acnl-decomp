#include "nw/snd/internal/driver/snd_WaveSoundLoader.h"

#include "nw/snd/internal/snd_LoaderManager.h"
#include "nw/snd/snd_SoundPlayer.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004CD9B8 | fefates:callgraph [tier C]
DECOMP_NOINLINE bool nw::snd::internal::driver::WaveSoundLoader::IsInUse()
{
    return !m_Task.m_Event.TryWait();
}

// 0x004CD9D4 | fefates:bytes [tier B]
DECOMP_NOINLINE void nw::snd::internal::driver::WaveSoundLoader::Finalize()
{
    if (m_Task.m_pPlayerHeap != nullptr) {
        m_Task.m_Arg.soundPlayer->detail_FreePlayerHeap(m_Task.m_pPlayerHeap);
    }
}

} // namespace driver

// 0x007E7898 (name is ours)
// 0x007E7888 (deleting dtor)
template nw::snd::internal::LoaderManager<nw::snd::internal::driver::WaveSoundLoader>::~LoaderManager();
// 0x007E7824 slot 0x08 | fefates:bytes [tier B]
template void nw::snd::internal::LoaderManager<nw::snd::internal::driver::WaveSoundLoader>::OnBeginSoundFrame();

} // namespace internal
} // namespace snd
} // namespace nw
