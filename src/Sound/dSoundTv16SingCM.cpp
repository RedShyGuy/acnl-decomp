#include "Sound/dSoundTv16SingCM.h"
#include "Sound/dSoundLabel.h"

// 0x002A64E0
// 0x002A64D0 (deleting dtor)
SoundTv16SingCM::~SoundTv16SingCM()
{
    // (only the base is destroyed)
}

// 0x002A6454
void SoundTv16SingCM::Start()
{
    m_ItemId = GROUP_TV_16;
    SoundTvBase::Start();
}

// 0x002A64C0
void SoundTv16SingCM::OnLoaded()
{
    PlaySound(0, SE_TV_16);
}
