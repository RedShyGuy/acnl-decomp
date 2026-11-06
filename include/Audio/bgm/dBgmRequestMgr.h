#pragma once

#include "decomp.h"

namespace audio {
namespace bgm {
// The scheduler of the music (in the file of the BGM manager, no RTTI; all names are ours): the game requests music of a kind
// (the kind is the priority, lower first); each frame the first request that may play is selected and the player starts and
// stops the music of its kind through the tables of handlers. A request can also switch on a mix of the tracks (TrackMixer).

// Request::m_Kind (the handlers of the player; the meaning of the other kinds is not known yet)
enum Kind
{
    KIND_BGM = 0,          // SPlayBgmNow / SStopBgmFade
    KIND_SUB = 1,          // SStartSubPrepared / SStopSub
    KIND_JINGLE_A = 2,     // SStartJinglePrepared / SStopJingle
    KIND_JINGLE_B = 3,
    KIND_JINGLE_C = 4,
    KIND_BINGO = 5,        // SPlayBingo / SStopSubNow
    KIND_COMMAND = 7,      // the music of id (1 in the low byte: Kapp'n's song, SPlayBarcarollePacked)
    KIND_FIELD = 6,        // STRM_BGM_FLD_HORNET / _RACKET / _RESET (FieldMusic)
    KIND_OBJ_SONG = 11,    // SPlayObjSong / SStopObjSong
    KIND_SHOP_CLOSE = 12,  // STRM_BGM_IND_KODANUKI_CLOSE (StageOpusShopBase)
    KIND_MUSIC_OBJ = 13,   // the music object of the game 0x950320
    KIND_SHOP = 15,        // the music of a shop (StageOpusShopBase)
    KIND_CARNIVAL = 21,    // EvOpusCarnival
    KIND_STAGE_EVENT = 22, // the music of an event in the town (EvOpusBase)
    KIND_STAGE_SPECIAL = 23, // the music of a stage of the kind 0x200 (StageOpusVillageDowntown)
    KIND_BGM_PREPARED = 24, // SStartBgmPrepared
    KIND_STAGE = 25,       // the music of the town (StageOpusVillageDowntown)
    KIND_JINGLE_DELAYED = 27,
    KIND_HELD_SOUND = 28,  // SStartHeldSound
    KIND_NUM = 29,
    KIND_FREE = 29,
};

struct Request
{
    DECOMP_NOINLINE Request(); // 0x0058CF60

    Request* m_pNext;   // 0x00
    u8 m_Kind;          // 0x04 Kind
    u32 m_Id;           // 0x08
    s32 m_Delay;        // 0x0C frames until it may play
    s32 m_Time;         // 0x10 frames it stays (-1: until it is removed)
    u32 m_Fade;         // 0x14 frames of the fade out
    s32 m_StopWait;     // 0x18 frames the player waits after the stop
    u8 m_IsAccepted;    // 0x1C the player has taken the request
    s8 m_IsStarted;     // 0x1D the music (or the mix) of the request was started
    u8 m_WaitsForStage; // 0x1E
    u8 m_IsRemoved;     // 0x1F
    u8 m_IsEvent;       // 0x20 the request switches on a mix
    u8 m_StartFlag;     // 0x21
    u8 m_IsPrepared;    // 0x22 the music was prepared when the request was made
    u8 m_EventIndex;    // 0x23 the mix (TrackMixer)

    // the request ends: its mix goes, the player stops the music of the request
    void Finish(struct Player* player, s32 fade); // 0x0058CD5C
};

struct Player
{
    // m_State
    enum State
    {
        STATE_IDLE = 1,
        STATE_PLAYING = 2,
        STATE_WAITING = 4,
        STATE_STOPPING = 8,
    };

    // the handlers of a kind (fade: the frames of the fade, for a start: the start is the one of an event)
    typedef void (Player::*Handler)(u32 arg, u32 id);

    // starts the music of the request (or of the event)
    void Start(bool isEvent); // 0x0058B724
    void Stop(u32 fade, bool isEvent); // 0x0058B7E4
    void Reset(); // 0x0058B5EC
    // the music of the kind and id plays
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsPlaying(u8 kind, u32 id) const // 0x00754074
    {
        return m_State != STATE_IDLE && m_Id == id && m_Kind == kind;
    }

    void StartNothing(u32 arg, u32 id); // 0x0058B650
    void StopNothing(u32 arg, u32 id); // 0x0058B654
    void StartBgm(u32 arg, u32 id); // 0x0058DF20
    void StopBgm(u32 arg, u32 id); // 0x0058B688
    void StartSub(u32 arg, u32 id); // 0x00584634
    void StopSub(u32 arg, u32 id); // 0x00583DC4
    void StartJingle(u32 arg, u32 id); // 0x0058B658
    void StopJingle(u32 arg, u32 id); // 0x0058B6AC
    void StartBingo(u32 arg, u32 id); // 0x00584000
    void StopBingo(u32 arg, u32 id); // 0x00583F00
    void StartCommand(u32 arg, u32 id); // 0x00583AA0
    void StopCommand(u32 arg, u32 id); // 0x0058B6EC
    void StartObjSong(u32 arg, u32 id); // 0x00584650
    void StopObjSong(u32 arg, u32 id); // 0x00584578
    void StartMusicObj(u32 arg, u32 id); // 0x0058B65C
    void StopMusicObj(u32 arg, u32 id); // 0x0058B6D0
    void StartBgmPrepared(u32 arg, u32 id); // 0x00584308
    void StartHeldSound(u32 arg, u32 id); // 0x0058B5DC

