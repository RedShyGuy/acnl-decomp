#include "sead/seadAudioFx.h"

namespace sead {
// 0x001391DC | nintendogs:bytes [tier B]
sead::AudioFx::AudioFx() : mMemoryMgr(nullptr), mHolder(nullptr), mBus(AudioGlobal::AUX_BUS_NONE), mNode()
{
}

// 0x0014397C slot 0x08
// 0x0055DF98 (deleting dtor)
sead::AudioFx::~AudioFx()
{
    // empty (in the original too)
}
} // namespace sead
