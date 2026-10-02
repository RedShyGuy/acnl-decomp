#pragma once

#include "decomp.h"

namespace nw {
namespace gfx {
class CalculatedTransform
{
public:
    void SetRotateXYZ(float, float, float); // 0x0049FBBC | nintendogs:bytes [tier B]
    void SetTransform(const nn::math::Transform3&); // 0x0049FDE8 | nintendogs:bytes [tier A]
    void UpdateRotateFlags(); // 0x004A0020 | nintendogs:bytes [tier A]
    void UpdateCompositeFlags(); // 0x004A0120 | nintendogs:bytes [tier A]
    void UpdateTranslateFlags(); // 0x004A0154 | nintendogs:bytes [tier A]
    void NormalizeRotateMatrix(); // 0x004A01B0 | nintendogs:bytes-fuzzy [tier A]
    void SetRotateAndTranslate(const nn::math::VEC3&, const nn::math::VEC3&); // 0x004A02E8 | nintendogs:callgraph [tier A]
};
} // namespace gfx
} // namespace nw
