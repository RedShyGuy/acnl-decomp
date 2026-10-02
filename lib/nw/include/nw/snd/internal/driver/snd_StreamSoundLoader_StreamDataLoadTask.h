#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_StreamSoundLoader.h"
#include "nw/snd/internal/snd_Task.h"

// RTTI N2nw3snd8internal6driver17StreamSoundLoader18StreamDataLoadTaskE @ 0x008D0ACC
// vtable 0x009033E0 (vptr 0x009033E8), offset_to_top 0, 3 entries
class nw::snd::internal::driver::StreamSoundLoader::StreamDataLoadTask : public ::nw::snd::internal::Task
{
public:
    virtual void vf_0x00(); // 0x004CF67C slot 0x00 | virtual slot, introduced by nw::snd::internal::Task
    virtual void vf_0x04(); // 0x004CF66C slot 0x04 | virtual slot, introduced by nw::snd::internal::Task
    virtual void Execute(); // 0x004CF548 slot 0x08 | fefates:bytes
    StreamDataLoadTask(); // 0x004CF640 | fefates:bytes [tier B]
};
