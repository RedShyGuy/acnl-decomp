#include "nn/math/math_MTX44.h"

namespace nn {
namespace math {
// 0x0047E51C | nintendogs:callgraph [tier A]
const nn::math::MTX44& nn::math::MTX44::Identity()
{
    // 0x00AF9224
    static const MTX44 s_Identity(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    return s_Identity;
}

} // namespace math
} // namespace nn
