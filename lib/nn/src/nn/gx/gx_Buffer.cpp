#include "nn/gx/detail/gx_Buffer.h"
#include "nn/gx/gx_Api.h"
#include <cstring>

// The buffer objects of the GL functions (glGenBuffers / glBindBuffer / glBufferData /
// glDeleteBuffers) and the vertex buffer manager.

using namespace nn::gx::detail;

namespace nn {
namespace gx {
namespace detail {
// 0x0097E7D4 (name is ours)
BufferManager s_BufferManager;
} // namespace detail
} // namespace gx
} // namespace nn

namespace {
// the targets (the OpenGL ES values; 0x6800 binds a vertex state collection)
const GLenum TARGET_VERTEX_STATE_COLLECTION = 0x6800;
const GLenum TARGET_ARRAY_BUFFER = 0x8892;
const GLenum TARGET_ELEMENT_ARRAY_BUFFER = 0x8893;
// the usage at the start (GL_STATIC_DRAW)
const GLenum USAGE_STATIC_DRAW = 0x88E4;
// the purpose of the vertex buffer memory for the allocator
const GLenum MEMORY_PURPOSE_VERTEXBUFFER = 0x102;
// the kinds of the buffer objects
const u32 KIND_BUFFER = 0;
const u32 KIND_COLLECTION = 1;
// the dirty flags of the vertex buffers / of the vertex state
const u32 DIRTY_VERTEX_BUFFER = 0x2;
const u32 DIRTY_VERTEX_STATE = 0xC0;

inline u32 GetBucket(GLuint name)
{
    return name % BUFFER_BUCKET_COUNT;
}

inline BufferObject* FindBuffer(GLuint name)
{
    BufferObject* p = s_BufferManager.m_pTables->m_pBuffers[GetBucket(name)];
    while (p != nullptr && p->m_Name != name) {
        p = p->m_pNext;
    }
    return p;
}

inline void* AllocateObject(GLsizei size)
{
    GxAllocateFunction pAllocate = s_Allocator.m_pAllocate;
    return pAllocate != nullptr ? pAllocate(MEMORY_AREA_MAIN, MEMORY_PURPOSE_OBJECT, 0, size) : nullptr;
}

inline void FreeObject(void* p)
{
    if (s_Allocator.m_pFree != nullptr) {
        s_Allocator.m_pFree(MEMORY_AREA_MAIN, MEMORY_PURPOSE_OBJECT, 0, p);
    }
}

// the data of a new buffer object: a vertex state collection or a buffer
inline void CreateBufferData(BufferObject* pObject, GLenum target)
{
    if (target == TARGET_VERTEX_STATE_COLLECTION) {
        pObject->m_pData = CreateVertexStateCollection();
        pObject->m_Kind = KIND_COLLECTION;
        return;
    }
    BufferData* pData = static_cast<BufferData*>(AllocateObject(sizeof(BufferData)));
    if (pData != nullptr) {
        pData->m_Address = 0;
        pData->m_Copy = 0;
        pData->m_Source = 0;
        pData->m_Size = 0;
        pData->m_Area = 0;
        pData->m_Usage = USAGE_STATIC_DRAW;
        pData->m_Location = 0;
    }
    pObject->m_pData = pData;
    pObject->m_Kind = KIND_BUFFER;
}
} // namespace

