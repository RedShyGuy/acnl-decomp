#include "nn/pia/transport/transport_PacketHandler_Iterator.h"

namespace nn {
namespace pia {
namespace transport {
// 0x00734D3C | fefates:bytes [tier B]
const nn::pia::transport::ProtocolMessageReader* nn::pia::transport::PacketHandler::Iterator::GetMessageReader() const
{
    const ProtocolMessageReader& reader = m_pPacketHandler->m_MessageReader;
    return reader.IsValid() ? &reader : nullptr;
}

} // namespace transport
} // namespace pia
} // namespace nn
