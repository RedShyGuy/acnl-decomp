#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session17IMatchmakeSessionE @ 0x008CFFFC
//
// The interface of a matchmake session (CommonMatchmakeSession introduces the slots).
class IMatchmakeSession : public ::nn::pia::common::RootObject
{
public:
    IMatchmakeSession() {} // (inline)
    virtual ~IMatchmakeSession() {} // slot 0x00 (inline)
};
} // namespace session
} // namespace pia
} // namespace nn
