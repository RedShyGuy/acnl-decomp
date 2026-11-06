#pragma once

#include "decomp.h"
#include "Sound/dSoundCreateArg.h"
#include "Sound/dSoundFader.h"
#include "Sound/dSoundObjExtVolume.h"
#include "nn/math/math_Vector3.h"
#include "nw/snd/snd_SequenceSoundHandle.h"
#include "nw/snd/internal/snd_SequenceSound.h"

// RTTI 11SoundTvBase @ 0x008CB360
// vtable 0x008ECE04 (vptr 0x008ECE0C), offset_to_top 0, 5 entries
// The sound of a TV program: the sound data of the program (m_ItemId) is loaded into the stage heap 2 or 3, then the program
// plays its sounds by a timeline over the frame counter of the TV (each program is a class SoundTvNN*); the volume fades in
// and out, some programs put a biquad filter on the sound player 15 (the class name is from the RTTI, everything else is
// ours).
class SoundTvBase
{
public:
    // m_State
    enum State
    {
        STATE_NONE = 0,
        STATE_STARTED = 1, // the sound data is loading
        STATE_READY = 2,   // loaded, silent
        STATE_PLAYING = 3,
    };
    // m_LoadState
    enum LoadState
    {
        LOAD_NONE = 0,
        LOAD_BUSY = 1,
        LOAD_DONE = 2,
    };
    enum
    {
        HEAP_NONE = 6,
        // the programs with the louder sound and the filter (the meaning of the numbers is not known)
        PROGRAM_LOUD_A = 0x10D,
        PROGRAM_LOUD_B = 0x10E,
    };

    DECOMP_NOINLINE SoundTvBase(); // 0x001DBB70
    // 0x001DBC0C
    // 0x001DBBF8 (deleting dtor)
    DECOMP_NOINLINE virtual ~SoundTvBase();
    // starts the object (the programs set their sound data first)
    virtual void Start(); // 0x001DB544 slot 0x08
    // the sound data is loaded
    virtual void OnLoaded(); // 0x001DB7CC slot 0x0C
    // the timeline of the program
    virtual void UpdateProgram(s32 frame); // 0x001DB5A4 slot 0x10

    // STATE_PLAYING -> STATE_READY
    void Mute(); // 0x001DB5B0
    // STATE_READY -> STATE_PLAYING
    void Play(u8 arg); // 0x001DB68C
    // loads the sound data; true when it is there
    bool Load(); // 0x001DB6D4
    // each frame: the volume for the program, the timeline and the position (relative to the camera)
    void Update(s32 frame, u16 program, const nn::math::VEC3* position); // 0x001DB7D0
    void Stop(); // 0x001DB990
    // a sound on one of the two handles
    void PlaySound(u32 index, u32 soundId); // 0x001DBA9C

    sead::SoundHandle& GetHandle(s32 index) { return m_Obj.m_HandleSet.m_Handles[index]; }
    // the sequence of a handle (names are ours)
    void WriteVariable(s32 index, int varNo, s16 value)
    {
        nw::snd::SequenceSoundHandle sequence(&GetHandle(index));
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->WriteVariable(varNo, value);
        }
    }
    void ReadVariable(s32 index, int varNo, s16* value)
    {
        nw::snd::SequenceSoundHandle sequence(&GetHandle(index));
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->ReadVariable(varNo, value);
        }
    }
    void SetTrackVolume(s32 index, u32 tracks, f32 volume)
    {
        nw::snd::SequenceSoundHandle sequence(&GetHandle(index));
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->SetTrackVolume(tracks, volume);
        }
    }
    void SetTrackMute(s32 index, u32 tracks, bool isMute)
    {
        nw::snd::SequenceSoundHandle sequence(&GetHandle(index));
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->SetTrackMute(tracks, isMute);
        }
    }
    void SetTrackSilence(s32 index, u32 tracks, bool isSilence, int frames)
    {
        nw::snd::SequenceSoundHandle sequence(&GetHandle(index));
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->SetTrackSilence(tracks, isSilence, frames);
        }
    }

    // (static initializer 0x0078505C)
    static nn::math::VEC3 s_Origin;

    SoundObjExtVolume m_Obj;       // 0x04
    nn::math::VEC3 m_Position;     // 0xA0 follows the TV by at most 10 per frame in x and y
    u16 m_Program;                 // 0xAC
    u32 m_ItemId;                  // 0xB0 the sound data of the program
    u8 m_State;                    // 0xB4 State
    u8 m_LoadState;                // 0xB5 LoadState
    u8 m_HeapIndex;                // 0xB6 of SoundDataLoadMgr
    u8 m_PlayArg;                  // 0xB7
    SoundFader m_FilterFader;      // 0xB8 the biquad filter of the sound player 15
    SoundFader m_VolumeFader;      // 0xCC
    bool m_IsFirstProgram;         // 0xE0 the first program fades in at once
};
ASSERT_OFFSET(SoundTvBase, m_Position, 0xA0);
ASSERT_OFFSET(SoundTvBase, m_State, 0xB4);
ASSERT_OFFSET(SoundTvBase, m_FilterFader, 0xB8);
ASSERT_OFFSET(SoundTvBase, m_IsFirstProgram, 0xE0);

// The handle of the game for the sound of a TV (8 bytes; the class and its names are ours, the functions lie in the code of
// the game around 0x605FC0, the copies of Mute/Play beside SoundTvBase).
class SoundTvHandle
{
public:
    typedef SoundCreateArg CreateArg;

    template <typename T>
    static SoundTvHandle* Create(const CreateArg* arg);
    // by the index of the program (0..21, 22: weather)
    static SoundTvHandle* Create(u32 index, const CreateArg* arg); // 0x00606004
    static void Destroy(SoundTvHandle* handle); // 0x00606018

    void Start(); // 0x00605FC0
    void Load(); // 0x00605FE4
    void Update(s32 frame, u16 program, const nn::math::VEC3* world); // 0x00606054
    void Stop(); // 0x00606090
    void Mute(); // 0x001DB5A8
    void Play(u8 arg); // 0x001DB684

    SoundTvBase* m_pTv; // 0x00
    bool m_IsStarted;   // 0x04
    bool m_IsLoaded;    // 0x05
};
ASSERT_SIZE(SoundTvHandle, 8);
