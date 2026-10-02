#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_SceneObject.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx13ParticleShapeE @ 0x008D0604
// vtable 0x00902530 (vptr 0x00902538), offset_to_top 0, 3 entries
class ParticleShape : public ::nw::gfx::SceneObject
{
public:
    ParticleShape(); // ctor candidate(s) 0x00495860 (unverified)
    virtual ~ParticleShape(); // 0x00495A88 slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x00495A14 slot 0x04 | virtual slot, introduced by nw::gfx::ParticleShape
    virtual void vf_0x08(); // 0x00738A30 slot 0x08 | virtual slot, introduced by nw::gfx::ParticleShape
    void AddVertexParam(int, unsigned, int, float*, unsigned char**); // 0x00494EC8 | nintendogs:bytes [tier A]
    void AddVertexStream(int, unsigned, int, int, unsigned char**); // 0x00494FB8 | nintendogs:bytes [tier A]
    void AddVertexParamSize(unsigned, int, int); // 0x004950C4 | nintendogs:bytes [tier A]
    void CreateCommandCache(nw::gfx::ParticleSet*); // 0x004950E8 | nintendogs:callseq [tier A]
    void AddVertexStreamSize(unsigned, int, int, int); // 0x00495768 | nintendogs:bytes [tier A]
    void GetMemorySizeInternal(nw::os::MemorySizeCalculator*, int); // 0x004957A4 | nintendogs:bytes [tier A]
    void GetDeviceMemorySizeInternal(nw::os::MemorySizeCalculator*, int); // 0x004957EC | nintendogs:bytes [tier A]
    void Create(nw::gfx::res::ResSceneObject, int, nw::os::IAllocator*, nw::os::IAllocator*); // 0x00495860 | nintendogs:bytes [tier A]
};
} // namespace gfx
} // namespace nw
