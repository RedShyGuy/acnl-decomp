#include "nw/lyt/lyt_Api.h"

#include "nn/gx/gx_Api.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "nn/math/math_ARMv6.h"
#include "nn/math/math_Vector3.h"
#include "nw/lyt/internal/lyt_GlConstants.h"
#include "nw/lyt/lyt_Pane.h"
#include "nw/lyt/lyt_TexResource.h"

namespace nw {
namespace lyt {
namespace {
// the GL format of a texture format (glFormat / glType of glTexImage2D, the internal format of
// glCompressedTexImage2D) and the smallest texture size (names are ours)
struct TexFormatInfo
{
    u32 format;         // 0x00 TexFormat
    GLenum glFormat;    // 0x04
    GLenum glType;      // 0x08
    u32 minSize;        // 0x0C
    u8 isCompressed;    // 0x10
    u8 is4BitComponent; // 0x11
};

// the formats and types are the values the gx library takes for the hardware formats
// 0x008A3B20 (name is ours)
const TexFormatInfo s_TexFormatInfos[TEXFORMAT_MAX] = {
    {TEXFORMAT_L8, 0x6757, 0x1401, 8, false, false},
    {TEXFORMAT_A8, 0x6756, 0x1401, 8, false, false},
    {TEXFORMAT_LA4, 0x6758, 0x6760, 8, false, true},
    {TEXFORMAT_LA8, 0x6758, 0x1401, 8, false, false},
    {TEXFORMAT_HILO8, 0x6759, 0x1401, 8, false, false},
    {TEXFORMAT_RGB565, 0x6754, 0x8363, 8, false, false},
    {TEXFORMAT_RGB8, 0x6754, 0x1401, 8, false, false},
    {TEXFORMAT_RGB5A1, 0x6752, 0x8034, 8, false, false},
    {TEXFORMAT_RGBA4, 0x6752, 0x8033, 8, false, false},
    {TEXFORMAT_RGBA8, 0x6752, 0x1401, 8, false, false},
    {TEXFORMAT_ETC1, 0x675A, 0, 16, true, false},
    {TEXFORMAT_ETC1A4, 0x675B, 0, 8, true, true},
    {TEXFORMAT_L4, 0x6757, 0x6761, 8, false, true},
    {TEXFORMAT_A4, 0x6756, 0x6761, 8, false, true},
};

// the targets of a texture (the memory area in bits 16-17) and the purpose of the VRAM of a
// texture for the gx allocator (names are ours)
const u32 TEXTURE_TARGET_FCRAM = 0x02010000;
const u32 TEXTURE_TARGET_VRAMA = 0x02020000;
const u32 TEXTURE_TARGET_VRAMB = 0x02030000;
const GLenum ALLOCATE_PURPOSE_TEXTURE = 0x0101;

// (inline) the texture size of the image size: a power of two from the smallest size on
inline u16 CalcRealSize(u16 size, u32 minSize)
{
    u16 realSize = minSize;
    while (realSize != 0 && size > realSize) {
        realSize <<= 1;
    }
    return realSize;
}
} // namespace

namespace internal {
// 0x00976160 (name is ours)
bool g_IsUseGlTexture = false;
} // namespace internal

// 0x004B4B78 | nintendogs:bytes [tier B]
TextureInfo LoadTexture(const void* pImgRes, u32 size, int texFormat)
{
    TexResource texRes;
    texRes.Set(const_cast<void*>(pImgRes), size);
    if (texRes.m_pData == nullptr) {
        return TextureInfo();
    }

    const res::Image* pImage = texRes.m_pImage;
    const u32 imageSize = *texRes.m_pFileSize;
    const void* pImageData = texRes.m_pData;
    const u8 format = pImage->format;
    const TexFormatInfo& formatInfo = s_TexFormatInfos[format];
    const u16 width = pImage->width;
    const u16 realWidth = CalcRealSize(width, formatInfo.minSize);
    const u16 height = pImage->height;
    const u16 realHeight = CalcRealSize(height, formatInfo.minSize);

    if (texFormat == 0) {
        switch (pImage->flag & 3) {
        case 0:
            texFormat = TEXTURE_TARGET_FCRAM;
            break;
        case 1:
            texFormat = TEXTURE_TARGET_VRAMA;
            break;
        case 2:
            texFormat = TEXTURE_TARGET_VRAMB;
            break;
        default:
            return TextureInfo();
        }
    }

    u32 physicalAddress = 0;
    GLuint texName = 0;
    if (internal::g_IsUseGlTexture) {
        glGenTextures(1, &texName);
        glBindTexture(internal::gl::TEXTURE_2D, texName);
        if (formatInfo.isCompressed) {
            glCompressedTexImage2D(texFormat | internal::gl::TEXTURE_2D, 0, formatInfo.glFormat, realWidth, realHeight, 0, imageSize, pImageData);
        } else {
            glTexImage2D(texFormat | internal::gl::TEXTURE_2D, 0, formatInfo.glFormat, realWidth, realHeight, 0, formatInfo.glFormat, formatInfo.glType,
                         pImageData);
        }
        glTexParameterf(internal::gl::TEXTURE_2D, internal::gl::TEXTURE_LOD_BIAS, 0.0f);
        glTexParameteri(internal::gl::TEXTURE_2D, internal::gl::TEXTURE_MIN_LOD, -1000);
        GLint addr = 0;
        glGetTexParameteriv(internal::gl::TEXTURE_2D, internal::gl::TEXTURE_DATA_ADDRESS, &addr);
        physicalAddress = nngxGetPhysicalAddr(addr);
    } else {
        switch (texFormat & 0xFFFF0000) {
        case TEXTURE_TARGET_FCRAM:
            nngxlowStoreDataCache(reinterpret_cast<uptr>(pImageData), imageSize);
            physicalAddress = nngxGetPhysicalAddr(reinterpret_cast<uptr>(pImageData));
            break;
        case TEXTURE_TARGET_VRAMA:
        case TEXTURE_TARGET_VRAMB: {
            GxAllocateFunction pAllocate = nullptr;
            nngxGetAllocator(&pAllocate, nullptr);
            if (pAllocate == nullptr) {
                return TextureInfo();
            }
            const u32 area = texFormat & 0x30000;
            void* pVram = pAllocate(area, ALLOCATE_PURPOSE_TEXTURE, texName, imageSize);
            if (pVram != nullptr) {
                nngxAddVramDmaCommand(pImageData, pVram, imageSize);
                physicalAddress = nngxGetPhysicalAddr(reinterpret_cast<uptr>(pVram));
                texName = reinterpret_cast<uptr>(pVram) | (area >> 16);
            }
            break;
        }
        }
    }

    TextureInfo texInfo;
    texInfo.texObject = texName;
    texInfo.physicalAddress = physicalAddress;
    texInfo.size = TexSize(width, height);
    texInfo.realSize = TexSize(realWidth, realHeight);
    texInfo.format = format;
    return texInfo;
}

// 0x004B5280 (name is ours)
void DeleteTexture(const TextureInfo& texInfo)
{
    GLuint texObject = texInfo.texObject;
    if (internal::g_IsUseGlTexture) {
        if (texObject != 0) {
            glDeleteTextures(1, &texObject);
        }
    } else if (texObject != 0) {
        GxFreeFunction pFree = nullptr;
        nngxGetAllocator(nullptr, &pFree);
        pFree((texObject & 3) << 16, ALLOCATE_PURPOSE_TEXTURE, 0, reinterpret_cast<void*>(texObject & ~3));
    }
}

// 0x004BF5C4 | nintendogs:bytes [tier A]
bool IsContain(Pane* pPane, const nn::math::VEC2& pos)
{
    nn::math::MTX34 invGlbMtx;
    nn::math::ARMv6::MTX34InverseAsm(&invGlbMtx, &pPane->m_GlbMtx);

    const nn::math::VEC3 pos3(pos.x, pos.y, 0.0f);
    nn::math::VEC3 lclPos;
    nn::math::ARMv6::VEC3TransformAsm(&lclPos, &invGlbMtx, &pos3);

    const ut::Rect rect = pPane->GetPaneRect();
    return rect.left <= lclPos.x && lclPos.x <= rect.right && rect.bottom <= lclPos.y && lclPos.y <= rect.top;
}

} // namespace lyt
} // namespace nw
