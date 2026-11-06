#include "Sound/dSoundTv255Weather.h"
#include "Sound/dSoundLabel.h"
#include "netgame/dGameFunctions.h"

// 0x002D3F70
// 0x002D3F60 (deleting dtor)
SoundTv255Weather::~SoundTv255Weather()
{
    // (only the base is destroyed)
}

// 0x002D3D78
void SoundTv255Weather::Start()
{
    m_ItemId = GROUP_TV_255;
    m_Day = 0;
    m_Song = netgame::Func2FF63C(3);
    m_Report = 0;
    SoundTvBase::Start();
}

// 0x002D3F48
void SoundTv255Weather::OnLoaded()
{
    PlaySound(0, SE_TV_255_KK_SWING + m_Song);
}

// 0x002D3DFC
void SoundTv255Weather::UpdateProgram(s32 frame)
{
    switch (frame % 600) {
        case 15:
            if (m_Report == 0) {
                PlaySound(1, SE_TV_255_v00);
            } else if (m_Report == 1) {
                PlaySound(1, SE_TV_255_v10);
            }
            break;
        case 195:
            if (m_Report == 0) {
                PlaySound(1, SE_TV_255_v01);
            }
            break;
        case 225:
            if (m_Report == 1) {
                PlaySound(1, SE_TV_255_v11);
            }
            break;
        case 599:
            m_Day++;
            if (m_Day == 7) {
                m_Day = 0;
                u32 song;
                do {
                    song = netgame::Func2FF63C(3);
                } while (m_Song == song);
                m_Song = song;
                PlaySound(0, SE_TV_255_KK_SWING + m_Song);
            }
            m_Report++;
            if (m_Report == 3) {
                m_Report = 0;
            }
            break;
    }
}
