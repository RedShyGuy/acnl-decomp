#pragma once

#include "decomp.h"

namespace nw {
namespace gfx {
class ParticleUtil
{
public:
    void DuplicateResParticleUpdater(const nw::gfx::res::ResParticleUpdater*, nw::os::IAllocator*); // 0x0048EA38 | nintendogs:bytes [tier A]
    void DuplicateResParticleInitializer(const nw::gfx::res::ResParticleInitializer*, nw::os::IAllocator*); // 0x0048EFC0 | nintendogs:bytes [tier A]
    void GetMemorySizeForDuplicateResParticleUpdaterInternal(nw::os::MemorySizeCalculator*, const nw::gfx::res::ResParticleUpdater*); // 0x0048F0BC | nintendogs:bytes [tier B]
    void GetMemorySizeForDuplicateResParticleInitializerInternal(nw::os::MemorySizeCalculator*, const nw::gfx::res::ResParticleInitializer*); // 0x0048F574 | nintendogs:bytes [tier A]
};
} // namespace gfx
} // namespace nw
