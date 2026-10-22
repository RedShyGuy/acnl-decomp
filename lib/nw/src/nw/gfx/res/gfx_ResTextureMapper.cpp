#include "nw/gfx/res/gfx_ResTextureMapper.h"

#include <new>

#include "nn/gxlow/CTR/CTR_Api.h"
#include "nn/gr/CTR/detail/gr_Command.h"

namespace nw {
namespace gfx {
namespace res {
namespace {
typedef u32 (*SetupFunc)(ResTextureMapperData* pData, os::IAllocator* pAllocator, ResGraphicsFile graphicsFile);
typedef void (*CleanupFunc)(ResTextureMapperData* pData);
typedef ResTextureMapperData* (*CloneFunc)(ResTextureMapperData* pData, os::IAllocator* pAllocator);
typedef void (*DestroyFunc)(ResTextureMapperData* pData, os::IAllocator* pAllocator);

// the bits of TEXUNIT0_PARAM (3dbrew "GPU/Internal Registers"; names are ours)
const u32 PARAM_MAG_FILTER_LINEAR = 1 << 1;
const u32 PARAM_MIN_FILTER_SHIFT = 2;
const u32 PARAM_ETC1 = 2 << 4;
const u32 PARAM_WRAP_T_MASK = 7 << 8;
const u32 PARAM_WRAP_S_MASK = 7 << 12;
const u32 PARAM_WRAP_CLAMP_TO_BORDER = 1;
const u32 PARAM_SHADOW = 1 << 20;
const u32 PARAM_MIP_FILTER_SHIFT = 24;
const u32 PARAM_TYPE_MASK = 7 << 28;
const u32 PARAM_TYPE_SHADOW_2D = 2 << 28;

// the PICA bits of a min filter: the min filter, the mipmap filter (names are ours)
// 0x0090CB48 (name is ours)
const u32 s_MinFilterBits[] = {0, 1, 0, 0, 1, 1};
// 0x0090CB60 (name is ours)
const u32 s_MipFilterBits[] = {0, 0, 0, 1, 0, 1};

// (inline) the address of an image for the GPU
inline u32 GetImageAddress(const ResImageData* pImage)
{
    const void* pAddress = pImage->pLocationAddress != nullptr ? pImage->pLocationAddress : pImage->data.ToPtr();
    return nngxGetPhysicalAddr(reinterpret_cast<unsigned int>(pAddress));
}

// (inline) the commands common to all kinds: format, dimensions, ETC1 and the mipmap levels
inline void SetTextureCommand(ResPixelBasedTextureMapperData* pMapper, const ResPixelBasedTextureData* pTexture, u32 registerCount)
{
    u32* pCommands = pMapper->commands;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_TYPE] |= pTexture->format & 0xF;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_BORDER_COLOR_HEADER] |= registerCount << 20;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_DIM] |= pTexture->height & 0x7FF;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_DIM] |= (pTexture->width & ~0xF800) << 16;
}

// (inline) the highest mipmap level, if the sampler uses mipmaps
inline void SetMipmapLevelCommand(ResPixelBasedTextureMapperData* pMapper, const ResPixelBasedTextureData* pTexture)
{
    const u32 minFilter = pMapper->sampler.To<ResTextureSamplerData>()->minFilter & 0xFF;
    if (minFilter != ResTextureSamplerData::MIN_FILTER_NEAREST && minFilter != ResTextureSamplerData::MIN_FILTER_LINEAR) {
        pMapper->commands[ResPixelBasedTextureMapperData::COMMAND_LOD] |= ((pTexture->mipmapLevels - 1) & 0xF) << 16;
    }
}

// 0x004A6A8C (name is ours)
// the texture of a mapper set up; a reference is resolved first
DECOMP_NOINLINE u32 SetupMapperTexture(os::IAllocator* pAllocator, ResTexture texture, ResGraphicsFile graphicsFile)
{
    u32 result = RESULT_SUCCESS;
    if (!texture.IsValid()) {
        return result;
    }
    ResTexture target = texture;
    const ResReferenceTextureData* pReference =
        DynamicCastData<ResReferenceTextureData>(texture.ptr(), ResTexture::TYPE_REFERENCE);
    if (pReference != nullptr) {
        if (pReference->target.ToPtr() != nullptr) {
            target = ResTexture(pReference->target.ToPtr());
        } else {
            ReferenceTarget<ResTexture> reference = GetReferenceTextureTarget(ResReferenceTexture(pReference), graphicsFile);
            if (!reference.isFound) {
                result |= RESULT_TEXTURE_NOT_FOUND;
                return result;
            }
            target = reference.target;
        }
    }
    result |= target.Setup(pAllocator, graphicsFile);
    return result;
}

