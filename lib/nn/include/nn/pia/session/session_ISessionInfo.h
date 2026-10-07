#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session12ISessionInfoE @ 0x008CFF3C
//
// The base of the information about a session the matchmaking found (inet::NexSessionInfo,
// local::LocalSessionInfo); the slots after the destructor are introduced by the derived classes.
class ISessionInfo : public ::nn::pia::common::RootObject
{
public:
    ISessionInfo() {} // (inline)
    virtual ~ISessionInfo() {} // slot 0x00
    // slot 0x04 (deleting dtor)
};
} // namespace session
} // namespace pia
} // namespace nn
