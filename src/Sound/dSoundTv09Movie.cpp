#include "Sound/dSoundTv09Movie.h"
#include "Sound/dSoundLabel.h"

// 0x00279324
// 0x00279314 (deleting dtor)
SoundTv09Movie::~SoundTv09Movie()
{
    // (only the base is destroyed)
}

// 0x0027907C
void SoundTv09Movie::Start()
{
    m_ItemId = GROUP_TV_09;
    SoundTvBase::Start();
}

// 0x002792E4
void SoundTv09Movie::OnLoaded()
{
    PlaySound(0, SE_TV_09_L0);
    PlaySound(1, SE_TV_09_S0);
}

// 0x002790E8
void SoundTv09Movie::UpdateProgram(s32 frame)
{
    switch (frame % 450) {
        case 0:
            if (GetHandle(0).m_pSound == NULL) {
                PlaySound(0, SE_TV_09_L0);
            }
            if (GetHandle(1).m_pSound == NULL) {
                PlaySound(1, SE_TV_09_S0);
            }
            WriteVariable(0, 0, 1);
            WriteVariable(1, 0, 1);
            break;
        case 135:
            WriteVariable(0, 0, 2);
            WriteVariable(1, 0, 2);
            break;
        case 210:
            WriteVariable(0, 0, 3);
            break;
        case 337:
            WriteVariable(0, 0, 4);
            break;
        case 352:
            WriteVariable(0, 0, 5);
            break;
        case 367:
            WriteVariable(1, 0, 3);
            break;
    }
}
