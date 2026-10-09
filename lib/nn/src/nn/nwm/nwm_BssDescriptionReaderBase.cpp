#include "nn/nwm/nwm_BssDescriptionReaderBase.h"

namespace nn {
namespace nwm {
// 0x0072EC6C | tier C
u8 nn::nwm::BssDescriptionReaderBase::GetChannel() const
{
    if (m_pBss == NULL) {
        return 0;
    }
    return m_pBss->channel;
}

// 0x0072EC7C (name is ours)
s16 nn::nwm::BssDescriptionReaderBase::GetSignalStrength() const
{
    if (m_pBss == NULL) {
        return 0;
    }
    return m_pBss->signalStrength;
}

// 0x0072EC8C | fefates:bytes [tier B]
nn::nwm::Mac nn::nwm::BssDescriptionReaderBase::GetBssid() const
{
    if (m_pBss != NULL) {
        return m_pBss->bssid;
    }
    nn::nwm::Mac none = { { 0, 0, 0, 0, 0, 0 } };
    return none;
}

} // namespace nwm
} // namespace nn
