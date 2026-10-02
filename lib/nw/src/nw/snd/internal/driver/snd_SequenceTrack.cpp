#include "nw/snd/internal/driver/snd_Channel.h"
#include "nw/snd/internal/driver/snd_SequenceTrack.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004CCA8C slot 0x00 | fefates:bytes
nw::snd::internal::driver::SequenceTrack::~SequenceTrack()
{
}

// 0x004CCA0C slot 0x04 | virtual slot, introduced by nw::snd::internal::driver::SequenceTrack
void nw::snd::internal::driver::SequenceTrack::vf_0x04()
{
}

// 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
void nw::snd::internal::driver::SequenceTrack::Parse(bool)
{
}

// 0x004CBBF0 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::SequenceTrack::SetSeqData(const void*, int)
{
}

// 0x004CBC00 | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceTrack::SetSilence(bool, int)
{
}

// 0x004CBCAC | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceTrack::ParseNextTick(bool)
{
}

// 0x004CBDDC | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::SequenceTrack::FreeAllChannel()
{
}

// 0x004CBE10 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::SequenceTrack::GetVariablePtr(int)
{
}

// 0x004CBE2C | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceTrack::PauseAllChannel(bool)
{
}

// 0x004CBEA4 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::SequenceTrack::ReleaseAllChannel(int)
{
}

// 0x004CBEF8 | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceTrack::UpdateChannelParam()
{
}

// 0x004CC290 | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceTrack::ChannelCallbackFunc(nw::snd::internal::driver::Channel*, nw::snd::internal::driver::Channel::ChannelCallbackStatus, unsigned int)
{
}

// 0x004CC30C | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceTrack::UpdateChannelLength()
{
}

// 0x004CC388 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::SequenceTrack::Open()
{
}

// 0x004CC3A4 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::SequenceTrack::Close()
{
}

// 0x004CC408 | nintendogs:callgraph [tier A]
void nw::snd::internal::driver::SequenceTrack::NoteOn(int, int, int, bool)
{
}

// 0x004CC720 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::SequenceTrack::SetMute(nw::snd::SeqMute)
{
}

// 0x004CC7EC | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceTrack::InitParam()
{
}

// 0x004CC96C | fefates:bytes [tier B]
nw::snd::internal::driver::SequenceTrack::SequenceTrack()
{
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
