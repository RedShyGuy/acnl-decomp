#include "Audio/bgm/dBgmRequestMgr.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Sound/dSoundMgr.h"
#include "Sound/sound_Extern.h"
#include "stage/stage_Api.h"

// the music object of the game (a global of the game)
// 0x00950320
Unknown950320* g_p950320;

namespace audio {
namespace bgm {

// the fade of the requests that wait for the stage in a museum or showroom (dBgmConstants.cpp)
extern const u32 s_ShowroomStageFade;

// 0x008463A4
const Player::Handler Player::s_StartHandlers[KIND_NUM] = {
    &Player::StartBgm,         &Player::StartSub,        &Player::StartJingle,     &Player::StartJingle,
    &Player::StartJingle,      &Player::StartBingo,      &Player::StartBgm,        &Player::StartCommand,
    &Player::StartNothing,     &Player::StartBgm,        &Player::StartNothing,    &Player::StartObjSong,
    &Player::StartBgm,         &Player::StartMusicObj,   &Player::StartNothing,    &Player::StartBgm,
    &Player::StartBgm,         &Player::StartNothing,    &Player::StartBgm,        &Player::StartBgm,
    &Player::StartBgm,         &Player::StartBgm,        &Player::StartBgm,        &Player::StartBgm,
    &Player::StartBgmPrepared, &Player::StartBgm,        &Player::StartNothing,    &Player::StartJingle,
    &Player::StartHeldSound,
};

// 0x0084648C
const Player::Handler Player::s_StopHandlers[KIND_NUM] = {
    &Player::StopBgm,      &Player::StopSub,     &Player::StopJingle,  &Player::StopJingle,  &Player::StopJingle,
    &Player::StopBingo,    &Player::StopBgm,     &Player::StopCommand, &Player::StopNothing, &Player::StopBgm,
    &Player::StopNothing,  &Player::StopObjSong, &Player::StopBgm,     &Player::StopMusicObj, &Player::StopNothing,
    &Player::StopBgm,      &Player::StopBgm,     &Player::StopNothing, &Player::StopBgm,     &Player::StopBgm,
    &Player::StopBgm,      &Player::StopBgm,     &Player::StopBgm,     &Player::StopBgm,     &Player::StopBgm,
    &Player::StopBgm,      &Player::StopNothing, &Player::StopNothing, &Player::StopNothing,
};

} // namespace bgm
} // namespace audio

// 0x0058CF60
audio::bgm::Request::Request()
    : m_pNext(NULL), m_Kind(KIND_FREE), m_Delay(0), m_Time(0), m_IsAccepted(0), m_IsRemoved(0), m_IsEvent(0),
      m_StartFlag(0), m_IsPrepared(0)
{
}

// 0x0058CD5C
void audio::bgm::Request::Finish(Player* player, s32 fade)
{
    s8 isStarted = m_IsStarted;
    if (m_IsEvent && !m_IsRemoved) {
        // the event opus of the request takes over the music of the request
        if (isStarted != 1) {
            m_IsAccepted = 0;
            return;
        }
        if (player->m_State == Player::STATE_IDLE || player->m_Id != m_Id || player->m_Kind != m_Kind) {
            m_IsStarted = 0;
            m_IsAccepted = 0;
            return;
        }
        if (player->m_EventState != 0) {
            m_IsAccepted = 0;
            return;
        }
        s32 stopWait = m_StopWait;
        player->m_EventId = player->m_Id;
        player->m_EventKind = player->m_Kind;
        player->m_EventState = 1;
        player->m_EventIndex = m_EventIndex;
        s_EventFade = fade;
        GetTrackMixer()->AddMix(player->m_EventIndex);
        player->m_Wait = fade + stopWait;
        player->m_State = Player::STATE_STOPPING;
        m_IsAccepted = 0;
        return;
    }
    if (isStarted == 0) {
        if (m_IsPrepared == 1) {
            (player->*Player::s_StopHandlers[m_Kind])(0, m_Id);
        }
        m_IsAccepted = 0;
        return;
    }
    u32 id = m_Id;
    s32 stopWait = m_StopWait;
    if (player->m_EventId == id && player->m_EventState != 0) {
        player->m_EventState = 0;
        GetTrackMixer()->RemoveMix(player->m_EventIndex);
        player->Stop(0, true);
        if (player->m_Id == id && player->m_State == Player::STATE_STOPPING) {
            player->m_Wait = 0;
        }
    } else {
        u8 state = player->m_State;
        if (state == Player::STATE_IDLE && player->m_Id == id) {
            player->m_Wait = stopWait;
            player->m_State = Player::STATE_STOPPING;
        } else if (player->m_Id == id) {
            if (state == Player::STATE_WAITING) {
                player->m_Wait = stopWait;
            } else if (state == Player::STATE_PLAYING) {
                if (stopWait != -1) {
                    player->m_Wait = stopWait + fade;
                } else {
                    player->m_Wait = stopWait;
                }
            }
            player->Stop(fade, false);
        } else if (player->m_HasNext && player->m_NextId == id) {
            player->m_HasNext = 0;
        }
    }
    m_IsStarted = 0;
    m_IsAccepted = 0;
}

