#include "sead/seadColor4f.h"
#include "sead/seadVector3.h"

namespace sead {
namespace {
inline f32 clamp01(f32 value)
{
    if (value < 0.0f) {
        return 0.0f;
    }
    if (value > 1.0f) {
        return 1.0f;
    }
    return value;
}
} // namespace

// 0x0055DFF8 | nintendogs:bytes [tier A]
void sead::Color4f::adjustOverflow_()
{
    r = clamp01(r);
    g = clamp01(g);
    b = clamp01(b);
    a = clamp01(a);
}

// 0x0055E0A8 (name is ours)
sead::Color4f sead::Color4f::lerp(const Color4f& a, const Color4f& b, f32 t)
{
    t = clamp01(t);
    Color4f color;
    color.a = a.a + (b.a - a.a) * t;
    color.r = a.r + (b.r - a.r) * t;
    color.g = a.g + (b.g - a.g) * t;
    color.b = a.b + (b.b - a.b) * t;
    return color;
}

// 0x0055E11C | nintendogs:bytes [tier A]
void sead::Color4f::setLerp(const Color4f& a, const Color4f& b, f32 t)
{
    t = clamp01(t);
    this->a = a.a + (b.a - a.a) * t;
    this->r = a.r + (b.r - a.r) * t;
    this->g = a.g + (b.g - a.g) * t;
    this->b = a.b + (b.b - a.b) * t;
}

// the instance lies behind Color4f::setLerp in the original, whose end (r, g and b) falls into it
// 0x0055E154 | nintendogs:bytes [tier A]
template void sead::Vector3<float>::setLerp<float>(const Vector3<float>& a, const Vector3<float>& b, float t);

} // namespace sead
