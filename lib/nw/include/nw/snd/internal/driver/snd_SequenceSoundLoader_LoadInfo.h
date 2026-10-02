#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_SequenceSoundLoader.h"

class nw::snd::internal::driver::SequenceSoundLoader::LoadInfo
{
public:
    LoadInfo(); // TODO: default ctor added so derived stubs compile - may not exist
    LoadInfo(const nw::snd::SoundArchive*, const nw::snd::SoundDataManager*, nw::snd::internal::LoadItemInfo*, nw::snd::internal::LoadItemInfo*, nw::snd::SoundPlayer*); // 0x004D2694 | fefates:bytes [tier B]
};
