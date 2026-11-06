#pragma once

#include "decomp.h"
#include "Sound/dSoundFxMgr.h"
#include "Sound/dSoundRing.h"
#include "nw/snd/snd_SoundHandle.h"
#include "sead/seadAudioSubsetBase.h"
#include "sead/seadCheckedArray.h"

// RTTI 18SoundDoubutsugoMgr @ 0x008CC8C8
// vtable 0x008F4AA4 (vptr 0x008F4AAC), offset_to_top 0, 20 entries
// The speech of the characters ("doubutsugo"): the text is queued character by character, each character plays a syllable
// SE_DOU_* of the voice group of the speaker (BOY, GAL, MAN, BOY_GHOST, GAL_GHOST; 140 sounds each). The pitch moves up and
// down by the parameters of the voice and the mood; up to three more voices of other groups speak along. Japanese characters
// are syllable indices, the other languages read pairs of letters (the class name is from the RTTI, everything else is ours).
class SoundDoubutsugoMgr : public ::sead::AudioSubsetBase
{
public:
    enum
    {
        QUEUE_SIZE = 6,
        HANDLE_NUM = 3,        // the syllables overlap by turns
        EXTRA_VOICE_NUM = 3,
        VOICE_NUM = 29,
        MOOD_NUM = 6,
        LEVEL_NUM = 5,
        VOICE_PARAM_NUM = 87,
        SYLLABLE_NUM = 140,    // the sounds of a voice group
        EXTENDED_CHAR_NUM = 96, // the characters 123..218
        HANGUL_INITIAL_NUM = 19,
        HANGUL_MEDIAL_NUM = 21,
        HANGUL_FINAL_NUM = 28,
        IDLE_RESET_FRAMES = 60,
        PITCH_STEPS = 12,      // the frames of a step of the pitch movement
    };

    // m_Language (from the region of the game)
    enum Language
    {
        LANGUAGE_JAPANESE = 0,
        LANGUAGE_ENGLISH = 1,
        LANGUAGE_GERMAN = 2,
        LANGUAGE_FRENCH = 3,
        LANGUAGE_ITALIAN = 4,
        LANGUAGE_SPANISH = 5,
        LANGUAGE_KOREAN = 6,
        LANGUAGE_NUM = 7,
    };

    // m_Effect: set by the control characters 157..166 and '#'
    enum Effect
    {
        EFFECT_NONE = 0,
        EFFECT_HOLD = 1,    // the pitch stays
        EFFECT_NO_PITCH = 2, // the pitch of the sounds is not set
        EFFECT_RISE = 3,    // the pitch rises over riseFrames
        EFFECT_CUT = 4,     // the sounds before stop
    };

    // the special characters of the queue
    enum
    {
        CHAR_LONG = 157,     // the long form of the syllable before
        CHAR_DOT = 158,      // SE_SYS_SCRIPT_DOT
        CHAR_MONOLOGUE = 159, // SE_SYS_SCRIPT_MONOLOGY
        CHAR_S = 155,        // "s" with the vowel after it
    };

    // the movement of the pitch of a voice in a mood (36 bytes)
    struct VoiceParam
    {
        f32 pitchMin;        // 0x00 the range of a step: random in [pitchMin, pitchMax]
        u8 riseFramesMin;    // 0x04 the frames until the pitch falls back
        u8 fallFramesMin;    // 0x05
        f32 pitchMax;        // 0x08
        u8 riseFramesMax;    // 0x0C
        u8 fallFramesMax;    // 0x0D
        f32 volume;          // 0x10
        f32 pitchScale;      // 0x14
        f32 fallLimit;       // 0x18 the pitch falls back below 1 + fallLimit
        f32 riseAmount;      // 0x1C EFFECT_RISE
        u8 riseFrames;       // 0x20 EFFECT_RISE
        u8 steps;            // 0x21 of the pitch movement
        u8 isStartMid;       // 0x22 the first step starts in the middle
    };
    ASSERT_SIZE(VoiceParam, 0x24);

