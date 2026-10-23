#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "nw/gfx/res/gfx_ResSceneObject.h"
#include "nw/ut/ut_Offset.h"

namespace nw {
namespace gfx {
namespace res {
// The particle resources of a CGFX file (not on 3dbrew; the layouts are from the code, all names
// are ours). The particle simulation itself is not in the binary (the game uses sead::ptcl), only
// the setup and the copies of the resources, so the meaning of most fields is unknown. The kinds of
// initializers, updaters and volumes are told apart by typeInfo; only their sizes are known.

// (inline) the size of an initializer resource by its type (0: an unknown type)
inline u32 GetParticleInitializerSize(u32 typeInfo)
{
    switch (typeInfo) {
    case 0x00000000:
        return 12;
    case 0x00080000:
    case 0x00800000:
    case 0x08000000:
    case 0x10000000:
    case 0x20000000:
        return 16;
    case 0x00400000:
    case 0x01000000:
    case 0x02000000:
        return 20;
    case 0x04000000:
        return 24;
    case 0x80000000:
    case 0x00200000:
        return 28;
    case 0x40000000:
        return 32;
    case 0x00100000:
        return 36;
    default:
        return 0;
    }
}

// (inline) the size of an updater resource by its type (0: an unknown type)
inline u32 GetParticleUpdaterSize(u32 typeInfo)
{
    switch (typeInfo) {
    case 0x00000000:
    case 0x40000000:
        return 16;
    case 0x80000000:
    case 0x00080000:
    case 0x00400000:
    case 0x00400001:
    case 0x02000000:
        return 20;
    case 0x01000000:
        return 24;
    case 0x00040000:
    case 0x00800000:
    case 0x00800001:
    case 0x00800002:
    case 0x00800004:
        return 28;
    case 0x04000000:
    case 0x08000000:
    case 0x10000000:
    case 0x20000000:
        return 32;
    case 0x00020000:
        return 36;
    case 0x00200000:
        return 44;
    case 0x00100000:
        return 84;
    default:
        return 0;
    }
}

// the volume particles start in (of an emitter or an updater; typeInfo first, the size by it)
struct ResParticleVolumeData
{
    // (inline) the size of a volume by its type, as a part of an updater (0: an unknown type)
    static u32 GetSize(u32 typeInfo)
    {
        switch (typeInfo) {
        case 0x00000000:
        case 0x10000000:
            return 8;
        case 0x08000000:
            return 24;
        case 0x80000000:
            return 28;
        case 0x20000000:
            return 36;
        case 0x04000000:
        case 0x40000000:
            return 40;
        default:
            return 0;
        }
    }

    u32 typeInfo;          // 0x0
    bool isResourceCopied; // 0x4 an emitter uses a copy
};

// an initializer of new particles
struct ResParticleInitializerData
{
    u32 typeInfo;           // 0x0 (the size by GetParticleInitializerSize)
    bool isResourceCopied;  // 0x4 a particle set uses a copy
    s32 targetUsage;        // 0x8 the attribute of the collection it writes (negative: none)
};

// the values an updater animates: a table of keys over the life of a particle
struct ResParticleAnimationData
{
    // (inline) the size of the interpolator by its type
    static u32 GetInterpolatorSize(u32 typeInfo)
    {
        switch (typeInfo) {
        case 0x80000000:
        case 0x00000000:
        case 0x20000000:
            return 8;
        case 0x40000000:
            return 16;
        default:
            return 0;
        }
    }

    u32 typeInfo;             // 0x00
    ut::Offset interpolator;  // 0x04 (u32 typeInfo first)
    u32 m_Unknown0x08[3];     // 0x08
    s32 keyCount;             // 0x14 the bytes of keys
    ut::Offset keys;          // 0x18
    s32 valueCount;           // 0x1C the words of values
    ut::Offset values;        // 0x20
};
ASSERT_SIZE(ResParticleAnimationData, 0x24);

// an updater of the particles
struct ResParticleUpdaterData
{
    u32 typeInfo;           // 0x00 (the size by GetParticleUpdaterSize)
    bool isResourceCopied;  // 0x04 a particle set uses a copy
    s32 targetUsage;        // 0x08 the attribute of the collection it changes (negative: none)
    ut::Offset animation;   // 0x0C ResParticleAnimationData (NULL: none)
};
ASSERT_SIZE(ResParticleUpdaterData, 0x10);

// the updaters whose animation values a particle set looks at (one per kind of value)
const u32 PARTICLE_UPDATER_ANIMATED_0 = 0x00800001;
const u32 PARTICLE_UPDATER_ANIMATED_1 = 0x00800002;
const u32 PARTICLE_UPDATER_ANIMATED_2 = 0x00800004;

// an updater with a flag that a particle set keeps (ParticleSet::m_UpdaterFlag)
struct ResParticleFlagUpdaterData : public ResParticleUpdaterData
{
    static const u32 TYPE = 0x00080000;

    u8 flag; // 0x10 (1: set)
};

// an updater with a name, a volume and a second part of its own type
struct ResParticleUserUpdaterData : public ResParticleUpdaterData
{
    static const u32 TYPE = 0x04000000;

