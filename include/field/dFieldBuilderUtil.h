#pragma once

#include "decomp.h"
#include "field/dField.h"
#include "field/dFieldRect.h"
#include "ssys/ma/dLoadSplit.h"
#include "Sv/dSvBgName.h"
#include "sead/seadObjArray.h"

namespace sead {
class Heap;
class Random;
} // namespace sead

// The helpers of the field builders: the item layouts of the grounds and the steps for each block of a field (all names
// are ours).

// an item of a layout (6 bytes in the file)
struct FgDataItem
{
    u16 id;    // 0x0 SvFgName::ID
    u16 flags; // 0x2 bits 14..15: the orientation of a usual item
    u8 x;      // 0x4 the unit in the block
    u8 z;      // 0x5
};
ASSERT_SIZE(FgDataItem, 0x6);

// The items of the grounds (or of other keys) from a file, e.g. "FgDataTour/L0.bin" in the BsTourFieldResMgr: the number
// of items for each key from byte 2 on (the u16 at 0 is the size of this table), then the items of all keys in their
// order. The class name is from the RTTI of BaseResourceMgr<FgDataLoader, 18u>, the functions are ours.
class FgDataLoader
{
public:
    // the buffer of the items of one key for the builders
    static const u32 ITEM_BUFFER_SIZE = 0x600;

    // (only the member is constructed; its call became a fall-through)
    FgDataLoader(); // 0x0012D3C4
    ~FgDataLoader(); // 0x00201120

    // starts loading the file (true without path or heap)
    bool Load(const char* path, sead::Heap* heap); // 0x00201028
    void Unload(); // 0x0020106C
    // the items of the ground bg are copied to out (0: no file, none or out is too small)
    u32 GetItems(FgDataItem* out, u32 size, u32 bg) const; // 0x00201070
    DECOMP_EMIT_COPY bool IsLoaded() const // 0x00713150
    {
        return m_File.m_pData != NULL;
    }

    ::ssys::ma::LoadSplit m_File; // 0x00
};

// the function for each block of the field (the blocks of rect), the function object is returned
template <typename F>
F ForEachBlock(const FieldRect& rect, F function)
{
    for (FieldRect::iterator it = rect.begin(); it != rect.end(); ++it) {
        function(*it);
    }
    return function;
}

// the items of the layout for each block (usual items keep the orientation of their flags)
struct BlockItemsSetter
{
    // (the items are not initialized)
    BlockItemsSetter(Field* field, const FgDataLoader* layout) : m_pField(field), m_pLayout(layout) {}
    void operator()(const Vector2i& block); // 0x00201158

    Field* m_pField;                                                       // 0x000
    const FgDataLoader* m_pLayout;                                             // 0x004
    FgDataItem m_Items[FgDataLoader::ITEM_BUFFER_SIZE / sizeof(FgDataItem)]; // 0x008
};
ASSERT_SIZE(BlockItemsSetter, 0x608);

// the heights of the units of the ground of each block
struct BlockHeightsSetter
{
    explicit BlockHeightsSetter(Field* field) : m_pField(field) {}
    void operator()(const Vector2i& block); // 0x0020125C

    Field* m_pField; // 0x0
};

// the items of both layers of each block are removed
struct BlockItemsCleaner
{
    explicit BlockItemsCleaner(Field* field) : m_pField(field) {}
    void operator()(const Vector2i& block); // 0x0020245C

    Field* m_pField; // 0x0
};

// the grounds of the blocks from a table (one SvBgName per block, row by row, as in stage::FieldLayout)
struct BlockNamesSetter
{
    BlockNamesSetter(Field* field, const u16* names) : m_pField(field), m_pNames(names) {}
    // (inline everywhere; a copy)
    DECOMP_EMIT_COPY void operator()(const Vector2i& block) // 0x00202B20
    {
        if (m_pField == NULL) {
            return;
        }
        const s32 width = m_pField->GetBlockBounds().GetWidth();
        BgInfo* bg = m_pField->GetBg(block.x, block.y);
        if (bg == NULL) {
            return;
        }
        bg->SetName(static_cast<SvBgName>(m_pNames[block.y * width + block.x]));
    }

    Field* m_pField;          // 0x0
    const u16* m_pNames; // 0x4
};

// a random acre of the kind of the acre map for each block (FieldFactory::GetRandomAcre; the map has
// FieldFactory::ACRE_MAP_WIDTH kinds per row)
struct RandomAcreSetter
{
    RandomAcreSetter(Field* field, const u8* map, sead::Random* random) : m_pField(field), m_pMap(map), m_pRandom(random)
    {
    }

    void operator()(const Vector2i& block); // 0x00202BCC

    Field* m_pField;         // 0x0
    const u8* m_pMap;        // 0x4
    sead::Random* m_pRandom; // 0x8
};

// the blocks of the acre maps (in the original a copy of the constructor)
template <>
DECOMP_NOINLINE sead::FixedObjArray<Vector2i, 50>::FixedObjArray();

// (the loop with the grounds is inline everywhere in the original)
template <>
DECOMP_ALWAYS_INLINE BlockNamesSetter ForEachBlock<BlockNamesSetter>(const FieldRect& rect, BlockNamesSetter function)
{
    for (FieldRect::iterator it = rect.begin(); it != rect.end(); ++it) {
        function(*it);
    }
    return function;
}

extern template BlockItemsSetter ForEachBlock<BlockItemsSetter>(const FieldRect& rect, BlockItemsSetter function);
extern template BlockHeightsSetter ForEachBlock<BlockHeightsSetter>(const FieldRect& rect, BlockHeightsSetter function);
extern template BlockItemsCleaner ForEachBlock<BlockItemsCleaner>(const FieldRect& rect, BlockItemsCleaner function);
extern template RandomAcreSetter ForEachBlock<RandomAcreSetter>(const FieldRect& rect, RandomAcreSetter function);
