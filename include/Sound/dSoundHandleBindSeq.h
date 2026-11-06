#pragma once

#include "decomp.h"
#include "sead/seadSoundHandle.h"
#include "nw/snd/snd_SequenceSoundHandle.h"

// RTTI 18SoundHandleBindSeq @ 0x008CC8D4
// vtable 0x008F4AFC (vptr 0x008F4B04), offset_to_top 0, 2 entries
// A sound handle together with a handle to the same sound as a sequence sound (base at +4 after the RTTI; the
// member name is ours).
class SoundHandleBindSeq : public ::sead::SoundHandle
{
public:
    SoundHandleBindSeq();
    virtual ~SoundHandleBindSeq();
    // the handle of the sequence is bound when the sound is a sequence
    void BindSequence(); // 0x002E82EC

    nw::snd::SequenceSoundHandle m_SequenceHandle; // 0x08
};
ASSERT_SIZE(SoundHandleBindSeq, 0xC);
