#include "nn/pia/inet/inet_SocketStreamBase.h"
#include <string.h>

namespace nn {
namespace pia {
namespace inet {
// 0x003E7F84 | fefates:callgraph [tier C]
nn::Result nn::pia::inet::SocketStreamBase::ImportSocket(nn::pia::inet::Socket* pSocket)
{
    m_pSocket = pSocket;
    return nn::Result();
}

// 0x003E7F90 (name is ours)
void nn::pia::inet::SocketStreamBase::Cleanup()
{
    m_IsStarted = false;
}

// 0x003E7F9C | fefates:callgraph [tier C]
nn::Result nn::pia::inet::SocketStreamBase::Startup()
{
    m_IsStarted = true;
    return nn::Result();
}

// 0x003E7FAC | fefates:bytes [tier B]
nn::pia::inet::SocketStreamBase::SocketStreamBase()
{
    memset(m_Buffer, 0, sizeof(m_Buffer));
    m_IsStarted = false;
    m_pSocket = nullptr;
}

// 0x003E8000 | fefates:bytes
// 0x003E7FE4 (deleting dtor)
nn::pia::inet::SocketStreamBase::~SocketStreamBase()
{
    m_pSocket = nullptr;
}

} // namespace inet
} // namespace pia
} // namespace nn
