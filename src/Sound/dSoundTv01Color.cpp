#include "Sound/dSoundTv01Color.h"
#include "Sound/dSoundLabel.h"

// 0x00278EB0
// 0x00278EA0 (deleting dtor)
SoundTv01Color::~SoundTv01Color()
{
    // (only the base is destroyed)
}

// 0x00278E24
void SoundTv01Color::Start()
{
    m_ItemId = GROUP_TV_01;
    SoundTvBase::Start();
}

// 0x00278E90
void SoundTv01Color::OnLoaded()
{
    PlaySound(0, SE_TV_01);
}