// 0x004A6C20 (name is ours)
// a reference texture to the texture (or to what it refers to) in memory of the allocator
ResReferenceTextureData* CreateReferenceTexture(os::IAllocator* pAllocator, const ResTextureData* pTexture)
{
    ResReferenceTextureData* pReference = static_cast<ResReferenceTextureData*>(pAllocator->Alloc(sizeof(ResReferenceTextureData), 4));
    if (pReference == nullptr) {
        return pReference;
    }
    new (pReference) ResReferenceTextureData();
    pReference->typeInfo = ResTexture::TYPE_REFERENCE;
    switch (pTexture->typeInfo) {
    case ResTexture::TYPE_PROCEDURAL:
    case ResTexture::TYPE_CUBE:
    case ResTexture::TYPE_IMAGE:
    case ResTexture::TYPE_SHADOW:
        pReference->target.Set(pTexture);
        break;
    case ResTexture::TYPE_REFERENCE: {
        const void* pTarget = static_cast<const ResReferenceTextureData*>(pTexture)->target.ToPtr();
        if (pTarget != nullptr) {
            pReference->target.Set(pTarget);
        }
        break;
    }
    default:
        break;
    }
    return pReference;
}

// 0x004A9A0C (name is ours)
u32 SetupPixelBasedTextureMapper(ResTextureMapperData* pData, os::IAllocator* pAllocator, ResGraphicsFile graphicsFile)
{
    ResPixelBasedTextureMapperData* pMapper = static_cast<ResPixelBasedTextureMapperData*>(pData);
    const ResTextureData* pTexture = pMapper->texture.To<ResTextureData>();
    const u32 result = SetupMapperTexture(pAllocator, ResTexture(pTexture), graphicsFile);
    if (!(result & RESULT_FAILURE_MASK)) {
        internal::BuildTextureCommand(pMapper, internal::DereferenceTexture(pTexture));
    }
    return result;
}

// 0x004A6A74 (name is ours)
u32 SetupProceduralTextureMapper(ResTextureMapperData* pData, os::IAllocator* pAllocator, ResGraphicsFile graphicsFile)
{
    return SetupMapperTexture(pAllocator, ResTexture(pData->texture.ToPtr()), graphicsFile);
}

// 0x004A9ABC (name is ours)
void CleanupPixelBasedTextureMapper(ResTextureMapperData* pData)
{
    static_cast<ResPixelBasedTextureMapperData*>(pData)->commands[ResPixelBasedTextureMapperData::COMMAND_ADDR1] = 0;
    ResTexture texture(pData->texture.ToPtr());
    if (texture.IsValid()) {
        texture.Cleanup();
    }
}

// 0x004A9AE8 (name is ours)
void CleanupProceduralTextureMapper(ResTextureMapperData* pData)
{
    ResTexture texture(pData->texture.ToPtr());
    if (texture.IsValid()) {
        texture.Cleanup();
    }
}

// 0x004A9D8C (name is ours)
void DestroyProceduralTextureMapper(ResTextureMapperData* pMapper, os::IAllocator* pAllocator)
{
    if (pMapper->texture.ToPtr() != nullptr) {
        pAllocator->Free(pMapper->texture.ToPtr());
    }
    pAllocator->Free(pMapper);
}

// 0x004A9B10 (name is ours)
ResTextureMapperData* ClonePixelBasedTextureMapper(ResTextureMapperData* pData, os::IAllocator* pAllocator)
{
    ResPixelBasedTextureMapperData* pClone =
        static_cast<ResPixelBasedTextureMapperData*>(pAllocator->Alloc(sizeof(ResPixelBasedTextureMapperData), 4));
    if (pClone == nullptr) {
        return nullptr;
    }
    *pClone = *static_cast<ResPixelBasedTextureMapperData*>(pData);
    const ResTextureData* pTexture = pData->texture.To<ResTextureData>();
    if (pTexture != nullptr) {
        ResReferenceTextureData* pReference = CreateReferenceTexture(pAllocator, pTexture);
        if (pReference == nullptr) {
            internal::DestroyPixelBasedTextureMapper(pClone, pAllocator);
            return nullptr;
        }
        pClone->texture.Set(pReference);
    }
    const ResTextureSamplerData* pSampler = DynamicCastData<ResTextureSamplerData>(
        static_cast<ResPixelBasedTextureMapperData*>(pData)->sampler.ToPtr(), ResTextureSampler::TYPE_STANDARD);
    if (pSampler != nullptr) {
        ResTextureSamplerData* pSamplerClone = static_cast<ResTextureSamplerData*>(pAllocator->Alloc(sizeof(ResTextureSamplerData), 4));
        if (pSamplerClone == nullptr) {
            internal::DestroyPixelBasedTextureMapper(pClone, pAllocator);
            return nullptr;
        }
        *pSamplerClone = *pSampler;
        pClone->sampler.Set(pSamplerClone);
        pSamplerClone->owner.Set(pClone);
    }
    return pClone;
}

