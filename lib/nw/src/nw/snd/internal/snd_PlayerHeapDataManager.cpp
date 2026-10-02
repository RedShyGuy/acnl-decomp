#include "nw/snd/internal/snd_SoundArchiveLoader.h"
#include "nw/snd/internal/driver/snd_DisposeCallback.h"
#include "nw/snd/internal/snd_PlayerHeapDataManager.h"

namespace nw {
namespace snd {
namespace internal {
// 0x004C94E0 slot 0x00 | fefates:bytes
nw::snd::internal::PlayerHeapDataManager::~PlayerHeapDataManager()
{
}

// 0x004C9488 slot 0x04 | virtual slot, introduced by nw::snd::internal::driver::DisposeCallback
void nw::snd::internal::PlayerHeapDataManager::vf_0x04()
{
}

// 0x004C9378 slot 0x08 | nintendogs:bytes
void nw::snd::internal::PlayerHeapDataManager::InvalidateData(const void*, const void*)
{
}

// 0x004C93AC slot 0x0C | virtual slot, introduced by nw::snd::internal::PlayerHeapDataManager
void nw::snd::internal::PlayerHeapDataManager::vf_0x0C()
{
}

// 0x0074098C slot 0x10 | slot vf_0x10 of nw::snd::internal::PlayerHeapDataManager
void nw::snd::internal::PlayerHeapDataManager::GetFileAddressFromTable(unsigned int) const
{
}

// 0x00740980 slot 0x14 | virtual slot, introduced by nw::snd::internal::PlayerHeapDataManager
void nw::snd::internal::PlayerHeapDataManager::vf_0x14()
{
}

// 0x004C8ADC | nintendogs:bytes [tier A]
void nw::snd::internal::PlayerHeapDataManager::Initialize(const nw::snd::SoundArchive*)
{
}

// 0x004C9418 | nintendogs:bytes [tier A]
void nw::snd::internal::PlayerHeapDataManager::Finalize()
{
}

// 0x004C9444 | nintendogs:bytes [tier A]
nw::snd::internal::PlayerHeapDataManager::PlayerHeapDataManager()
{
}

} // namespace internal
} // namespace snd
} // namespace nw
