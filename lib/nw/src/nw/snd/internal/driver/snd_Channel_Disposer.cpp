#include "nw/snd/internal/driver/snd_DisposeCallback.h"
#include "nw/snd/internal/driver/snd_Channel_Disposer.h"

// ctor candidate(s) 0x004D4800 (unverified)
nw::snd::internal::driver::Channel::Disposer::Disposer()
{
}

// 0x004D46C4 slot 0x00 | slot vf_0x00 of nw::snd::internal::driver::DisposeCallback
nw::snd::internal::driver::Channel::Disposer::~Disposer()
{
}

// 0x004D46C0 slot 0x04 | virtual slot, introduced by nw::snd::internal::driver::DisposeCallback
void nw::snd::internal::driver::Channel::Disposer::vf_0x04()
{
}

// 0x004D4558 slot 0x08 | nintendogs:callseq
void nw::snd::internal::driver::Channel::Disposer::InvalidateData(const void*, const void*)
{
}

