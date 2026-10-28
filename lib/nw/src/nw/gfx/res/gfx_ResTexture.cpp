#include "nw/gfx/res/gfx_ResTexture.h"

#include "nn/gx/detail/gx_Context.h"
#include "nn/gx/gx_Api.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "nn/nstd/nstd_String.h"
#include "nw/os/os_IAllocator.h"
#include <new>

namespace nw {
namespace gfx {
namespace res {
namespace {
// the purpose of the memory for the allocator of nngx (a texture)
const GLenum PURPOSE_TEXTURE = 0x101;

typedef void (*TextureFunc)(ResTextureData* pData);

// per texture format (3dbrew "CGFX": RGBA8, RGB8, RGBA5551, RGB565, RGBA4, LA8, HILO8, L8, A8, LA4, L4, A4,
// ETC1, ETC1A4): the GL format, the GL type and the bytes per pixel
// 0x008A39B8
const u32 s_GlFormats[] = {
    0x6752, 0x6754, 0x6752, 0x6754, 0x6752, 0x6758, 0x6759, 0x6757, 0x6756, 0x6758, 0x6757, 0x6756, 0x675A, 0x675B,
};
// 0x008A39F0
const u32 s_GlTypes[] = {
    0x1401, 0x1401, 0x8034, 0x8363, 0x8033, 0x1401, 0x1401, 0x1401, 0x1401, 0x6760, 0x6761, 0x6761, 0, 0,
};
// 0x008A3A28
const f32 s_BytesPerPixel[] = {
    4.0f, 3.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f,
};

// the signature of a dynamic image texture ("TXIM") and its revision
const u32 SIGNATURE_IMAGE_TEXTURE = 0x4D495854;
const u32 DYNAMIC_IMAGE_TEXTURE_REVISION = 0x05000000;
// the smallest width and height of a mipmap level (ETC1: the blocks are bigger)
const s32 MIN_MIPMAP_SIZE = 8;
const s32 MIN_MIPMAP_SIZE_ETC1 = 16;

// 0x004A5B60 (name is ours)
// the image copied to the area of the location flag (or flushed from the cache if it stays in place)
void UploadImage(const void* pOwner, ResImageData* pImage, u32 location)
{
    u32 size = pImage->size;
    if (size == 0) {
        size = (pImage->width * pImage->height * pImage->bitsPerPixel) >> 3;
    }
    const u32 area = location & 0xFFFF0000;
    switch (area) {
    case ResTexture::LOCATION_NO_COPY:
        nngxlowStoreDataCache(reinterpret_cast<unsigned>(pImage->data.ToPtr()), size);
        break;
    case ResTexture::LOCATION_FCRAM_COPY: {
        void* pCopy = nn::gx::detail::s_Allocator.m_pAllocate(0x10000, PURPOSE_TEXTURE, reinterpret_cast<GLuint>(pOwner), size);
        if (pImage->data.ToPtr() != nullptr) {
            nnnstdMemCpy(pCopy, pImage->data.ToPtr(), size);
            nngxlowStoreDataCache(reinterpret_cast<unsigned>(pCopy), size);
        }
        pImage->pLocationAddress = pCopy;
        pImage->locationArea = 0x10000;
        break;
    }
    case ResTexture::LOCATION_VRAM_A:
    case ResTexture::LOCATION_VRAM_B: {
        const u32 vramArea = area & 0x30000;
        void* pCopy = nn::gx::detail::s_Allocator.m_pAllocate(vramArea, PURPOSE_TEXTURE, reinterpret_cast<GLuint>(pOwner), size);
        if (pImage->data.ToPtr() != nullptr) {
            g_pAddVramDmaCommand(pImage->data.ToPtr(), pCopy, size);
        }
        pImage->pLocationAddress = pCopy;
        pImage->locationArea = vramArea;
        break;
    }
    default:
        break;
    }
}

// (inline) an image uploaded once (an image that already has its copy keeps it)
inline void SetupImage(ResTextureData* pTexture, ResImageData* pImage, u32 location)
{
    if (pImage->pLocationAddress != nullptr) {
        pImage->locationArea = 0;
        return;
    }
    if (location == 0) {
        location = ResTexture::LOCATION_NO_COPY;
    }
    UploadImage(pTexture, pImage, location);
}

// (inline) the copy of an image freed
inline void CleanupImage(const void* pOwner, ResImageData* pImage)
{
    if (pImage->locationArea != 0) {
        nn::gx::detail::s_Allocator.m_pFree(pImage->locationArea, PURPOSE_TEXTURE, reinterpret_cast<GLuint>(pOwner), pImage->pLocationAddress);
        pImage->pLocationAddress = nullptr;
    }
}

// 0x004A8500 (name is ours)
void SetupImageTexture(ResTextureData* pData)
{
    ResImageTextureData* pTexture = const_cast<ResImageTextureData*>(DynamicCastData<ResImageTextureData>(pData, ResTexture::TYPE_IMAGE));
    if (pTexture->pLoaded != nullptr) {
        return;
    }
    pTexture->pLoaded = pData;
    SetupImage(pData, pTexture->image.To<ResImageData>(), pTexture->locationFlag);
}

// 0x004A7F3C (name is ours)
void SetupCubeTexture(ResTextureData* pData)
{
    ResCubeTextureData* pTexture = const_cast<ResCubeTextureData*>(DynamicCastData<ResCubeTextureData>(pData, ResTexture::TYPE_CUBE));
    if (pTexture->pLoaded != nullptr) {
        return;
    }
    pTexture->pLoaded = pData;
    const u32 location = pTexture->locationFlag;
    for (u8 face = 0; face < 6; ++face) {
        SetupImage(pData, pTexture->images[face].To<ResImageData>(), location);
    }
}

// 0x004A8624 (name is ours)
void SetupShadowTexture(ResTextureData* pData)
{
    ResShadowTextureData* pTexture = const_cast<ResShadowTextureData*>(DynamicCastData<ResShadowTextureData>(pData, ResTexture::TYPE_SHADOW));
    if (pTexture->pLoaded != nullptr) {
        return;
    }
    pTexture->pLoaded = pData;
    SetupImage(pData, pTexture->image.To<ResImageData>(), pTexture->locationFlag);
}

// 0x004A8E50 (name is ours)
void SetupReferenceTexture(ResTextureData* pData)
{
    ResTextureData* pTarget = static_cast<ResReferenceTextureData*>(pData)->target.To<ResTextureData>();
    switch (pTarget->typeInfo) {
    case ResTexture::TYPE_IMAGE:
        SetupImageTexture(pTarget);
        break;
    case ResTexture::TYPE_CUBE:
        SetupCubeTexture(pTarget);
        break;
    case ResTexture::TYPE_SHADOW:
        SetupShadowTexture(pTarget);
        break;
    case ResTexture::TYPE_REFERENCE:
        SetupReferenceTexture(pTarget);
        break;
    default:
        break;
    }
}

// 0x004A96BC (name is ours)
void SetupProceduralTexture(ResTextureData* /*pData*/)
{
    // empty (in the original too)
}

// 0x004A8698 (name is ours)
void CleanupImageTexture(ResTextureData* pData)
{
    ResImageTextureData* pTexture = const_cast<ResImageTextureData*>(DynamicCastData<ResImageTextureData>(pData, ResTexture::TYPE_IMAGE));
    if (pTexture->pLoaded == nullptr) {
        return;
    }
    CleanupImage(pTexture->pLoaded, pTexture->image.To<ResImageData>());
    pTexture->pLoaded = nullptr;
}

// 0x004A8584 (name is ours)
void CleanupCubeTexture(ResTextureData* pData)
{
    ResCubeTextureData* pTexture = const_cast<ResCubeTextureData*>(DynamicCastData<ResCubeTextureData>(pData, ResTexture::TYPE_CUBE));
    const void* pLoaded = pTexture->pLoaded;
    if (pLoaded == nullptr) {
        return;
    }
    for (u8 face = 0; face < 6; ++face) {
        CleanupImage(pLoaded, pTexture->images[face].To<ResImageData>());
    }
    pTexture->pLoaded = nullptr;
}

// 0x004A97B0 (name is ours)
void CleanupReferenceTexture(ResTextureData* pData)
{
    static_cast<ResReferenceTextureData*>(pData)->target.m_Offset = 0;
}

// 0x004A9960 (name is ours)
void CleanupProceduralTexture(ResTextureData* /*pData*/)
{
    // empty (in the original too)
}

// 0x004A8CA8 (name is ours)
void CleanupShadowTexture(ResTextureData* pData)
{
    ResShadowTextureData* pTexture = const_cast<ResShadowTextureData*>(DynamicCastData<ResShadowTextureData>(pData, ResTexture::TYPE_SHADOW));
    if (pTexture->pLoaded == nullptr) {
        return;
    }
    CleanupImage(pTexture->pLoaded, pTexture->image.To<ResImageData>());
    pTexture->pLoaded = nullptr;
}
} // namespace

// the functions per type of texture: image, cube, reference, procedural, shadow (names are ours)
// 0x0097610C (name is ours)
TextureFunc s_TextureSetupFuncs[] = {SetupImageTexture, SetupCubeTexture, SetupReferenceTexture, SetupProceduralTexture, SetupShadowTexture};
// 0x00976120 (name is ours)
TextureFunc s_TextureCleanupFuncs[] = {CleanupImageTexture, CleanupCubeTexture, CleanupReferenceTexture, CleanupProceduralTexture,
                                       CleanupShadowTexture};
// 0x0097F4B0 (name is ours)
void (*g_pAddVramDmaCommand)(const void* pSource, void* pDestination, int size) = nngxAddVramDmaCommand;

// 0x001375CC | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::res::ResTexture::Cleanup()
{
    ResTextureData* pData = const_cast<ResTextureData*>(m_pData);
    switch (pData->typeInfo) {
    case TYPE_IMAGE:
        s_TextureCleanupFuncs[0](pData);
        break;
    case TYPE_CUBE:
        s_TextureCleanupFuncs[1](pData);
        break;
    case TYPE_REFERENCE:
        s_TextureCleanupFuncs[2](pData);
        break;
    case TYPE_PROCEDURAL:
        s_TextureCleanupFuncs[3](pData);
        break;
    case TYPE_SHADOW:
        s_TextureCleanupFuncs[4](pData);
        break;
    default:
        break;
    }
}

// 0x004A5AB8 | nintendogs:bytes [tier A]
u32 nw::gfx::res::ResTexture::Setup(os::IAllocator* /*pAllocator*/, ResGraphicsFile /*graphicsFile*/)
{
    ResTextureData* pData = const_cast<ResTextureData*>(m_pData);
    switch (pData->typeInfo) {
    case TYPE_IMAGE:
        s_TextureSetupFuncs[0](pData);
        break;
    case TYPE_CUBE:
        s_TextureSetupFuncs[1](pData);
        break;
    case TYPE_REFERENCE:
        s_TextureSetupFuncs[2](pData);
        break;
    case TYPE_PROCEDURAL:
        s_TextureSetupFuncs[3](pData);
        break;
    case TYPE_SHADOW:
        s_TextureSetupFuncs[4](pData);
        break;
    default:
        break;
    }
    return RESULT_SUCCESS;
}

// 0x004A8D28 | nintendogs:bytes [tier A]
ReferenceTarget<ResTexture> GetReferenceTextureTarget(ResReferenceTexture texture, ResGraphicsFile graphicsFile)
{
    bool isFound = false;
    const void* pFound = graphicsFile.ptr()->textures.GetData(texture.ptr()->path.To<char>());
    if (pFound != nullptr) {
        const void* pReference = DynamicCastData<ResTextureData>(pFound, ResTexture::TYPE_REFERENCE);
        if (pReference != nullptr) {
            ReferenceTarget<ResTexture> target = GetReferenceTextureTarget(ResReferenceTexture(pReference), graphicsFile);
            if (target.isFound) {
                texture.ptr()->target.Set(target.target.ptr());
                isFound = true;
            }
        } else {
            texture.ptr()->target.Set(pFound);
            isFound = true;
        }
    }
    ReferenceTarget<ResTexture> target;
    target.target = ResTexture(texture.ptr()->target.ToPtr());
    target.isFound = isFound;
    return target;
}

// 0x004A8718 (name is ours)
nw::gfx::res::ResTexture nw::gfx::res::ResTexture::SetName(const char* name)
{
    const_cast<ResTextureData*>(m_pData)->name.Set(name);
    return *this;
}

// 0x004A72B0 (name is ours)
nw::gfx::res::ResImageTexture nw::gfx::res::ResImageTexture::CreateDynamic(const DynamicDescription& description, os::IAllocator* pAllocator)
{
    const s32 minSize = description.format == FORMAT_ETC1 ? MIN_MIPMAP_SIZE_ETC1 : MIN_MIPMAP_SIZE;
    ResImageTextureData* pTexture = new (pAllocator->Alloc(sizeof(ResImageTextureData), 4)) ResImageTextureData();
    pTexture->typeInfo = TYPE_IMAGE;
    pTexture->signature = SIGNATURE_IMAGE_TEXTURE;
    pTexture->m_Unknown0x8 = DYNAMIC_IMAGE_TEXTURE_REVISION;
    pTexture->name.Set(nullptr);
    pTexture->userDataCount = 0;
    pTexture->userDataDic.Set(nullptr);
    pTexture->height = description.height;
    pTexture->width = description.width;
    pTexture->glFormat = s_GlFormats[description.format];
    pTexture->glType = s_GlTypes[description.format];
    pTexture->mipmapLevels = description.mipmapLevels;
    pTexture->pLoaded = nullptr;
    pTexture->locationFlag = description.locationFlag;
    pTexture->format = description.format;

    ResImageData* pImage = new (pAllocator->Alloc(sizeof(ResImageData), 4)) ResImageData();
    pImage->pAllocator = pAllocator;
    pImage->height = description.height;
    pImage->width = description.width;
    pImage->pLocationAddress = description.pLocationAddress;
    const f32 bytesPerPixel = s_BytesPerPixel[description.format];
    pImage->bitsPerPixel = static_cast<u32>(bytesPerPixel * 8.0f);

    // the size of all mipmap levels (down to the smallest size)
    u32 size = 0;
    s32 height = description.height;
    s32 width = description.width;
    const s32 mipmapLevels = description.mipmapLevels;
    for (s32 level = mipmapLevels; level > 0; --level) {
        size += static_cast<u32>(bytesPerPixel * static_cast<f32>(height) * static_cast<f32>(width));
        if (height == minSize && width == minSize) {
            pTexture->mipmapLevels = mipmapLevels - level + 1;
            break;
        }
        if (height > minSize) {
            height /= 2;
        }
        if (width > minSize) {
            width /= 2;
        }
    }

    if (description.isAllocated) {
        u8* pPixels = static_cast<u8*>(pAllocator->Alloc(size, 128));
        if (description.isCleared) {
            for (u32 i = 0; i < size; ++i) {
                pPixels[i] = 0;
            }
        }
        pImage->size = size;
        pImage->data.Set(pPixels);
    } else {
        pImage->size = 0;
        pImage->data.Set(nullptr);
    }
    pTexture->image.Set(pImage);
    return ResImageTexture(pTexture);
}

// 0x004A7530 (name is ours)
void nw::gfx::res::ResImageTexture::DestroyDynamic()
{
    os::IAllocator* pAllocator = ptr()->image.To<ResImageData>()->pAllocator;
    if (ptr() != nullptr) {
        ResImageData* pImage = ptr()->image.To<ResImageData>();
        if (pImage != nullptr) {
            if (pImage->data.ToPtr() != nullptr) {
                pAllocator->Free(pImage->data.ToPtr());
            }
            pAllocator->Free(pImage);
        }
        pAllocator->Free(ptr());
        m_pData = nullptr;
    }
}

} // namespace res
} // namespace gfx
} // namespace nw