namespace nn {
namespace gx {
namespace detail {
// 0x0012EBB0 (name is ours)
VertexStateCollection* CreateVertexStateCollection()
{
    VertexStateCollection* p = static_cast<VertexStateCollection*>(AllocateObject(sizeof(VertexStateCollection)));
    if (p == nullptr) {
        return nullptr;
    }
    memset(p, 0, sizeof(VertexStateCollection));
    for (u32 i = 0; i < VERTEX_ATTRIBUTE_COUNT; i++) {
        p->m_Attributes[i].m_Size = 4;
        p->m_Attributes[i].m_Type = 0x1406;
        p->m_Values.m_Values[i][0] = 0.0f;
        p->m_Values.m_Values[i][1] = 0.0f;
        p->m_Values.m_Values[i][2] = 0.0f;
        p->m_Values.m_Values[i][3] = 1.0f;
        // (0, 0, 0, 1 as float24)
        p->m_Values.m_PackedValues[i][0] = 0x3F000000;
        p->m_Values.m_PackedValues[i][1] = 0;
        p->m_Values.m_PackedValues[i][2] = 0;
    }
    return p;
}

// 0x007C638C (name is ours)
bool AllocateBufferMemory(BufferObject* pObject)
{
    bool isAllocated = true;
    BufferData* pData = static_cast<BufferData*>(pObject->m_pData);
    GxAllocateFunction pAllocate = s_Allocator.m_pAllocate;
    switch (pData->m_Location) {
    case LOCATION_MAIN: {
        void* p = pAllocate != nullptr ? pAllocate(MEMORY_AREA_MAIN, MEMORY_PURPOSE_VERTEXBUFFER, pObject->m_Name, pData->m_Size) : nullptr;
        if (p == nullptr) {
            isAllocated = false;
        }
        pData->m_Copy = reinterpret_cast<u32>(p);
        pData->m_Address = reinterpret_cast<u32>(p);
        break;
    }
    case LOCATION_VRAM_A_WITH_COPY:
    case LOCATION_VRAM_B_WITH_COPY: {
        GLenum area = pData->m_Location == LOCATION_VRAM_A_WITH_COPY ? MEMORY_AREA_VRAM_A : MEMORY_AREA_VRAM_B;
        pData->m_Area = area;
        pData->m_Address = reinterpret_cast<u32>(pAllocate != nullptr ? pAllocate(area, MEMORY_PURPOSE_VERTEXBUFFER, pObject->m_Name, pData->m_Size) : nullptr);
        pAllocate = s_Allocator.m_pAllocate;
        pData->m_Copy = reinterpret_cast<u32>(pAllocate != nullptr ? pAllocate(MEMORY_AREA_MAIN, MEMORY_PURPOSE_VERTEXBUFFER, pObject->m_Name, pData->m_Size) : nullptr);
        if (pData->m_Address == 0 || pData->m_Copy == 0) {
            // both or none
            isAllocated = false;
            if (pData->m_Address != 0 && s_Allocator.m_pFree != nullptr) {
                s_Allocator.m_pFree(pData->m_Area, MEMORY_PURPOSE_VERTEXBUFFER, pObject->m_Name, reinterpret_cast<void*>(pData->m_Address));
            }
            if (pData->m_Copy != 0 && s_Allocator.m_pFree != nullptr) {
                s_Allocator.m_pFree(MEMORY_AREA_MAIN, MEMORY_PURPOSE_VERTEXBUFFER, pObject->m_Name, reinterpret_cast<void*>(pData->m_Copy));
            }
        }
        break;
    }
    case LOCATION_VRAM_A:
    case LOCATION_VRAM_B: {
        GLenum area = pData->m_Location == LOCATION_VRAM_A ? MEMORY_AREA_VRAM_A : MEMORY_AREA_VRAM_B;
        pData->m_Area = area;
        void* p = pAllocate != nullptr ? pAllocate(area, MEMORY_PURPOSE_VERTEXBUFFER, pObject->m_Name, pData->m_Size) : nullptr;
        if (p == nullptr) {
            isAllocated = false;
        }
        pData->m_Address = reinterpret_cast<u32>(p);
        break;
    }
    default:
        break;
    }
    return isAllocated;
}

// 0x007C6528 (name is ours)
void ReleaseBufferMemory(BufferObject* pObject)
{
    BufferData* pData = static_cast<BufferData*>(pObject->m_pData);
    GxFreeFunction pFree = s_Allocator.m_pFree;
    switch (pData->m_Location) {
    case LOCATION_MAIN:
        if (pData->m_Copy != 0 && pFree != nullptr) {
            pFree(MEMORY_AREA_MAIN, MEMORY_PURPOSE_VERTEXBUFFER, pObject->m_Name, reinterpret_cast<void*>(pData->m_Copy));
        }
        break;
    case LOCATION_VRAM_A_WITH_COPY:
    case LOCATION_VRAM_B_WITH_COPY:
        if (pData->m_Address != 0 && pFree != nullptr) {
            pFree(pData->m_Area, MEMORY_PURPOSE_VERTEXBUFFER, pObject->m_Name, reinterpret_cast<void*>(pData->m_Address));
        }
        if (pData->m_Copy != 0 && s_Allocator.m_pFree != nullptr) {
            s_Allocator.m_pFree(MEMORY_AREA_MAIN, MEMORY_PURPOSE_VERTEXBUFFER, pObject->m_Name, reinterpret_cast<void*>(pData->m_Copy));
        }
        break;
    case LOCATION_VRAM_A:
    case LOCATION_VRAM_B:
        if (pData->m_Address != 0 && pFree != nullptr) {
            pFree(pData->m_Area, MEMORY_PURPOSE_VERTEXBUFFER, pObject->m_Name, reinterpret_cast<void*>(pData->m_Address));
        }
        break;
    default:
        break;
    }
    pData->m_Address = 0;
    pData->m_Copy = 0;
}
} // namespace detail
} // namespace gx
} // namespace nn

