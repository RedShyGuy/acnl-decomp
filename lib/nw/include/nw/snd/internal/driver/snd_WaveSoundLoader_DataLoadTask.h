#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_WaveSoundLoader.h"
#include "nw/snd/internal/snd_Task.h"

// RTTI N2nw3snd8internal6driver15WaveSoundLoader12DataLoadTaskE @ 0x008D0A84
// vtable 0x00903334 (vptr 0x0090333C), offset_to_top 0, 3 entries
class nw::snd::internal::driver::WaveSoundLoader::DataLoadTask : public ::nw::snd::internal::Task
{
public:
    DataLoadTask(); // ctor candidate(s) 0x004C0D84 (unverified)
    virtual void vf_0x00(); // 0x004C9BB8 slot 0x00 | virtual slot, introduced by nw::snd::internal::Task
    virtual void vf_0x04(); // 0x004CD9A8 slot 0x04 | virtual slot, introduced by nw::snd::internal::Task
    virtual void Execute(); // 0x004CD88C slot 0x08 | fefates:bytes
};