// 0x0058B724
void audio::bgm::Player::Start(bool isEvent)
{
    if (isEvent) {
        if (m_StartFlag != 0) {
            SoundMgr::SStartBgmPrepared();
        } else {
            (this->*s_StartHandlers[m_EventKind])(isEvent, m_EventId);
        }
    } else {
        if (m_StartFlag != 0) {
            SoundMgr::SStartBgmPrepared();
        } else {
            (this->*s_StartHandlers[m_Kind])(isEvent, m_Id);
        }
        m_HasNext = 0;
        m_State = STATE_PLAYING;
    }
    m_StartFlag = 0;
}

// 0x0058B7E4
void audio::bgm::Player::Stop(u32 fade, bool isEvent)
{
    if (isEvent) {
        (this->*s_StopHandlers[m_EventKind])(fade, m_EventId);
        return;
    }
    (this->*s_StopHandlers[m_Kind])(fade, m_Id);
    m_State = STATE_STOPPING;
}

// 0x0058B5EC
void audio::bgm::Player::Reset()
{
    if (m_State == STATE_STOPPING) {
        Stop(0, false);
        m_Wait = 0;
    }
    if (m_EventState != 0) {
        m_EventState = 0;
        GetTrackMixer()->RemoveMix(m_EventIndex);
        Stop(0, true);
    }
}

// 0x0058B650
void audio::bgm::Player::StartNothing(u32 arg, u32 id)
{
    // empty (in the original too)
}

// 0x0058B654
void audio::bgm::Player::StopNothing(u32 arg, u32 id)
{
    // empty (in the original too)
}

// 0x0058DF20
void audio::bgm::Player::StartBgm(u32 arg, u32 id)
{
    SoundMgr::SPlayBgmNow(id);
}

// 0x0058B688
void audio::bgm::Player::StopBgm(u32 arg, u32 id)
{
    if (SoundMgr::SIsBgmActive()) {
        SoundMgr::SStopBgmFade(arg);
    }
}

// 0x00584634
void audio::bgm::Player::StartSub(u32 arg, u32 id)
{
    SoundMgr::SStartSubPrepared();
}

// 0x00583DC4
void audio::bgm::Player::StopSub(u32 arg, u32 id)
{
    SoundMgr::SStopSub(arg);
}

// 0x0058B658
void audio::bgm::Player::StartJingle(u32 arg, u32 id)
{
    SoundMgr::SStartJinglePrepared();
}

// 0x0058B6AC
void audio::bgm::Player::StopJingle(u32 arg, u32 id)
{
    if (SoundMgr::SIsJingleAttached()) {
        SoundMgr::SStopJingle(arg);
    }
}

// 0x00584000
void audio::bgm::Player::StartBingo(u32 arg, u32 id)
{
    SoundMgr::SPlayBingo();
}

// 0x00583F00
void audio::bgm::Player::StopBingo(u32 arg, u32 id)
{
    SoundMgr::SStopSubNow(arg);
}

// 0x00583AA0
void audio::bgm::Player::StartCommand(u32 arg, u32 id)
{
    if ((id & 0xFF) == 1) {
        SoundMgr::SPlayBarcarollePacked(id);
    } else {
        SoundMgr::SPlayBgmNow(id);
    }
}

// 0x0058B6EC
void audio::bgm::Player::StopCommand(u32 arg, u32 id)
{
    if (static_cast<u8>(id) == 1) {
        SoundMgr::SStopBarcarolleAndClear();
    } else if (SoundMgr::SIsBgmActive()) {
        SoundMgr::SStopBgmFade(arg);
    }
}

