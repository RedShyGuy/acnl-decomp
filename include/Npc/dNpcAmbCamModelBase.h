#pragma once

#include "decomp.h"
#include "math/dAngle16.h"
#include "nn/math/math_MTX34.h"

class Actor;

// A human model of the amiibo camera (class name from the RTTI, ModuleAmiiboCamera builds them in its actors; the members
// and their names are ours): the model of T turned by its own rotation (x, then y) at the translation of the world
// matrix.
// Instantiations found in the binary:
//   NpcAmbCamModelBase<NpcModel>  typeinfo 0x008CC898  vtable 0x008F49E4
//   NpcAmbCamModelBase<NpcTopsChangeModel>  typeinfo 0x008CC88C  vtable 0x008F498C
//   NpcAmbCamModelBase<PlayerModel>  typeinfo 0x008CC880  vtable 0x008F4958
template <typename T>
class NpcAmbCamModelBase : public T
{
public:
    // (inline in the factories of the actors of the module; args: the arguments of the constructor of T)
    template <typename... TArgs>
    explicit NpcAmbCamModelBase(Actor* owner, TArgs... args) : T(args...), m_pOwner(owner), m_AmbScale(1.0f)
    {
        m_AmbRotation.x.value = 0;
        m_AmbRotation.y.value = 0;
        m_AmbRotation.z.value = 0;
    }
    virtual ~NpcAmbCamModelBase(); // slot 0x1C
    // the world matrix: the rotation of the model at the translation of mtx (the rotation of mtx is not used)
    void SetMtx(const nn::math::MTX34* mtx) override; // slot 0x24

    Actor* m_pOwner;                 // +0x0 behind T: the actor of the module
    math::Angle16Vec3 m_AmbRotation; // +0x4 (z is not used here)
    f32 m_AmbScale;                  // +0xC (1; not used here)
};

#include "ssys/ma/ma_Math.h"

template <typename T>
DECOMP_NOINLINE DECOMP_NOIPA inline NpcAmbCamModelBase<T>::~NpcAmbCamModelBase()
{
    // (only the base is destroyed)
}

template <typename T>
inline void NpcAmbCamModelBase<T>::SetMtx(const nn::math::MTX34* mtx)
{
    nn::math::MTX34 rotation;
    ssys::ma::SetRotateX(&rotation, m_AmbRotation.x.value);
    ssys::ma::RotateY(&rotation, m_AmbRotation.y.value);
    rotation.m[0][3] = mtx->m[0][3];
    rotation.m[1][3] = mtx->m[1][3];
    rotation.m[2][3] = mtx->m[2][3];
    T::SetMtx(&rotation);
}
