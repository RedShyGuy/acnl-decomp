#pragma once

#include "decomp.h"

namespace sead {
class FileDeviceMgr
{
public:
    class SingletonDisposer_;
    void unmount(sead::FileDevice*); // 0x00542E80 | nintendogs:bytes [tier A]
};
} // namespace sead
