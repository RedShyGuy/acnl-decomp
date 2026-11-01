#include "cfl/cfl_Model.h"

#include <cstring>

namespace {
// the memory areas and the use of nngx (values of the library; names are ours)
const GLenum MEM_FCRAM = 0x10000;
const GLenum MEM_VRAMA = 0x20000;
const GLenum MEM_VRAMB = 0x30000;
const GLenum MEM_TEXTURE = 0x101;
} // namespace

// the bits per pixel of the texture formats (name is ours)
// 0x008A4114
const s32 s_TexFormatBits[12] = {4, 8, 4, 8, 8, 16, 16, 16, 24, 16, 16, 32};

namespace {
// the size of a texture with all mip levels (inline; name is ours)
inline u32 GetTextureSize(int width, int height, int mipLevels, int format)
{
    int bits = s_TexFormatBits[format];
    u32 size = 0;
    for (int i = 0; i < mipLevels; i++) {
        size += width * height * bits / 8;
        width >>= 1;
        height >>= 1;
    }
    return size;
}

// rounds up to a power of two (inline; name is ours)
inline int RoundUpPow2(int value)
{
    while ((value & -value) != value) {
        value += value & -value;
    }
    return value;
}
} // namespace

// (the flags are what the caller had in r3: the reference symbols name only two parameters)
// 0x0022BD8C | nintendogs:bytes [tier B]
void CFLiTexHandle::CreateFromRes(const CFLiTexHeader* header, unsigned area)
{
    if (header == NULL) {
        self = NULL;
        return;
    }
    Create(header->width, header->height, header->mipLevels, static_cast<CFLTexWrap>(header->wrapS),
           static_cast<CFLTexWrap>(header->wrapT), static_cast<CFLTexFmt>(header->format), header + 1, area, 0);
}

// a texture in the area; data in FCRAM is used in place when flags are set
// 0x0022BDDC | nintendogs:bytes [tier B]
void CFLiTexHandle::Create(int width, int height, int mipLevels, CFLTexWrap wrapS, CFLTexWrap wrapT,
                           CFLTexFmt format, const void* data, unsigned area, unsigned char flags)
{
    this->format = format;
    this->data = data;
    this->width = width;
    this->height = height;
    this->mipLevels = mipLevels;
    this->wrapS = wrapS;
    this->wrapT = wrapT;
    this->allocatedWidth = RoundUpPow2(width);
    this->allocatedHeight = RoundUpPow2(height);
    this->area = area;
    bool allocate = !(data != NULL && area == MEM_FCRAM && flags != 0);
    this->allocated = allocate;
    self = this;
    u32 size = GetTextureSize(allocatedWidth, allocatedHeight, mipLevels, format);
    if (!allocate) {
        address = reinterpret_cast<uptr>(data);
    } else {
        GxAllocateFunction allocateFunction;
        nngxGetAllocator(&allocateFunction, NULL);
        address = reinterpret_cast<uptr>(allocateFunction(area, MEM_TEXTURE, reinterpret_cast<u32>(this), size));
        if (data != NULL) {
            if (area != MEM_FCRAM) {
                nngxlowStoreDataCache(reinterpret_cast<uptr>(data), size);
                nngxAddVramDmaCommand(data, reinterpret_cast<GLvoid*>(address), size);
                return;
            }
            std::memcpy(reinterpret_cast<void*>(address), data, size);
            nngxlowStoreDataCache(address, size);
            return;
        }
    }
    if (area == MEM_FCRAM) {
        nngxlowStoreDataCache(address, size);
    }
}

// 0x0022BFA4 | nintendogs:bytes [tier B]
void CFLiTexHandle::Delete()
{
    if (self == NULL) {
        return;
    }
    if (allocated) {
        GxFreeFunction freeFunction;
        nngxGetAllocator(NULL, &freeFunction);
        freeFunction(area, MEM_TEXTURE, reinterpret_cast<u32>(self), reinterpret_cast<void*>(address));
    }
    self = NULL;
    width = 0;
    height = 0;
    allocated = false;
}
