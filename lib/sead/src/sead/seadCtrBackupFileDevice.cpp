#include "sead/seadCtrBackupFileDevice.h"

namespace sead {
// 0x0054A160 slot 0x00
// 0x0054A150 (deleting dtor)
sead::CtrBackupFileDevice::~CtrBackupFileDevice()
{
    // (only the bases are destroyed)
}

// 0x0074CCD4 slot 0x74 (name is ours)
const char* sead::CtrBackupFileDevice::getMountName() const
{
    return "data";
}

} // namespace sead
