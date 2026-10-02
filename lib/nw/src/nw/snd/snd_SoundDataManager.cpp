#include "nw/snd/internal/snd_SoundArchiveLoader.h"
#include "nw/snd/internal/driver/snd_DisposeCallback.h"
#include "nw/snd/snd_SoundDataManager.h"

namespace nw {
namespace snd {
// 0x004C089C slot 0x00 | fefates:bytes
nw::snd::SoundDataManager::~SoundDataManager()
{
}

// 0x004C0884 slot 0x04 | virtual slot, introduced by nw::snd::internal::driver::DisposeCallback
void nw::snd::SoundDataManager::vf_0x04()
{
}

// 0x004C06B8 slot 0x08 | nintendogs:callseq
void nw::snd::SoundDataManager::InvalidateData(const void*, const void*)
{
}

// 0x004C0800 slot 0x0C | mk7dlp:bytes
void nw::snd::SoundDataManager::SetFileAddressToTable(unsigned, const void*)
{
}

// 0x0073EE84 slot 0x10 | nintendogs:bytes
void nw::snd::SoundDataManager::GetFileAddressFromTable(unsigned) const
{
}

// 0x0073EDF4 slot 0x14 | fefates:bytes
void nw::snd::SoundDataManager::GetFileAddressImpl(unsigned int) const
{
}

// 0x00137C74 | nintendogs:bytes [tier A]
nw::snd::SoundDataManager::SoundDataManager()
{
}

// 0x004C05AC | nintendogs:bytes-fuzzy [tier A]
void nw::snd::SoundDataManager::Initialize(const nw::snd::SoundArchive*, void*, unsigned)
{
}

// 0x004C0824 | nintendogs:callseq [tier A]
void nw::snd::SoundDataManager::Finalize()
{
}

// 0x0073EE58 | nintendogs:bytes [tier A]
void nw::snd::SoundDataManager::GetRequiredMemSize(const nw::snd::SoundArchive*) const
{
}

} // namespace snd
} // namespace nw
