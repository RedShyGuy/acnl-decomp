#include "nw/snd/internal/driver/snd_MultiVoice.h"
#include "nw/snd/internal/driver/snd_Channel.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004CCC2C | fefates:bytes [tier B]
void nw::snd::internal::driver::Channel::VoiceCallbackFunc(nw::snd::internal::driver::MultiVoice*, nw::snd::internal::driver::MultiVoice::VoiceCallbackStatus, void*)
{
}

// 0x004D3C70 | nintendogs:callgraph [tier A]
void nw::snd::internal::driver::Channel::FreeChannel(nw::snd::internal::driver::Channel*)
{
}

// 0x004D3C88 | fefates:bytes [tier B]
void nw::snd::internal::driver::Channel::SetLoopFlag(bool)
{
}

// 0x004D3CE4 | fefates:bytes [tier B]
void nw::snd::internal::driver::Channel::UpdateSweep(int)
{
}

// 0x004D3D04 | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::driver::Channel::AllocChannel(int, int, void(*)(nw::snd::internal::driver::Channel*, nw::snd::internal::driver::Channel::ChannelCallbackStatus, unsigned), unsigned)
{
}

// 0x004D3D8C | fefates:bytes [tier B]
void nw::snd::internal::driver::Channel::SetSweepParam(float, int, bool)
{
}

// 0x004D3DB0 | nintendogs:callseq [tier A]
void nw::snd::internal::driver::Channel::AppendWaveBuffer(const nw::snd::internal::WaveInfo&, unsigned)
{
}

// 0x004D4018 | nintendogs:callseq [tier A]
void nw::snd::internal::driver::Channel::Stop()
{
}

// 0x004D4088 | fefates:bytes [tier B]
void nw::snd::internal::driver::Channel::Start(const nw::snd::internal::WaveInfo&, int, unsigned int)
{
}

// 0x004D4118 | nintendogs:callseq [tier A]
void nw::snd::internal::driver::Channel::Update(bool)
{
}

// 0x004D44C4 | fefates:bytes [tier B]
void nw::snd::internal::driver::Channel::NoteOff()
{
}

// 0x004D4514 | fefates:bytes [tier B]
void nw::snd::internal::driver::Channel::Release()
{
}

// 0x004D46C8 | fefates:bytes [tier B]
void nw::snd::internal::driver::Channel::InitParam(void (*)(nw::snd::internal::driver::Channel*,nw::snd::internal::driver::Channel::ChannelCallbackStatus,unsigned int), unsigned int)
{
}

// 0x004D4800 | fefates:bytes [tier B]
nw::snd::internal::driver::Channel::Channel()
{
}

// 0x004D489C | fefates:bytes [tier B]
nw::snd::internal::driver::Channel::~Channel()
{
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
