#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_Channel.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// RTTI N2nw3snd8internal6driver13SequenceTrackE @ 0x008D0A6C
// vtable 0x0090330C (vptr 0x00903314), offset_to_top 0, 3 entries
class SequenceTrack
{
public:
    virtual ~SequenceTrack(); // 0x004CCA8C slot 0x00 | fefates:bytes
    virtual void vf_0x04(); // 0x004CCA0C slot 0x04 | virtual slot, introduced by nw::snd::internal::driver::SequenceTrack
    virtual void Parse(bool); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
    void SetSeqData(const void*, int); // 0x004CBBF0 | nintendogs:callseq-callee [tier A]
    void SetSilence(bool, int); // 0x004CBC00 | fefates:bytes [tier B]
    void ParseNextTick(bool); // 0x004CBCAC | fefates:bytes [tier B]
    void FreeAllChannel(); // 0x004CBDDC | nintendogs:callseq-callee [tier A]
    void GetVariablePtr(int); // 0x004CBE10 | nintendogs:callseq-callee [tier A]
    void PauseAllChannel(bool); // 0x004CBE2C | fefates:bytes [tier B]
    void ReleaseAllChannel(int); // 0x004CBEA4 | nintendogs:callseq-callee [tier A]
    void UpdateChannelParam(); // 0x004CBEF8 | fefates:bytes [tier B]
    void ChannelCallbackFunc(nw::snd::internal::driver::Channel*, nw::snd::internal::driver::Channel::ChannelCallbackStatus, unsigned int); // 0x004CC290 | fefates:bytes [tier B]
    void UpdateChannelLength(); // 0x004CC30C | fefates:bytes [tier B]
    void Open(); // 0x004CC388 | nintendogs:callseq-callee [tier A]
    void Close(); // 0x004CC3A4 | nintendogs:callseq-callee [tier A]
    void NoteOn(int, int, int, bool); // 0x004CC408 | nintendogs:callgraph [tier A]
    void SetMute(nw::snd::SeqMute); // 0x004CC720 | nintendogs:callseq-callee [tier A]
    void InitParam(); // 0x004CC7EC | fefates:bytes [tier B]
    SequenceTrack(); // 0x004CC96C | fefates:bytes [tier B]
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
