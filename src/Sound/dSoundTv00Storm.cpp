#include "Sound/dSoundTv00Storm.h"
#include "Sound/dSoundLabel.h"

// 0x00278E20
// 0x00278E10 (deleting dtor)
SoundTv00Storm::~SoundTv00Storm()
{
    // (only the base is destroyed)
}

// 0x00278D94
void SoundTv00Storm::Start()
{
    m_ItemId = GROUP_TV_00;
    SoundTvBase::Start();
}

// 0x00278E00
void SoundTv00Storm::OnLoaded()
{
    PlaySound(0, SE_TV_00);
}
