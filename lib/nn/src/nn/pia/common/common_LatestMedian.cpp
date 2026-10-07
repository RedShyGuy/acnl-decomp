#include "nn/pia/common/common_LatestMedian.h"

namespace nn {
namespace pia {
namespace common {
// 0x00827DA0 | fefates:bytes [tier B]
template int LatestMedian<int, 16>::GetMedian(unsigned int num) const;
} // namespace common
} // namespace pia
} // namespace nn
