#pragma once

#include "decomp.h"
#include "nn/math/math_Vector2.h"
#include "nn/math/math_Vector3.h"
#include "sead/seadAudioSubsetBase.h"
#include "sead/seadCheckedArray.h"

class SoundObjSimple;

// RTTI 17SoundSharedObjMgr @ 0x008CC6DC
// vtable 0x008F3DBC (vptr 0x008F3DC4), offset_to_top 0, 20 entries
// Four sound objects for owners that have no sound object of their own: a sound takes a free object and gives it back when
// it ends (the class name is from the RTTI, everything else is ours).
class SoundSharedObjMgr : public ::sead::AudioSubsetBase
{
public:
    // an object and who uses it
    struct Entry
    {
        SoundObjSimple* obj; // 0x00
        const void* owner;   // 0x04 (NULL: free)
        u32 soundId;         // 0x08
    };

    SoundSharedObjMgr(); // 0x00128CB8
    // 0x0013E340
    // 0x002D3A2C (deleting dtor)
    virtual ~SoundSharedObjMgr();
    virtual void initialize(sead::AudioMgr* mgr, sead::Heap* heap) override; // 0x002D3200 slot 0x14
    virtual void finalize() override; // 0x002D3804 slot 0x18
    virtual void calc() override; // 0x002D3570 slot 0x1C

    // a sound on a free object
    void PlaySound(u32 soundId, const nn::math::VEC3* world, const nn::math::VEC3* relative, const void* owner, s16 value,
                   bool isIgnoreMaster); // 0x002D332C
    void PlaySound(u32 soundId, const nn::math::VEC3* world, const nn::math::VEC3* relative, const void* owner,
                   bool isIgnoreMaster); // 0x002D37B8
    void PlaySoundWithValue(u32 soundId, const nn::math::VEC3* world, const nn::math::VEC3* relative, const void* owner,
                            s16 value, bool isIgnoreMaster); // 0x002D37E4
    // a held sound (the object that has it, else a free one)
    void HoldSound(u32 soundId, const nn::math::VEC3* world, const nn::math::VEC3* relative,
                   const void* owner); // 0x002D3634
    // a sequence variable of the sound of the owner (-1: variable 15)
    void WriteVariable(u32 soundId, const void* owner, s16 value, s32 variable); // 0x002D344C
    void OnEnterStage(); // 0x002D3518
    void SetVolumeScale(const void* owner, f32 scale); // 0x002D38B4
    // all sounds (-1: 5 frames)
    void StopSounds(s32 fadeFrames); // 0x002D38FC
    void StopSounds(const void* owner, s32 fadeFrames); // 0x002D3964
    void SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative, const void* owner); // 0x002D39D0
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsUsedBy(const void* owner) const // 0x00720560
    {
        for (s32 i = 0; i < 4; i++) {
            if (m_Entries[i].owner == owner) {
                return true;
            }
        }
        return false;
    }

    bool m_IsInitialized;                   // 0x0C
    bool m_IsDisabled;                      // 0x0D
    sead::CheckedArray<Entry, 4> m_Entries; // 0x10
};
ASSERT_OFFSET(SoundSharedObjMgr, m_Entries, 0x10);
ASSERT_SIZE(SoundSharedObjMgr, 0x40);
