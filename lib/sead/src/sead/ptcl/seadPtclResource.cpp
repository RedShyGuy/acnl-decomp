#include "sead/ptcl/seadPtclResource.h"
#include "nn/gx/detail/gx_Context.h"
#include "nn/gx/gx_Api.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "sead/ptcl/seadEmitterData.h"
#include "sead/ptcl/seadPtclSystem.h"
#include "sead/seadNew.h"

#include <string.h>

namespace sead {
namespace ptcl {
namespace {
// the emitter data of an emitter set (at most eight)
inline s32 getEmitterData_(const EmitterSetTbl* set, void** data)
{
    s32 num = set->numEmitter;
    for (s32 i = 0; i < num; i++) {
        data[i] = set->emitterTbl[i].data;
    }
    return num;
}

// the wrap mode of nn::gr for one of the resource
inline u32 convertWrap_(u32 wrap)
{
    u32 mode;
    switch (wrap) {
    case 0:
        mode = 3;
        break;
    case 1:
        mode = 2;
        break;
    case 2:
        mode = 0;
        break;
    }
    return mode;
}
} // namespace

// 0x00557578 | mk7dlp:bytes [tier B]
void sead::ptcl::PtclResource::bindTarget(s32 emitterSetId, ResourceBind* bind, EmitterTblData* emitterTbl, s32 numEmitter,
                                          const SafeStringBase<char>& name, u32 userData)
{
    EmitterSetTbl* set = &mEmitterSetTbl[emitterSetId];
    void* oldData[8];
    s32 num = getEmitterData_(set, oldData);
    bind->setData = set->data;
    bind->emitterSetId = emitterSetId;
    bind->resourceId = mResourceID;
    bind->name = set->name;
    bind->numEmitter = set->numEmitter;
    bind->emitterTbl = set->emitterTbl;
    bind->userData = set->userData;
    set->emitterTbl = emitterTbl;
    set->numEmitter = numEmitter;
    set->name = name.getCString();
    set->userData = userData;
    mSystem->reCreateEmitter(oldData, num, mResourceID, emitterSetId, false);
}

// 0x00557698 | mk7dlp
void* sead::ptcl::PtclResource::createEmtpryData(Heap* heap, s32 num)
{
    u32 strTblOffset = 28 + num * sizeof(EmitterSetData);
    u32 size = strTblOffset + num * 8 + 128;
    u8* data = new (heap, 128) u8[size];
    memset(data, 0, size);
    ResourceHeader* header = reinterpret_cast<ResourceHeader*>(data);
    memcpy(header->magic, "SPBD", 4);
    header->version = 11;
    header->numEmitterSet = num;
    header->strTblOffset = strTblOffset;
    char* strTbl = reinterpret_cast<char*>(data) + strTblOffset;
    // ("reserved for the editor")
    memcpy(strTbl, "エディタ予約", 18);
    u32 nameOffset = 19;
    for (s32 i = 0; i < num; i++) {
        header->sets[i].nameOffset = nameOffset;
        FixedSafeString<64> name;
        name.format("set%02d", i);
        memcpy(strTbl + nameOffset, name.getCString(), name.getLength());
        nameOffset += name.getLength() + 1;
    }
    return data;
}

// 0x00557868 | mk7dlp
sead::ptcl::TextureHandle* sead::ptcl::PtclResource::createTextureHandle(Heap* heap, void* image, const TextureRes& res)
{
    u8 format = res.format;
    u8 mipLevel = res.mipLevel;
    u16 width = res.width;
    u16 height = res.height;
    u8 wrap = res.wrap;
    u8 filter = res.filter;
    TextureHandle* handle = new (heap, 4) TextureHandle();
    nn::gr::CTR::Texture::Unit0& unit = handle->texture.m_Unit0;
    unit.m_TextureType = 0;
    unit.m_PhysicalAddr = nngxGetPhysicalAddr(reinterpret_cast<uptr>(image));
    unit.m_Width = width;
    unit.m_Height = height;
    unit.m_Format = format;
    u32 wrapS = convertWrap_(wrap & 0xF);
    u32 wrapT = convertWrap_(wrap >> 4);
    u32 magFilter;
    u32 minFilter;
    if (filter == 0) {
        magFilter = 2;
        minFilter = 1;
    } else {
        magFilter = 0;
        minFilter = 0;
    }
    u32 isEtc = format == 12 ? 2 : 0;
    handle->param = (wrapS << 12) | (wrapT << 8) | (isEtc << 4) | (minFilter << 2) | magFilter;
    handle->size = (unit.m_Width << 16) | unit.m_Height;
    if (mipLevel > 0) {
        u8 maxLevel = mipLevel - 1;
        unit.m_MinLodLevel = 0;
        unit.m_MaxLodLevel = maxLevel;
        unit.m_LodBias = 0.0f;
        handle->lod = maxLevel << 16;
    } else {
        handle->lod = 0;
    }
    return handle;
}

// 0x00557988 | mk7dlp:bytes [tier B]
void sead::ptcl::PtclResource::unbind(ResourceBind* bind, bool reCreate, bool kill)
{
    EmitterSetTbl* set = &mEmitterSetTbl[bind->emitterSetId];
    void* oldData[8];
    s32 num = getEmitterData_(set, oldData);
    set->emitterTbl = bind->emitterTbl;
    set->numEmitter = bind->numEmitter;
    set->name = bind->name;
    set->userData = bind->userData;
    bind->setData = nullptr;
    if (reCreate) {
        mSystem->reCreateEmitter(oldData, num, mResourceID, bind->emitterSetId, false);
    }
    if (kill) {
        mSystem->reCreateEmitter(oldData, num, mResourceID, bind->emitterSetId, true);
    }
}

// 0x00557AA0 | mk7dlp
sead::ptcl::PtclResource::PtclResource(Heap* heap, void* data, s32 resId, PtclSystem* system, BufferLocation location)
    : hostio::Node(), mName()
{
    mSystem = system;
    mResourceID = resId;
    mHeader = static_cast<ResourceHeader*>(data);
    u8* base = static_cast<u8*>(data);
    mStrTbl = reinterpret_cast<const char*>(base + mHeader->strTblOffset);
    u8* textureTbl = base + mHeader->textureTblOffset;
    mTextureTbl = textureTbl;
    u32 textureSize = mHeader->textureTblSize;
    if (textureSize != 0) {
        nngxUpdateBuffer(textureTbl, textureSize);
    }
    if (location.value == 1 || location.value == 2) {
        GxAllocateFunction allocate;
        GxFreeFunction free;
        nngxGetAllocator(&allocate, &free);
        GLenum area = location.value == 1 ? nn::gx::detail::MEMORY_AREA_VRAM_A : nn::gx::detail::MEMORY_AREA_VRAM_B;
        void* vram = allocate(area, nn::gx::detail::MEMORY_PURPOSE_TEXTURE, 0, textureSize);
        nngxAddVramDmaCommand(textureTbl, vram, textureSize);
        textureTbl = static_cast<u8*>(vram);
    }
    mTextureTbl = textureTbl;

    mEmitterSetTbl = new (heap, 4) EmitterSetTbl[mHeader->numEmitterSet];
    for (s32 i = 0; i < mHeader->numEmitterSet; i++) {
        EmitterSetData* setData = &mHeader->sets[i];
        EmitterSetTbl* set = &mEmitterSetTbl[i];
        set->data = setData;
        setData->name = mStrTbl + setData->nameOffset;
        set->name = setData->name;
        set->numEmitter = setData->numEmitter;
        set->userData = setData->userData;
        set->field_0x24 = false;
        if (setData->emitterTblOffset == 0) {
            set->emitterTbl = nullptr;
        } else {
            set->emitterTbl = reinterpret_cast<EmitterTblData*>(base + setData->emitterTblOffset);
            for (s32 j = 0; j < set->numEmitter; j++) {
                EmitterTblData* emitter = &set->emitterTbl[j];
                if (emitter->dataOffset == 0) {
                    emitter->data = nullptr;
                    continue;
                }
                emitter->data = reinterpret_cast<SimpleEmitterData*>(base + emitter->dataOffset);
                emitter->data->name = mStrTbl + emitter->data->nameOffset;
                emitter->data->texture =
                    createTextureHandle(heap, mTextureTbl + emitter->data->textureDataOffset, emitter->data->textureRes);
                SimpleEmitterData* res = emitter->data;
                if (res->type == 1 && (res->childFlag & 1)) {
                    res->child.texture =
                        createTextureHandle(heap, mTextureTbl + res->child.textureDataOffset, res->child.textureRes);
                }
            }
        }
        set->orgData = set->data;
        set->orgEmitterTbl = set->emitterTbl;
        set->orgNumEmitter = set->numEmitter;
        set->orgUserData = set->userData;
    }
}

// 0x00557D7C slot 0x04 | mk7dlp
// 0x00557D6C (deleting dtor)
sead::ptcl::PtclResource::~PtclResource()
{
    for (s32 i = 0; i < mHeader->numEmitterSet; i++) {
        EmitterSetTbl* set = &mEmitterSetTbl[i];
        for (s32 j = 0; j < set->data->numEmitter; j++) {
            SimpleEmitterData* res = set->emitterTbl[j].data;
            delete res->texture;
            if (res->type == 1 && (res->childFlag & 1)) {
                delete res->child.texture;
            }
        }
    }
    delete[] mEmitterSetTbl;

    uptr textureTbl = reinterpret_cast<uptr>(mTextureTbl);
    GLenum area;
    if (nngxGetVramStartAddr(nn::gx::detail::MEMORY_AREA_VRAM_A) <= textureTbl &&
        textureTbl < nngxGetVramEndAddr(nn::gx::detail::MEMORY_AREA_VRAM_A)) {
        area = nn::gx::detail::MEMORY_AREA_VRAM_A;
    } else if (nngxGetVramStartAddr(nn::gx::detail::MEMORY_AREA_VRAM_B) <= textureTbl &&
               textureTbl < nngxGetVramEndAddr(nn::gx::detail::MEMORY_AREA_VRAM_B)) {
        area = nn::gx::detail::MEMORY_AREA_VRAM_B;
    } else {
        return;
    }
    GxAllocateFunction allocate;
    GxFreeFunction free;
    nngxGetAllocator(&allocate, &free);
    free(area, nn::gx::detail::MEMORY_PURPOSE_TEXTURE, 0, mTextureTbl);
}
} // namespace ptcl
} // namespace sead