// 0x00584650
void audio::bgm::Player::StartObjSong(u32 arg, u32 id)
{
    SoundMgr::SPlayObjSong(id);
}

// 0x00584578
void audio::bgm::Player::StopObjSong(u32 arg, u32 id)
{
    SoundMgr::SStopObjSong(arg);
}

// 0x0058B65C
void audio::bgm::Player::StartMusicObj(u32 arg, u32 id)
{
    if (arg == 0) {
        m_State = STATE_WAITING;
    }
    if (g_p950320 != NULL) {
        Func694AC8(g_p950320, static_cast<u8>(id));
    }
}

// 0x0058B6D0
void audio::bgm::Player::StopMusicObj(u32 arg, u32 id)
{
    if (g_p950320 != NULL) {
        Func694770(g_p950320);
    }
}

// 0x00584308
void audio::bgm::Player::StartBgmPrepared(u32 arg, u32 id)
{
    SoundMgr::SStartBgmPrepared();
}

// 0x0058B5DC
void audio::bgm::Player::StartHeldSound(u32 arg, u32 id)
{
    SoundMgr::SStartHeldSound(id, 0.0f);
}

// 0x0058C8E8
audio::bgm::RequestMgr::RequestMgr() : m_pFirst(NULL), m_pCurrent(NULL), m_IsSuspended(0)
{
    m_Player.m_Wait = 0;
    m_Player.m_State = Player::STATE_IDLE;
    m_Player.m_HasNext = 0;
    m_Player.m_EventState = 0;
}

// 0x0058BB54
audio::bgm::Request* audio::bgm::RequestMgr::FindFree()
{
    for (s32 i = 0; i < REQUEST_NUM; i++) {
        if (m_Requests[i].m_Kind == KIND_FREE) {
            return &m_Requests[i];
        }
    }
    return NULL;
}

// 0x0058BA38
void audio::bgm::RequestMgr::Insert(::audio::bgm::Request* request)
{
    request->m_pNext = NULL;
    ::audio::bgm::Request* prev = NULL;
    for (::audio::bgm::Request* node = m_pFirst; node != NULL; node = node->m_pNext) {
        if (request->m_Kind <= node->m_Kind) {
            request->m_pNext = node;
            break;
        }
        prev = node;
    }
    if (prev != NULL) {
        prev->m_pNext = request;
    } else {
        m_pFirst = request;
    }
}

// 0x0058BA9C
bool audio::bgm::RequestMgr::Has(u8 kind, u32 id) const
{
    for (::audio::bgm::Request* node = m_pFirst; node != NULL; node = node->m_pNext) {
        if (node->m_Kind == kind && node->m_Id == id) {
            return true;
        }
        if (node->m_Kind > kind) {
            break;
        }
    }
    return false;
}

// 0x0058B8D4
void audio::bgm::RequestMgr::SetTime(u8 kind, u32 id, s32 time)
{
    for (::audio::bgm::Request* node = m_pFirst; node != NULL; node = node->m_pNext) {
        if (node->m_Kind == kind && node->m_Id == id) {
            node->m_Time = time;
            node->m_IsRemoved = time == 0;
        } else if (node->m_Kind > kind) {
            break;
        }
    }
}

// 0x0058B988
void audio::bgm::RequestMgr::MarkRemoved(u8 kind, u32 id)
{
    for (::audio::bgm::Request* node = m_pFirst; node != NULL && node->m_Kind <= kind; node = node->m_pNext) {
        if (node->m_Kind == kind && node->m_Id == id) {
            node->m_IsRemoved = 1;
        }
    }
}

// 0x0058B9D0
void audio::bgm::RequestMgr::SetFade(u8 kind, u32 id, u32 fade, s32 stopWait, s32 time)
{
    for (::audio::bgm::Request* node = m_pFirst; node != NULL && node->m_Kind <= kind; node = node->m_pNext) {
        if (node->m_Kind == kind && node->m_Id == id) {
            node->m_Fade = fade;
            node->m_StopWait = stopWait;
            node->m_Time = time;
            node->m_IsRemoved = time == 0;
        }
    }
}