    SoundDoubutsugoMgr(); // 0x00128D1C
    // 0x002E829C
    // 0x002E824C (deleting dtor)
    virtual ~SoundDoubutsugoMgr();
    virtual void calc() override; // 0x002E7C3C slot 0x1C

    // the mood of the speaker (0..5)
    void SetMood(u32 mood); // 0x002E703C
    // the voices of the speakers (0 and 29 and above: none); the first one is the main voice
    void SetVoices(const sead::CheckedArray<u8, 4>& voices); // 0x002E70C8
    // the frames a character stays in the queue (1..5)
    void SetCharFrames(u32 frames); // 0x002E72E8
    void OnEnterStage(); // 0x002E72FC
    // a character of the text; level: the loudness 0..255
    void Queue(u16 c, s32 level); // 0x002E8178
    // the biquad filter 70 on the syllables (it stays until the next reset)
    DECOMP_NOINLINE void SetFilter(bool isOn); // 0x002E7A3C
    // one character as a single sound (the voice of the player)
    void PlayChar(u32 c, f32 pan); // 0x002E7A50
    void PlayLongChar(u32 c, f32 pan); // 0x002E7BA4
    // a sound of the voice of the player: BOY or GAL by Func5C116C
    void PlaySingle(u32 soundId, f32 pan); // 0x002E7A7C
    // stops the voice sound if one plays (name is ours)
    void StopVoice(); // 0x002E7C18

    // the character as the syllable index or the letter the sounds use
    u32 ConvertChar(u32 c) const; // 0x007217B0
    // the sound of a character (0: none); isSpeech: the sounds of the speech (the letters of the language)
    u32 GetSound(u32 c, u32 next, bool isSpeech) const; // 0x00721554
    // the sound of a letter with the letter after it (0: the letter alone)
    u32 GetEnglishSound(u32 c, u32 next) const; // 0x00721904
    u32 GetGermanSound(u32 c, u32 next) const; // 0x007228FC
    u32 GetFrenchSound(u32 c, u32 next) const; // 0x00722160
    u32 GetItalianSound(u32 c, u32 next) const; // 0x00723078
    u32 GetSpanishSound(u32 c, u32 next) const; // 0x007237E4

    static const f32 s_LevelVolumes[LEVEL_NUM];
    static const f32 s_ExtraPitches[EXTRA_VOICE_NUM];
    static const f32 s_FxSends[SoundFxMgr::FX_NUM];
    // the voice group of a voice (0 BOY, 1 GAL, 2 MAN, 3 BOY_GHOST, 4 GAL_GHOST)
    static const u32 s_VoiceGroups[VOICE_NUM];
    static const u16 s_ExtendedChars[EXTENDED_CHAR_NUM];
    static const u16 s_CompatibilityJamo[51];
    static const u16 s_InitialJamo[HANGUL_INITIAL_NUM];
    static const u16 s_MedialJamo[HANGUL_MEDIAL_NUM];
    static const u16 s_FinalJamo[27];
    static const u16 s_HangulSyllables[HANGUL_INITIAL_NUM][HANGUL_MEDIAL_NUM];
    static const u32 s_DigitSounds[LANGUAGE_NUM][10];
    static const u32 s_LetterSounds[LANGUAGE_NUM][26];
    // the index of the long form of a syllable (255: none)
    static const u8 s_LongSounds[SYLLABLE_NUM];
    static const VoiceParam s_VoiceParams[VOICE_PARAM_NUM];
    static const VoiceParam* s_pVoiceParams[VOICE_NUM][MOOD_NUM];
    // the pan of PlaySingle
    static const f32 s_SinglePanScale;