// 0x004A9C0C (name is ours)
ResTextureMapperData* CloneProceduralTextureMapper(ResTextureMapperData* pData, os::IAllocator* pAllocator)
{
    ResProceduralTextureMapperData* pClone =
        static_cast<ResProceduralTextureMapperData*>(pAllocator->Alloc(sizeof(ResProceduralTextureMapperData), 4));
    if (pClone == nullptr) {
        return nullptr;
    }
    *pClone = *static_cast<ResProceduralTextureMapperData*>(pData);
    const ResTextureData* pTexture = pData->texture.To<ResTextureData>();
    if (pTexture != nullptr) {
        ResReferenceTextureData* pReference = CreateReferenceTexture(pAllocator, pTexture);
        if (pReference == nullptr) {
            DestroyProceduralTextureMapper(pClone, pAllocator);
            return nullptr;
        }
        pClone->texture.Set(pReference);
    }
    return pClone;
}
} // namespace

// the functions per type of mapper: pixel-based, procedural (names are ours)
// 0x00976134 (name is ours)
SetupFunc s_TextureMapperSetupFuncs[] = {SetupPixelBasedTextureMapper, SetupProceduralTextureMapper};
// 0x0097613C (name is ours)
CleanupFunc s_TextureMapperCleanupFuncs[] = {CleanupPixelBasedTextureMapper, CleanupProceduralTextureMapper};
// 0x00976144 (name is ours)
CloneFunc s_TextureMapperCloneFuncs[] = {ClonePixelBasedTextureMapper, CloneProceduralTextureMapper};
// 0x0097614C (name is ours)
DestroyFunc s_TextureMapperDestroyFuncs[] = {internal::DestroyPixelBasedTextureMapper, DestroyProceduralTextureMapper};

// 0x004A9048 (name is ours)
DECOMP_NOINLINE void internal::BuildCubeTextureCommand(ResPixelBasedTextureMapperData* pMapper, const ResTextureData* pTexture)
{
    const ResCubeTextureData* pCube = static_cast<const ResCubeTextureData*>(pTexture);
    u32* pCommands = pMapper->commands;
    pMapper->commandSize = 56;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_TYPE] &= ~0xF;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_BORDER_COLOR_HEADER] &= ~0x0FF00000;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_DIM] &= 0xF800F800;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_PARAM] &= ~(3 << 4);
    pCommands[ResPixelBasedTextureMapperData::COMMAND_LOD] &= ~0xF0000;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_ADDR1] &= 0xF0000000;
    for (int i = 0; i < 5; ++i) {
        pCommands[ResPixelBasedTextureMapperData::COMMAND_ADDR2 + i] = (pCommands[ResPixelBasedTextureMapperData::COMMAND_ADDR2 + i] >> 22) << 22;
    }
    SetTextureCommand(pMapper, pCube, 9);
    if (pCube->format == ResTexture::FORMAT_ETC1) {
        pCommands[ResPixelBasedTextureMapperData::COMMAND_PARAM] |= PARAM_ETC1;
    }
    SetMipmapLevelCommand(pMapper, pCube);
    for (u8 face = 0; face < 6; ++face) {
        const u32 address = GetImageAddress(pCube->images[face].To<ResImageData>());
        if (face == 0) {
            pCommands[ResPixelBasedTextureMapperData::COMMAND_ADDR1] |= (address << 1) >> 4;
        } else {
            pCommands[ResPixelBasedTextureMapperData::COMMAND_ADDR1 + face] |= (address >> 3) & ~0x1FC00000;
        }
    }
}