// 0x0058BAE0
void audio::bgm::RequestMgr::SetWait(u8 kind, u32 id, s32 wait)
{
    for (::audio::bgm::Request* node = m_pFirst; node != NULL && node->m_Kind <= kind; node = node->m_pNext) {
        if (node->m_Kind == kind && node->m_Id == static_cast<u32>(wait)) {
            node->m_Delay = wait;
        }
    }
    if (m_Player.m_Kind == kind && m_Player.m_Id == id && m_Player.m_State == Player::STATE_STOPPING) {
        m_Player.m_Wait = wait;
    }
}

// 0x0058BB48
void audio::bgm::RequestMgr::Suspend()
{
    m_IsSuspended = 1;
}

// 0x0058BBB4
void audio::bgm::RequestMgr::Collect(::audio::bgm::Request* node, ::audio::bgm::Request* prev)
{
    if (node == NULL) {
        return;
    }
    Collect(node->m_pNext, node);
    if (!node->m_IsRemoved) {
        return;
    }
    if (prev == NULL) {
        m_pFirst = node->m_pNext;
    } else {
        prev->m_pNext = node->m_pNext;
    }
    if (node->m_IsStarted) {
        node->Finish(&m_Player, 1);
    }
    node->m_pNext = NULL;
    node->m_Kind = KIND_FREE;
    node->m_IsAccepted = 0;
    node->m_IsRemoved = 0;
    node->m_IsEvent = 0;
    node->m_StartFlag = 0;
    node->m_IsPrepared = 0;
}

// 0x0058BC3C
bool audio::bgm::RequestMgr::RequestHeldSound(u32 id, s32 delay)
{
    ::audio::bgm::Request* request = FindFree();
    if (request == NULL) {
        return false;
    }
    request->m_pNext = NULL;
    request->m_IsAccepted = 0;
    request->m_IsRemoved = 0;
    request->m_IsEvent = 0;
    request->m_StartFlag = 0;
    request->m_IsPrepared = 0;
    request->m_Delay = delay;
    request->m_Kind = KIND_HELD_SOUND;
    request->m_Time = -1;
    request->m_Id = id;
    Insert(request);
    SoundMgr::SPrepareSoundInFreeHandle(id);
    return true;
}

// 0x0058C1E4
bool audio::bgm::RequestMgr::RequestJingleDelayed(u32 id, s32 delay)
{
    ::audio::bgm::Request* request = FindFree();
    if (request == NULL) {
        return false;
    }
    request->m_pNext = NULL;
    request->m_IsAccepted = 0;
    request->m_IsRemoved = 0;
    request->m_IsEvent = 0;
    request->m_StartFlag = 0;
    request->m_IsPrepared = 0;
    request->m_Delay = delay;
    request->m_Kind = KIND_JINGLE_DELAYED;
    request->m_Time = -1;
    request->m_Id = id;
    Insert(request);
    SoundMgr::SPlayJingleDelayed(id);
    return true;
}

void audio::bgm::RequestMgr::StopKind(u8 kind)
{
    if (m_Player.m_Kind == kind && m_Player.m_State != Player::STATE_IDLE) {
        m_Player.Stop(0, false);
        m_Player.m_Wait = 0;
        m_Player.m_State = Player::STATE_STOPPING;
    }
    // (compares the next id with the kind: kept from the original)
    if (m_Player.m_NextId == kind) {
        m_Player.m_HasNext = 0;
    }
    if (m_Player.m_EventKind == kind && m_Player.m_EventState != 0) {
        m_Player.Stop(0, true);
        m_Player.m_EventState = 0;
        GetTrackMixer()->RemoveMix(m_Player.m_EventIndex);
    }
}

// 0x0058C614
void audio::bgm::RequestMgr::Remove(u8 kind, u32 id)
{
    for (::audio::bgm::Request* node = m_pFirst; node != NULL && node->m_Kind <= kind; node = node->m_pNext) {
        if (node->m_Kind == kind && node->m_Id == id) {
            node->m_Time = 0;
            node->m_Fade = 0;
            node->m_StopWait = 0;
            node->m_IsRemoved = 1;
        }
    }
    StopKind(kind);
}

// 0x0058BD64
void audio::bgm::RequestMgr::Reset()
{
    RemoveAll();
}

