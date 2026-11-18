#include "nn/math/math_MTX34.h"

// the identity matrix of the game (name is ours; the Mii module sends it as the view matrix of its models and icons)
// 0x00AE6818
extern const nn::math::MTX34 MTX34_IDENTITY(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

// sets MTX34_IDENTITY
// 0x007A03AC (static initializer)
