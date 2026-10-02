#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_StreamSoundLoader.h"
#include "nw/snd/internal/snd_Task.h"

// RTTI N2nw3snd8internal6driver17StreamSoundLoader20StreamHeaderLoadTaskE @ 0x008D0AD8
// vtable 0x009033F4 (vptr 0x009033FC), offset_to_top 0, 3 entries
class nw::snd::internal::driver::StreamSoundLoader::StreamHeaderLoadTask : public ::nw::snd::internal::Task
{
public:
    StreamHeaderLoadTask(); // ctor candidate(s) 0x004D0510 (unverified)
    virtual void vf_0x00(); // 0x004CF724 slot 0x00 | virtual slot, introduced by nw::snd::internal::Task
    virtual void vf_0x04(); // 0x004CF714 slot 0x04 | virtual slot, introduced by nw::snd::internal::Task
    virtual void Execute(); // 0x004CF680 slot 0x08 | fefates:bytes
};
