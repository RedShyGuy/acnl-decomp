#include "nn/pia/local/local_LocalStreamBase.h"

namespace nn {
namespace pia {
namespace local {
// 0x00416A2C
nn::pia::local::LocalStreamBase::LocalStreamBase()
{
    // only the vptr (in the original too)
}

// 0x00416A44
// 0x00416A3C (deleting dtor)
nn::pia::local::LocalStreamBase::~LocalStreamBase()
{
    // empty (in the original too)
}

// 0x00730244
void nn::pia::local::LocalStreamBase::vf_0x08()
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