// 0x0058BD68
void audio::bgm::RequestMgr::RemoveAll()
{
    for (s32 i = 0; i < REQUEST_NUM; i++) {
        u8 kind = m_Requests[i].m_Kind;
        if (kind == KIND_FREE) {
            continue;
        }
        u32 id = m_Requests[i].m_Id;
        for (::audio::bgm::Request* node = m_pFirst; node != NULL && node->m_Kind <= kind; node = node->m_pNext) {
            if (node->m_Kind == kind && node->m_Id == id) {
                node->m_StopWait = 0;
                node->m_Time = 0;
                node->m_Fade = 0;
                node->m_IsRemoved = 1;
            }
        }
        StopKind(kind);
    }
}

// 0x0058BCA4
void audio::bgm::RequestMgr::Clear()
{
    for (::audio::bgm::Request* node = m_pFirst; node != NULL; node = node->m_pNext) {
        u8 kind = node->m_Kind;
        if (kind == 13 || kind == 6 || kind == 21) {
            node->m_Fade = 0;
            node->m_IsRemoved = 1;
        }
    }
    m_IsSuspended = 0;
    m_Player.Reset();
    if (SoundMgr::SIsReset()) {
        RemoveAll();
    }
}

// 0x0058C100
bool audio::bgm::RequestMgr::RequestEvent(u8 kind, u32 id, u8 eventIndex, EventParam param, u8 startFlag)
{
    ::audio::bgm::Request* request = FindFree();
    if (request == NULL) {
        return false;
    }
    request->m_pNext = NULL;
    request->m_IsAccepted = 0;
    request->m_IsRemoved = 0;
    request->m_IsPrepared = 0;
    request->m_Kind = kind;
    request->m_Id = id;
    request->m_Fade = param.fade;
    request->m_StopWait = param.stopWait;
    request->m_Delay = param.delay;
    request->m_Time = param.time;
    request->m_EventIndex = eventIndex;
    request->m_StartFlag = startFlag;
    request->m_IsEvent = 1;
    request->m_WaitsForStage = 0;
    Insert(request);
    return true;
}

// 0x0058C17C
void audio::bgm::RequestMgr::CancelEvent(u8 kind, u8 eventIndex)
{
    for (::audio::bgm::Request* node = m_pFirst; node != NULL && node->m_Kind <= kind; node = node->m_pNext) {
        if (node->m_IsEvent && node->m_Kind == kind && node->m_EventIndex == eventIndex) {
            node->m_IsRemoved = 1;
        }
    }
}

// 0x0058C724
void audio::bgm::RequestMgr::SetDelay(u8 kind, u32 id, s32 delay)
{
    for (::audio::bgm::Request* node = m_pFirst; node != NULL; node = node->m_pNext) {
        if (node->m_Kind == kind && node->m_Id == id) {
            node->m_Delay = delay;
        } else if (node->m_Kind > kind) {
            break;
        }
    }
}

// 0x0058C764
bool audio::bgm::RequestMgr::Request(u8 kind, u32 id, u32 fade, Times times)
{
    ::audio::bgm::Request* request = FindFree();
    if (request == NULL) {
        return false;
    }
    request->m_pNext = NULL;
    request->m_IsAccepted = 0;
    request->m_IsRemoved = 0;
    request->m_IsEvent = 0;
    request->m_StartFlag = 0;
    request->m_IsPrepared = 0;
    request->m_Kind = kind;
    request->m_Id = id;
    request->m_Fade = fade;
    request->m_StopWait = times.stopWait;
    request->m_Delay = times.delay;
    request->m_Time = times.time;
    request->m_WaitsForStage = 0;
    request->m_EventIndex = 50;
    Insert(request);
    // the music of some kinds is prepared at once
    request->m_IsPrepared = 1;
    if (kind == KIND_JINGLE_C || kind == KIND_JINGLE_B || kind == KIND_JINGLE_A) {
        SoundMgr::SPlayJingleDelayed(id);
    } else if (kind == KIND_SUB) {
        SoundMgr::SPlaySymbolTree();
    } else if (kind == KIND_BGM_PREPARED) {
        SoundMgr::SPlayBgm(id);
    } else {
        request->m_IsPrepared = 0;
    }
    return true;
}

