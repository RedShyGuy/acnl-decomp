#include "Other/dSeadParticleHandle.h"

namespace {
// the factors of the emitter records are 8.8 fixed point numbers
const f32 FIXED_POINT_ONE = 256.0f;
} // namespace

// (name is ours)
// 0x00ABF0AC
ssys::st::List SeadParticleHandle::s_List;

// constructs s_List and registers its destructor
// 0x0079CED8 (static initializer)

// 0x002E659C (name is ours)
void SeadParticleHandle::SetEmitterVolumeScale(const nn::math::VEC3* scale)
{
    sead::ptcl::EmitterSet* set = GetEmitterSet();
    if (set != NULL) {
        set->mEmitterVolumeScale.x = scale->x;
        set->mEmitterVolumeScale.y = scale->y;
        set->mEmitterVolumeScale.z = scale->z;
    }
}

// 0x002E65E0 (name is ours)
void SeadParticleHandle::SetEmitRate(f32 rate)
{
    sead::ptcl::EmitterSet* set = GetEmitterSet();
    if (set != NULL) {
        s32 value = static_cast<s32>(rate * FIXED_POINT_ONE);
        for (s32 i = 0; i < set->mNumEmitter; i++) {
            set->mRecord[i].emitRate = value;
        }
    }
}

// 0x002E6734 (name is ours)
void SeadParticleHandle::SetEmitInterval(f32 interval)
{
    sead::ptcl::EmitterSet* set = GetEmitterSet();
    if (set != NULL) {
        s32 value = static_cast<s32>(interval * FIXED_POINT_ONE);
        for (s32 i = 0; i < set->mNumEmitter; i++) {
            set->mRecord[i].emitInterval = value;
        }
    }
}

// 0x002E6F10 slot 0x00
// 0x002E6EC4 (deleting dtor)
SeadParticleHandle::~SeadParticleHandle()
{
    if (m_Flags & 1) {
        s_List.Erase(this);
        m_Flags &= ~1;
    }
}

// 0x00316940 (name is ours)
SeadParticleHandle::SeadParticleHandle() : m_pEmitterSet(NULL), m_Flags(0)
{
}

// 0x00721490 (name is ours)
bool SeadParticleHandle::IsAlive() const
{
    sead::ptcl::EmitterSet* set = GetEmitterSet();
    return set != NULL && set->mNumCreated > 0;
}
