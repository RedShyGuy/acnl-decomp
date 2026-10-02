#pragma once

#include "decomp.h"

namespace nn {
namespace math {
// ARMv6: the assembly / fast C versions of the math functions (free functions: the callers pass
// no object)
namespace ARMv6 {
    void MTX34CopyAsm(nn::math::MTX34*, const nn::math::MTX34*); // 0x00135C3C | nintendogs:bytes [tier A]
    void VEC3TransformAsm(nn::math::VEC3*, const nn::math::MTX33*, const nn::math::VEC3*); // 0x0014892C | nintendogs:callgraph [tier A]
    void MTX34MultAsm(nn::math::MTX34*, const nn::math::MTX34*, const nn::math::MTX34*); // 0x00148960 | nintendogs:callgraph [tier A]
    void MTX34MultScaleAsm(nn::math::MTX34*, const nn::math::MTX34*, const nn::math::VEC3*); // 0x00148A44 | nintendogs:bytes [tier A]
    void MTX34InverseAsm(nn::math::MTX34*, const nn::math::MTX34*); // 0x00148A78 | nintendogs:bytes [tier A]
    void MTX34InvTransposeAsm(nn::math::MTX34*, const nn::math::MTX34*); // 0x00148B94 | nintendogs:bytes [tier A]
    void MTX34MultTranslateAsm(nn::math::MTX34*, const nn::math::MTX34*, const nn::math::VEC3*); // 0x00148C90 | nintendogs:bytes [tier A]
    void VEC3TransformAsm(nn::math::VEC3*, const nn::math::MTX34*, const nn::math::VEC3*); // 0x00148CC4 | nintendogs:callgraph [tier A]
    void MTX34TransposeAsm(nn::math::MTX34*, const nn::math::MTX34*); // 0x00148D00 | nintendogs:bytes [tier A]
    void MTX44CopyAsm(nn::math::MTX44*, const nn::math::MTX44*); // 0x00148D44 | nintendogs:bytes [tier A]
    void MTX34ToMTX33Asm(nn::math::MTX33*, const nn::math::MTX34*); // 0x00148D58 | nintendogs:bytes [tier A]
    void HermiteC_FAST(float, float, float, float, float, float); // 0x0047DEF0 | nintendogs:bytes [tier A]
    void MTX44PivotC_FAST(nn::math::MTX44*, nn::math::PivotDirection); // 0x0047DFCC | mk7dlp:bytes [tier A]
    void MTX34LookAtC_FAST(nn::math::MTX34*, const nn::math::VEC3*, const nn::math::VEC3*, const nn::math::VEC3*); // 0x0047E088 | nintendogs:bytes [tier A]
    void MTX34ToQUATC_FAST(nn::math::QUAT*, const nn::math::MTX34*); // 0x0047E180 | nintendogs:bytes [tier A]
    void QUATToMTX34C_FAST(nn::math::MTX34*, const nn::math::QUAT*, bool); // 0x0047E324 | fefates:bytes [tier B]
    void MTX44FrustumC_FAST(nn::math::MTX44*, float, float, float, float, float, float); // 0x0047E3F8 | nintendogs:bytes [tier A]
} // namespace ARMv6
} // namespace math
} // namespace nn
