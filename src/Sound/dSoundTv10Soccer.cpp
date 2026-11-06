#include "Sound/dSoundTv10Soccer.h"
#include "Sound/dSoundLabel.h"
#include "netgame/dGameFunctions.h"

// 0x002A6450
// 0x002A6440 (deleting dtor)
SoundTv10Soccer::~SoundTv10Soccer()
{
    // (only the base is destroyed)
}

// 0x002A5CDC
void SoundTv10Soccer::Start()
{
    m_ItemId = GROUP_TV_10;
    m_IsChanged = 0;
    m_State = 0;
    m_LastState = 0;
    m_StateFrames = 0;
    SoundTvBase::Start();
}

// 0x002A6430
void SoundTv10Soccer::OnLoaded()
{
    PlaySound(0, SE_TV_10_NORMAL);
}

// 0x002A5D5C
void SoundTv10Soccer::UpdateProgram(s32 frame)
{
    s32 t = frame % 720;
    // the chances of the groups and the kinds of play
    s32 groupA = 0;
    s32 groupB = 0;
    s32 normal = 0;
    s32 call = 0;
    s32 song = 0;
    s32 finePlay = 0;
    u16 frames = ++m_StateFrames;
    u16 state = m_State;
    if ((state == STATE_GOAL && frames >= 210) || (state == STATE_MISS && frames >= 150)) {
        m_IsChanged = 1;
        m_StateFrames = 0;
        groupA = 50;
        groupB = 75;
        normal = 25;
        call = 50;
        song = 75;
        finePlay = 88;
    } else {
        u32 kind = state % KIND_NUM;
        if (kind <= 2 && frames >= 180) {
            m_IsChanged = 1;
            m_StateFrames = 0;
            if (state < 5) {
                groupA = 50;
                groupB = 75;
            } else if (static_cast<u32>(state - 5) < 5) {
                groupA = 40;
                groupB = 80;
            } else {
                groupA = 40;
                groupB = 60;
            }
            normal = 25;
            call = 50;
            song = 75;
            finePlay = 88;
        } else if (kind >= 3 && frames >= 105) {
            m_IsChanged = 1;
            m_StateFrames = 0;
            if (state < 5) {
                groupA = 50;
                groupB = 75;
            } else if (static_cast<u32>(state - 5) < 5) {
                groupA = 40;
                groupB = 80;
            } else {
                groupA = 40;
                groupB = 60;
            }
            normal = 35;
            call = 70;
            song = 100;
            finePlay = 100;
        }
    }
    if (m_IsChanged != 1) {
        return;
    }
    GetHandle(1).Stop(0);
    m_LastState = m_State;
    s32 random = netgame::Func2FF63C(100);
    s32 group;
    if (random <= groupA) {
        group = 0;
    } else if (random <= groupB) {
        group = 5;
    } else {
        group = 10;
    }
    random = netgame::Func2FF63C(100);
    u32 sound;
    if (random <= normal) {
        m_State = group;
        sound = SE_TV_10_NORMAL;
    } else if (random <= call) {
        m_State = group + 1;
        sound = SE_TV_10_CALL;
    } else if (random <= song) {
        m_State = group + 2;
        sound = SE_TV_10_SONG;
    } else if (random <= finePlay) {
        m_State = group + 3;
        sound = SE_TV_10_FINEPLAY;
    } else {
        m_State = group + 4;
        sound = SE_TV_10_ROUGHPLAY;
    }
    if (m_LastState % KIND_NUM != m_State % KIND_NUM) {
        GetHandle(0).Stop(0);
        PlaySound(0, sound);
    }
    if (group < 5) {
        SetTrackVolume(0, 3, 100.0f / 127.0f);
        if (netgame::Func2FF63C(100) < 40 && t > 45) {
            PlaySound(1, SE_TV_10_RUN);
            SetTrackVolume(1, 3, 100.0f / 127.0f);
        }
    } else if (group / 5 == 1) {
        SetTrackVolume(0, 3, 75.0f / 127.0f);
        PlaySound(1, SE_TV_10_TALK);
    } else {
        SetTrackVolume(0, 3, 1.0f);
        if (netgame::Func2FF63C(100) < 40 && t > 45 && m_State <= 13) {
            PlaySound(1, SE_TV_10_RUN);
            SetTrackVolume(1, 3, 80.0f / 127.0f);
        }
    }
    // a chance near the goal
    if (m_LastState == 11 || m_LastState == 13) {
        s32 chance = netgame::Func2FF63C(100);
        if (chance <= 20) {
            GetHandle(0).Stop(0);
            PlaySound(0, SE_TV_10_GOAL);
            m_State = STATE_GOAL;
        } else if (chance <= 40) {
            GetHandle(0).Stop(0);
            PlaySound(0, SE_TV_10_MISS);
            m_State = STATE_MISS;
        }
    }
    m_IsChanged = 0;
}
