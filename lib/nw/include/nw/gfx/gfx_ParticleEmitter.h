#pragma once

#include "decomp.h"
#include "nw/anim/anim_AnimFrameController.h"
#include "nw/gfx/gfx_TransformNode.h"
#include "nw/gfx/res/gfx_ResParticle.h"
#include "nw/os/os_MemorySizeCalculator.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx15ParticleEmitterE @ 0x008D0658
// vtable 0x00902654 (vptr 0x0090265C), offset_to_top 0, 11 entries
// A transform node that emits particles from a volume (the function names are from the reference
// symbols, the rest is ours). It keeps copies of the parameters and the volume of its resource when
// the resource asks for them.
class ParticleEmitter : public ::nw::gfx::TransformNode
{
public:
    struct Description : public TransformNode::Description
    {
    };

    // the bytes of the parameters of an emitter
    static const u32 PARAMETER_SIZE = 32;

    // (inline)
    ParticleEmitter(os::IAllocator* pAllocator, res::ResTransformNode resource, const Description& description, void* pParameters,
                    res::ResParticleVolumeData* pVolume)
        : TransformNode(pAllocator, resource, description), m_Unknown0x148(true), m_Unknown0x14C(0.0f), m_Unknown0x150(0),
          m_Unknown0x154(0), m_Unknown0x158(0), m_pParameters(pParameters), m_pVolume(pVolume)
    {
        m_FrameController.m_EndFrame = 16777215.0f;
        m_FrameController.m_PlayPolicy = anim::PlayPolicy_Loop;
    }
    virtual ~ParticleEmitter(); // 0x0049BBFC slot 0x00 | nintendogs:bytes

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x00738BCC slot 0x08
    virtual void Accept(ISceneVisitor* pVisitor); // 0x0049B6FC slot 0x10 | nintendogs:bytes

    // (inline) the size of the copy of a volume by its type (0: an unknown type)
    static u32 GetVolumeSize(u32 typeInfo)
    {
        switch (typeInfo) {
        case 0x80000000:
            return 28;
        case 0x40000000:
            return 40;
        case 0x20000000:
            return 36;
        case 0x10000000:
            return 8;
        case 0x08000000:
            return 24;
        case 0x04000000:
            return 40;
        default:
            return 0;
        }
    }

    static void GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResTransformNode resource, const Description& description); // 0x0049B418 (name is ours)
    // the emitter attached to pParent (NULL if there is no memory or the volume is of an unknown type)
    static ParticleEmitter* Create(SceneNode* pParent, res::ResSceneObject resObject, const Description& description, os::IAllocator* pAllocator); // 0x0049B750 | nintendogs:bytes-fuzzy [tier A]

    static const TypeInfo s_TypeInfo;

    bool m_Unknown0x148;                         // 0x148 (true)
    f32 m_Unknown0x14C;                          // 0x14C
    u32 m_Unknown0x150;                          // 0x150
    u32 m_Unknown0x154;                          // 0x154
    u32 m_Unknown0x158;                          // 0x158
    anim::AnimFrameController m_FrameController; // 0x15C (looping)
    void* m_pParameters;                         // 0x180 the copy of the parameters (NULL: those of the resource)
    res::ResParticleVolumeData* m_pVolume;       // 0x184 the copy of the volume (NULL: that of the resource)
};
ASSERT_SIZE(ParticleEmitter, 0x188);
} // namespace gfx
} // namespace nw