// 0x004A922C (name is ours)
DECOMP_NOINLINE void internal::BuildImageTextureCommand(ResPixelBasedTextureMapperData* pMapper, const ResTextureData* pTexture)
{
    const ResImageTextureData* pImageTexture = static_cast<const ResImageTextureData*>(pTexture);
    u32* pCommands = pMapper->commands;
    pMapper->commandSize = 32;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_TYPE] &= ~0xF;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_BORDER_COLOR_HEADER] &= ~0x0FF00000;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_DIM] &= 0xF800F800;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_PARAM] &= ~(3 << 4);
    pCommands[ResPixelBasedTextureMapperData::COMMAND_LOD] &= ~0xF0000;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_ADDR1] &= 0xF0000000;
    const ResImageData* pImage = pImageTexture->image.To<ResImageData>();
    const void* pAddress = pImage->pLocationAddress != nullptr ? pImage->pLocationAddress : pImage->data.ToPtr();
    SetTextureCommand(pMapper, pImageTexture, 4);
    pCommands[ResPixelBasedTextureMapperData::COMMAND_TYPE_HEADER] |= 0xF << 16;
    if (pImageTexture->format == ResTexture::FORMAT_ETC1) {
        pCommands[ResPixelBasedTextureMapperData::COMMAND_PARAM] |= PARAM_ETC1;
    }
    SetMipmapLevelCommand(pMapper, pImageTexture);
    pCommands[ResPixelBasedTextureMapperData::COMMAND_ADDR1] |= (nngxGetPhysicalAddr(reinterpret_cast<unsigned int>(pAddress)) << 1) >> 4;
}

// 0x004A9388 (name is ours)
DECOMP_NOINLINE void internal::BuildShadowTextureCommand(ResPixelBasedTextureMapperData* pMapper, const ResTextureData* pTexture)
{
    const ResShadowTextureData* pShadow = static_cast<const ResShadowTextureData*>(pTexture);
    u32* pCommands = pMapper->commands;
    pMapper->commandSize = 56;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_TYPE] &= ~0xF;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_BORDER_COLOR_HEADER] &= ~0x0FF00000;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_DIM] &= 0xF800F800;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_PARAM] &= ~(3 << 4);
    pCommands[ResPixelBasedTextureMapperData::COMMAND_PARAM] &= ~PARAM_TYPE_MASK;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_LOD] &= ~0xF0000;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_ADDR1] &= 0xF0000000;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_SHADOW] = 0;
    const ResImageData* pImage = pShadow->image.To<ResImageData>();
    const void* pAddress = pImage->pLocationAddress != nullptr ? pImage->pLocationAddress : pImage->data.ToPtr();
    SetTextureCommand(pMapper, pShadow, 10);
    pCommands[ResPixelBasedTextureMapperData::COMMAND_TYPE_HEADER] |= 0xF << 16;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_PARAM] |= PARAM_TYPE_SHADOW_2D;

    // a shadow texture is sampled linearly, clamped to the border
    ResTextureSampler sampler(DynamicCastData<ResTextureSamplerData>(pMapper->sampler.ToPtr(), ResTextureSampler::TYPE_STANDARD));
    pCommands[ResPixelBasedTextureMapperData::COMMAND_PARAM] |= PARAM_SHADOW;
    sampler.ptr()->minFilter = ResTextureSamplerData::MIN_FILTER_LINEAR;
    u32* pOwnerParam = &sampler.ptr()->owner.To<ResPixelBasedTextureMapperData>()->commands[ResPixelBasedTextureMapperData::COMMAND_PARAM];
    *pOwnerParam = (*pOwnerParam & ~(1 << PARAM_MIN_FILTER_SHIFT)) | ((s_MinFilterBits[ResTextureSamplerData::MIN_FILTER_LINEAR] & 1) << PARAM_MIN_FILTER_SHIFT);
    *pOwnerParam = (*pOwnerParam & ~(1 << PARAM_MIP_FILTER_SHIFT)) | ((s_MipFilterBits[ResTextureSamplerData::MIN_FILTER_LINEAR] & 1) << PARAM_MIP_FILTER_SHIFT);
    sampler.SetTextureMipmapCommand();
    pOwnerParam = &sampler.ptr()->owner.To<ResPixelBasedTextureMapperData>()->commands[ResPixelBasedTextureMapperData::COMMAND_PARAM];
    *pOwnerParam |= PARAM_MAG_FILTER_LINEAR;
    pOwnerParam = &sampler.ptr()->owner.To<ResPixelBasedTextureMapperData>()->commands[ResPixelBasedTextureMapperData::COMMAND_PARAM];
    *pOwnerParam = (*pOwnerParam & ~PARAM_WRAP_S_MASK) | (PARAM_WRAP_CLAMP_TO_BORDER << 12);
    pOwnerParam = &sampler.ptr()->owner.To<ResPixelBasedTextureMapperData>()->commands[ResPixelBasedTextureMapperData::COMMAND_PARAM];
    *pOwnerParam = (*pOwnerParam & ~PARAM_WRAP_T_MASK) | (PARAM_WRAP_CLAMP_TO_BORDER << 8);

    pCommands[ResPixelBasedTextureMapperData::COMMAND_ADDR1] |= (nngxGetPhysicalAddr(reinterpret_cast<unsigned int>(pAddress)) << 1) >> 4;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_SHADOW] |= pShadow->isOrthographic ? 0 : 1;
    pCommands[ResPixelBasedTextureMapperData::COMMAND_SHADOW] |= nn::gr::CTR::detail::Float32ToUnsignedFix24(pShadow->bias) & ~1 & 0xFFFFFF;
}

