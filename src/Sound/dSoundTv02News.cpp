#include "Sound/dSoundTv02News.h"
#include "Sound/dSoundLabel.h"

// 0x001DBC08
// 0x00247CC8 (deleting dtor)
SoundTv02News::~SoundTv02News()
{
    // (only the base is destroyed)
}

// 0x002478A4
void SoundTv02News::Start()
{
    m_ItemId = GROUP_TV_02;
    m_Part = -1;
    SoundTvBase::Start();
}

// 0x00247CB8
void SoundTv02News::OnLoaded()
{
    PlaySound(1, SE_TV_02_v00);
}

// 0x00247918
void SoundTv02News::UpdateProgram(s32 frame)
{
    switch (frame % 600) {
        case 1:
            if (GetHandle(1).m_pSound == NULL) {
                PlaySound(1, SE_TV_02_v00);
            }
            m_Part++;
            if (m_Part == 3) {
                m_Part = 0;
            }
            switch (m_Part) {
                case 0:
                    PlaySound(0, SE_TV_02_b0);
                    break;
                case 1:
                    PlaySound(0, SE_TV_02_KK_DRUMN);
                    break;
                case 2:
                    m_Obj.SetVolume(0, 40.0f / 127.0f, 45);
                    break;
            }
            break;
        case 60:
            WriteVariable(0, 1, 1);
            break;
        case 75:
            WriteVariable(0, 0, 2);
            break;
        case 225:
            PlaySound(1, SE_TV_02_v10);
            WriteVariable(0, 0, 0);
            break;
        case 300:
            PlaySound(1, SE_TV_02_v20);
            WriteVariable(0, 1, 2);
            break;
        case 450:
            WriteVariable(0, 0, 3);
            break;
        case 562:
            PlaySound(1, SE_TV_02_v30);
            break;
        case 577:
            if (m_Part == 2) {
                GetHandle(0).Stop(45);
            }
            break;
    }
    switch (m_Part) {
        case 0:
            SetTrackVolume(0, 0xFFF, 1.0f);
            SetTrackVolume(1, 0xFFF, 1.0f);
            break;
        case 1:
            SetTrackVolume(0, 0xFFF, 100.0f / 127.0f);
            SetTrackVolume(1, 0xFFF, 0.0f);
            break;
        case 2:
            SetTrackVolume(1, 0xFFF, 100.0f / 127.0f);
            break;
    }
}
