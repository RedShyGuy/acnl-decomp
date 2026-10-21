#include "nn/gx/detail/gx_Texture.h"
#include "nn/gx/detail/gx_Framebuffer.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "nn/gx/gx_Api.h"
#include <cstring>

// The texture objects of the GL functions (glGenTextures / glBindTexture / glDeleteTextures) and the
// texture manager.

using namespace nn::gx::detail;


namespace nn {
namespace gx {
namespace detail {
// 0x0097E7CC (name is ours)
TextureManager s_TextureManager;
} // namespace detail
} // namespace gx
} // namespace nn

namespace {
// the targets (the OpenGL ES values; 0x6600 binds a texture collection, 0x6610-0x662F the lookup
// tables)
const GLenum TARGET_TEXTURE_2D = 0x0DE1;
const GLenum TARGET_TEXTURE_CUBE_MAP = 0x8513;
const GLenum TARGET_TEXTURE_COLLECTION = 0x6600;
const GLenum TARGET_LUT_FIRST = 0x6610;
const GLenum TARGET_LUT_LAST = 0x662F;
// the kinds of the texture objects
const u32 KIND_2D = 0;
const u32 KIND_CUBE = 1;
const u32 KIND_COLLECTION = 2;
const u32 KIND_LUT = 3;
// the defaults of the parameters (GL_NEAREST, GL_REPEAT)
const GLenum DEFAULT_FILTER = 0x2600;
const GLenum DEFAULT_WRAP = 0x2901;
const GLint DEFAULT_MIN_LOD = -1000;
// the dirty flags: the units from bit 10 on, the lookup tables
const s32 DIRTY_BIT_TEXTURE_UNIT_0 = 10;
const u32 DIRTY_LUTS = 0x4000;

inline u32 GetBucket(GLuint name)
{
    return name % TEXTURE_BUCKET_COUNT;
}

inline TextureObject* FindTexture(GLuint name)
{
    TextureObject* p = s_TextureManager.m_pTables->m_pTextures[GetBucket(name)];
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

// a bit of the dirty flags of the GL state (a bit set over its first words)
inline void SetDirtyBit(GlState* pState, s32 bit)
{
    reinterpret_cast<u32*>(pState)[bit >> 5] |= 1 << (bit & 31);
}

inline void InitializeParameters(TextureParameters* pParameters)
{
    pParameters->m_MagFilter = DEFAULT_FILTER;
    pParameters->m_MinFilter = DEFAULT_FILTER;
    pParameters->m_WrapS = DEFAULT_WRAP;
    pParameters->m_WrapT = DEFAULT_WRAP;
    pParameters->m_BorderColor[3] = 0.0f;
    pParameters->m_LodBias = 0.0f;
    pParameters->m_Location = 0;
    pParameters->m_MinLod = DEFAULT_MIN_LOD;
    pParameters->m_BorderColor[2] = 0.0f;
    pParameters->m_BorderColor[1] = 0.0f;
    pParameters->m_BorderColor[0] = 0.0f;
}

inline TextureCollection* CreateCollection(GLuint name)
{
    TextureCollection* p = static_cast<TextureCollection*>(AllocateObject(sizeof(TextureCollection)));
    if (p != nullptr) {
        memset(p, 0, sizeof(TextureCollection));
        p->m_Name = name;
    }
    return p;
}

inline TextureLut* CreateLut(GLuint name)
{
    TextureLut* p = static_cast<TextureLut*>(AllocateObject(sizeof(TextureLut)));
    if (p != nullptr) {
        memset(p, 0, sizeof(TextureLut));
        p->m_DirtyBuffers = 0xFFFFFFFF;
        p->m_Name = name;
    }
    return p;
}

// the surface of a texture leaves the framebuffer manager
inline void ReleaseTextureSurface(TextureParameters* pParameters, Surface* pSurface)
{
    __fb_detachSurfaceContainer(pSurface);
    ReleaseSurface(pParameters->m_Location, pSurface);
}

// the collection bound now (or the default one)
inline TextureCollection* GetCollection()
{
    TextureTables* pTables = s_TextureManager.m_pTables;
    return pTables->m_pBoundCollection != nullptr ? static_cast<TextureCollection*>(pTables->m_pBoundCollection->m_pData)
                                                  : pTables->m_pDefaultCollection;
}

// the object with the name, a new one of the kind if there is none (a new data block if it has none)
inline TextureObject* GetTextureObject(GLuint name, u32 kind)
{
    TextureObject* pObject = FindTexture(name);
    if (pObject == nullptr) {
        pObject = static_cast<TextureObject*>(AllocateObject(sizeof(TextureObject)));
        void* pData;
        switch (kind) {
        case KIND_2D:
            pData = CreateTexture2D(name);
            break;
        case KIND_CUBE:
            pData = CreateTextureCube(name);
            break;
        case KIND_COLLECTION:
            pData = CreateCollection(name);
            break;
        default:
            pData = CreateLut(name);
            break;
        }
        pObject->m_pData = pData;
        pObject->m_Kind = kind;
        pObject->m_Name = name;
        pObject->m_pNext = nullptr;
        InsertTexture(pObject, name);
    } else if (pObject->m_pData == nullptr) {
        switch (kind) {
        case KIND_2D:
            pObject->m_pData = CreateTexture2D(name);
            break;
        case KIND_CUBE:
            pObject->m_pData = CreateTextureCube(name);
            break;
        case KIND_COLLECTION:
            pObject->m_pData = CreateCollection(name);
            break;
        default:
            pObject->m_pData = CreateLut(name);
            break;
        }
        pObject->m_Kind = kind;
    }
    return pObject;
}
} // namespace

namespace nn {
namespace gx {
namespace detail {
// 0x0012EB18 (name is ours)
Texture2D* CreateTexture2D(GLuint name)
{
    Texture2D* p = static_cast<Texture2D*>(AllocateObject(sizeof(Texture2D)));
    if (p == nullptr) {
        return nullptr;
    }
    memset(p, 0, sizeof(Texture2D));
    InitializeParameters(&p->m_Parameters);
    p->m_Surface.m_Name = name;
    return p;
}

// 0x0012EA50 (name is ours)
TextureCube* CreateTextureCube(GLuint name)
{
    TextureCube* p = static_cast<TextureCube*>(AllocateObject(sizeof(TextureCube)));
    if (p == nullptr) {
        return nullptr;
    }
    memset(p, 0, sizeof(TextureCube));
    InitializeParameters(&p->m_Parameters);
    for (u32 i = 0; i < CUBE_FACE_COUNT; i++) {
        p->m_Faces[i].m_Name = name;
    }
    return p;
}

// 0x007C4194 (name is ours)
void InsertTexture(TextureObject* pObject, GLuint name)
{
    TextureObject** ppBucket = &s_TextureManager.m_pTables->m_pTextures[GetBucket(name)];
    TextureObject* pHead = *ppBucket;
    if (pHead == nullptr) {
        *ppBucket = pObject;
        return;
    }
    if (pHead->m_Name > name) {
        pObject->m_pNext = pHead;
        *ppBucket = pObject;
        return;
    }
    TextureObject* pPrev = pHead;
    TextureObject* p = pHead->m_pNext;
    while (p != nullptr && p->m_Name <= name) {
        pPrev = p;
        p = p->m_pNext;
    }
    pPrev->m_pNext = pObject;
    pObject->m_pNext = p;
}
} // namespace detail
} // namespace gx
} // namespace nn

extern "C" {
// 0x00127ADC | nintendogs:bytes [tier A]
int __tx_initializeTexManager(void* pMemory)
{
    int result = -1;
    s_TextureManager.m_pTables = static_cast<TextureTables*>(pMemory);
    memset(pMemory, 0, sizeof(TextureTables));
    s_TextureManager.m_pTables->m_pDefault2d = CreateTexture2D(0);
    s_TextureManager.m_pTables->m_pDefaultCube = CreateTextureCube(0);
    TextureCollection* pCollection = CreateCollection(0);
    TextureTables* pTables = s_TextureManager.m_pTables;
    pTables->m_pDefaultCollection = pCollection;
    Texture2D* p2d = pTables->m_pDefault2d;
    if (p2d != nullptr && pTables->m_pDefaultCube != nullptr && pCollection != nullptr) {
        result = 0;
    } else {
        // what could be created is freed again
        if (p2d != nullptr) {
            ReleaseTextureSurface(&p2d->m_Parameters, &p2d->m_Surface);
            FreeObject(p2d);
        }
        TextureCube* pCube = s_TextureManager.m_pTables->m_pDefaultCube;
        if (pCube != nullptr) {
            for (u32 i = 0; i < CUBE_FACE_COUNT; i++) {
                ReleaseTextureSurface(&pCube->m_Parameters, &pCube->m_Faces[i]);
            }
            FreeObject(pCube);
        }
        if (s_TextureManager.m_pTables->m_pDefaultCollection != nullptr) {
            if (s_Allocator.m_pFree == nullptr) {
                goto end;
            }
            s_Allocator.m_pFree(MEMORY_AREA_MAIN, MEMORY_PURPOSE_OBJECT, 0, s_TextureManager.m_pTables->m_pDefaultCollection);
        }
        if (s_Allocator.m_pFree != nullptr) {
            s_Allocator.m_pFree(MEMORY_AREA_MAIN, MEMORY_PURPOSE_OBJECT, 0, s_TextureManager.m_pTables);
        }
    }
end:
    s_TextureManager.m_NextName = 1;
    memset(s_TextureManager.m_pTables->m_pDefaultCollection, 0, sizeof(TextureCollection));
    return result;
}
} // extern "C"

// 0x007AA368 | nintendogs:bytes [tier B]
extern "C" void glGenTextures(GLsizei n, GLuint* pNames)
{
    for (GLsizei i = 0; i < n; i++) {
        TextureObject* pObject = static_cast<TextureObject*>(AllocateObject(sizeof(TextureObject)));
        pObject->m_pData = nullptr;
        pObject->m_Kind = 0;
        pObject->m_Name = 0;
        pObject->m_pNext = nullptr;
        GLuint name = s_TextureManager.m_NextName;
        for (;;) {
            TextureObject** ppBucket = &s_TextureManager.m_pTables->m_pTextures[GetBucket(name)];
            TextureObject* pHead = *ppBucket;
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
            TextureObject* pPrev = pHead;
            TextureObject* p = pHead->m_pNext;
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
        s_TextureManager.m_NextName = name + 1;
    }
}

// 0x007A7DB8 | nintendogs:bytes-fuzzy [tier A]
extern "C" void glBindTexture(GLenum target, GLuint texture)
{
    GlState* pState = s_pGlState;
    TextureObject* pObject = nullptr;
    if (target == TARGET_TEXTURE_2D) {
        s32 unit = pState->m_ActiveTextureUnit;
        if (pState->m_Texture2dNames[unit] == texture) {
            return;
        }
        if (texture != 0) {
            pObject = GetTextureObject(texture, KIND_2D);
        }
        GetCollection()->m_2dNames[pState->m_ActiveTextureUnit] = texture;
        pState->m_Texture2dNames[pState->m_ActiveTextureUnit] = texture;
        s_TextureManager.m_pTables->m_pBound2d[pState->m_ActiveTextureUnit] = pObject;
        SetDirtyBit(pState, pState->m_ActiveTextureUnit + DIRTY_BIT_TEXTURE_UNIT_0);
    } else if (target == TARGET_TEXTURE_CUBE_MAP) {
        s32 unit = pState->m_ActiveTextureUnit;
        if (pState->m_TextureCubeNames[unit] == texture) {
            return;
        }
        if (texture != 0) {
            pObject = GetTextureObject(texture, KIND_CUBE);
        }
        GetCollection()->m_CubeNames[pState->m_ActiveTextureUnit] = texture;
        pState->m_TextureCubeNames[pState->m_ActiveTextureUnit] = texture;
        s_TextureManager.m_pTables->m_pBoundCube[pState->m_ActiveTextureUnit] = pObject;
        SetDirtyBit(pState, pState->m_ActiveTextureUnit + DIRTY_BIT_TEXTURE_UNIT_0);
    } else if (target == TARGET_TEXTURE_COLLECTION) {
        if (pState->m_TextureCollectionName == texture) {
            return;
        }
        if (texture != 0) {
            pObject = GetTextureObject(texture, KIND_COLLECTION);
        }
        // the bindings of the collection become the bindings of the GL state
        TextureTables* pTables = s_TextureManager.m_pTables;
        TextureCollection* pCollection = texture != 0 ? static_cast<TextureCollection*>(pObject->m_pData) : pTables->m_pDefaultCollection;
        bool isLutChanged = false;
        for (u32 i = 0; i < LUT_COUNT; i++) {
            GLuint name = pCollection->m_LutNames[i];
            if (pState->m_LutNames[i] != name) {
                pState->m_LutNames[i] = name;
                pTables->m_pBoundLuts[i] = name != 0 ? FindTexture(name) : nullptr;
                isLutChanged = true;
            }
        }
        if (isLutChanged) {
            pState->m_DirtyFlags |= DIRTY_LUTS;
        }
        for (s32 unit = 0; unit < static_cast<s32>(TEXTURE_UNIT_COUNT); unit++) {
            TextureCollection* pSource = texture != 0 ? static_cast<TextureCollection*>(pObject->m_pData) : pTables->m_pDefaultCollection;
            GLuint name2d = pSource->m_2dNames[unit];
            GLuint nameCube = pSource->m_CubeNames[unit];
            if (pState->m_Texture2dNames[unit] == name2d && pState->m_TextureCubeNames[unit] == nameCube) {
                continue;
            }
            pState->m_Texture2dNames[unit] = name2d;
            pTables->m_pBound2d[unit] = name2d != 0 ? FindTexture(name2d) : nullptr;
            pState->m_TextureCubeNames[unit] = nameCube;
            pTables->m_pBoundCube[unit] = nameCube != 0 ? FindTexture(nameCube) : nullptr;
            SetDirtyBit(pState, unit + DIRTY_BIT_TEXTURE_UNIT_0);
        }
        pState->m_TextureCollectionName = texture;
        pTables->m_pBoundCollection = pObject;
        // the collection deleted while it was bound
        if (pTables->m_PendingDeleteName != 0) {
            GLuint name = pTables->m_PendingDeleteName;
            glDeleteTextures(1, &name);
            s_TextureManager.m_pTables->m_PendingDeleteName = 0;
        }
    } else if (target >= TARGET_LUT_FIRST && target <= TARGET_LUT_LAST) {
        u32 index = target - TARGET_LUT_FIRST;
        if (pState->m_LutNames[index] == texture) {
            return;
        }
        if (texture != 0) {
            pObject = GetTextureObject(texture, KIND_LUT);
        }
        GetCollection()->m_LutNames[index] = texture;
        pState->m_LutNames[index] = texture;
        s_TextureManager.m_pTables->m_pBoundLuts[index] = pObject;
        pState->m_DirtyFlags |= DIRTY_LUTS;
    }
}

// (the bound texture collection is deleted with the next binding of a collection)
// 0x00142184 | nintendogs:bytes [tier A]
extern "C" void glDeleteTextures(GLsizei n, const GLuint* pNames)
{
    GlState* pState = s_pGlState;
    for (GLsizei i = 0; i < n; i++) {
        GLuint name = pNames[i];
        if (name == 0) {
            continue;
        }
        TextureTables* pTables = s_TextureManager.m_pTables;
        TextureObject* pPrev = nullptr;
        TextureObject* pObject = pTables->m_pTextures[GetBucket(name)];
        while (pObject != nullptr && name > pObject->m_Name) {
            pPrev = pObject;
            pObject = pObject->m_pNext;
        }
        if (pObject == nullptr || pObject->m_Name != name) {
            continue;
        }
        if (pObject->m_pData != nullptr) {
            switch (pObject->m_Kind) {
            case KIND_2D: {
                for (u32 unit = 0; unit < TEXTURE_UNIT_COUNT; unit++) {
                    if (pState->m_Texture2dNames[unit] == pNames[i]) {
                        pState->m_Texture2dNames[unit] = 0;
                        GetCollection()->m_2dNames[unit] = 0;
                        pTables->m_pBound2d[unit] = nullptr;
                        if (pState->m_IsTexture2dEnabled[unit]) {
                            pState->m_DirtyFlags |= 1 << (unit + DIRTY_BIT_TEXTURE_UNIT_0);
                        }
                    }
                }
                Texture2D* p2d = static_cast<Texture2D*>(pObject->m_pData);
                ReleaseTextureSurface(&p2d->m_Parameters, &p2d->m_Surface);
                FreeObject(p2d);
                pObject->m_pData = nullptr;
                break;
            }
            case KIND_CUBE: {
                for (u32 unit = 0; unit < TEXTURE_UNIT_COUNT; unit++) {
                    if (pState->m_TextureCubeNames[unit] == pNames[i]) {
                        pState->m_TextureCubeNames[unit] = 0;
                        GetCollection()->m_CubeNames[unit] = 0;
                        pTables->m_pBoundCube[unit] = nullptr;
                        if (pState->m_IsTextureCubeEnabled[unit]) {
                            pState->m_DirtyFlags |= 1 << (unit + DIRTY_BIT_TEXTURE_UNIT_0);
                        }
                    }
                }
                TextureCube* pCube = static_cast<TextureCube*>(pObject->m_pData);
                for (u32 face = 0; face < CUBE_FACE_COUNT; face++) {
                    ReleaseTextureSurface(&pCube->m_Parameters, &pCube->m_Faces[face]);
                }
                FreeObject(pCube);
                pObject->m_pData = nullptr;
                break;
            }
            case KIND_COLLECTION:
                if (pState->m_TextureCollectionName == name) {
                    pTables->m_PendingDeleteName = name;
                    continue;
                }
                FreeObject(pObject->m_pData);
                pObject->m_pData = nullptr;
                break;
            case KIND_LUT: {
                for (u32 j = 0; j < LUT_COUNT; j++) {
                    if (pState->m_LutNames[j] == pNames[i]) {
                        pState->m_LutNames[j] = 0;
                        GetCollection()->m_LutNames[j] = 0;
                        pTables->m_pBoundLuts[j] = nullptr;
                    }
                }
                for (u32 j = 0; j < 33; j++) {
                    if (pState->m_UsedLutNames[j] == pNames[i]) {
                        pState->m_UsedLutNames[j] = 0;
                        pState->m_LutUpdateCounts[j] = 0;
                        pState->m_DirtyFlags |= DIRTY_LUTS;
                    }
                }
                TextureLut* pLut = static_cast<TextureLut*>(pObject->m_pData);
                for (u32 j = 0; j < 5; j++) {
                    if (pLut->m_pBuffers[j] != nullptr && s_Allocator.m_pFree != nullptr) {
                        s_Allocator.m_pFree(MEMORY_AREA_MAIN, MEMORY_PURPOSE_OBJECT, 0, pLut->m_pBuffers[j]);
                    }
                }
                if (pLut->m_pBuffers[5] != nullptr) {
                    if (s_Allocator.m_pFree == nullptr) {
                        goto lutFreed;
                    }
                    s_Allocator.m_pFree(MEMORY_AREA_MAIN, MEMORY_PURPOSE_OBJECT, 0, pLut->m_pBuffers[5]);
                }
                FreeObject(pLut);
            lutFreed:
                pObject->m_pData = nullptr;
                break;
            }
            default:
                break;
            }
        }
        if (pNames[i] < s_TextureManager.m_NextName) {
            s_TextureManager.m_NextName = pNames[i];
        }
        if (pPrev != nullptr) {
            pPrev->m_pNext = pObject->m_pNext;
        } else {
            s_TextureManager.m_pTables->m_pTextures[GetBucket(pNames[i])] = s_TextureManager.m_pTables->m_pTextures[GetBucket(pNames[i])]->m_pNext;
        }
        FreeObject(pObject);
    }
}

namespace {
// the parameters (the OpenGL ES values; 0x6790 the image addresses and 0x8558 the LOD bias are
// extensions, names ours)
const GLenum PARAMETER_BORDER_COLOR = 0x1004;
const GLenum PARAMETER_MAG_FILTER = 0x2800;
const GLenum PARAMETER_MIN_FILTER = 0x2801;
const GLenum PARAMETER_WRAP_S = 0x2802;
const GLenum PARAMETER_WRAP_T = 0x2803;
const GLenum PARAMETER_MIN_LOD = 0x813A;
const GLenum PARAMETER_GENERATE_MIPMAP = 0x8191;
const GLenum PARAMETER_LOD_BIAS = 0x8558;
const GLenum PARAMETER_IMAGE_ADDRESS = 0x6790;

// the bound 2D texture of the active unit (the default one if none is bound)
inline Texture2D* GetBound2d(GlState* pState)
{
    TextureTables* pTables = s_TextureManager.m_pTables;
    s32 unit = pState->m_ActiveTextureUnit;
    return pState->m_Texture2dNames[unit] != 0 ? static_cast<Texture2D*>(pTables->m_pBound2d[unit]->m_pData) : pTables->m_pDefault2d;
}

// the same for the cube map
inline TextureCube* GetBoundCube(GlState* pState)
{
    TextureTables* pTables = s_TextureManager.m_pTables;
    s32 unit = pState->m_ActiveTextureUnit;
    return pState->m_TextureCubeNames[unit] != 0 ? static_cast<TextureCube*>(pTables->m_pBoundCube[unit]->m_pData)
                                                  : pTables->m_pDefaultCube;
}

inline TextureParameters* GetBound2dParameters(GlState* pState)
{
    return &GetBound2d(pState)->m_Parameters;
}

inline TextureParameters* GetBoundCubeParameters(GlState* pState)
{
    return &GetBoundCube(pState)->m_Parameters;
}

inline f32 ClampColor(f32 value)
{
    if (value <= 0.0f) {
        return 0.0f;
    }
    return value > 1.0f ? 1.0f : value;
}

inline f32 ClampColor(GLint value)
{
    if (value <= 0) {
        return 0.0f;
    }
    return value > 1 ? 1.0f : static_cast<f32>(value);
}
} // namespace

// 0x007AD7F4 | nintendogs:bytes [tier A]
extern "C" void glTexParameterfv(GLenum target, GLenum pname, const GLfloat* pParams)
{
    GlState* pState = s_pGlState;
    TextureParameters* pParameters;
    if (target == TARGET_TEXTURE_2D) {
        pParameters = GetBound2dParameters(pState);
    } else if (target == TARGET_TEXTURE_CUBE_MAP) {
        pParameters = GetBoundCubeParameters(pState);
    } else {
        return;
    }
    switch (pname) {
    case PARAMETER_MAG_FILTER:
        pParameters->m_MagFilter = static_cast<GLint>(pParams[0]);
        break;
    case PARAMETER_MIN_FILTER:
        pParameters->m_MinFilter = static_cast<GLint>(pParams[0]);
        break;
    case PARAMETER_WRAP_S:
        pParameters->m_WrapS = static_cast<GLint>(pParams[0]);
        break;
    case PARAMETER_WRAP_T:
        pParameters->m_WrapT = static_cast<GLint>(pParams[0]);
        break;
    case PARAMETER_BORDER_COLOR:
        pParameters->m_BorderColor[0] = ClampColor(pParams[0]);
        pParameters->m_BorderColor[1] = ClampColor(pParams[1]);
        pParameters->m_BorderColor[2] = ClampColor(pParams[2]);
        pParameters->m_BorderColor[3] = ClampColor(pParams[3]);
        break;
    case PARAMETER_MIN_LOD:
        pParameters->m_MinLod = static_cast<GLint>(pParams[0]);
        break;
    case PARAMETER_GENERATE_MIPMAP:
        pParameters->m_IsGenerateMipmap = pParams[0] != 0.0f;
        break;
    case PARAMETER_LOD_BIAS:
        pParameters->m_LodBias = pParams[0];
        break;
    default:
        return;
    }
    SetDirtyBit(pState, pState->m_ActiveTextureUnit + DIRTY_BIT_TEXTURE_UNIT_0);
}

// 0x007ADB3C | nintendogs:bytes [tier A]
extern "C" void glTexParameteriv(GLenum target, GLenum pname, const GLint* pParams)
{
    GlState* pState = s_pGlState;
    TextureParameters* pParameters;
    if (target == TARGET_TEXTURE_2D) {
        pParameters = GetBound2dParameters(pState);
    } else if (target == TARGET_TEXTURE_CUBE_MAP) {
        pParameters = GetBoundCubeParameters(pState);
    } else {
        return;
    }
    switch (pname) {
    case PARAMETER_MAG_FILTER:
        pParameters->m_MagFilter = pParams[0];
        break;
    case PARAMETER_MIN_FILTER:
        pParameters->m_MinFilter = pParams[0];
        break;
    case PARAMETER_WRAP_S:
        pParameters->m_WrapS = pParams[0];
        break;
    case PARAMETER_WRAP_T:
        pParameters->m_WrapT = pParams[0];
        break;
    case PARAMETER_BORDER_COLOR:
        pParameters->m_BorderColor[0] = ClampColor(pParams[0]);
        pParameters->m_BorderColor[1] = ClampColor(pParams[1]);
        pParameters->m_BorderColor[2] = ClampColor(pParams[2]);
        pParameters->m_BorderColor[3] = ClampColor(pParams[3]);
        break;
    case PARAMETER_MIN_LOD:
        pParameters->m_MinLod = pParams[0];
        break;
    case PARAMETER_GENERATE_MIPMAP:
        pParameters->m_IsGenerateMipmap = pParams[0];
        break;
    case PARAMETER_LOD_BIAS:
        pParameters->m_LodBias = static_cast<f32>(pParams[0]);
        break;
    default:
        return;
    }
    SetDirtyBit(pState, pState->m_ActiveTextureUnit + DIRTY_BIT_TEXTURE_UNIT_0);
}

// 0x007AD7DC | nintendogs:bytes [tier A]
extern "C" void glTexParameterf(GLenum target, GLenum pname, GLfloat param)
{
    glTexParameterfv(target, pname, &param);
}

// 0x007ADB28 | nintendogs:bytes [tier A]
extern "C" void glTexParameteri(GLenum target, GLenum pname, GLint param)
{
    glTexParameteriv(target, pname, &param);
}

// 0x007AA4A4 | nintendogs:bytes [tier B]
extern "C" void glGetTexParameteriv(GLenum target, GLenum pname, GLint* pParams)
{
    GlState* pState = s_pGlState;
    TextureParameters* pParameters;
    if (target == TARGET_TEXTURE_2D) {
        pParameters = GetBound2dParameters(pState);
    } else if (target == TARGET_TEXTURE_CUBE_MAP) {
        pParameters = GetBoundCubeParameters(pState);
    } else {
        return;
    }
    switch (pname) {
    case PARAMETER_IMAGE_ADDRESS:
        if (target == TARGET_TEXTURE_2D) {
            pParams[0] = reinterpret_cast<Texture2D*>(pParameters)->m_Surface.m_Address;
        } else {
            for (u32 face = 0; face < CUBE_FACE_COUNT; face++) {
                pParams[face] = reinterpret_cast<TextureCube*>(pParameters)->m_Faces[face].m_Address;
            }
        }
        break;
    case PARAMETER_MAG_FILTER:
        pParams[0] = pParameters->m_MagFilter;
        break;
    case PARAMETER_MIN_FILTER:
        pParams[0] = pParameters->m_MinFilter;
        break;
    case PARAMETER_WRAP_S:
        pParams[0] = pParameters->m_WrapS;
        break;
    case PARAMETER_WRAP_T:
        pParams[0] = pParameters->m_WrapT;
        break;
    case PARAMETER_BORDER_COLOR:
        pParams[0] = static_cast<GLint>(pParameters->m_BorderColor[0]);
        pParams[1] = static_cast<GLint>(pParameters->m_BorderColor[1]);
        pParams[2] = static_cast<GLint>(pParameters->m_BorderColor[2]);
        pParams[3] = static_cast<GLint>(pParameters->m_BorderColor[3]);
        break;
    case PARAMETER_MIN_LOD:
        pParams[0] = pParameters->m_MinLod;
        break;
    case PARAMETER_GENERATE_MIPMAP:
        pParams[0] = pParameters->m_IsGenerateMipmap;
        break;
    case PARAMETER_LOD_BIAS:
        pParams[0] = static_cast<GLint>(pParameters->m_LodBias);
        break;
    default:
        break;
    }
}

namespace {
// the faces of a cube map
const GLenum TARGET_CUBE_MAP_POSITIVE_X = 0x8515;
const GLenum TARGET_CUBE_MAP_NEGATIVE_X = 0x8516;
const GLenum TARGET_CUBE_MAP_POSITIVE_Y = 0x8517;
const GLenum TARGET_CUBE_MAP_NEGATIVE_Y = 0x8518;
const GLenum TARGET_CUBE_MAP_POSITIVE_Z = 0x8519;
const GLenum TARGET_CUBE_MAP_NEGATIVE_Z = 0x851A;
// the formats of a display transfer (the GX pixel formats on 3dbrew)
const u32 TRANSFER_FORMAT_RGBA8 = 0;
const u32 TRANSFER_FORMAT_RGB8 = 1;
const u32 TRANSFER_FORMAT_RGB565 = 2;
const u32 TRANSFER_FORMAT_RGB5A1 = 3;
const u32 TRANSFER_FORMAT_RGBA4 = 4;
// the sides at which glCopyTexImage2D cuts the rectangle to the framebuffer (names are ours)
const u32 CLIP_LEFT = 1 << 0;
const u32 CLIP_RIGHT = 1 << 1;
const u32 CLIP_BOTTOM = 1 << 2;
const u32 CLIP_TOP = 1 << 3;

// a native format becomes the format of its data (its data is in the order of the GPU already)
// 0x007C40BC (name is ours)
DECOMP_NOINLINE void ConvertNativeFormat(GLenum format, GLenum* pFormat, bool* pIsNative)
{
    *pIsNative = true;
    switch (static_cast<u16>(format)) {
    case FORMAT_NATIVE_RGBA:
        *pFormat = FORMAT_RGBA;
        break;
    case FORMAT_NATIVE_RGB:
        *pFormat = FORMAT_RGB;
        break;
    case FORMAT_NATIVE_ALPHA:
        *pFormat = FORMAT_ALPHA;
        break;
    case FORMAT_NATIVE_LUMINANCE:
        *pFormat = FORMAT_LUMINANCE;
        break;
    case FORMAT_NATIVE_LUMINANCE_ALPHA:
        *pFormat = FORMAT_LUMINANCE_ALPHA;
        break;
    case FORMAT_NATIVE_HILO8:
        *pFormat = FORMAT_HILO8;
        break;
    case FORMAT_ETC1:
        *pFormat = FORMAT_ETC1;
        break;
    case FORMAT_ETC1A4:
        *pFormat = FORMAT_ETC1A4;
        break;
    case FORMAT_NATIVE_SHADOW:
        *pFormat = FORMAT_SHADOW;
        break;
    case FORMAT_NATIVE_UNKNOWN_0x6050:
        *pFormat = FORMAT_UNKNOWN_0x6050;
        break;
    default:
        *pFormat = format;
        *pIsNative = false;
        break;
    }
}

// the pixels of an image go into the 8x8 tiles of the GPU (Morton order inside a tile, the lines
// from the bottom if flipped); pixels outside the destination or the source are skipped
inline void CopyToTiles(u8* pDestination, u32 destinationWidth, u32 destinationHeight, u32 destinationX, u32 destinationY,
                        const u8* pSource, u32 sourceWidth, u32 sourceHeight, u32 width, u32 height, s32 bytesPerPixel,
                        bool isHalfByte, bool isFlipped)
{
    for (u32 y = 0; y < height; y++) {
        u32 dy = destinationY + y;
        u32 sy = y * sourceHeight / height;
        if (isFlipped) {
            dy = destinationY + (height - y) - 1;
        }
        for (u32 x = 0; x < width; x++) {
            u32 dx = destinationX + x;
            u32 d = ((dx >> 3) + (((dy >> 3) * destinationWidth) >> 3)) << 6;
            u32 sx = x * sourceWidth / width;
            for (s32 i = 0; (1 << i) < 8; i++) {
                s32 bit = 1 << i;
                d += ((dx & bit) + ((dy & bit) << 1)) * bit;
            }
            u32 si = sy * sourceWidth + sx;
            if (d >= destinationWidth * destinationHeight || si >= sourceWidth * sourceHeight) {
                continue;
            }
            if (isHalfByte) {
                u8* pD = &pDestination[(d * bytesPerPixel) >> 1];
                const u8* pS = &pSource[(si * bytesPerPixel) >> 1];
                if ((x & 1) != 0) {
                    *pD = (*pS & 0xF0) | (*pD & 0x0F);
                } else {
                    *pD = (*pD & 0xF0) | (*pS & 0x0F);
                }
            } else {
                u8* pD = &pDestination[d * bytesPerPixel];
                const u8* pS = &pSource[si * bytesPerPixel];
                for (s32 i = 0; i < bytesPerPixel; i++) {
                    pD[i] = pS[i];
                }
            }
        }
    }
}

// the 4x4 blocks of an ETC image go into the order of the GPU (2x2 blocks in Z order)
inline void CopyToBlocks(u32* pDestination, u32 destinationWidth, u32 destinationHeight, u32 destinationX, u32 destinationY,
                         const u32* pSource, u32 sourceWidth, u32 sourceHeight, u32 width, u32 height, u32 hwFormat)
{
    for (u32 by = 0; by < height; by++) {
        u32 dy = destinationY + by;
        for (u32 bx = 0; bx < width; bx++) {
            u32 dx = destinationX + bx;
            u32 d = (((dx >> 1) + (((dy >> 1) * destinationWidth) >> 1)) << 2) + (dx & 1) + ((dy & 1) << 1);
            u32 si = by * sourceWidth + bx;
            if (d >= destinationWidth * destinationHeight || si >= sourceWidth * sourceHeight) {
                continue;
            }
            if (hwFormat == HW_FORMAT_ETC1) {
                pDestination[d * 2] = pSource[si * 2];
                pDestination[d * 2 + 1] = pSource[si * 2 + 1];
            } else {
                pDestination[d * 4] = pSource[si * 4];
                pDestination[d * 4 + 1] = pSource[si * 4 + 1];
                pDestination[d * 4 + 2] = pSource[si * 4 + 2];
                pDestination[d * 4 + 3] = pSource[si * 4 + 3];
            }
        }
    }
}

// the levels after the first one by display transfers that halve the image
inline void GenerateMipmaps(Surface* pSurface)
{
    s32 height = pSurface->m_Height;
    s32 width = pSurface->m_Width;
    u32 address = pSurface->m_Address;
    u32 format;
    switch (pSurface->m_HwFormat) {
    case HW_FORMAT_RGBA8:
        format = TRANSFER_FORMAT_RGBA8;
        break;
    case HW_FORMAT_RGB8:
        format = TRANSFER_FORMAT_RGB8;
        break;
    case HW_FORMAT_RGB5A1:
        format = TRANSFER_FORMAT_RGB5A1;
        break;
    case HW_FORMAT_RGB565:
        format = TRANSFER_FORMAT_RGB565;
        break;
    case HW_FORMAT_RGBA4:
        format = TRANSFER_FORMAT_RGBA4;
        break;
    default:
        break;
    }
    for (s32 level = 1; level < pSurface->m_LevelCount; level++) {
        u32 next = address + width * height * pSurface->m_BitsPerPixel / 8;
        __sys_setB2BCommandRequest(address, next, width, height, format);
        address = next;
        width >>= 1;
        height >>= 1;
    }
}

// the image of glTexImage2D / glCompressedTexImage2D goes into the surface (new memory if the
// surface does not fit; the pixels are converted and tiled unless native, then go to VRAM)
// 0x007C4214 (name is ours)
DECOMP_NOINLINE void UploadImage(TextureParameters* pParameters, Surface* pSurface, s32 levelCount, GLsizei width, GLsizei height, GLenum format,
                 GLenum type, const GLvoid* pPixels, u32 imageSize, bool isNative, u32 location)
{
    bool isReady = false;
    if (pSurface->m_Address != 0) {
        if (pSurface->m_Width == width && pSurface->m_Height == height && pSurface->m_Format == format &&
            pParameters->m_Location == location && pSurface->m_LevelCount == levelCount && pSurface->m_Type == type) {
            isReady = true;
        } else {
            ReleaseSurface(pParameters->m_Location, pSurface);
            isReady = pSurface->m_Address != 0;
        }
    }
    if (!isReady) {
        __srf_initSurface(width, height, format, type, levelCount, imageSize, pSurface);
        if (!AllocateSurfaceMemory(location, pSurface)) {
            return;
        }
    }
    if (pPixels != nullptr) {
        // with generated mipmaps only the first level comes from the caller
        s32 uploadLevelCount = levelCount;
        u32 size = pSurface->m_Size;
        if (pParameters->m_IsGenerateMipmap) {
            size = width * height * pSurface->m_BitsPerPixel / 8;
            uploadLevelCount = 1;
        }
        pSurface->m_Source = reinterpret_cast<u32>(pPixels);
        if (location == LOCATION_MAIN || location == LOCATION_VRAM_A_WITH_COPY || location == LOCATION_VRAM_B_WITH_COPY) {
            if (isNative) {
                memcpy(reinterpret_cast<void*>(pSurface->m_Copy), pPixels, size);
            } else {
                s32 levelWidth = width;
                s32 levelHeight = height;
                u32 offset = 0;
                for (s32 level = 0; level < uploadLevelCount; level++) {
                    u8* pDestination = reinterpret_cast<u8*>(pSurface->m_Copy) + offset;
                    const u8* pSource = static_cast<const u8*>(pPixels) + offset;
                    u32 hwFormat = pSurface->m_HwFormat;
                    s32 bytesPerPixel = 4;
                    bool isHalfByte = false;
                    bool isFlipped = true;
                    switch (hwFormat) {
                    case HW_FORMAT_RGB8:
                        bytesPerPixel = 3;
                        break;
                    case HW_FORMAT_RGB5A1:
                    case HW_FORMAT_RGB565:
                    case HW_FORMAT_RGBA4:
                    case HW_FORMAT_IA8:
                    case HW_FORMAT_HILO8:
                        bytesPerPixel = 2;
                        break;
                    case HW_FORMAT_I8:
                    case HW_FORMAT_A8:
                    case HW_FORMAT_IA4:
                        bytesPerPixel = 1;
                        break;
                    case HW_FORMAT_I4:
                    case HW_FORMAT_A4:
                        bytesPerPixel = 1;
                        isHalfByte = true;
                        break;
                    default:
                        break;
                    }
                    if (hwFormat == HW_FORMAT_ETC1 || hwFormat == HW_FORMAT_ETC1A4) {
                        u32 blockWidth = levelWidth / 4;
                        u32 blockHeight = levelHeight / 4;
                        CopyToBlocks(reinterpret_cast<u32*>(pDestination), blockWidth, blockHeight, 0, 0, reinterpret_cast<const u32*>(pSource),
                                     blockWidth, blockHeight, blockWidth, blockHeight, hwFormat);
                    } else {
                        // the components into the order of the GPU first
                        s32 bufferSize = levelWidth * levelHeight * bytesPerPixel;
                        if (isHalfByte) {
                            bufferSize /= 2;
                        }
                        u8* pBuffer = static_cast<u8*>(AllocateObject(bufferSize));
                        s32 count = levelWidth * levelHeight;
                        switch (hwFormat) {
                        case HW_FORMAT_RGBA8:
                            for (s32 i = 0; i < count; i++) {
                                u32 value = reinterpret_cast<const u32*>(pSource)[i];
                                reinterpret_cast<u32*>(pBuffer)[i] =
                                    ((value >> 8) & 0xFF00) | (value >> 24) | ((value << 8) & 0xFF0000) | (value << 24);
                            }
                            break;
                        case HW_FORMAT_RGB8:
                            for (s32 i = 0; i < count; i++) {
                                pBuffer[i * 3] = pSource[i * 3 + 2];
                                pBuffer[i * 3 + 1] = pSource[i * 3 + 1];
                                pBuffer[i * 3 + 2] = pSource[i * 3];
                            }
                            break;
                        case HW_FORMAT_RGB5A1:
                        case HW_FORMAT_RGB565:
                        case HW_FORMAT_RGBA4:
                            for (s32 i = 0; i < count; i++) {
                                reinterpret_cast<u16*>(pBuffer)[i] = reinterpret_cast<const u16*>(pSource)[i];
                            }
                            break;
                        case HW_FORMAT_IA8:
                        case HW_FORMAT_HILO8:
                            for (s32 i = 0; i < count; i++) {
                                u16 value = reinterpret_cast<const u16*>(pSource)[i];
                                reinterpret_cast<u16*>(pBuffer)[i] = ((value << 8) & 0xFF00) | (value >> 8);
                            }
                            break;
                        case HW_FORMAT_I8:
                        case HW_FORMAT_A8:
                        case HW_FORMAT_IA4:
                            for (s32 i = 0; i < count; i++) {
                                pBuffer[i] = pSource[i];
                            }
                            break;
                        case HW_FORMAT_I4:
                        case HW_FORMAT_A4:
                            for (s32 i = 0; i < count / 2; i++) {
                                pBuffer[i] = pSource[i];
                            }
                            break;
                        default:
                            break;
                        }
                        CopyToTiles(pDestination, levelWidth, levelHeight, 0, 0, pBuffer, levelWidth, levelHeight, levelWidth, levelHeight,
                                    bytesPerPixel, isHalfByte, isFlipped);
                        FreeObject(pBuffer);
                    }
                    offset += levelWidth * levelHeight * pSurface->m_BitsPerPixel / 8;
                    levelWidth >>= 1;
                    levelHeight >>= 1;
                }
            }
        } else if (location == LOCATION_DIRECT) {
            pSurface->m_Address = reinterpret_cast<u32>(pPixels);
        }
        // the data goes to VRAM by DMA
        if (location == LOCATION_VRAM_A_WITH_COPY || location == LOCATION_VRAM_B_WITH_COPY) {
            __sys_setDMACommandRequest(pSurface->m_Address, pSurface->m_Copy, size);
        } else if (location == LOCATION_VRAM_A || location == LOCATION_VRAM_B) {
            __sys_setDMACommandRequest(pSurface->m_Address, pSurface->m_Source, size);
        }
        if (location == LOCATION_MAIN || location == LOCATION_DIRECT) {
            nngxlowFlushDataCache(pSurface->m_Address, size);
        }
        if (pParameters->m_IsGenerateMipmap) {
            GenerateMipmaps(pSurface);
        }
    }
    pParameters->m_Location = location;
}

// the image of glCopyTexImage2D comes from the current framebuffer (its color or depth buffer) by a
// texture copy; the rectangle is cut to the framebuffer
// 0x007C3CB4 (name is ours)
DECOMP_NOINLINE void CopyImage(TextureParameters* pParameters, Surface* pSurface, s32 levelCount, GLenum internalformat, GLint x, GLint y,
               GLsizei width, GLsizei height, u32 location)
{
    GlState* pState = s_pGlState;
    Surface* pSource = pState->m_IsCopyFromDepth ? __fb_getCurrentFBDepthSurface() : __fb_getCurrentFBColorSurface();
    GLenum format;
    bool isNative;
    ConvertNativeFormat(internalformat, &format, &isNative);
    GLenum type = TYPE_UNSIGNED_BYTE;
    if (!pState->m_IsCopyFromDepth) {
        switch (pSource->m_HwFormat) {
        case HW_FORMAT_RGBA8:
            if (format == FORMAT_UNKNOWN_0x6050) {
                type = TYPE_UNSIGNED_SHORT;
            }
            break;
        case HW_FORMAT_RGB5A1:
            type = TYPE_UNSIGNED_SHORT_5_5_5_1;
            break;
        case HW_FORMAT_RGB565:
            type = TYPE_UNSIGNED_SHORT_5_6_5;
            break;
        case HW_FORMAT_RGBA4:
            type = TYPE_UNSIGNED_SHORT_4_4_4_4;
            break;
        default:
            return;
        }
    }
    bool isReady = false;
    if (pSurface->m_Address != 0) {
        if (pSurface->m_Width == width && pSurface->m_Height == height && pSurface->m_Format == format &&
            pParameters->m_Location == location && pSurface->m_LevelCount == levelCount && pSurface->m_Type == type) {
            isReady = true;
        } else {
            ReleaseSurface(pParameters->m_Location, pSurface);
            isReady = pSurface->m_Address != 0;
        }
    }
    if (!isReady) {
        __srf_initSurface(width, height, format, type, levelCount, 0, pSurface);
        if (!AllocateSurfaceMemory(location, pSurface)) {
            return;
        }
    }
    s32 right;
    s32 bottom;
    if (pSource->m_Width > x && pSource->m_Height > y && (right = x + width) > 0 && (bottom = y + height) > 0) {
        u32 clip = 0;
        nngxSplitDrawCmdlist();
        if (x < 0) {
            clip = CLIP_LEFT;
        }
        if (right > pSource->m_Width) {
            clip |= CLIP_RIGHT;
        }
        if (y < 0) {
            clip |= CLIP_BOTTOM;
        }
        if (bottom > pSource->m_Height) {
            clip |= CLIP_TOP;
        }
        if (clip == 0) {
            // the framebuffer lines are counted from its top
            s32 bitsPerPixel = pSurface->m_BitsPerPixel;
            s32 offset = ((pSource->m_Height - y - height) * pSource->m_Width + x * 8) * bitsPerPixel;
            __sys_setCTXCommandRequest(pSurface->m_Address, pSource->m_Address + offset / 8, width * height * bitsPerPixel / 8,
                                       ((pSource->m_Width - width) * bitsPerPixel) >> 4, (bitsPerPixel * width) >> 4, 0,
                                       (bitsPerPixel * width) >> 4);
        } else {
            u32 copyWidth = width;
            u32 sourceX;
            u32 destinationX;
            if ((clip & CLIP_LEFT) != 0) {
                sourceX = 0;
                destinationX = -x;
                copyWidth = width + x;
            } else {
                destinationX = 0;
                sourceX = x;
            }
            if ((clip & CLIP_RIGHT) != 0) {
                copyWidth -= right - pSource->m_Width;
            }
            u32 copyHeight = height;
            u32 sourceBottom = bottom;
            u32 destinationTop = height;
            if ((clip & CLIP_BOTTOM) != 0) {
                copyHeight = height + y;
            }
            if ((clip & CLIP_TOP) != 0) {
                copyHeight -= bottom - pSource->m_Height;
                sourceBottom = pSource->m_Height;
                destinationTop = pSource->m_Height - y;
            }
            u32 bitsPerPixel = pSurface->m_BitsPerPixel;
            u32 size = (copyWidth * copyHeight * bitsPerPixel) >> 3;
            u32 lineWidth = (bitsPerPixel * copyWidth) >> 4;
            u32 sourceOffset = ((pSource->m_Height - sourceBottom) * pSource->m_Width + sourceX * 8) * bitsPerPixel;
            u32 destinationOffset = ((height - destinationTop) * width + destinationX * 8) * bitsPerPixel;
            __sys_setCTXCommandRequest(pSurface->m_Address + (destinationOffset >> 3), pSource->m_Address + (sourceOffset >> 3), size,
                                       ((pSource->m_Width - copyWidth) * bitsPerPixel) >> 4, lineWidth,
                                       ((width - copyWidth) * bitsPerPixel) >> 4, lineWidth);
        }
        if (pParameters->m_IsGenerateMipmap) {
            GenerateMipmaps(pSurface);
        }
    }
    pParameters->m_Location = location;
}

// the number of levels: level 0 is one level, a negative level the number of levels
inline s32 GetLevelCount(GLint level)
{
    return level == 0 ? 1 : -level;
}
} // namespace

// (the upper half of the target is the location of the image, see LOCATION_*; 0: in the main memory
// with pixels, in VRAM B without; the level: see GetLevelCount)
// 0x007AD61C | nintendogs:bytes [tier A]
extern "C" void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border,
                             GLenum format, GLenum type, const GLvoid* pPixels)
{
    GlState* pState = s_pGlState;
    GLenum imageFormat;
    bool isNative;
    ConvertNativeFormat(format, &imageFormat, &isNative);
    u32 location = target & 0xFFFF0000;
    GLenum face = target & 0xFFFF;
    if ((location & 0xFF000000) == 0) {
        location |= pPixels != nullptr ? 0x1000000 : 0x2000000;
    }
    if ((location & 0xFF0000) == 0) {
        location |= pPixels != nullptr ? MEMORY_AREA_MAIN : MEMORY_AREA_VRAM_B;
    }
    switch (face) {
    case TARGET_TEXTURE_2D: {
        Texture2D* p = GetBound2d(pState);
        UploadImage(&p->m_Parameters, &p->m_Surface, GetLevelCount(level), width, height, imageFormat, type, pPixels, 0, isNative,
                    location);
        break;
    }
    case TARGET_CUBE_MAP_POSITIVE_X:
    case TARGET_CUBE_MAP_NEGATIVE_X:
    case TARGET_CUBE_MAP_POSITIVE_Y:
    case TARGET_CUBE_MAP_NEGATIVE_Y:
    case TARGET_CUBE_MAP_POSITIVE_Z:
    case TARGET_CUBE_MAP_NEGATIVE_Z: {
        TextureCube* p = GetBoundCube(pState);
        UploadImage(&p->m_Parameters, &p->m_Faces[face - TARGET_CUBE_MAP_POSITIVE_X], GetLevelCount(level), width, height, imageFormat,
                    type, pPixels, 0, isNative, location);
        break;
    }
    default:
        return;
    }
    SetDirtyBit(pState, pState->m_ActiveTextureUnit + DIRTY_BIT_TEXTURE_UNIT_0);
}

// (the location as with glTexImage2D, 0: in the main memory)
// 0x007A8D7C | nintendogs:bytes [tier B]
extern "C" void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height,
                                       GLint border, GLsizei imageSize, const GLvoid* pData)
{
    GlState* pState = s_pGlState;
    GLenum imageFormat;
    bool isNative;
    ConvertNativeFormat(internalformat, &imageFormat, &isNative);
    u32 location = target & 0xFFFF0000;
    GLenum face = target & 0xFFFF;
    if ((location & 0xFF000000) == 0) {
        location |= 0x1000000;
    }
    if ((location & 0xFF0000) == 0) {
        location |= MEMORY_AREA_MAIN;
    }
    switch (face) {
    case TARGET_TEXTURE_2D: {
        Texture2D* p = GetBound2d(pState);
        UploadImage(&p->m_Parameters, &p->m_Surface, GetLevelCount(level), width, height, imageFormat, TYPE_UNSIGNED_BYTE, pData,
                    imageSize, isNative, location);
        break;
    }
    case TARGET_CUBE_MAP_POSITIVE_X:
    case TARGET_CUBE_MAP_NEGATIVE_X:
    case TARGET_CUBE_MAP_POSITIVE_Y:
    case TARGET_CUBE_MAP_NEGATIVE_Y:
    case TARGET_CUBE_MAP_POSITIVE_Z:
    case TARGET_CUBE_MAP_NEGATIVE_Z: {
        TextureCube* p = GetBoundCube(pState);
        UploadImage(&p->m_Parameters, &p->m_Faces[face - TARGET_CUBE_MAP_POSITIVE_X], GetLevelCount(level), width, height, imageFormat,
                    TYPE_UNSIGNED_BYTE, pData, imageSize, isNative, location);
        break;
    }
    default:
        return;
    }
    SetDirtyBit(pState, pState->m_ActiveTextureUnit + DIRTY_BIT_TEXTURE_UNIT_0);
}