extern "C" {
// 0x00127C90 | nintendogs:bytes [tier A]
int __vb_initializeVBManager(void* pMemory)
{
    s_BufferManager.m_pTables = static_cast<BufferTables*>(pMemory);
    memset(pMemory, 0, sizeof(BufferTables));
    VertexStateCollection* pDefault = CreateVertexStateCollection();
    s_BufferManager.m_pTables->m_pDefaultCollection = pDefault;
    if (pDefault == nullptr) {
        FreeObject(s_BufferManager.m_pTables);
        s_BufferManager.m_pTables = nullptr;
        return -1;
    }
    s_BufferManager.m_NextName = 1;
    return 0;
}
} // extern "C"

// 0x007AA238 | nintendogs:bytes [tier A]
extern "C" void glGenBuffers(GLsizei n, GLuint* pNames)
{
    for (GLsizei i = 0; i < n; i++) {
        BufferObject* pObject = static_cast<BufferObject*>(AllocateObject(sizeof(BufferObject)));
        pObject->m_Name = 0;
        pObject->m_Kind = 0;
        pObject->m_pData = nullptr;
        pObject->m_pNext = nullptr;
        GLuint name = s_BufferManager.m_NextName;
        for (;;) {
            BufferObject** ppBucket = &s_BufferManager.m_pTables->m_pBuffers[GetBucket(name)];
            BufferObject* pHead = *ppBucket;
            if (pHead == nullptr) {
                pNames[i] = name;
                pObject->m_Name = name;
                pObject->m_pNext = nullptr;
                *ppBucket = pObject;
                break;
            }
            if (pHead->m_Name == name) {
                name++;
                continue;
            }
            if (pHead->m_Name > name) {
                pNames[i] = name;
                pObject->m_Name = name;
                pObject->m_pNext = pHead;
                *ppBucket = pObject;
                break;
            }
            BufferObject* pPrev = pHead;
            BufferObject* p = pHead->m_pNext;
            bool isTaken = false;
            for (; p != nullptr; pPrev = p, p = p->m_pNext) {
                if (p->m_Name == name) {
                    isTaken = true;
                    break;
                }
                if (p->m_Name > name) {
                    break;
                }
            }
            if (isTaken) {
                name++;
                continue;
            }
            pNames[i] = name;
            pObject->m_Name = name;
            pPrev->m_pNext = pObject;
            pObject->m_pNext = p;
            break;
        }
        s_BufferManager.m_NextName = name + 1;
    }
}

