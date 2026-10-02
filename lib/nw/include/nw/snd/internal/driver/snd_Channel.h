#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_MultiVoice.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
class Channel
{
public:
    class Disposer;
    struct ChannelCallbackStatus { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void VoiceCallbackFunc(nw::snd::internal::driver::MultiVoice*, nw::snd::internal::driver::MultiVoice::VoiceCallbackStatus, void*); // 0x004CCC2C | fefates:bytes [tier B]
    void FreeChannel(nw::snd::internal::driver::Channel*); // 0x004D3C70 | nintendogs:callgraph [tier A]
    void SetLoopFlag(bool); // 0x004D3C88 | fefates:bytes [tier B]
    void UpdateSweep(int); // 0x004D3CE4 | fefates:bytes [tier B]
    void AllocChannel(int, int, void(*)(nw::snd::internal::driver::Channel*, nw::snd::internal::driver::Channel::ChannelCallbackStatus, unsigned), unsigned); // 0x004D3D04 | nintendogs:bytes-fuzzy [tier A]
    void SetSweepParam(float, int, bool); // 0x004D3D8C | fefates:bytes [tier B]
    void AppendWaveBuffer(const nw::snd::internal::WaveInfo&, unsigned); // 0x004D3DB0 | nintendogs:callseq [tier A]
    void Stop(); // 0x004D4018 | nintendogs:callseq [tier A]
    void Start(const nw::snd::internal::WaveInfo&, int, unsigned int); // 0x004D4088 | fefates:bytes [tier B]
    void Update(bool); // 0x004D4118 | nintendogs:callseq [tier A]
    void NoteOff(); // 0x004D44C4 | fefates:bytes [tier B]
    void Release(); // 0x004D4514 | fefates:bytes [tier B]
    void InitParam(void (*)(nw::snd::internal::driver::Channel*,nw::snd::internal::driver::Channel::ChannelCallbackStatus,unsigned int), unsigned int); // 0x004D46C8 | fefates:bytes [tier B]
    Channel(); // 0x004D4800 | fefates:bytes [tier B]
    ~Channel(); // 0x004D489C | fefates:bytes [tier B]
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
