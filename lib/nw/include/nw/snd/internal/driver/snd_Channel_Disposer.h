#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_Channel.h"
#include "nw/snd/internal/driver/snd_DisposeCallback.h"

// RTTI N2nw3snd8internal6driver7Channel8DisposerE @ 0x008D0B4C
// vtable 0x009034E8 (vptr 0x009034F0), offset_to_top 0, 3 entries
class nw::snd::internal::driver::Channel::Disposer : public ::nw::snd::internal::driver::DisposeCallback
{
public:
    Disposer(); // ctor candidate(s) 0x004D4800 (unverified)
    virtual ~Disposer(); // 0x004D46C4 slot 0x00 | slot vf_0x00 of nw::snd::internal::driver::DisposeCallback
    virtual void vf_0x04(); // 0x004D46C0 slot 0x04 | virtual slot, introduced by nw::snd::internal::driver::DisposeCallback
    virtual void InvalidateData(const void*, const void*); // 0x004D4558 slot 0x08 | nintendogs:callseq
};