// 0x007A7974 | nintendogs:bytes [tier A]
extern "C" void glBindBuffer(GLenum target, GLuint buffer)
{
    GlState* pState = s_pGlState;
    BufferObject* pObject = nullptr;
    if (buffer != 0) {
        pObject = FindBuffer(buffer);
        if (pObject == nullptr) {
            // a new object with the name
            pObject = static_cast<BufferObject*>(AllocateObject(sizeof(BufferObject)));
            pObject->m_Name = buffer;
            pObject->m_pNext = nullptr;
            CreateBufferData(pObject, target);
            BufferObject** ppBucket = &s_BufferManager.m_pTables->m_pBuffers[GetBucket(buffer)];
            BufferObject* pHead = *ppBucket;
            if (pHead == nullptr) {
                *ppBucket = pObject;
            } else if (pHead->m_Name > buffer) {
                pObject->m_pNext = pHead;
                *ppBucket = pObject;
            } else {
                BufferObject* pPrev = pHead;
                BufferObject* p = pHead->m_pNext;
                while (p != nullptr && p->m_Name <= buffer) {
                    pPrev = p;
                    p = p->m_pNext;
                }
                pPrev->m_pNext = pObject;
                pObject->m_pNext = p;
            }
        } else if (pObject->m_pData == nullptr) {
            CreateBufferData(pObject, target);
        }
    }

    if (target == TARGET_ARRAY_BUFFER) {
        pState->m_ArrayBufferName = buffer;
        s_BufferManager.m_pTables->m_pArrayBuffer = pObject;
    } else if (target == TARGET_ELEMENT_ARRAY_BUFFER) {
        pState->m_ElementBufferName = buffer;
        s_BufferManager.m_pTables->m_pElementBuffer = pObject;
    } else if (target == TARGET_VERTEX_STATE_COLLECTION && pState->m_CollectionName != buffer) {
        BufferTables* pTables = s_BufferManager.m_pTables;
        // the vertex state goes to the collection bound so far (or the default one)
        VertexStateCollection* pOld = pState->m_CollectionName != 0 ? static_cast<VertexStateCollection*>(pTables->m_pBoundCollection->m_pData)
                                                                   : pTables->m_pDefaultCollection;
        memcpy(pOld->m_Attributes, pState->m_Attributes, sizeof(pOld->m_Attributes));
        pOld->m_ElementBufferName = pState->m_ElementBufferName;
        pOld->m_ArrayBufferName = pState->m_ArrayBufferName;
        memcpy(&pOld->m_Values, &pState->m_AttributeValues, sizeof(pOld->m_Values));
        // the new one becomes the vertex state
        VertexStateCollection* pNew = buffer != 0 ? static_cast<VertexStateCollection*>(pObject->m_pData) : s_BufferManager.m_pTables->m_pDefaultCollection;
        memcpy(pState->m_Attributes, pNew->m_Attributes, sizeof(pState->m_Attributes));
        pState->m_ElementBufferName = pNew->m_ElementBufferName;
        pState->m_ArrayBufferName = pNew->m_ArrayBufferName;
        memcpy(&pState->m_AttributeValues, &pNew->m_Values, sizeof(pState->m_AttributeValues));
        pState->m_CollectionName = buffer;
        pTables = s_BufferManager.m_pTables;
        pTables->m_pBoundCollection = pObject;
        for (u32 i = 0; i < VERTEX_ATTRIBUTE_COUNT; i++) {
            GLuint name = pState->m_Attributes[i].m_BufferName;
            pTables->m_pAttributeBuffers[i] = name != 0 ? FindBuffer(name) : nullptr;
        }
        pTables->m_pArrayBuffer = pState->m_ArrayBufferName != 0 ? FindBuffer(pState->m_ArrayBufferName) : nullptr;
        pTables->m_pElementBuffer = pState->m_ElementBufferName != 0 ? FindBuffer(pState->m_ElementBufferName) : nullptr;
        pState->m_DirtyFlags |= DIRTY_VERTEX_STATE;
        // the collection deleted while it was bound
        if (pTables->m_PendingDeleteName != 0) {
            GLuint name = pTables->m_PendingDeleteName;
            glDeleteBuffers(1, &name);
            s_BufferManager.m_pTables->m_PendingDeleteName = 0;
        }
    }
    pState->m_DirtyFlags |= DIRTY_VERTEX_BUFFER;
}