// (the upper half of the target is the memory area of the image: 0 or MEMORY_AREA_* without a copy)
// 0x007A8F3C | nintendogs:bytes [tier B]
extern "C" void glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height,
                                 GLint border)
{
    GlState* pState = s_pGlState;
    GLenum face = target & 0xFFFF;
    u32 location;
    switch (target & 0xFFFF0000) {
    case 0:
    case MEMORY_AREA_MAIN:
        location = LOCATION_MAIN;
        break;
    case MEMORY_AREA_VRAM_A:
        location = LOCATION_VRAM_A;
        break;
    case MEMORY_AREA_VRAM_B:
        location = LOCATION_VRAM_B;
        break;
    default:
        return;
    }
    switch (face) {
    case TARGET_TEXTURE_2D: {
        Texture2D* p = GetBound2d(pState);
        CopyImage(&p->m_Parameters, &p->m_Surface, GetLevelCount(level), internalformat, x, y, width, height, location);
        break;
    }
    case TARGET_CUBE_MAP_POSITIVE_X:
    case TARGET_CUBE_MAP_NEGATIVE_X:
    case TARGET_CUBE_MAP_POSITIVE_Y:
    case TARGET_CUBE_MAP_NEGATIVE_Y:
    case TARGET_CUBE_MAP_POSITIVE_Z:
    case TARGET_CUBE_MAP_NEGATIVE_Z: {
        TextureCube* p = GetBoundCube(pState);
        CopyImage(&p->m_Parameters, &p->m_Faces[face - TARGET_CUBE_MAP_POSITIVE_X], GetLevelCount(level), internalformat, x, y, width,
                  height, location);
        break;
    }
    default:
        return;
    }
    SetDirtyBit(pState, pState->m_ActiveTextureUnit + DIRTY_BIT_TEXTURE_UNIT_0);
}

extern "C" {
// the lookup table bound to a slot (null if none)
// 0x007A0E80 | nintendogs:bytes [tier B]
TextureLut* __tx_getBoundTextureLut(u32 index)
{
    if (index < LUT_COUNT) {
        TextureObject* p = s_TextureManager.m_pTables->m_pBoundLuts[index];
        if (p != nullptr) {
            return static_cast<TextureLut*>(p->m_pData);
        }
    }
    return nullptr;
}
} // extern "C"
