#include "nn/math/math_MTX34.h"

namespace nn {
namespace math {
// 0x0047E49C | nintendogs:callgraph [tier A]
const nn::math::MTX34& nn::math::MTX34::Identity()
{
    // 0x00AF91F4
    static const MTX34 s_Identity(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    return s_Identity;
}

} // namespace math
} // namespace nn
