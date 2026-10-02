#include "nw/snd/internal/driver/snd_SoundThread.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x00140F34 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::SoundThread::UnregisterPlayerCallback(nw::snd::internal::driver::SoundThread::PlayerCallback*)
{
}

// 0x004CB43C | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SoundThread::SoundThreadFunc(unsigned)
{
}

// 0x004CB4B8 | fefates:bytes [tier B]
void nw::snd::internal::driver::SoundThread::FrameProcess()
{
}

// 0x004CB620 | nintendogs:callgraph [tier A]
void nw::snd::internal::driver::SoundThread::CalcProcessCost(const nn::os::Tick&)
{
}

// 0x004CB854 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SoundThread::SetProfileBuffer(nw::snd::SoundThreadProfile*, int)
{
}

// 0x004CB8A8 | nintendogs:callseq [tier A]
void nw::snd::internal::driver::SoundThread::CreateSoundThread(unsigned, unsigned, int, unsigned, unsigned, int, int, bool)
{
}

// 0x004CB9E4 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SoundThread::ClearProfileBuffer()
{
}

// 0x004CBA18 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SoundThread::UserThreadCallback(unsigned)
{
}

// 0x004CBA38 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SoundThread::RegisterPlayerCallback(nw::snd::internal::driver::SoundThread::PlayerCallback*)
{
}

// 0x004CBA4C | fefates:bytes [tier B]
void nw::snd::internal::driver::SoundThread::RegisterSoundFrameCallback(nw::snd::internal::driver::SoundThread::SoundFrameCallback*)
{
}

// 0x004CBA80 | fefates:bytes [tier B]
void nw::snd::internal::driver::SoundThread::UnregisterSoundFrameCallback(nw::snd::internal::driver::SoundThread::SoundFrameCallback*)
{
}

// 0x004CBAB0 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SoundThread::Destroy()
{
}

// 0x004CBADC | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SoundThread::Finalize()
{
}

// 0x004CBB44 | nintendogs:bytes [tier A]
nw::snd::internal::driver::SoundThread::SoundThread()
{
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
