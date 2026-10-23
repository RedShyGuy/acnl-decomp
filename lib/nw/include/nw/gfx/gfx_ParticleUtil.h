#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResParticle.h"
#include "nw/os/os_IAllocator.h"
#include "nw/os/os_MemorySizeCalculator.h"

namespace nw {
namespace gfx {
// Copies of particle resources that a particle set changes (all parts in one block of 4-byte
// aligned pieces, the offsets fixed up to the copies).
class ParticleUtil
{
public:
    // the copy of the updater with its animation and parts (NULL: no memory)
    static res::ResParticleUpdaterData* DuplicateResParticleUpdater(const res::ResParticleUpdater* pUpdater, os::IAllocator* pAllocator); // 0x0048EA38 | nintendogs:bytes [tier A]
    // the copy of the initializer (NULL: no memory)
    static res::ResParticleInitializerData* DuplicateResParticleInitializer(const res::ResParticleInitializer* pInitializer, os::IAllocator* pAllocator); // 0x0048EFC0 | nintendogs:bytes [tier A]
    static void GetMemorySizeForDuplicateResParticleUpdaterInternal(os::MemorySizeCalculator* pSize, const res::ResParticleUpdater* pUpdater); // 0x0048F0BC | nintendogs:bytes [tier B]
    static void GetMemorySizeForDuplicateResParticleInitializerInternal(os::MemorySizeCalculator* pSize, const res::ResParticleInitializer* pInitializer); // 0x0048F574 | nintendogs:bytes [tier A]
};
} // namespace gfx
} // namespace nw