    static const Handler s_StartHandlers[KIND_NUM];
    static const Handler s_StopHandlers[KIND_NUM];

    s32 m_Wait;         // 0x00 frames of STATE_STOPPING
    u8 m_State;         // 0x04 State
    u32 m_Id;           // 0x08
    u8 m_Kind;          // 0x0C
    u32 m_NextId;       // 0x10
    u8 m_NextKind;      // 0x14
    u8 m_HasNext;       // 0x15
    u32 m_EventId;      // 0x18
    u8 m_EventKind;     // 0x1C
    s8 m_EventState;    // 0x1D 0 none, 1 running, 2 the request of the event is the current one
    u8 m_EventIndex;    // 0x1E
    s8 m_StartFlag;     // 0x1F the music was prepared (SStartBgmPrepared)
};

class RequestMgr
{
public:
    enum
    {
        REQUEST_NUM = 29,
    };

    RequestMgr(); // 0x0058C8E8

    // the times of a request (passed by value)
    struct Times
    {
        Times(s32 delay_, s32 time_, s32 stopWait_) : delay(delay_), time(time_), stopWait(stopWait_) {}

        s32 delay;
        s32 time;
        s32 stopWait;
    };
    struct EventParam
    {
        EventParam(u32 fade_, s32 delay_, s32 time_, s32 stopWait_)
            : fade(fade_), delay(delay_), time(time_), stopWait(stopWait_)
        {
        }

        u32 fade;
        s32 delay;
        s32 time;
        s32 stopWait;
    };
    struct StageTimes
    {
        StageTimes(s32 time_, s32 stopWait_) : time(time_), stopWait(stopWait_) {}

        s32 time;
        s32 stopWait;
    };

    bool Request(u8 kind, u32 id, u32 fade, Times times); // 0x0058C764
    // a request with a mix of the tracks
    bool RequestEvent(u8 kind, u32 id, u8 eventIndex, EventParam param, u8 startFlag); // 0x0058C100
    // a request with mix 19 that waits for the stage
    bool RequestStage(u8 kind, u32 id, s32 delay, StageTimes times); // 0x0058C838
    bool RequestHeldSound(u32 id, s32 delay); // 0x0058BC3C
    bool RequestJingleDelayed(u32 id, s32 delay); // 0x0058C1E4
    void CancelEvent(u8 kind, u8 eventIndex); // 0x0058C17C
    void Remove(u8 kind, u32 id); // 0x0058C614
    void MarkRemoved(u8 kind, u32 id); // 0x0058B988
    bool Has(u8 kind, u32 id) const; // 0x0058BA9C
    void SetTime(u8 kind, u32 id, s32 time); // 0x0058B8D4
    void SetFade(u8 kind, u32 id, u32 fade, s32 stopWait, s32 time); // 0x0058B9D0
    void SetDelay(u8 kind, u32 id, s32 delay); // 0x0058C724
    // (the requests are compared with the wait, not with the id: kept from the original)
    void SetWait(u8 kind, u32 id, s32 wait); // 0x0058BAE0
    void Suspend(); // 0x0058BB48
    void Clear(); // 0x0058BCA4
    void RemoveAll(); // 0x0058BD68
    void Reset(); // 0x0058BD64
    void Update(); // 0x0058C24C

    ::audio::bgm::Request m_Requests[REQUEST_NUM]; // 0x000
    ::audio::bgm::Request* m_pFirst;   // 0x414 sorted by the kind
    ::audio::bgm::Request* m_pCurrent; // 0x418
    Player m_Player;                   // 0x41C
    u8 m_IsSuspended;                  // 0x43C

private:
    DECOMP_NOINLINE ::audio::bgm::Request* FindFree(); // 0x0058BB54
    DECOMP_NOINLINE void Insert(::audio::bgm::Request* request); // 0x0058BA38
    // the removed requests from node on (recursively from the end)
    void Collect(::audio::bgm::Request* node, ::audio::bgm::Request* prev); // 0x0058BBB4
    void Select(); // 0x0058BE84
    DECOMP_ALWAYS_INLINE void StopKind(u8 kind);
};

extern RequestMgr s_RequestMgr;

// the request manager of the game
DECOMP_NOINLINE RequestMgr* GetRequestMgr(); // 0x00584B5C
// the requests of kind 10 go
void RemoveKind10(); // 0x0058C5FC
} // namespace bgm
} // namespace audio
