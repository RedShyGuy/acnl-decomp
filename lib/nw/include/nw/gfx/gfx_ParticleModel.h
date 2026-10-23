#pragma once

#include "decomp.h"
#include "nw/anim/anim_AnimFrameController.h"
#include "nw/gfx/gfx_Model.h"
#include "nw/gfx/res/gfx_ResParticle.h"
#include "nw/os/os_MemorySizeCalculator.h"
#include "nw/ut/ut_MoveArray.h"

namespace nw {
namespace gfx {
class ParticleSet;
class ParticleShape;

// RTTI N2nw3gfx13ParticleModelE @ 0x008D05F8
// vtable 0x009024FC (vptr 0x00902504), offset_to_top 0, 11 entries
// A model whose shapes draw particle sets: per set of the resource a ParticleShape and a ParticleSet
// as a child (the function names are from the reference symbols, the rest is ours).
class ParticleModel : public ::nw::gfx::Model
{
public:
    struct Description : public Model::Description
    {
        u32 maxParticleSets; // 0x1C
    };

    // (inline)
    ParticleModel(os::IAllocator* pAllocator, res::ResParticleModel resource, const Description& description)
        : Model(pAllocator, resource, description), m_MaxParticleSets(description.maxParticleSets), m_ParticleSetCount(0),
          m_ParticleShapeCount(0)
    {
        m_FrameController.m_EndFrame = 16777215.0f;
        m_FrameController.m_PlayPolicy = anim::PlayPolicy_Loop;
    }
    virtual ~ParticleModel(); // 0x004947E4 slot 0x00 | nintendogs:bytes

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x00738A24 slot 0x08
    virtual void Accept(ISceneVisitor* pVisitor); // 0x00494148 slot 0x10 | nintendogs:bytes
    virtual u32 Initialize(os::IAllocator* pAllocator); // 0x0049368C slot 0x24 | slot vf_0x24 of nw::gfx::SceneNode

    static void GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResParticleModel resource, const Description& description); // 0x00493AE0 | nintendogs:bytes [tier A]
    static void GetMemorySizeForInitialize(os::MemorySizeCalculator* pSize, res::ResParticleModel resource, const Description& description); // 0x00493BD0 | nintendogs:callgraph [tier A]
    static void GetDeviceMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResParticleModel resource, const Description& description); // 0x004940AC | nintendogs:bytes [tier A]
    // the model with its particle sets, attached to pParent (NULL if there is no memory)
    static ParticleModel* Create(SceneNode* pParent, res::ResSceneObject resObject, const Description& description, os::IAllocator* pAllocator,
                                 os::IAllocator* pDeviceAllocator); // 0x0049419C | nintendogs:callseq [tier A]

    static const TypeInfo s_TypeInfo;

    u32 m_MaxParticleSets;                        // 0x220
    s32 m_ParticleSetCount;                       // 0x224
    ut::MoveArray<ParticleSet*> m_ParticleSets;   // 0x228
    s32 m_ParticleShapeCount;                     // 0x238
    ut::MoveArray<ParticleShape*> m_ParticleShapes; // 0x23C
    anim::AnimFrameController m_FrameController;  // 0x24C (looping)
};
ASSERT_SIZE(ParticleModel, 0x270);
} // namespace gfx
} // namespace nw
