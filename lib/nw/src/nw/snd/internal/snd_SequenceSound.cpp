#include "nw/snd/internal/snd_BasicSound.h"
#include "nw/snd/internal/driver/snd_SequenceSoundPlayer.h"
#include "nw/snd/internal/driver/snd_SequenceSoundLoader.h"
#include "nw/snd/internal/snd_SequenceSound.h"

namespace nw {
namespace snd {
namespace internal {
// ctor candidate(s) 0x004C80D0 (unverified)
nw::snd::internal::SequenceSound::SequenceSound()
{
}

// 0x0073F460 slot 0x00 | slot vf_0x00 of nw::snd::internal::BasicSound
void nw::snd::internal::SequenceSound::GetRuntimeTypeInfo() const
{
}

// 0x004C8138 slot 0x04 | fefates:bytes
nw::snd::internal::SequenceSound::~SequenceSound()
{
}

// 0x004C8108 slot 0x08 | virtual slot, introduced by nw::snd::internal::BasicSound
void nw::snd::internal::SequenceSound::vf_0x08()
{
}

// 0x004C7A68 slot 0x0C | slot vf_0x0C of nw::snd::internal::BasicSound
void nw::snd::internal::SequenceSound::Initialize()
{
}

// 0x004C8070 slot 0x10 | slot vf_0x10 of nw::snd::internal::BasicSound
void nw::snd::internal::SequenceSound::Finalize()
{
}

// 0x0073F404 slot 0x14 | fefates:bytes
void nw::snd::internal::SequenceSound::IsPrepared() const
{
}

// 0x004C7EB4 slot 0x18 | slot vf_0x18 of nw::snd::internal::BasicSound
void nw::snd::internal::SequenceSound::IsAttachedTempSpecialHandle()
{
}

// 0x00132528 slot 0x1C | slot vf_0x1C of nw::snd::internal::BasicSound
void nw::snd::internal::SequenceSound::DetachTempSpecialHandle()
{
}

// 0x004C7EAC slot 0x20 | slot vf_0x20 of nw::snd::internal::BasicSound
void nw::snd::internal::SequenceSound::GetBasicSoundPlayerHandle()
{
}

// 0x004C7E20 slot 0x24 | slot vf_0x24 of nw::snd::internal::BasicSound
void nw::snd::internal::SequenceSound::OnUpdatePlayerPriority()
{
}

// 0x004C7B34 slot 0x2C | slot vf_0x2C of nw::snd::internal::BasicSound
void nw::snd::internal::SequenceSound::OnUpdateParam()
{
}

// 0x004C7B38 | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::SequenceSound::SetTempoRatio(float)
{
}

// 0x004C7B88 | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::SequenceSound::WriteVariable(int, short)
{
}

// 0x004C7D78 | fefates:bytes [tier B]
void nw::snd::internal::SequenceSound::RegisterDataLoadTask(const nw::snd::internal::driver::SequenceSoundLoader::LoadInfo&, const nw::snd::internal::driver::SequenceSoundPlayer::StartInfo&)
{
}

// 0x004C7EC4 | fefates:bytes [tier B]
void nw::snd::internal::SequenceSound::Setup(nw::snd::internal::driver::SequenceTrackAllocator*, unsigned int, nw::snd::internal::driver::NoteOnCallback*, int, bool, void (*)(unsigned short,nw::snd::SequenceUserprocCallbackParam*,void*), void*)
{
}

// 0x004C7F3C | nintendogs:callseq [tier A]
void nw::snd::internal::SequenceSound::Prepare(const void*, const nw::snd::internal::SequenceSound::StartInfo&, const nw::snd::internal::LoadItemInfo*, bool)
{
}

// 0x004C80D0 | fefates:bytes [tier B]
nw::snd::internal::SequenceSound::SequenceSound(nw::snd::internal::SoundInstanceManager<nw::snd::internal::SequenceSound>&)
{
}

} // namespace internal
} // namespace snd
} // namespace nw
