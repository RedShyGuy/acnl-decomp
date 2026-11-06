#include "Sound/dSoundTv03Drama.h"
#include "Sound/dSoundLabel.h"

// 0x00279078
// 0x00279068 (deleting dtor)
SoundTv03Drama::~SoundTv03Drama()
{
    // (only the base is destroyed)
}

// 0x00278EB4
void SoundTv03Drama::Start()
{
    m_ItemId = GROUP_TV_03;
    m_Episode = 0;
    SoundTvBase::Start();
}

// 0x00279058
void SoundTv03Drama::OnLoaded()
{
    PlaySound(0, SE_TV_03_HATO);
}

// 0x00278F28
void SoundTv03Drama::UpdateProgram(s32 frame)
{
    switch (frame % 540) {
        case 1:
            if (m_Episode == 0) {
                PlaySound(1, SE_TV_03_v0);
            }
            break;
        case 180:
            if (m_Episode == 1) {
                PlaySound(1, SE_TV_03_v1);
            }
            break;
        case 285:
            if (m_Episode == 0) {
                PlaySound(1, SE_TV_03_v2);
            }
            break;
        case 315:
            if (m_Episode == 1) {
                PlaySound(1, SE_TV_03_v4);
            }
            break;
        case 450:
            if (m_Episode == 0) {
                PlaySound(1, SE_TV_03_v3);
            }
            break;
        case 523:
            m_Episode++;
            if (m_Episode == 2) {
                m_Episode = 0;
            }
            break;
    }
}
