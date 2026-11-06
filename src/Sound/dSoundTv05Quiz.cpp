#include "Sound/dSoundTv05Quiz.h"
#include "Sound/dSoundLabel.h"

// 0x00248378
// 0x00248368 (deleting dtor)
SoundTv05Quiz::~SoundTv05Quiz()
{
    // (only the base is destroyed)
}

// 0x00247E88
void SoundTv05Quiz::Start()
{
    m_ItemId = GROUP_TV_05;
    m_Round = 0;
    SoundTvBase::Start();
}

// 0x00247EFC
void SoundTv05Quiz::UpdateProgram(s32 frame)
{
    s32 t = frame % 720;
    if (GetHandle(1).m_pSound == NULL) {
        // the background music again, at the part of the program
        PlaySound(1, SE_TV_05_BG);
        if (static_cast<u32>(t - 105) < 225) {
            WriteVariable(1, 1, 1);
        } else if (static_cast<u32>(t - 330) < 90) {
            WriteVariable(1, 1, 0);
        } else if (static_cast<u32>(t - 420) < 135) {
            WriteVariable(1, 1, 1);
        } else if (t >= 555) {
            WriteVariable(1, 1, 0);
        }
    }
    switch (t) {
        case 1:
            if (m_Round != 0) {
                PlaySound(0, SE_TV_05_MC1);
            } else {
                PlaySound(0, SE_TV_05_MC0);
            }
            break;
        case 105:
            WriteVariable(1, 1, 1);
            break;
        case 315:
            WriteVariable(1, 2, 1);
            break;
        case 330:
            WriteVariable(1, 1, 0);
            break;
        case 345:
            if (m_Round != 0) {
                PlaySound(0, SE_TV_05_RES1);
            } else {
                PlaySound(0, SE_TV_05_RES0);
            }
            break;
        case 380:
            if (m_Round == 1) {
                WriteVariable(1, 2, 2);
            }
            break;
        case 387:
            if (m_Round == 0) {
                WriteVariable(1, 2, 2);
            }
            break;
        case 420:
            WriteVariable(1, 1, 1);
            break;
        case 555:
            WriteVariable(1, 1, 0);
            WriteVariable(1, 2, 1);
            break;
        case 575:
            if (m_Round == 1) {
                WriteVariable(0, 0, 2);
            }
            break;
        case 585:
            if (m_Round == 0) {
                WriteVariable(0, 0, 2);
            }
            break;
        case 608:
            if (m_Round == 1) {
                WriteVariable(1, 2, 3);
            }
            break;
        case 638:
            if (m_Round == 0) {
                WriteVariable(1, 2, 3);
            }
            break;
        case 719:
            m_Round++;
            if (m_Round == 2) {
                m_Round = 0;
            }
            break;
    }
}
