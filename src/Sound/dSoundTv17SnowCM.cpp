#include "Sound/dSoundTv17SnowCM.h"
#include "Sound/dSoundLabel.h"

// 0x002A6570
// 0x002A6560 (deleting dtor)
SoundTv17SnowCM::~SoundTv17SnowCM()
{
    // (only the base is destroyed)
}

// 0x002A64E4
void SoundTv17SnowCM::Start()
{
    m_ItemId = GROUP_TV_17;
    SoundTvBase::Start();
}

// 0x002A6550
void SoundTv17SnowCM::OnLoaded()
{
    PlaySound(0, SE_TV_17);
}
