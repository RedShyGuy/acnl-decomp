#pragma once

#include "decomp.h"
#include "Other/dSeadParticleHandle.h"
#include "fgobj/dObjectStump.h"
#include "g3d/dSkeletalAnim.h"
#include "nn/math/math_Vector3.h"

class SvFgName;
namespace math {
class Angle16;
} // namespace math

namespace fgobj {
// A bush that is dug out: an ObjectStump with the shake animation of the bush and the effects of its leaves and, in the
// right season part, of its flowers (the class name is from the RTTI, the rest is ours).
// RTTI N5fgobj15ObjectStumpAnimE @ 0x008D2A0C
// vtable 0x00908D84 (vptr 0x00908D8C), offset_to_top 0, 10 entries
class ObjectStumpAnim : public ::fgobj::ObjectStump
{
public:
    // the bush of item at the unit x, z that flies off in the direction of angle
    static ObjectStumpAnim* Create(const SvFgName& item, const s32* x, const s32* z, const math::Angle16* angle); // 0x005967D0

    virtual ~ObjectStumpAnim(); // 0x00596AB4 slot 0x00
    // 0x00596A58 (deleting dtor)
    // kills the effects, then ObjectBase::Finalize
    void Finalize() override; // 0x00596908 slot 0x14
    // ObjectStump::Update, the effects follow the bush
    s32 Execute() override; // 0x0059697C slot 0x20

    void Setup(const SvFgName& item, const s32* x, const s32* z); // 0x005964C8
    // the position of the effects: the bone "j_main", moved by the size of the bush
    bool GetEffectPosition(nn::math::VEC3* out) const; // 0x00596010
    // the effect of the leaves of the bush by the season part (Func1E6444(5); -1: none)
    s32 GetEffectId() const; // 0x00596600
    // the effects of the leaves, the leaf effect 211 (when Func63061C is true) and the flowers
    void StartEffects(); // 0x00596128

    // the effects of the leaves of the bushes by Func1E6444(5): holly, azaleas, hydrangeas, hibiscus, sweet olive
    static const s32 s_LeafEffects[5][18];

    g3d::SkeletalAnim m_SkeletalAnim;   // 0x144
    SeadParticleHandle m_Effect;        // 0x15C
    SeadParticleHandle m_LeafEffect;    // 0x174
    SeadParticleHandle m_FlowerEffect;  // 0x18C
};
ASSERT_SIZE(ObjectStumpAnim, 0x1A4);
} // namespace fgobj