// 0x0058C838
bool audio::bgm::RequestMgr::RequestStage(u8 kind, u32 id, s32 delay, StageTimes times)
{
    ::audio::bgm::Request* request = FindFree();
    if (request == NULL) {
        return false;
    }
    request->m_pNext = NULL;
    request->m_IsAccepted = 0;
    request->m_IsRemoved = 0;
    request->m_IsEvent = 0;
    request->m_StartFlag = 0;
    request->m_IsPrepared = 0;
    request->m_Kind = kind;
    request->m_Id = id;
    u32 fade;
    if (stage::IsKind(stage::KIND_MUSEUM_SHOWROOM, stage::GetCurrent())) {
        fade = s_ShowroomStageFade;
    } else {
        fade = 5;
    }
    request->m_IsEvent = 1;
    request->m_EventIndex = 19;
    request->m_WaitsForStage = 1;
    request->m_Fade = fade;
    request->m_Delay = delay;
    request->m_Time = times.time;
    request->m_StopWait = times.stopWait;
    Insert(request);
    return true;
}

// 0x0058BE84
void audio::bgm::RequestMgr::Select()
{
    ::audio::bgm::Request* selected;
    for (selected = m_pFirst; selected != NULL; selected = selected->m_pNext) {
        if (selected->m_IsRemoved || selected->m_Kind >= 26) {
            continue;
        }
        if (m_IsSuspended || !selected->m_WaitsForStage) {
            break;
        }
    }
    ::audio::bgm::Request* current = m_pCurrent;
    if (current == selected) {
        return;
    }
    Player* player = &m_Player;
    if (current != NULL) {
        if (current->m_IsAccepted) {
            u8 state = player->m_State;
            u8 kind = current->m_Kind;
            if (state != Player::STATE_IDLE && player->m_Id == current->m_Id && player->m_Kind == kind) {
                // the player plays the current request
                if (selected != NULL && selected->m_Kind <= kind) {
                    current->Finish(player, selected->m_Fade);
                } else {
                    current->Finish(player, current->m_Fade);
                }
            } else if (selected != NULL) {
                u8 selectedKind = selected->m_Kind;
                if (player->m_Kind > selectedKind && state == Player::STATE_STOPPING) {
                    player->Stop(0, false);
                    player->m_Wait = 0;
                }
                if (player->m_HasNext == 1 && player->m_NextKind > selectedKind) {
                    player->m_HasNext = 0;
                }
                if (player->m_EventState != 0 && player->m_EventKind > selectedKind) {
                    player->m_EventState = 0;
                    GetTrackMixer()->RemoveMix(player->m_EventIndex);
                    player->Stop(0, true);
                }
                if (selected->m_Kind < m_pCurrent->m_Kind) {
                    m_pCurrent->m_IsAccepted = 0;
                    m_pCurrent->m_IsStarted = 0;
                }
            }
        } else if (selected != NULL && selected->m_Kind <= current->m_Kind) {
            u8 selectedKind = selected->m_Kind;
            u32 fade = selected->m_Fade;
            if (selectedKind < player->m_Kind && player->m_State == Player::STATE_STOPPING) {
                player->Stop(fade, false);
                player->m_Wait = fade;
            }
            if (player->m_EventState != 0 && player->m_EventKind > selectedKind) {
                player->m_EventState = 0;
                GetTrackMixer()->RemoveMix(player->m_EventIndex);
                player->Stop(fade, true);
            }
        }
    } else {
        u8 selectedKind = selected->m_Kind;
        u32 fade = selected->m_Fade;
        if (player->m_Kind > selectedKind && player->m_State == Player::STATE_STOPPING) {
            player->Stop(fade, false);
            player->m_Wait = fade;
        }
        if (player->m_EventState != 0 && player->m_EventKind > selectedKind) {
            player->m_EventState = 0;
            GetTrackMixer()->RemoveMix(player->m_EventIndex);
            player->Stop(fade, true);
        }
    }
    m_pCurrent = selected;
}

