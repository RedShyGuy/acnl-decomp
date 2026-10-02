#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_SequenceSoundLoader.h"
#include "nw/snd/internal/snd_Task.h"

// RTTI N2nw3snd8internal6driver19SequenceSoundLoader12DataLoadTaskE @ 0x008D0B04
// vtable 0x00903454 (vptr 0x0090345C), offset_to_top 0, 3 entries
class nw::snd::internal::driver::SequenceSoundLoader::DataLoadTask : public ::nw::snd::internal::Task
{
public:
    DataLoadTask(); // ctor candidate(s) 0x004C1150 (unverified)
    virtual void vf_0x00(); // 0x004D265C slot 0x00 | virtual slot, introduced by nw::snd::internal::Task
    virtual void vf_0x04(); // 0x004D264C slot 0x04 | virtual slot, introduced by nw::snd::internal::Task
    virtual void Execute(); // 0x004D240C slot 0x08 | fefates:bytes
};
