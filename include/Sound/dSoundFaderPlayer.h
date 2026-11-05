#pragma once

#include "decomp.h"
#include "Sound/dSoundFader.h"

#include "nw/snd/snd_SoundPlayer.h"

// RTTI 16SoundFaderPlayer @ 0x008CC3D0
// vtable 0x008F2E18 (vptr 0x008F2E20), offset_to_top 0, 5 entries
// A fader for the volume of a sound player (names are ours).
class SoundFaderPlayer : public ::SoundFader
{
public:
    DECOMP_EMIT_COPY SoundFaderPlayer() : m_pPlayer(NULL), m_pScale(NULL) {} // 0x0012FA44
    virtual ~SoundFaderPlayer();
    // one frame; sets the volume of the player (value times the scale)
    virtual f32 Update() override; // 0x002BD1CC slot 0x0C

    // to the target; a fade-in that is under way keeps its speed
    void SetTarget(f32 target, s32 frames); // 0x002BD30C

    // the player at the volume
    DECOMP_EMIT_COPY void Initialize(nw::snd::SoundPlayer* player, f32 volume) // 0x004BFB04
    {
        SoundFader::Reset(volume);
        m_pPlayer = player;
        ApplyVolume();
    }
    void SetScale(const f32* scale) { m_pScale = scale; }

private:
    void ApplyVolume()
    {
        if (m_pScale != NULL) {
            m_pPlayer->SetVolume(m_Value * *m_pScale);
        } else {
            m_pPlayer->SetVolume(m_Value);
        }
    }

    nw::snd::SoundPlayer* m_pPlayer; // 0x14
    const f32* m_pScale;             // 0x18 an extra factor for the volume (optional)
};
ASSERT_SIZE(SoundFaderPlayer, 0x1C);
