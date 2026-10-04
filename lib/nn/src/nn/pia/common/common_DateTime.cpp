#include "nn/pia/common/common_DateTime.h"

namespace nn {
namespace pia {
namespace common {
// 0x00429434
// 0x00429430 (deleting dtor)
nn::pia::common::DateTime::~DateTime()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace common
} // namespace pia
} // namespace nn