    nw::snd::SoundHandle m_Handles[HANDLE_NUM]; // 0x0C
    nw::snd::SoundHandle m_VoiceHandle;  // 0x18 PlaySingle
    s32 m_HandleIndex;                   // 0x1C
    s32 m_Language;                      // 0x20 Language
    u32 m_Mood;                          // 0x24
    u32 m_Voice;                         // 0x28
    s32 m_CharFrames;                    // 0x2C
    u8 m_Level;                          // 0x30 0..4 (s_LevelVolumes)
    u16 m_Queue[QUEUE_SIZE];             // 0x32
    s32 m_QueueFrames[QUEUE_SIZE];       // 0x40
    s32 m_QueueRead;                     // 0x58
    s32 m_QueueNext;                     // 0x5C the one after m_QueueRead
    s32 m_QueueWrite;                    // 0x60
    s32 m_QueueCount;                    // 0x64
    u32 m_LastSyllable;                  // 0x68 for CHAR_LONG
    bool m_IsSyllable;                   // 0x6C the last sound was a syllable
    const VoiceParam* m_pParam;          // 0x70
    f32 m_StepRange;                     // 0x74
    f32 m_Pitch;                         // 0x78
    f32 m_StepFall;                      // 0x7C
    f32 m_StepScale;                     // 0x80 (steps + 1) / (steps * 12)
    s32 m_IdleFrames;                    // 0x84 without a syllable (a reset after IDLE_RESET_FRAMES)
    u32 m_StepRiseFrames;                // 0x88 12 / (steps + 1)
    u32 m_StepFrame;                     // 0x8C
    bool m_IsSpeaking;                   // 0x90
    bool m_IsFalling;                    // 0x91
    u32 m_PhaseFrame;                    // 0x94
    u32 m_PhaseFrames;                   // 0x98
    bool m_IsEffectEnabled[8];           // 0x9C for 157, 158, 160, 161, 162/165, 164, '#', 166
    bool _A4;                            // 0xA4
    s32 m_Effect;                        // 0xA8 Effect
    f32 m_EffectPitch;                   // 0xAC EFFECT_RISE from here
    u32 m_EffectFrame;                   // 0xB0
    SoundRing<u32, EXTRA_VOICE_NUM> m_ExtraGroups; // 0xB4
    nw::snd::SoundHandle m_ExtraHandles[EXTRA_VOICE_NUM][HANDLE_NUM]; // 0xD0
    bool m_IsVoicePlaying;               // 0xF4 PlaySingle in this frame
    bool m_IsFx6;                        // 0xF5 SoundFxMgr effect 6 is on
    bool m_IsFilterRequested;            // 0xF6
    bool m_IsFilterOn;                   // 0xF7

private:
    // the parameter of the voice and the mood
    DECOMP_ALWAYS_INLINE void UpdateParam();
    void Reset(); // 0x0012FAC8
    // the next character of the queue
    DECOMP_ALWAYS_INLINE void Pop();
    // the syllable of a character; next: the character after it (0: none)
    DECOMP_NOINLINE void Play(u32 c, u32 next); // 0x002E737C
    DECOMP_ALWAYS_INLINE u32 GetLanguageSound(u32 c, u32 next) const;
};
ASSERT_OFFSET(SoundDoubutsugoMgr, m_Handles, 0x0C);
ASSERT_OFFSET(SoundDoubutsugoMgr, m_Queue, 0x32);
ASSERT_OFFSET(SoundDoubutsugoMgr, m_pParam, 0x70);
ASSERT_OFFSET(SoundDoubutsugoMgr, m_IsEffectEnabled, 0x9C);
ASSERT_OFFSET(SoundDoubutsugoMgr, m_ExtraGroups, 0xB4);
ASSERT_OFFSET(SoundDoubutsugoMgr, m_ExtraHandles, 0xD0);
ASSERT_OFFSET(SoundDoubutsugoMgr, m_IsVoicePlaying, 0xF4);
ASSERT_SIZE(SoundDoubutsugoMgr, 0xF8);
