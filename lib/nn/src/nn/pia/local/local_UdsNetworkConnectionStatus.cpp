#include "nn/pia/local/local_UdsNetworkConnectionStatus.h"

namespace nn {
namespace pia {
namespace local {
// 0x007316B8 (name is ours)
u8 nn::pia::local::UdsNetworkConnectionStatus::GetNodeCount() const
{
    return m_Status.nodeCount;
}

} // namespace local
} // namespace pia
} // namespace nn
