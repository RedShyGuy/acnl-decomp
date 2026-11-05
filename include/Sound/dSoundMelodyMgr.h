#pragma once

#include "decomp.h"
#include "Sound/dSoundMelodyPlayerBase.h"
#include "Sound/dSoundMelodyPlayerOneSeqBase.h"
#include "Sound/dSoundMelodyPlayerOneSeqTimeTone.h"
#include "nw/snd/snd_SoundHandle.h"
#include "sead/seadAudioSubsetBase.h"
#include "sead/seadBufferedSafeStringBase.h"

// RTTI 14SoundMelodyMgr @ 0x008CBDA8
// vtable 0x008F0768 (vptr 0x008F0770), offset_to_top 0, 20 entries
// The melodies: a melody player (the letter melody, the melodies of characters), a one-sequence player, the time chime,
// single notes of the melody editor and the current melody (the town tune) (the class name is from the RTTI, everything
// else is ours).
class SoundMelodyMgr : public ::sead::AudioSubsetBase
{
public:
    SoundMelodyMgr(); // 0x001289F0
    // 0x002771F0
    // 0x0027718C (deleting dtor)
    virtual ~SoundMelodyMgr();
    virtual void initialize(sead::AudioMgr* mgr, sead::Heap* heap) override; // 0x004DBB90 slot 0x14
    virtual void calc() override; // 0x002770E8 slot 0x1C

    // a melody (SE_MELODY_LETTER starts after 12 frames)
    void Play(u32 soundId, const u8* notes); // 0x00276DC8
    // the melody of a character (by the 3-letter name: SE_MELODY_NPCNRM_<NAME> or SE_MELODY_NPCSP_<NAME>)
    void PlayCharacter(const sead::SafeStringBase<char>& name, const u8* notes); // 0x00276F20
    // a note of the melody editor (the pitch goes into the sequence variable 0)
    void PlayEditorNote(u32 soundId, const u8* note); // 0x00276E58
    void PlayTimeChime(); // 0x00276F0C
    void Stop(); // 0x00343284
    void SetMelody(const SoundMelody& melody); // 0x00277168

    // a default melody (index 4 and above: the first)
    DECOMP_EMIT_COPY DECOMP_NOINLINE const SoundMelody* GetDefaultMelody(u32 index) const // 0x0071AB90
    {
        if (index < 4) {
            return &s_DefaultMelodies[index];
        }
        return &s_DefaultMelodies[0];
    }
    // the fifth default melody
    DECOMP_EMIT_COPY DECOMP_NOINLINE const SoundMelody* GetExtraDefaultMelody() const // 0x00719BD8
    {
        return &s_DefaultMelodies[4];
    }
    // one of the first four default melodies
    const SoundMelody* GetRandomDefaultMelody() const;
    // the letters a..z of the string in upper case
    void ToUpper(sead::BufferedSafeStringBase<char>* str) const;

    static const SoundMelody s_DefaultMelodies[5];
    // the pitches of the notes of the melody editor (13: random)
    static const s16 s_EditorPitches[14];

    SoundMelodyPlayerBase m_Player;                // 0x0C
    SoundMelodyPlayerOneSeqBase m_OneSeqPlayer;    // 0x58
    SoundMelodyPlayerOneSeqTimeTone m_TimeChime;   // 0x6C
    SoundMelody m_Melody;                          // 0xAC
    nw::snd::SoundHandle m_EditorHandle;           // 0xBC
    s32 m_Delay;                                   // 0xC0 the frames until the delayed melody
    const u8* m_pDelayedNotes;                     // 0xC4
};
ASSERT_OFFSET(SoundMelodyMgr, m_Player, 0x0C);
ASSERT_OFFSET(SoundMelodyMgr, m_TimeChime, 0x6C);
ASSERT_OFFSET(SoundMelodyMgr, m_Melody, 0xAC);
ASSERT_SIZE(SoundMelodyMgr, 0xC8);
