#pragma once

#include "decomp.h"
#include "nn/math/math_MTX44.h"

namespace nn {
namespace math {
class MTX34;
struct MTX33;
struct QUAT;
struct VEC3;

// ARMv6: the assembly versions (*Asm, copied from the binary as naked functions) and the fast C
// versions (*C_FAST) of the math functions (free functions: the callers pass no object)
namespace ARMv6 {
#define MATH_ASM __attribute__((naked))
    void MTX34CopyAsm(nn::math::MTX34* pOut, const nn::math::MTX34* p) MATH_ASM; // 0x00135C3C | nintendogs:bytes [tier A]
    void VEC3TransformAsm(nn::math::VEC3* pOut, const nn::math::MTX33* pM, const nn::math::VEC3* pV) MATH_ASM; // 0x0014892C | nintendogs:callgraph [tier A]
    void MTX34MultAsm(nn::math::MTX34* pOut, const nn::math::MTX34* p1, const nn::math::MTX34* p2) MATH_ASM; // 0x00148960 | nintendogs:callgraph [tier A]
    void MTX34MultScaleAsm(nn::math::MTX34* pOut, const nn::math::MTX34* pM, const nn::math::VEC3* pScale) MATH_ASM; // 0x00148A44 | nintendogs:bytes [tier A]
    // false when the matrix cannot be inverted
    u32 MTX34InverseAsm(nn::math::MTX34* pOut, const nn::math::MTX34* p) MATH_ASM; // 0x00148A78 | nintendogs:bytes [tier A]
    u32 MTX34InvTransposeAsm(nn::math::MTX34* pOut, const nn::math::MTX34* p) MATH_ASM; // 0x00148B94 | nintendogs:bytes [tier A]
    void MTX34MultTranslateAsm(nn::math::MTX34* pOut, const nn::math::MTX34* pM, const nn::math::VEC3* pT) MATH_ASM; // 0x00148C90 | nintendogs:bytes [tier A]
    void VEC3TransformAsm(nn::math::VEC3* pOut, const nn::math::MTX34* pM, const nn::math::VEC3* pV) MATH_ASM; // 0x00148CC4 | nintendogs:callgraph [tier A]
    void MTX34TransposeAsm(nn::math::MTX34* pOut, const nn::math::MTX34* p) MATH_ASM; // 0x00148D00 | nintendogs:bytes [tier A]
    void MTX44CopyAsm(nn::math::MTX44* pOut, const nn::math::MTX44* p) MATH_ASM; // 0x00148D44 | nintendogs:bytes [tier A]
    void MTX34ToMTX33Asm(nn::math::MTX33* pOut, const nn::math::MTX34* p) MATH_ASM; // 0x00148D58 | nintendogs:bytes [tier A]
    // the 3x3 part of two matrices (rows of sizeof(T) / 3 bytes)
    template <typename T>
    void MTX33MultAsm(T* pOut, const T* p1, const T* p2) MATH_ASM;

    f32 HermiteC_FAST(f32 p1, f32 t1, f32 p2, f32 t2, f32 s, f32 d); // 0x0047DEF0 | nintendogs:bytes [tier A]
    // an orthographic projection (name is ours, after its neighbours)
    nn::math::MTX44* MTX44OrthoC_FAST(nn::math::MTX44* pOut, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f); // 0x0047DF3C (name is ours)
    nn::math::MTX44* MTX44PivotC_FAST(nn::math::MTX44* pM, nn::math::PivotDirection pivot); // 0x0047DFCC | mk7dlp:bytes [tier A]
    nn::math::MTX34* MTX34LookAtC_FAST(nn::math::MTX34* pOut, const nn::math::VEC3* pCameraPosition, const nn::math::VEC3* pCameraUp, const nn::math::VEC3* pTarget); // 0x0047E088 | nintendogs:bytes [tier A]
    nn::math::QUAT* MTX34ToQUATC_FAST(nn::math::QUAT* pOut, const nn::math::MTX34* pM); // 0x0047E180 | nintendogs:bytes [tier A]
    // with clearTranslation the translation is set to 0, else it is kept
    nn::math::MTX34* QUATToMTX34C_FAST(nn::math::MTX34* pOut, const nn::math::QUAT* pQ, bool clearTranslation); // 0x0047E324 | fefates:bytes [tier B]
    nn::math::MTX44* MTX44FrustumC_FAST(nn::math::MTX44* pOut, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f); // 0x0047E3F8 | nintendogs:bytes [tier A]
#undef MATH_ASM
} // namespace ARMv6
} // namespace math
} // namespace nn
