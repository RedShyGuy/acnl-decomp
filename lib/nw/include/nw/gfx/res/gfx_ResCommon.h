#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_GfxObject.h"
#include "nw/ut/ut_Offset.h"
#include "nw/ut/ut_ResDicPatricia.h"

namespace nw {
namespace gfx {
namespace res {
// Helpers of the CGFX resources (3dbrew "CGFX"; the names are ours). Every resource object starts
// with its type: a bit set in which a derived type has the bits of its bases.

// the results of the setup functions besides RESULT_SUCCESS (bit 31: failure; names are ours)
const u32 RESULT_TEXTURE_NOT_FOUND = 0x80000001; // a referenced texture is not in the file
const u32 RESULT_SHADER_NOT_FOUND = 0x80000002;  // a referenced shader is not in the file
const u32 RESULT_LUT_NOT_FOUND = 0x80000004;     // a referenced lookup table is not in the file
const u32 RESULT_LUT_MISSING = 0x8;              // a lighting sampler has no lookup table

// the copy of data to VRAM (nngxAddVramDmaCommand; a variable, so that it can be replaced; name is ours)
extern void (*g_pAddVramDmaCommand)(const void* pSource, void* pDestination, int size);

// (inline) whether the object has all bits of the type
inline bool IsTypeOf(const void* pData, u32 type)
{
    return (*static_cast<const u32*>(pData) & type) == type;
}
// (inline) the object if it is of the type (NULL: not, or no object)
template <typename T>
inline const T* DynamicCastData(const void* pData, u32 type)
{
    return (pData != nullptr && IsTypeOf(pData, type)) ? static_cast<const T*>(pData) : nullptr;
}

// the count and the dictionary of a kind of objects (count first, then the self-relative offset)
struct ResDicInfo
{
    s32 count;      // 0x0
    ut::Offset dic; // 0x4 ResDicPatricia

    // (inline)
    ut::ResDicPatricia GetDic() const
    {
        ut::ResDicPatricia dic = {this->dic.To<ut::ResDicPatricia::Data>()};
        return dic;
    }
    // the object at index (the order of the dictionary)
    void* GetData(int index) const { return GetDic().GetDataByIndex(index); }
    // the object of the name (NULL: none)
    void* GetData(const char* name) const { return GetDic().GetData(name); }
};
ASSERT_SIZE(ResDicInfo, 0x8);

// the count and the array of self-relative offsets of a kind of objects
struct ResArrayInfo
{
    s32 count;        // 0x0
    ut::Offset array; // 0x4 the array of ut::Offset

    // (inline) the object at index
    void* GetData(int index) const { return const_cast<void*>(array.To<ut::Offset>()[index].ToPtr()); }
};
ASSERT_SIZE(ResArrayInfo, 0x8);

// the result of resolving a reference by name: the target and whether it was found (names are ours)
template <typename T>
struct ReferenceTarget
{
    T target;     // 0x0
    bool isFound; // 0x4
};
} // namespace res
} // namespace gfx
} // namespace nw
