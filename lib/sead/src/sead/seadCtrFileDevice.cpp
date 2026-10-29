#include "sead/seadCtrFileDevice.h"

namespace sead {
// 0x0054224C slot 0x00
// 0x0054223C (deleting dtor)
sead::CtrFileDevice::~CtrFileDevice()
{
    // (only the bases are destroyed)
}

// 0x0074B6EC slot 0x74 (name is ours)
const char* sead::CtrFileDevice::getMountName() const
{
    return "rom";
}

} // namespace sead
