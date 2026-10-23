#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_SceneObject.h"
#include "nw/gfx/res/gfx_ResSceneObject.h"
#include "nw/os/os_MemorySizeCalculator.h"

namespace nw {
namespace gfx {
class ParticleSet;

// RTTI N2nw3gfx13ParticleShapeE @ 0x008D0604
// vtable 0x00902530 (vptr 0x00902538), offset_to_top 0, 3 entries
// The vertex attributes the particles of a set are drawn with and the command caches that turn
// them on and off (the function names are from the reference symbols, the rest is ours). Each
// attribute is a stream with a value per particle in two buffers (written and drawn in turns) or one
// constant value; the particle indices to draw lie in VRAM, two buffers as well.
class ParticleShape : public ::nw::gfx::SceneObject
{
public:
    static const int MAX_ATTRIBUTES = 16;

    // an attribute of the vertex shader
    struct VertexAttribute
    {
        // (inline)
        VertexAttribute()
        {
            pBuffers[0] = nullptr;
            pCommandValues[0] = nullptr;
            pBuffers[1] = nullptr;
            pCommandValues[1] = nullptr;
        }

        s32 usage;               // 0x00 the input register of the vertex shader
        u32 type;                // 0x04 GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT or GL_FLOAT
        u8 dimension;            // 0x08
        bool isStream;           // 0x09 a value per particle (else one constant value in pBuffers[0])
        void* pBuffers[2];       // 0x0C
        u32* pCommandValues[2];  // 0x14 where the activate commands of each side hold the address (the value)
    };

    // (inline)
    ParticleShape(os::IAllocator* pAllocator, res::ResSceneObject resObject, int maxParticles, os::IAllocator* pDeviceAllocator,
                  void* pDeviceMemory)
        : SceneObject(pAllocator, resObject), m_MaxParticles(maxParticles), m_IsBufferSwapped(false), m_AttributeCount(0),
          m_pDeviceAllocator(pDeviceAllocator), m_pDeviceMemory(pDeviceMemory)
    {
        m_pIndexBuffers[0] = nullptr;
        m_pIndexBuffers[1] = nullptr;
        m_IndexBufferOffsets[0] = 0;
        m_IndexBufferOffsets[1] = 0;
        m_pActivateCommands[0] = nullptr;
        m_ActivateCommandSizes[0] = 0;
        m_pActivateCommands[1] = nullptr;
        m_ActivateCommandSizes[1] = 0;
        m_pDeactivateCommand = nullptr;
        m_DeactivateCommandSize = 0;
        m_pPrimitiveCommand = nullptr;
        m_PrimitiveCommandSize = 0;
    }
    virtual ~ParticleShape(); // 0x00495A88 slot 0x00 | nintendogs:bytes

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x00738A30 slot 0x08

    // a constant attribute: the values copied to a 32-byte aligned piece of *ppMemory, which is
    // advanced past it
    VertexAttribute* AddVertexParam(int usage, u32 type, int dimension, f32* pValues, u8** ppMemory); // 0x00494EC8 | nintendogs:bytes [tier A]
    // a stream: two zeroed buffers for maxParticles (and 8 more) values from *ppMemory
    VertexAttribute* AddVertexStream(int usage, u32 type, int dimension, int maxParticles, u8** ppMemory); // 0x00494FB8 | nintendogs:bytes [tier A]
    // size with the memory AddVertexParam takes
    static int AddVertexParamSize(u32 type, int dimension, int size); // 0x004950C4 | nintendogs:bytes [tier A]
    // the activate commands of both sides, the deactivate command and the primitive command
    void CreateCommandCache(ParticleSet* pParticleSet); // 0x004950E8 | nintendogs:callseq [tier A]
    // size with the memory AddVertexStream takes
    static int AddVertexStreamSize(u32 type, int dimension, int maxParticles, int size); // 0x00495768 | nintendogs:bytes [tier A]
    static void GetMemorySizeInternal(os::MemorySizeCalculator* pSize, int maxParticles); // 0x004957A4 | nintendogs:bytes [tier A]
    static void GetDeviceMemorySizeInternal(os::MemorySizeCalculator* pSize, int maxParticles); // 0x004957EC | nintendogs:bytes [tier A]
    // NULL if there is no memory
    static ParticleShape* Create(res::ResSceneObject resObject, int maxParticles, os::IAllocator* pAllocator, os::IAllocator* pDeviceAllocator); // 0x00495860 | nintendogs:bytes [tier A]

    // (inline) the bytes of the particle indices in one buffer
    static u32 GetIndexBufferSize(int maxParticles) { return maxParticles * sizeof(u16) + 16; }
    // (inline) the buffer of a stream for a side (the sides are exchanged when swapped; NULL: a
    // constant attribute)
    void* GetStreamBuffer(int index, u8 side) const
    {
        if (!m_Attributes[index].isStream) {
            return nullptr;
        }
        return m_Attributes[index].pBuffers[m_IsBufferSwapped ? static_cast<u8>(1 - side) : side];
    }
    // (inline) the value of a constant attribute (NULL: a stream)
    void* GetConstantBuffer(int index) const { return m_Attributes[index].isStream ? nullptr : m_Attributes[index].pBuffers[0]; }

    static const TypeInfo s_TypeInfo;

    u32* m_pActivateCommands[2];             // 0x00C per side
    u32 m_ActivateCommandSizes[2];           // 0x014
    u32* m_pDeactivateCommand;               // 0x01C
    u32 m_DeactivateCommandSize;             // 0x020
    u32* m_pPrimitiveCommand;                // 0x024
    u32 m_PrimitiveCommandSize;              // 0x028
    s32 m_MaxParticles;                      // 0x02C
    bool m_IsBufferSwapped;                  // 0x030
    s32 m_AttributeCount;                    // 0x034
    VertexAttribute m_Attributes[MAX_ATTRIBUTES]; // 0x038
    void* m_pIndexBuffers[2];                // 0x1F8 in VRAM
    u32 m_IndexBufferOffsets[2];             // 0x200 their distance from the start of VRAM A
    os::IAllocator* m_pDeviceAllocator;      // 0x208
    void* m_pDeviceMemory;                   // 0x20C the index buffers
    u8 m_CommandBuffer[0x650];               // 0x210 the command caches (each aligned to 32)
};
ASSERT_SIZE(ParticleShape, 0x860);
} // namespace gfx
} // namespace nw