    // (inline) the size of the second part by its type
    static u32 GetSecondPartSize(u32 typeInfo)
    {
        switch (typeInfo) {
        case 0x80000000:
        case 0x40000000:
            return 4;
        case 0x10000000:
        case 0x20000000:
            return 16;
        default:
            return 0;
        }
    }

    ut::Offset name;        // 0x10
    ut::Offset volume;      // 0x14 ResParticleVolumeData
    ut::Offset secondPart;  // 0x18 (u32 typeInfo first)
    u32 m_Unknown0x1C;      // 0x1C
};
ASSERT_SIZE(ResParticleUserUpdaterData, 0x20);

// An attribute of the particles: a stream (a value per particle) or one constant value. The usage
// is the attribute of the vertex shader; 0-12 come from the resource, 13-15 a collection adds itself.
struct ResParticleAttributeData
{
    static const u32 TYPE_CONSTANT = 0x40000000;

    u32 typeInfo;           // 0x00 (TYPE_CONSTANT: one value)
    s32 usage;              // 0x04
    u32 m_Unknown0x08[2];   // 0x08
    ut::Offset value;       // 0x10 the constant value (f32[dimension] or 4 bytes)
};

// the particles of a set: how many at most and their attributes
struct ResParticleCollectionData
{
    s32 capacity;            // 0x0 the particles at most
    ResArrayInfo attributes; // 0x4 ResParticleAttributeData
};

// how the particles of a set are drawn (it picks the bool uniforms of the shader)
struct ResParticleRenderStyleData
{
    u32 typeInfo; // 0x0
};

// a set of particles: a scene node with the collection, the initializers and the updaters
struct ResParticleSetData : public ResSceneObjectData
{
    static const u32 TYPE = 0x40000001;

    ut::Offset collection;     // 0x30 ResParticleCollectionData
    ResArrayInfo initializers; // 0x34 ResParticleInitializerData
    ResArrayInfo updaters;     // 0x3C ResParticleUpdaterData
    ut::Offset renderStyle;    // 0x44 ResParticleRenderStyleData
};
ASSERT_OFFSET(ResParticleSetData, renderStyle, 0x44);

// the shape a particle model draws a set with (one per set; nothing of it is read)
struct ResParticleShapeData
{
    static const u32 TYPE = 0x10000002;

    u32 typeInfo; // 0x0
};

// a model of particle sets: its shapes are the shapes of the sets (by index)
struct ResParticleModelData : public ResModelData
{
    static const u32 TYPE = 0x40000112;

    u32 m_Unknown0xD8[2];      // 0xD8
    ResArrayInfo particleSets; // 0xE0 ResParticleSetData
};
ASSERT_OFFSET(ResParticleModelData, particleSets, 0xE0);

// an emitter of particles: a transform node with a volume and a block of parameters
struct ResParticleEmitterData : public ResTransformNodeData
{
    static const u32 TYPE = 0x40000006;

    u32 m_Unknown0xB4[2];    // 0xB4
    ut::Offset volume;       // 0xBC ResParticleVolumeData
    bool isParameterCopied;  // 0xC0 the emitter uses a copy of the parameters (the first byte of them)
    u8 m_Parameters0xC1[31]; // 0xC1
};
ASSERT_OFFSET(ResParticleEmitterData, isParameterCopied, 0xC0);

// the wrappers of the resources
class ResParticleInitializer
{
public:
    // (inline)
    explicit ResParticleInitializer(const void* pData = nullptr) : m_pData(static_cast<const ResParticleInitializerData*>(pData)) {}
    const ResParticleInitializerData* ptr() const { return m_pData; }

    const ResParticleInitializerData* m_pData; // 0x0
};
ASSERT_SIZE(ResParticleInitializer, 0x4);

class ResParticleUpdater
{
public:
    // (inline)
    explicit ResParticleUpdater(const void* pData = nullptr) : m_pData(static_cast<const ResParticleUpdaterData*>(pData)) {}
    const ResParticleUpdaterData* ptr() const { return m_pData; }

    const ResParticleUpdaterData* m_pData; // 0x0
};
ASSERT_SIZE(ResParticleUpdater, 0x4);

class ResParticleCollection
{
public:
    // (inline)
    explicit ResParticleCollection(const void* pData = nullptr) : m_pData(static_cast<const ResParticleCollectionData*>(pData)) {}
    const ResParticleCollectionData* ptr() const { return m_pData; }

    const ResParticleCollectionData* m_pData; // 0x0
};
ASSERT_SIZE(ResParticleCollection, 0x4);

class ResParticleSet : public ResSceneObject
{
public:
    // (inline)
    explicit ResParticleSet(const void* pData = nullptr) : ResSceneObject(pData) {}
    const ResParticleSetData* ptr() const { return static_cast<const ResParticleSetData*>(m_pData); }
    ResParticleCollection GetCollection() const { return ResParticleCollection(ptr()->collection.ToPtr()); }
};
ASSERT_SIZE(ResParticleSet, 0x4);

class ResParticleModel : public ResModel
{
public:
    // (inline)
    explicit ResParticleModel(const void* pData = nullptr) : ResModel(pData) {}
    const ResParticleModelData* ptr() const { return static_cast<const ResParticleModelData*>(m_pData); }
};
ASSERT_SIZE(ResParticleModel, 0x4);
} // namespace res
} // namespace gfx
} // namespace nw
