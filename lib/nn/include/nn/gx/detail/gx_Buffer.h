#pragma once

// The vertex buffer manager of the GL functions (__vb_initializeVBManager): buffer objects in a hash
// table of 512 lists sorted by name; an object holds the data of a buffer or a vertex state
// collection (a saved copy of the vertex attribute state). All names are ours.

#include "decomp.h"
#include "nn/gx/gx_Types.h"

namespace nn {
namespace gx {
namespace detail {
const u32 BUFFER_BUCKET_COUNT = 512;
const u32 VERTEX_ATTRIBUTE_COUNT = 16;

// the data of a buffer (glBufferData)
struct BufferData
{
    u32 m_Address;        // 0x00 (what the GPU reads)
    u32 m_Copy;           // 0x04 (the copy in the main memory)
    u32 m_Source;         // 0x08 (the data of glBufferData)
    GLsizeiptr m_Size;    // 0x0C
    GLenum m_Usage;       // 0x10
    GLenum m_Area;        // 0x14 (the VRAM area of the address)
    u32 m_Location;       // 0x18 (the upper half of the target of glBufferData)
};
ASSERT_SIZE(BufferData, 0x1C);

// a vertex attribute array of the GL state (glVertexAttribPointer; 0x18 bytes)
struct VertexAttribute
{
    const GLvoid* m_pPointer; // 0x00
    GLint m_Size;             // 0x04
    GLenum m_Type;            // 0x08
    GLsizei m_Stride;         // 0x0C
    GLuint m_BufferName;      // 0x10
    GLboolean m_IsNormalized; // 0x14
    bool m_IsEnabled;         // 0x15
    u8 _unknown0x16[2];
};
ASSERT_SIZE(VertexAttribute, 0x18);

// the constant values of the vertex attributes (glVertexAttrib*; 0x1C0 bytes)
struct VertexAttributeValues
{
    f32 m_Values[VERTEX_ATTRIBUTE_COUNT][4];        // 0x000 (0, 0, 0, 1 at the start)
    u32 m_PackedValues[VERTEX_ATTRIBUTE_COUNT][3];  // 0x100 (the four values as float24 for the GPU)
};
ASSERT_SIZE(VertexAttributeValues, 0x1C0);

// the vertex attribute state (a copy of the GL state; glBindBuffer with 0x6800)
struct VertexStateCollection
{
    VertexAttribute m_Attributes[VERTEX_ATTRIBUTE_COUNT];  // 0x000 (GlState 0x458)
    GLuint m_ElementBufferName;                            // 0x180 (GlState 0x5D8)
    GLuint m_ArrayBufferName;                              // 0x184 (GlState 0x5DC)
    VertexAttributeValues m_Values;                        // 0x188 (GlState 0x298)
};
ASSERT_SIZE(VertexStateCollection, 0x348);

// a buffer object
struct BufferObject
{
    GLuint m_Name;          // 0x0
    u32 m_Kind;             // 0x4 (0 buffer, 1 vertex state collection)
    void* m_pData;          // 0x8 (BufferData / VertexStateCollection)
    BufferObject* m_pNext;  // 0xC
};
ASSERT_SIZE(BufferObject, 0x10);

// the tables of the manager (in the memory of the GL state)
struct BufferTables
{
    BufferObject* m_pBuffers[BUFFER_BUCKET_COUNT];                 // 0x000
    VertexStateCollection* m_pDefaultCollection;                   // 0x800
    GLuint m_PendingDeleteName;                                    // 0x804 (the bound collection deleted)
    BufferObject* m_pArrayBuffer;                                  // 0x808
    BufferObject* m_pElementBuffer;                                // 0x80C
    BufferObject* m_pAttributeBuffers[VERTEX_ATTRIBUTE_COUNT];     // 0x810
    BufferObject* m_pBoundCollection;                              // 0x850
};
ASSERT_SIZE(BufferTables, 0x854);

struct BufferManager
{
    BufferTables* m_pTables;   // 0x0
    GLuint m_NextName;         // 0x4
};

extern BufferManager s_BufferManager;

// a vertex state collection with the default values
VertexStateCollection* CreateVertexStateCollection(); // 0x0012EBB0 (name is ours)
// the attribute array reads the bound array buffer
void UpdateAttributeBuffer(GLuint index); // 0x007A101C (name is ours)
// the memory of the data of a buffer for its location (false if it could not be allocated)
bool AllocateBufferMemory(BufferObject* pObject); // 0x007C638C (name is ours)
void ReleaseBufferMemory(BufferObject* pObject); // 0x007C6528 (name is ours)
} // namespace detail
} // namespace gx
} // namespace nn
