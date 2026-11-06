#include "Sound/dSoundTv15AppleCM.h"
#include "Sound/dSoundLabel.h"

// 0x002BE894
// 0x002BE884 (deleting dtor)
SoundTv15AppleCM::~SoundTv15AppleCM()
{
    // (only the base is destroyed)
}

// 0x002BE808
void SoundTv15AppleCM::Start()
{
    m_ItemId = GROUP_TV_15;
    SoundTvBase::Start();
}

// 0x002BE874
void SoundTv15AppleCM::OnLoaded()
{
    PlaySound(0, SE_TV_15);
}
