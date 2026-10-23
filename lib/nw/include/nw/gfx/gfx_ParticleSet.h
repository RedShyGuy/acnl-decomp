#pragma once

#include "decomp.h"
#include "nn/math/math_Vector3.h"
#include "nw/gfx/gfx_SceneNode.h"
#include "nw/gfx/res/gfx_ResParticle.h"
#include "nw/os/os_MemorySizeCalculator.h"
#include "nw/ut/ut_MoveArray.h"

namespace nw {
namespace gfx {
class ParticleCollection;
class ParticleShape;

// RTTI N2nw3gfx11ParticleSetE @ 0x008D0574
// vtable 0x009023A0 (vptr 0x009023A8), offset_to_top 0, 10 entries
// A scene node with a collection of particles and the initializers and updaters that work on it
// (the function names are from the reference symbols, the rest is ours). Initializers and updaters
// whose resource asks for it work on copies of their resources.
class ParticleSet : public ::nw::gfx::SceneNode
{
public:
    struct Description : public SceneNode::Description
    {
        // (inline)
        Description()
        {
            isFixedSizeMemory = true;
            isAnimationEnabled = true;
            maxCallbacks = 0;
            maxChildren = 0;
            maxAnimObjectsPerGroup = 1;
            maxInitializers = 0;
            maxUpdaters = 0;
        }

        s32 maxInitializers; // 0x10 (at least those of the resource)
        s32 maxUpdaters;     // 0x14 (at least those of the resource)
    };

    // an initializer: its resource and the buffers of the attribute it writes
    struct Initializer
    {
        // (inline) the target buffers stay undefined until the collection is made
        Initializer(bool isResourceOwner, const res::ResParticleInitializerData* pResource, u32 typeInfo)
            : isResourceOwner(isResourceOwner), pResource(pResource), pWork(nullptr), typeInfo(typeInfo)
        {
        }

        bool isResourceOwner;                           // 0x00 the resource is a copy of the set
        const res::ResParticleInitializerData* pResource; // 0x04
        void* pWork;                                    // 0x08
        u32 typeInfo;                                   // 0x0C
        void* pTargetBuffers[2];                        // 0x10 in memory order
    };

    // an updater: its resource and the buffers of the attribute it changes
    struct Updater
    {
        // (inline) the target buffers stay undefined until the collection is made
        Updater(bool isResourceOwner, const res::ResParticleUpdaterData* pResource, u32 typeInfo, bool hasAnimationValues)
            : isResourceOwner(isResourceOwner), pResource(pResource), pWork(nullptr), typeInfo(typeInfo),
              hasAnimationValues(hasAnimationValues)
        {
        }

        bool isResourceOwner;                        // 0x00 the resource is a copy of the set
        const res::ResParticleUpdaterData* pResource; // 0x04
        void* pWork;                                 // 0x08
        u32 typeInfo;                                // 0x0C
        void* pTargetBuffers[2];                     // 0x10 in memory order
        bool hasAnimationValues;                     // 0x18
    };

    ParticleSet(os::IAllocator* pAllocator, res::ResParticleSet resource, const Description& description); // 0x0048D57C | nintendogs:callseq-callee [tier A]
    virtual ~ParticleSet(); // 0x0048D65C slot 0x00 | nintendogs:bytes

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x00737874 slot 0x08
    virtual void Accept(ISceneVisitor* pVisitor); // 0x0048C914 slot 0x10 | nintendogs:bytes

    static void GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResParticleSet resource, const Description& description); // 0x0048C5E0 | nintendogs:bytes-fuzzy [tier A]
    // the set with its collection, attached to pParent (NULL if there is no memory)
    static ParticleSet* Create(SceneNode* pParent, res::ResSceneObject resObject, const Description& description, os::IAllocator* pAllocator,
                               os::IAllocator* pDeviceAllocator, ParticleShape* pShape); // 0x0048C968 | nintendogs:callseq [tier A]
    static void GetDeviceMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResParticleSet resource); // 0x0049EC90 | nintendogs:bytes [tier A]

    static const TypeInfo s_TypeInfo;

    ParticleCollection* m_pParticleCollection; // 0x4C
    u32 m_Unknown0x50;                         // 0x50
    ut::MoveArray<Initializer> m_Initializers; // 0x54
    ut::MoveArray<Updater> m_Updaters;         // 0x64
    nn::math::VEC3 m_Unknown0x74;              // 0x74 (1, 1, 1)
    nn::math::VEC3 m_Unknown0x80;              // 0x80 (0, 0, 0)
    bool m_UpdaterFlag;                        // 0x8C the flag of a ResParticleFlagUpdaterData is set
};
ASSERT_SIZE(ParticleSet, 0x90);
} // namespace gfx
} // namespace nw
