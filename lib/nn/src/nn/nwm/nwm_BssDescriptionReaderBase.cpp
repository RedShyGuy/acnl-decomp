#include "nn/nwm/nwm_BssDescriptionReaderBase.h"

namespace nn {
namespace nwm {
// ctor address unknown
nn::nwm::BssDescriptionReaderBase::BssDescriptionReaderBase()
{
}

// 0x0072EC8C | fefates:bytes [tier B]
nn::nwm::Mac nn::nwm::BssDescriptionReaderBase::GetBssid() const
{
}

} // namespace nwm
} // namespace nn