// 0x004A9CF0 (name is ours)
void internal::DestroyPixelBasedTextureMapper(ResTextureMapperData* pMapper, os::IAllocator* pAllocator)
{
    ResPixelBasedTextureMapperData* pPixelBased = static_cast<ResPixelBasedTextureMapperData*>(pMapper);
    if (pPixelBased->sampler.ToPtr() != nullptr) {
        pAllocator->Free(pPixelBased->sampler.ToPtr());
    }
    if (pPixelBased->texture.ToPtr() != nullptr) {
        pAllocator->Free(pPixelBased->texture.ToPtr());
    }
    pAllocator->Free(pPixelBased);
}

// 0x0013F13C | nintendogs:callgraph [tier A]
void nw::gfx::res::ResTextureMapper::Cleanup()
{
    ResTextureMapperData* pData = ptr();
    switch (pData->typeInfo) {
    case TYPE_PIXEL_BASED:
        s_TextureMapperCleanupFuncs[0](pData);
        break;
    case TYPE_PROCEDURAL:
        s_TextureMapperCleanupFuncs[1](pData);
        break;
    default:
        break;
    }
}

// 0x004A77D0 | nintendogs:bytes [tier A]
void nw::gfx::res::ResTextureMapper::SetTexture(ResTexture texture)
{
    ResReferenceTextureData* pReference = ptr()->texture.To<ResReferenceTextureData>();
    pReference->target.Set(internal::DereferenceTexture(texture.ptr()));
    if (m_pData->typeInfo == TYPE_PIXEL_BASED) {
        internal::BuildTextureCommand(static_cast<ResPixelBasedTextureMapperData*>(ptr()), internal::DereferenceTexture(texture.ptr()));
    }
}

// 0x004A7894 | nintendogs:bytes [tier A]
nw::gfx::res::ResTextureMapper nw::gfx::res::ResTextureMapper::CloneDynamic(os::IAllocator* pAllocator)
{
    ResTextureMapperData* pClone = nullptr;
    ResTextureMapperData* pData = ptr();
    switch (pData->typeInfo) {
    case TYPE_PIXEL_BASED:
        pClone = s_TextureMapperCloneFuncs[0](pData, pAllocator);
        break;
    case TYPE_PROCEDURAL:
        pClone = s_TextureMapperCloneFuncs[1](pData, pAllocator);
        break;
    default:
        return ResTextureMapper(pClone);
    }
    if (pClone != nullptr) {
        pClone->pDynamicAllocator = pAllocator;
    }
    return ResTextureMapper(pClone);
}

// 0x004A78F4 | nintendogs:bytes [tier A]
void nw::gfx::res::ResTextureMapper::DestroyDynamic()
{
    ResTextureMapperData* pData = ptr();
    if (pData->pDynamicAllocator == nullptr) {
        return;
    }
    switch (pData->typeInfo) {
    case TYPE_PIXEL_BASED:
        s_TextureMapperDestroyFuncs[0](pData, pData->pDynamicAllocator);
        break;
    case TYPE_PROCEDURAL:
        s_TextureMapperDestroyFuncs[1](pData, pData->pDynamicAllocator);
        break;
    default:
        break;
    }
}

// 0x004A7934 | nintendogs:bytes [tier A]
u32 nw::gfx::res::ResTextureMapper::Setup(os::IAllocator* pAllocator, ResGraphicsFile graphicsFile)
{
    u32 result = RESULT_SUCCESS;
    ResTextureMapperData* pData = ptr();
    switch (pData->typeInfo) {
    case TYPE_PIXEL_BASED:
        result |= s_TextureMapperSetupFuncs[0](pData, pAllocator, graphicsFile);
        break;
    case TYPE_PROCEDURAL:
        result |= s_TextureMapperSetupFuncs[1](pData, pAllocator, graphicsFile);
        break;
    default:
        break;
    }
    return result;
}

} // namespace res
} // namespace gfx
} // namespace nw
