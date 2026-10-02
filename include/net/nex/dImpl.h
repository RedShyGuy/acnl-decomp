#pragma once

#include "decomp.h"

namespace net {
namespace nex {
class Impl
{
public:
    class MessageRecvHandler;
    class NotificationEventHandler;
    void SearchSessions(nn::pia::session::SessionSearchCriteria const&, nn::pia::session::ISessionInfo**, unsigned int); // 0x0051173C | libgarden [tier A]
    void IsConnectedToMatchmakeService() const; // 0x00747D1C | libgarden [tier A]
};
} // namespace nex
} // namespace net
