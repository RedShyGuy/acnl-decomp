#pragma once

#include "decomp.h"

namespace sead {
class ResourceMgr
{
public:
    class SingletonDisposer_;
    void unregisterFactory(sead::ResourceFactory*); // 0x00540A8C | nintendogs:bytes [tier A]
};
} // namespace sead
