#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
class SoundThread
{
public:
    class PlayerCallback;
    class SoundFrameCallback;
    void UnregisterPlayerCallback(nw::snd::internal::driver::SoundThread::PlayerCallback*); // 0x00140F34 | nintendogs:callseq-callee [tier A]
    void SoundThreadFunc(unsigned); // 0x004CB43C | nintendogs:bytes [tier A]
    void FrameProcess(); // 0x004CB4B8 | fefates:bytes [tier B]
    void CalcProcessCost(const nn::os::Tick&); // 0x004CB620 | nintendogs:callgraph [tier A]
    void SetProfileBuffer(nw::snd::SoundThreadProfile*, int); // 0x004CB854 | nintendogs:bytes [tier A]
    void CreateSoundThread(unsigned, unsigned, int, unsigned, unsigned, int, int, bool); // 0x004CB8A8 | nintendogs:callseq [tier A]
    void ClearProfileBuffer(); // 0x004CB9E4 | nintendogs:bytes [tier A]
    void UserThreadCallback(unsigned); // 0x004CBA18 | nintendogs:bytes [tier A]
    void RegisterPlayerCallback(nw::snd::internal::driver::SoundThread::PlayerCallback*); // 0x004CBA38 | nintendogs:bytes [tier A]
    void RegisterSoundFrameCallback(nw::snd::internal::driver::SoundThread::SoundFrameCallback*); // 0x004CBA4C | fefates:bytes [tier B]
    void UnregisterSoundFrameCallback(nw::snd::internal::driver::SoundThread::SoundFrameCallback*); // 0x004CBA80 | fefates:bytes [tier B]
    void Destroy(); // 0x004CBAB0 | nintendogs:bytes [tier A]
    void Finalize(); // 0x004CBADC | nintendogs:bytes [tier A]
    SoundThread(); // 0x004CBB44 | nintendogs:bytes [tier A]
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
