#include "nn/pia/transport/transport_TransportAnalysisData.h"

namespace nn {
namespace pia {
namespace transport {
// 0x00736560 | fefates:bytes [tier B]
void nn::pia::transport::TransportAnalysisData::Print(bool printAll) const
{
    m_SendData.Print(printAll);
    m_ReceiveData.Print(printAll);
    m_ConnectionData.Print(printAll);
}

} // namespace transport
} // namespace pia
} // namespace nn
