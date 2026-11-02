#include "ssys/ma/lyt/dArcResourceAccessorVRAM.h"
#include "ssys/ma/lyt/dArcResAccReader.h"
#include "ssys/ma/dFlushCache.h"
#include "nn/gx/gx_Api.h"
#include "nn/gx/detail/gx_Context.h"
#include "nw/lyt/lyt_ResourceFormat.h"
#include "nw/ut/ut_Api.h"

#include <new>

namespace ssys {
namespace ma {
namespace lyt {
namespace {
// the memory areas and the purpose "texture" of the allocator of nngx (values of the library; names are ours)
const u32 MEM_FCRAM = 0x10000;
const u32 MEM_AREA_FLAGS = 0x03000000;
const GLenum MEM_TEXTURE = 0x101;
} // namespace

// 0x008AF168
const TexFormatInfo s_TexFormatInfos[14] = {
    {0, 0x6757, 0x1401, 8, 0},     {1, 0x6756, 0x1401, 8, 0},     {2, 0x6758, 0x6760, 8, 0x100},
    {3, 0x6758, 0x1401, 8, 0},     {4, 0x6759, 0x1401, 8, 0},     {5, 0x6754, 0x8363, 8, 0},
    {6, 0x6754, 0x1401, 8, 0},     {7, 0x6752, 0x8034, 8, 0},     {8, 0x6752, 0x8033, 8, 0},
    {9, 0x6752, 0x1401, 8, 0},     {10, 0x675A, 0, 16, 1},        {11, 0x675B, 0, 8, 0x101},
    {12, 0x6757, 0x6761, 8, 0x100}, {13, 0x6756, 0x6761, 8, 0x100},
};

// 0x00568BF8 slot 0x14
nw::lyt::TextureInfo ssys::ma::lyt::ArcResourceAccessorVRAM::LoadTexture(const char* name)
{
    u32 size = 0;
    void* data = GetResource('timg', name, &size);
    if (data != NULL && size != 0) {
        nw::lyt::TextureInfo info = LoadTextureImage(data, size, 0x02020000);
        if (info.physicalAddress != 0) {
            nw::lyt::TextureInfo result;
            result = info;
            return result;
        }
    }
    return nw::lyt::TextureInfo();
}

// 0x00568CD0 (name is ours)
void ssys::ma::lyt::ArcResourceAccessorVRAM::ReleaseTextures()
{
    FreeImages();
    m_TexBufList.DeleteAll();
    m_TextureList.Clear();
}

// (inline)
inline void ssys::ma::lyt::ArcResourceAccessorVRAM::FreeImages()
{
    u32 area = m_Area & ~MEM_AREA_FLAGS;
    if (area == MEM_FCRAM) {
        return;
    }
    GxAllocateFunction allocateFunction;
    GxFreeFunction freeFunction;
    nngxGetAllocator(&allocateFunction, &freeFunction);
    for (TexBufListNode* node = static_cast<TexBufListNode*>(m_TexBufList.GetFirst()); node != NULL;
         node = static_cast<TexBufListNode*>(node->m_pNext)) {
        freeFunction(area, MEM_TEXTURE, 0, node->m_pAddress);
    }
}

// 0x00568D90 (name is ours)
nw::lyt::TextureInfo ssys::ma::lyt::ArcResourceAccessorVRAM::LoadTextureImage(const void* data, u32 size, u32 area)
{
    u32 imageSize = *reinterpret_cast<const u32*>(static_cast<const u8*>(data) + size - 4);
    nw::ut::BinaryFileHeader* header = reinterpret_cast<nw::ut::BinaryFileHeader*>(
        const_cast<u8*>(static_cast<const u8*>(data)) + nw::ut::RoundUp(imageSize, 4));
    nw::lyt::TextureInfo info;
    if (!nw::ut::IsValidBinaryFile(header, nw::lyt::res::FILE_SIGNATURE_IMAGE, nw::lyt::res::FILE_VERSION, 1)) {
        info.texObject = 0;
        info.physicalAddress = 0;
        return info;
    }
    nw::ut::BinaryBlockHeader* image = NULL;
    nw::ut::BinaryBlockHeader* block = NULL;
    for (int i = 0; i < header->dataBlocks; i++) {
        block = nw::ut::GetNextBinaryBlockHeader(header, block);
        if (block->kind == nw::lyt::res::BLOCK_KIND_IMAGE) {
            image = block;
        }
    }
    if (image == NULL) {
        info.texObject = 0;
        info.physicalAddress = 0;
        return info;
    }
    const u8* imageData = reinterpret_cast<const u8*>(image);
    u8 format = imageData[12];
    u16 width = *reinterpret_cast<const u16*>(imageData + 8);
    u16 height = *reinterpret_cast<const u16*>(imageData + 10);
    u32 minSize = s_TexFormatInfos[format].minSize;
    u16 realWidth = minSize;
    while (realWidth != 0 && width > realWidth) {
        realWidth = realWidth << 1;
    }
    u16 realHeight = minSize;
    while (realHeight != 0 && height > realHeight) {
        realHeight = realHeight << 1;
    }
    if (area == 0) {
        area = 0x02010000;
    }
    m_Area = area;
    u32 memoryArea = area & ~MEM_AREA_FLAGS;
    uptr address;
    if (memoryArea == MEM_FCRAM) {
        if (ArcResAccReader::s_UseFlushCache) {
            FlushCache::Add(reinterpret_cast<uptr>(data), imageSize);
        } else {
            nngxUpdateBuffer(data, imageSize);
        }
        address = nngxGetPhysicalAddr(reinterpret_cast<uptr>(data));
    } else {
        GxAllocateFunction allocateFunction;
        GxFreeFunction freeFunction;
        nngxGetAllocator(&allocateFunction, &freeFunction);
        void* vram = allocateFunction(memoryArea, MEM_TEXTURE, 0, imageSize);
        if (ArcResAccReader::s_UseFlushCache) {
            FlushCache::Flush(reinterpret_cast<uptr>(data), imageSize);
            nngxAddVramDmaCommandNoCacheFlush(data, vram, imageSize);
        } else {
            nngxAddVramDmaCommand(data, vram, imageSize);
        }
        address = nngxGetPhysicalAddr(reinterpret_cast<uptr>(vram));
        void* memory = nw::lyt::Layout::AllocMemory(sizeof(TexBufListNode), 4);
        if (memory != NULL) {
            m_TexBufList.PushBack(new (memory) TexBufListNode(vram));
        }
    }
    info.texObject = 0;
    info.physicalAddress = address;
    info.size = nw::lyt::TexSize(width, height);
    info.realSize = nw::lyt::TexSize(realWidth, realHeight);
    info.format = format;
    return info;
}

// 0x00569024 slot 0x00
// 0x00569014 (deleting dtor)
ssys::ma::lyt::ArcResourceAccessorVRAM::~ArcResourceAccessorVRAM()
{
    FreeImages();
}

} // namespace lyt
} // namespace ma
} // namespace ssys
