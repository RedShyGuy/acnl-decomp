#include "net/dSender.h"

namespace net {
// 0x0051B248 (name is ours)
net::Sender::Handle::Handle()
    : m_Section(nn::os::CriticalSection::InitializeTag()), m_StationId(0xFFFFFFFD, 0xFFFFFFFF), m_pData(NULL), m_Size(0),
      m_IsSent(false)
{
}

// 0x0051B2A0 (name is ours)
// 0x0051B290 (deleting dtor)
net::Sender::Handle::~Handle()
{
    // (only the members are destroyed)
}
} // namespace net
