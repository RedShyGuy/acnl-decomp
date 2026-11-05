#pragma once

#include "decomp.h"
#include "Sound/dSoundFader.h"
#include "Sound/dSoundMelodyPlayerOneSeqBase.h"

// RTTI 31SoundMelodyPlayerOneSeqTimeTone @ 0x008CD13C
// vtable 0x008F82B8 (vptr 0x008F82C0), offset_to_top 0, 3 entries
// The one-sequence player of the time chime, with a low-pass filter and a volume that fade (the class name is from the
// RTTI, everything else is ours).
class SoundMelodyPlayerOneSeqTimeTone : public ::SoundMelodyPlayerOneSeqBase
{
public:
    SoundMelodyPlayerOneSeqTimeTone(); // 0x00132874
    // 0x004DBDD4
    // 0x004DBD98 (deleting dtor)
    virtual ~SoundMelodyPlayerOneSeqTimeTone();

    void Initialize(); // 0x004DBB98
    void Start(u32 soundId, const u8* notes); // 0x004DBBC8
    void Update(); // 0x001B0A54
    // a light low-pass filter (fades in 20 frames)
    void SetMuffled(bool isMuffled); // 0x004DBCF4
    // a stronger low-pass filter and less volume (fades in 90 frames, back in 150)
    void SetDistant(bool isDistant); // 0x004DBC2C

    bool m_IsMuffled;          // 0x12
    bool m_IsDistant;          // 0x13
    u8 m_IsDisabled;           // 0x14 Start does nothing
    SoundFader m_FilterFader;  // 0x18 the low-pass filter (biquad 1)
    SoundFader m_VolumeFader;  // 0x2C

private:
    void ApplyFaders();
};
ASSERT_OFFSET(SoundMelodyPlayerOneSeqTimeTone, m_FilterFader, 0x18);
ASSERT_SIZE(SoundMelodyPlayerOneSeqTimeTone, 0x40);
