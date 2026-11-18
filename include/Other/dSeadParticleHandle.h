#pragma once

#include "decomp.h"
#include "math/dAngle16.h"
#include "nn/math/math_Vector3.h"
#include "ssys/st/dList.h"
#include "ssys/st/dListNode.h"
#include "sead/ptcl/seadEmitterSet.h"

// RTTI 18SeadParticleHandle @ 0x008CC8BC
// vtable 0x008F4A94 (vptr 0x008F4A9C), offset_to_top 0, 2 entries
class SeadParticleHandle : public ::ssys::st::ListNode
{
public:
    SeadParticleHandle(); // 0x00316940
    // (leaves s_List when it is in it)
    virtual ~SeadParticleHandle(); // 0x002E6F10 slot 0x00
    // 0x002E6EC4 (deleting dtor)

    // (names are ours) the emitter set while it still belongs to the handle (NULL: none; inline)
    sead::ptcl::EmitterSet* GetEmitterSet() const
    {
        if (m_pEmitterSet != NULL && m_CreateId == m_pEmitterSet->mCreateID) {
            return m_pEmitterSet;
        }
        return NULL;
    }
    // whether the effect still runs: the emitter set still belongs to the handle and has emitters
    bool IsAlive() const; // 0x00721490
    // (names after the addresses) a flag of the emitter set / its velocity (meaning unknown) and its matrix (EmitterSet::setMtx)
    void Func2E6848(u32 value); // 0x002E6848
    void Func2E64A4(const nn::math::VEC3* value); // 0x002E64A4
    void Func2E6550(const nn::math::VEC3* direction); // 0x002E6550
    void SetMatrix(const nn::math::MTX34* matrix); // 0x002E6C58
    // kills the effect
    void Kill(); // 0x002E6934
    // the scale of the volumes of the emitters (EmitterSet::mEmitterVolumeScale)
    void SetEmitterVolumeScale(const nn::math::VEC3* scale); // 0x002E659C
    // the factors of the emit rate / the emit interval of all emitters (1: as in the resource)
    void SetEmitRate(f32 rate); // 0x002E65E0
    void SetEmitInterval(f32 interval); // 0x002E6734
    // the callback for the particles of the emitter at index (-1: of all emitters)
    void SetPtclCallback(sead::ptcl::PtclCallback callback, s32 index); // 0x002E6880
    // the matrix of the emitter set at position with rotate (on the curve of the world on stages that have it,
    // isCurvedFromCamera: relative to the camera)
    void SetTransform(const nn::math::VEC3* position, const math::Angle16Vec3* rotate, bool isCurvedFromCamera); // 0x002E6ACC

    // the handles with bit 0 of m_Flags
    static ssys::st::List s_List;

    sead::ptcl::EmitterSet* m_pEmitterSet; // 0x0C
    u32 m_CreateId;                        // 0x10 the create id of the emitter set when the effect started
    u8 m_Flags;                            // 0x14 (bit 0: the destructor removes the handle from a list)
};
ASSERT_SIZE(SeadParticleHandle, 0x18);

// starts an effect at a position (the name is after the address until the owner is known)
bool Func207998(SeadParticleHandle* handle, u32 effectId, const nn::math::VEC3* position, const math::Angle16Vec3* rotate,
                const nn::math::VEC3* scale, bool isCurvedFromCamera, u32 unknown); // 0x00207998
// a particle callback of the game (sets something to 0x183; name after the address)
void Func2080B0(sead::ptcl::EmitterInstance* emitter, sead::ptcl::PtclInstance* ptcl); // 0x002080B0
// the same without keeping the handle
bool Func207B90(u32 effectId, const nn::math::VEC3* position, const math::Angle16Vec3* rotate, const nn::math::VEC3* scale,
                bool isCurvedFromCamera, u32 unknown); // 0x00207B90
