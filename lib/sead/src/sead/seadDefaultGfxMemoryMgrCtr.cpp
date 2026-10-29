#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadGfxMemoryMgrCtr.h"
#include "sead/seadDefaultGfxMemoryMgrCtr.h"
#include "nn/gx/gx_Api.h"
#include "sead/seadNew.h"

namespace sead {
namespace {
// x rounded away from zero to a multiple of alignment
inline s32 roundUp(s32 x, s32 alignment)
{
    if (x >= 0) {
        return (x + alignment - 1) / alignment * alignment;
    }
    return (x - alignment + 1) / alignment * alignment;
}

const u32 AREA_MAIN = 0x10000;
const u32 AREA_VRAM_A = 0x20000;
const u32 AREA_VRAM_B = 0x30000;
} // namespace

// 0x0011EBAC (name is ours)
sead::DefaultGfxMemoryMgrCtr::DefaultGfxMemoryMgrCtr(Heap* heap)
    : GfxMemoryMgrCtr(), mHeap(heap), mVramACurrent(0), mVramBCurrent(0), mVramSizeOfKind102(0), mVramSizeOfKind101(0)
{
    mVramACurrent = nngxGetVramStartAddr(AREA_VRAM_A);
    mVramBCurrent = nngxGetVramStartAddr(AREA_VRAM_B);
    mVramAEnd = nngxGetVramEndAddr(AREA_VRAM_A);
    mVramBEnd = nngxGetVramEndAddr(AREA_VRAM_B);
}

// 0x0054BAC4 slot 0x08 (name is ours)
void* sead::DefaultGfxMemoryMgrCtr::allocate(u32 area, u32 kind, u32, s32 size)
{
    s32 alignment = 128;
    switch (kind) {
    case 0x100:
        alignment = 4;
        break;
    case 0x101:
        break;
    case 0x102:
    case 0x104:
    case 0x105:
        alignment = 16;
        break;
    case 0x103:
        alignment = 192;
        break;
    }
    switch (kind) {
    case 0x101:
        if (area == AREA_MAIN) {
            break;
        }
        mVramSizeOfKind101 += size;
        break;
    case 0x102:
        if (area == AREA_MAIN) {
            break;
        }
        mVramSizeOfKind102 += size;
        break;
    }
    if (area == AREA_MAIN) {
        return new (mHeap, alignment) u8[size];
    }
    if (area == AREA_VRAM_A) {
        s32 addr = roundUp(mVramACurrent, alignment);
        if (static_cast<s64>(mVramAEnd) - static_cast<s64>(static_cast<u32>(addr + size)) >= 0) {
            mVramACurrent = addr + size;
            return reinterpret_cast<void*>(addr);
        }
        return nullptr;
    }
    if (area == AREA_VRAM_B) {
        s32 addr = roundUp(mVramBCurrent, alignment);
        if (static_cast<s64>(mVramBEnd) - static_cast<s64>(static_cast<u32>(addr + size)) >= 0) {
            mVramBCurrent = addr + size;
            return reinterpret_cast<void*>(addr);
        }
        return nullptr;
    }
    return nullptr;
}

// 0x0054BAB0 slot 0x0C (name is ours)
void sead::DefaultGfxMemoryMgrCtr::deallocate(u32 area, u32, u32, void* addr)
{
    if (area == AREA_MAIN) {
        ::operator delete(addr);
    }
}

// 0x0054BC5C slot 0x10
// 0x0054BC58 (deleting dtor)
sead::DefaultGfxMemoryMgrCtr::~DefaultGfxMemoryMgrCtr()
{
    // empty (in the original too)
}

} // namespace sead

// the type information of the classes below GfxMemoryMgrCtr
// 0x00828FE0 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::GfxMemoryMgrCtr>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
