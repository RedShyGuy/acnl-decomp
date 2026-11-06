#include "Sound/dSoundObjFurnitureAudio.h"
#include "Sound/dSoundFurnitureMgr.h"
#include "Sound/dSoundRoomMusicPlayer.h"

namespace {
const s32 TYPE_AUDIO = 9;
const s32 STOP_FADE_FRAMES = 5;
} // namespace

// 0x00332268
SoundObjFurnitureAudio::SoundObjFurnitureAudio()
{
    // (only the base is constructed)
}

// 0x003322F8
// 0x00332280 (deleting dtor)
SoundObjFurnitureAudio::~SoundObjFurnitureAudio()
{
    SoundObjFurnitureAudio::Stop();
}

// 0x0033202C
void SoundObjFurnitureAudio::Start(const nn::math::VEC3* world, const nn::math::VEC3* relative, u32 kind, bool isOn)
{
    if (!m_IsActive) {
        m_Type = TYPE_AUDIO;
        SoundObjFurniture::Start(world, relative, kind, isOn);
    }
}

// 0x0033205C
void SoundObjFurnitureAudio::StopMusic(s32 fadeFrames)
{
    SoundRoomMusicPlayer* player = SoundRoomMusicPlayer::s_pInstance;
    if (player == NULL) {
        return;
    }
    if (player->m_pObj == this) {
        player->Stop(fadeFrames);
    }
    if (player->m_pObj == this) {
        player->m_pObj = NULL;
    }
}

// 0x003320A4
void SoundObjFurnitureAudio::PlayMusic(s8 isFromStart, u32 milliSeconds)
{
    SoundRoomMusicPlayer* player = SoundRoomMusicPlayer::s_pInstance;
    if (player != NULL) {
        player->Play(isFromStart, milliSeconds);
    }
}

// 0x003320C8
void SoundObjFurnitureAudio::LoadMusic(u32 song)
{
    SoundRoomMusicPlayer* player = SoundRoomMusicPlayer::s_pInstance;
    if (player != NULL) {
        player->m_pObj = this;
        player->Load(song);
    }
}

// 0x003320EC
void SoundObjFurnitureAudio::Update(const nn::math::VEC3* world, const nn::math::VEC3* relative, f32 frame)
{
    (void)frame;
    if (m_pInfo != NULL) {
        SoundRoomMusicPlayer* player = SoundRoomMusicPlayer::s_pInstance;
        if (player != NULL && player->IsPlaying()) {
            m_IsOn = true;
            HoldLoop();
            if (m_IsHeld) {
                m_IsHeld = false;
            } else {
                SoundFurnitureMgr::s_pInstance->Stop(m_Index);
            }
        } else {
            m_IsOn = false;
        }
    }
    SoundObj<2>::SetPosition(world, relative);
}

// 0x00332208
void SoundObjFurnitureAudio::Stop()
{
    if (!m_IsActive) {
        return;
    }
    SoundRoomMusicPlayer* player = SoundRoomMusicPlayer::s_pInstance;
    if (player == NULL) {
        return;
    }
    if (player->m_pObj == this) {
        player->Stop(STOP_FADE_FRAMES);
        if (player->m_pObj == this) {
            player->m_pObj = NULL;
        }
    }
    SoundObjFurniture::Stop();
}

// 0x00726368
s32 SoundObjFurnitureAudio::GetBiquadType() const
{
    switch (m_Kind) {
    case 242:
    case 254:
    case 264:
        return 66;
    case 243:
    case 244:
    case 253:
    case 258:
        return 67;
    case 246:
    case 247:
    case 248:
    case 252:
        return 68;
    case 249:
    case 260:
    case 261:
    case 262:
    case 266:
        return 69;
    }
    return 0;
}