// (the bound vertex state collection is deleted with the next binding of a collection)
// 0x007A92D0 | nintendogs:bytes [tier B]
extern "C" void glDeleteBuffers(GLsizei n, const GLuint* pNames)
{
    GlState* pState = s_pGlState;
    for (GLsizei i = 0; i < n; i++) {
        GLuint name = pNames[i];
        if (name == 0) {
            continue;
        }
        BufferTables* pTables = s_BufferManager.m_pTables;
        BufferObject* pPrev = nullptr;
        BufferObject* pObject = pTables->m_pBuffers[GetBucket(name)];
        while (pObject != nullptr && name > pObject->m_Name) {
            pPrev = pObject;
            pObject = pObject->m_pNext;
        }
        if (pObject == nullptr || pObject->m_Name != name) {
            continue;
        }
        if (pState->m_CollectionName == name) {
            pTables->m_PendingDeleteName = name;
            continue;
        }
        if (pState->m_ArrayBufferName == name) {
            pState->m_DirtyFlags |= DIRTY_VERTEX_BUFFER;
            pState->m_ArrayBufferName = 0;
            pTables->m_pArrayBuffer = nullptr;
        }
        if (pState->m_ElementBufferName == pNames[i]) {
            pState->m_DirtyFlags |= DIRTY_VERTEX_BUFFER;
            pState->m_ElementBufferName = 0;
            pTables->m_pElementBuffer = nullptr;
        }
        for (u32 j = 0; j < VERTEX_ATTRIBUTE_COUNT; j++) {
            if (pState->m_Attributes[j].m_BufferName == pNames[i]) {
                pTables->m_pAttributeBuffers[j] = nullptr;
            }
        }
        if (pNames[i] < s_BufferManager.m_NextName) {
            s_BufferManager.m_NextName = pNames[i];
        }
        if (pPrev != nullptr) {
            pPrev->m_pNext = pObject->m_pNext;
        } else {
            pTables->m_pBuffers[GetBucket(pNames[i])] = pTables->m_pBuffers[GetBucket(pNames[i])]->m_pNext;
        }
        if (pObject->m_Kind == KIND_BUFFER) {
            if (pObject->m_pData != nullptr) {
                ReleaseBufferMemory(pObject);
                if (s_Allocator.m_pFree == nullptr) {
                    continue;
                }
                s_Allocator.m_pFree(MEMORY_AREA_MAIN, MEMORY_PURPOSE_OBJECT, 0, pObject->m_pData);
            }
            FreeObject(pObject);
        } else if (pObject->m_Kind == KIND_COLLECTION) {
            if (pObject->m_pData != nullptr) {
                if (s_Allocator.m_pFree == nullptr) {
                    continue;
                }
                s_Allocator.m_pFree(MEMORY_AREA_MAIN, MEMORY_PURPOSE_OBJECT, 0, pObject->m_pData);
            }
            FreeObject(pObject);
        }
    }
}

// (the upper half of the target is the location of the data, see LOCATION_*; 0: the defaults)
// 0x007A8C1C | nintendogs:bytes [tier A]
extern "C" void glBufferData(GLenum target, GLsizeiptr size, const GLvoid* pData, GLenum usage)
{
    GlState* pState = s_pGlState;
    if (size == 0) {
        return;
    }
    u32 location = target & 0xFFFF0000;
    BufferObject* pObject;
    if ((target & 0xFFFF) == TARGET_ARRAY_BUFFER) {
        pObject = s_BufferManager.m_pTables->m_pArrayBuffer;
    } else if ((target & 0xFFFF) == TARGET_ELEMENT_ARRAY_BUFFER) {
        pObject = s_BufferManager.m_pTables->m_pElementBuffer;
    } else {
        return;
    }
    if ((location & 0xFF000000) == 0) {
        location |= 0x1000000;
    }
    if ((location & 0xFF0000) == 0) {
        location |= 0x10000;
    }
    BufferData* pBuffer = static_cast<BufferData*>(pObject->m_pData);
    bool isReady = false;
    if (pBuffer->m_Address != 0) {
        if (pBuffer->m_Size == size && pBuffer->m_Location == location) {
            isReady = true;
        } else {
            ReleaseBufferMemory(pObject);
            isReady = pBuffer->m_Address != 0;
        }
    }
    if (!isReady) {
        pBuffer->m_Usage = usage & 0xFFFF;
        pBuffer->m_Location = location;
        pBuffer->m_Size = size;
        if (!AllocateBufferMemory(pObject)) {
            return;
        }
    }
    if (pData != nullptr) {
        pBuffer->m_Source = reinterpret_cast<u32>(pData);
        if (location == LOCATION_MAIN || location == LOCATION_VRAM_A_WITH_COPY || location == LOCATION_VRAM_B_WITH_COPY) {
            memcpy(reinterpret_cast<void*>(pBuffer->m_Copy), pData, size);
        } else if (location == LOCATION_DIRECT) {
            pBuffer->m_Address = reinterpret_cast<u32>(pData);
        }
        // the data goes to VRAM by DMA
        if (location == LOCATION_VRAM_A_WITH_COPY || location == LOCATION_VRAM_B_WITH_COPY) {
            __sys_setDMACommandRequest(pBuffer->m_Address, pBuffer->m_Copy, pBuffer->m_Size);
        } else if (location == LOCATION_VRAM_A || location == LOCATION_VRAM_B) {
            __sys_setDMACommandRequest(pBuffer->m_Address, pBuffer->m_Source, pBuffer->m_Size);
        }
        if (location == LOCATION_MAIN || location == LOCATION_DIRECT) {
            nngxlowFlushDataCache(pBuffer->m_Address, pBuffer->m_Size);
        }
    }
    pState->m_DirtyFlags |= DIRTY_VERTEX_BUFFER;
}
