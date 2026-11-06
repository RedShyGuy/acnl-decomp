#include "Sound/dSoundTv04Song.h"
#include "Sound/dSoundLabel.h"

// 0x00247E84
// 0x00247E74 (deleting dtor)
SoundTv04Song::~SoundTv04Song()
{
    // (only the base is destroyed)
}

// 0x00247CD8
void SoundTv04Song::Start()
{
    m_ItemId = GROUP_TV_04;
    m_Verse = -1;
    m_Variant = 0;
    SoundTvBase::Start();
}

// 0x00247E64
void SoundTv04Song::OnLoaded()
{
    PlaySound(1, SE_TV_04_KK_JAZZ);
}

// 0x00247D54
void SoundTv04Song::UpdateProgram(s32 frame)
{
    s32 variant = m_Variant;
    s16 verse;
    ReadVariable(1, 0, &verse);
    s32 current;
    if (m_Verse == verse) {
        current = -1;
        m_Verse = verse;
    } else {
        current = verse;
        m_Verse = verse;
    }
    switch (current) {
        case 1:
            PlaySound(0, SE_TV_04_v0 + variant);
            break;
        case 2:
            PlaySound(0, SE_TV_04_v1 + variant);
            break;
        case 3:
            PlaySound(0, SE_TV_04_v2 + variant);
            break;
        case 4:
            PlaySound(0, SE_TV_04_v3 + variant);
            m_Variant = (m_Variant + 1) % 2;
            break;
    }
}
