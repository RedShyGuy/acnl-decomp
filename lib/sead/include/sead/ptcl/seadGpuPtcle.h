#pragma once

#include "decomp.h"

namespace sead {
namespace ptcl {
class GpuPtcle
{
public:
    void InitShader(const unsigned char*, int, int); // 0x0055C5C4 | mk7dlp:bytes [tier B]
    void InitPrimitive(); // 0x0055C778 | mk7dlp:bytes [tier B]
    void Draw(unsigned*, nn::math::MTX34&, nn::math::VEC4&, nn::math::VEC4&, nn::math::VEC4&, nn::math::VEC4&, nn::math::VEC4&, nn::math::VEC4&, nn::math::VEC4&, nn::math::VEC4&, nn::math::VEC4&, nn::math::VEC4&, nn::math::VEC4&, nn::math::VEC4&, int, int, int); // 0x0055C8CC | mk7dlp:bytes [tier B]
    void Init(sead::ptcl::PtclRenderer*, const unsigned char*, int, int); // 0x0055CBB0 | mk7dlp:bytes [tier B]
    void PreDraw(unsigned*); // 0x0055CCA0 | mk7dlp:bytes [tier B]
    void PostDraw(unsigned*); // 0x0055CD64 | mk7dlp:bytes [tier B]
    GpuPtcle(); // 0x0055CD98 | mk7dlp:bytes-fuzzy [tier B]
};
} // namespace ptcl
} // namespace sead
