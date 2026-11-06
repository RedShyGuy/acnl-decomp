#include "Sound/dSoundTv21LaundryCM.h"
#include "Sound/dSoundLabel.h"

// 0x002EA2B8
// 0x002EA2A8 (deleting dtor)
SoundTv21LaundryCM::~SoundTv21LaundryCM()
{
    // (only the base is destroyed)
}

// 0x002EA020
void SoundTv21LaundryCM::Start()
{
    m_ItemId = GROUP_TV_21;
    SoundTvBase::Start();
    m_Scene = -1;
}

// 0x002EA210
void SoundTv21LaundryCM::SetScene(s8 scene, s8 variant)
{
    if (m_Scene == scene) {
        return;
    }
    PlaySound(1, SE_TV_21);
    if (GetHandle(1).m_pSound != NULL) {
        WriteVariable(1, 0, scene);
        WriteVariable(1, 1, variant);
    }
    m_Scene = scene;
}

// 0x002EA094
void SoundTv21LaundryCM::UpdateProgram(s32 frame)
{
    switch (frame) {
        case 3:
            SetScene(0, 0);
            break;
        case 25:
            SetScene(1, 0);
            break;
        case 39:
            SetScene(2, 0);
            break;
        case 67:
            SetScene(3, 0);
            break;
        case 92:
            SetScene(4, 0);
            break;
        case 109:
            SetScene(5, 0);
            break;
        case 129:
            SetScene(6, 0);
            break;
        case 140:
            PlaySound(0, SE_TV_21_WASH);
            break;
        case 151:
            SetScene(7, 0);
            break;
        case 196:
            SetScene(8, 0);
            break;
        case 210:
            PlaySound(0, SE_TV_21_CLEAN);
            break;
        case 221:
            SetScene(9, 0);
            break;
        case 261:
            SetScene(10, 0);
            break;
        case 280:
            PlaySound(0, SE_TV_21_DRY);
            break;
        case 310:
            SetScene(11, 0);
            break;
        case 332:
            SetScene(12, 0);
            break;
        case 370:
            PlaySound(0, SE_TV_21_LOGO);
            break;
    }
}
