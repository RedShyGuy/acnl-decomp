#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace session {
class ISessionInfo;

// RTTI N2nn3pia7session16ISessionInfoListE @ 0x008CFFD8
//
// The list of the sessions a search found (Session holds one, the matchmake sessions fill it).
// SessionInfoList<T> is the only implementation; the slot names are ours.
class ISessionInfoList : public ::nn::pia::common::RootObject
{
public:
    ISessionInfoList() {} // (inline)
    virtual ~ISessionInfoList() {} // slot 0x00
    // slot 0x04 (deleting dtor)
    // the infos (const and non-const)
    virtual ISessionInfo** Begin() const = 0; // slot 0x08
    virtual ISessionInfo** Begin() = 0; // slot 0x0C
    virtual ISessionInfo** End() const = 0; // slot 0x10
    virtual ISessionInfo** End() = 0; // slot 0x14
    // the number of infos found
    virtual u32 GetSize() const = 0; // slot 0x18
    virtual u32 GetCapacity() const = 0; // slot 0x1C
    // no infos found and all of them cleared
    virtual void Clear() = 0; // slot 0x20
};
} // namespace session
} // namespace pia
} // namespace nn
