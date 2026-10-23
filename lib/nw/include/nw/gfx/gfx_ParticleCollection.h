#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_GfxObject.h"
#include "nw/gfx/res/gfx_ResParticle.h"
#include "nw/os/os_MemorySizeCalculator.h"

namespace nw {
namespace gfx {
class ParticleSet;
class ParticleShape;

// RTTI N2nw3gfx18ParticleCollectionE @ 0x008D06D0
// vtable 0x00902770 (vptr 0x00902778), offset_to_top 0, 3 entries
// The particles of a set: per attribute (usage of the vertex shader) a stream with a value per
// particle in two buffers that swap sides, or a constant value (the function names are from the
// reference symbols, the rest is ours). Usages 0-8 are vertex attributes of the shape, 9-12 only
// live here; 13-15 the collection adds itself: 13 the indices of the particles to draw (the index
// buffers of the shape), 14 the free particles as a stack of indices, 15 a word per particle.
class ParticleCollection : public ::nw::gfx::GfxObject
{
public:
    static const int MAX_ATTRIBUTES = 16;

    enum Usage
    {
        USAGE_DRAW_INDEX = 13,
        USAGE_FREE_INDEX = 14,
        USAGE_15 = 15,
    };

    // an attribute with memory of the collection itself (usages 10-12 and 14-15)
    struct Attribute
    {
        s32 usage;     // 0x0 (-1: none)
        bool isStream; // 0x4
        void* pBuffer; // 0x8
    };

    // (inline)
    ParticleCollection(os::IAllocator* pAllocator, res::ResParticleCollection resource, os::IAllocator* pDeviceAllocator, void* pDeviceMemory)
        : GfxObject(pAllocator), m_Capacity(0), m_Count(0), m_IsBufferSwapped(false), m_Unknown0x1A4(0), m_Unknown0x1A6(0),
          m_Resource(resource), m_pShape(nullptr), m_pParticleSet(nullptr), m_pDeviceAllocator(pDeviceAllocator),
          m_pDeviceMemory(pDeviceMemory), m_Unknown0x1BC(0)
    {
        for (int i = 0; i < MAX_ATTRIBUTES; ++i) {
            m_Attributes[i].usage = -1;
            m_Attributes[i].pBuffer = nullptr;
            m_Strides[i] = 0;
            m_IsStream[i] = false;
            m_pBuffers[i][0] = nullptr;
            m_pBuffers[i][1] = nullptr;
        }
    }
    virtual ~ParticleCollection(); // 0x0049FA00 slot 0x00 | nintendogs:bytes

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0073A8A0 slot 0x08

    // no particles: the draw indices cleared (0xFFFF), all particles free
    void Clear(); // 0x0049EDE4 | nintendogs:bytes [tier A]
    // the collection of the set (NULL if there is no memory)
    static ParticleCollection* Create(ParticleSet* pParticleSet, res::ResParticleCollection resource, os::IAllocator* pAllocator,
                                      os::IAllocator* pDeviceAllocator, ParticleShape* pShape); // 0x0049EED4 | nintendogs:callseq [tier A]
    static void GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResParticleCollection resource); // 0x0049E9E0 (name is ours)
    static void GetDeviceMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResParticleCollection resource); // 0x0049ECA4 (name is ours)

    // (inline) the buffer of a stream for a side (the sides are exchanged when swapped; NULL: no
    // stream)
    void* GetStreamBuffer(int usage, u8 side) const
    {
        if (!m_IsStream[usage]) {
            return nullptr;
        }
        return m_pBuffers[usage][m_IsBufferSwapped ? static_cast<u8>(1 - side) : side];
    }

    static const TypeInfo s_TypeInfo;

    Attribute m_Attributes[MAX_ATTRIBUTES];  // 0x008
    s32 m_Capacity;                          // 0x0C8 the particles at most
    s32 m_Count;                             // 0x0CC
    bool m_IsBufferSwapped;                  // 0x0D0
    bool m_IsStream[MAX_ATTRIBUTES];         // 0x0D1
    void* m_pBuffers[MAX_ATTRIBUTES][2];     // 0x0E4 both buffers of a stream, the value of a constant twice
    s32 m_Strides[MAX_ATTRIBUTES];           // 0x164 the bytes per particle (0: constant)
    u16 m_Unknown0x1A4;                      // 0x1A4 (0xFFFF after Clear)
    u16 m_Unknown0x1A6;                      // 0x1A6
    res::ResParticleCollection m_Resource;   // 0x1A8
    ParticleShape* m_pShape;                 // 0x1AC
    ParticleSet* m_pParticleSet;             // 0x1B0
    os::IAllocator* m_pDeviceAllocator;      // 0x1B4
    void* m_pDeviceMemory;                   // 0x1B8 the streams of the shape
    u32 m_Unknown0x1BC;                      // 0x1BC
};
ASSERT_SIZE(ParticleCollection, 0x1C0);
} // namespace gfx
} // namespace nw
