#pragma once

#include "decomp.h"
#include "Sound/dSoundHandleSet.h"
#include "Sound/dSoundObjBase.h"

// Instantiations found in the binary:
//   SoundObj<1>  typeinfo 0x008CD578  vtable 0x008F9E30
//   SoundObj<2>  typeinfo 0x008CD584  vtable 0x008F9E70
//   SoundObj<3>  typeinfo 0x008CD590  vtable 0x008F9EB0
//   SoundObj<4>  typeinfo 0x008CD59C  vtable 0x008F9EF0
// A sound source with N sounds at the same time; the sounds follow the volume and the pans of the object (the class name
// is from the RTTI, everything else is ours).
template <auto T0>
class SoundObj : public ::SoundObjBase
{
public:
    SoundObj() {}
    virtual ~SoundObj();
    virtual void Stop() override;
    virtual void SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative) override;
    virtual bool UpdateChanged() override;
    virtual sead::SoundHandle* StartSound(u32 soundId); // slot 0x1C
    // with the sequence variable 15
    virtual sead::SoundHandle* StartSound(u32 soundId, s16 value); // slot 0x20
    virtual sead::SoundHandle* HoldSound(u32 soundId); // slot 0x24
    virtual sead::SoundHandle* HoldSound(u32 soundId, s16 value); // slot 0x28
    virtual sead::SoundHandle* PrepareSound(u32 soundId); // slot 0x2C
    virtual void StopSounds(s32 fadeFrames); // slot 0x30
    virtual void SetSoundVolume(f32 volume, s32 index); // slot 0x34

    // the volume and the parameters of the object for a playing sound (name is ours; the second half of SetPosition)
    void UpdateHandle(s32 index)
    {
        if (m_HandleSet.m_Handles[index].m_pSound != NULL) {
            SetSoundVolume(m_Volume, index);
            SoundHandleSetBase::ApplyParam(&m_HandleSet.m_Handles[index], m_PlayerPriority, &m_HandleSet.m_Params[index], m_Pan,
                                           m_SurroundPan);
        }
    }

    SoundHandleSet<T0> m_HandleSet; // 0x44

private:
    // the index of the handle for a new sound and its parameters (-1: it does not start)
    DECOMP_ALWAYS_INLINE s32 PrepareStart(u32 soundId, bool isStopEqual, const SoundParam** outParam);
    DECOMP_ALWAYS_INLINE void ApplyToHandle(s32 index, const SoundParam* param);
};
