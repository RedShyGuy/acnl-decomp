#include "nn/nex/nex_NATCheckMessage.h"

namespace nn {
namespace nex {
// 0x0037A694 (name is ours)
nn::nex::NATCheckMessage::NATCheckMessage() : m_Type(0), m_Port(0), m_Address(0), m_Unknown0xC(0)
{
}

// 0x0037A62C | fefates:bytes [tier C]
void nn::nex::NATCheckMessage::ToNetworkByteOrder()
{
    m_Type = __builtin_bswap32(m_Type);
    m_Port = __builtin_bswap32(m_Port);
    m_Address = __builtin_bswap32(m_Address);
    m_Unknown0xC = __builtin_bswap32(m_Unknown0xC);
}

// 0x0037A660 (name is ours)
void nn::nex::NATCheckMessage::ToHostByteOrder()
{
    m_Type = __builtin_bswap32(m_Type);
    m_Port = __builtin_bswap32(m_Port);
    m_Address = __builtin_bswap32(m_Address);
    m_Unknown0xC = __builtin_bswap32(m_Unknown0xC);
}

} // namespace nex
} // namespace nn