// 0x0058C24C
void audio::bgm::RequestMgr::Update()
{
    Player* player = &m_Player;
    if (m_pCurrent != NULL && m_pCurrent->m_IsRemoved) {
        m_pCurrent->Finish(player, m_pCurrent->m_Fade);
        m_pCurrent = NULL;
    }
    // the removed requests
    ::audio::bgm::Request* first = m_pFirst;
    if (first != NULL) {
        Collect(first->m_pNext, first);
        if (first->m_IsRemoved) {
            m_pFirst = first->m_pNext;
            if (first->m_IsStarted) {
                first->Finish(player, 1);
            }
            first->m_pNext = NULL;
            first->m_Kind = KIND_FREE;
            first->m_IsAccepted = 0;
            first->m_IsRemoved = 0;
            first->m_IsEvent = 0;
            first->m_StartFlag = 0;
            first->m_IsPrepared = 0;
        }
    }
    // the delays and the times
    for (s32 i = 0; i < REQUEST_NUM; i++) {
        ::audio::bgm::Request* request = &m_Requests[i];
        if (request->m_Kind == KIND_FREE) {
            continue;
        }
        if (request->m_Delay > 0) {
            request->m_Delay--;
        }
        if (request->m_Delay != 0) {
            continue;
        }
        if (request->m_Time > 0) {
            request->m_Time--;
        }
        if (request->m_Time == 0) {
            request->m_IsRemoved = 1;
        }
    }
    Select();
    // the selected request goes to the player
    ::audio::bgm::Request* current = m_pCurrent;
    if (current != NULL && current->m_Delay == 0 && !current->m_IsAccepted) {
        u8 kind = current->m_Kind;
        u32 id = current->m_Id;
        player->m_StartFlag = current->m_StartFlag;
        if (player->m_EventState == 1 && player->m_EventId == id) {
            m_Player.m_EventState = 2;
        } else if (!(player->m_State & (Player::STATE_PLAYING | Player::STATE_WAITING))) {
            if (player->m_State == Player::STATE_IDLE) {
                player->m_Id = id;
                player->m_Kind = kind;
                player->Start(false);
            } else {
                player->m_NextId = id;
                player->m_HasNext = 1;
                m_Player.m_NextKind = kind;
            }
        }
        current->m_IsAccepted = 1;
        current->m_IsStarted = 1;
    }
    // the event opus of the requests, the jingles and held sounds
    for (s32 i = 0; i < REQUEST_NUM; i++) {
        ::audio::bgm::Request* request = &m_Requests[i];
        u8 kind = request->m_Kind;
        if (kind == KIND_FREE) {
            continue;
        }
        if (!m_IsSuspended && request->m_WaitsForStage) {
            continue;
        }
        if (request->m_Delay != 0) {
            continue;
        }
        if (request->m_IsEvent && !request->m_IsStarted) {
            if (kind == 20 && player->m_State != Player::STATE_IDLE && player->m_Kind == 19) {
                continue;
            }
            player->m_EventState = 1;
            player->m_EventId = request->m_Id;
            player->m_EventKind = kind;
            player->m_EventIndex = request->m_EventIndex;
            player->m_StartFlag = request->m_StartFlag;
            s_EventFade = 1;
            GetTrackMixer()->AddMix(player->m_EventIndex);
            player->Start(true);
            request->m_IsStarted = 1;
        } else if (kind == KIND_JINGLE_DELAYED || kind == KIND_HELD_SOUND) {
            u32 id = request->m_Id;
            if (kind == KIND_JINGLE_DELAYED) {
                SoundMgr::SStartJinglePrepared();
            } else if (kind == KIND_HELD_SOUND) {
                SoundMgr::SStartHeldSound(id, 0.0f);
            }
            request->m_IsRemoved = 1;
        }
    }
    // the stop of the player
    if (player->m_State == Player::STATE_STOPPING) {
        if (player->m_Wait > 0) {
            player->m_Wait--;
        }
        if (player->m_Wait != 0) {
            return;
        }
        if (!SoundMgr::SIsBgmActive() || player->m_EventState != 0) {
            if (player->m_EventState != 2) {
                if (!player->m_HasNext) {
                    player->m_State = Player::STATE_IDLE;
                    return;
                }
                player->m_Id = player->m_NextId;
                player->m_HasNext = 0;
                player->m_Kind = player->m_NextKind;
                player->Start(false);
                return;
            }
            goto takeEvent;
        }
    }
    if (player->m_State != Player::STATE_IDLE || player->m_EventState != 2) {
        return;
    }
takeEvent:
    // the music of the event opus is the music of the request now
    player->m_State = Player::STATE_PLAYING;
    player->m_Id = player->m_EventId;
    player->m_EventState = 0;
    player->m_Kind = player->m_EventKind;
    GetTrackMixer()->RemoveMix(player->m_EventIndex);
}

// 0x00584B5C
audio::bgm::RequestMgr* audio::bgm::GetRequestMgr()
{
    return &s_RequestMgr;
}

// 0x0058C5FC
void audio::bgm::RemoveKind10()
{
    GetRequestMgr()->Remove(10, 0);
}
