#pragma once

#include "decomp.h"
#include "field/dFieldRect.h"
#include "field/dFieldTypes.h"
#include "nn/math/math_Vector3.h"
#include "Sv/dSvBgName.h"
#include "Sv/dSvFgBlock.h"
#include "sead/seadCheckedArray.h"

class JmpBlock;
namespace sead {
class Heap;
}

// One block of a field: its ground and its items per layer (name and layout after libgarden field/Field.hpp; the
// functions are ours).
class BgInfo
{
public:
    DECOMP_EMIT_COPY BgInfo() : m_Name(BG_NAME_NONE) {} // 0x0057C898
    DECOMP_EMIT_COPY ~BgInfo() {} // 0x0057C8A8

    // (the block position is not used)
    bool Initialize(s32 blockX, s32 blockZ); // 0x0057C860
    void Finalize(); // 0x0057C87C
    void SetBlocks(SvFgBlock* layer0, SvFgBlock* layer1); // 0x0057C708
    void SetName(SvBgName name); // 0x0057C874

    // the item of a unit of the block (NULL outside of the block or without the layer)
    DECOMP_NOINLINE SvFgName* GetFg(s32 unitX, s32 unitZ, u8 layer); // 0x0075383C
    DECOMP_EMIT_COPY DECOMP_NOINLINE const SvFgName* GetFg(s32 unitX, s32 unitZ, u8 layer) const // 0x00753838
    {
        return const_cast<BgInfo*>(this)->GetFg(unitX, unitZ, layer);
    }
    // places an item, not while the save data is written; false if the unit has no item
    bool SetFg(const SvFgName& fg, s32 unitX, s32 unitZ, u8 layer); // 0x0057C7C8

    DECOMP_EMIT_COPY DECOMP_NOINLINE SvBgName GetName() const { return m_Name; } // 0x00753930

    // whether a unit position is inside of a block (0..15)
    static bool IsInside(s32 unitX, s32 unitZ); // 0x0057C714

private:
    friend class FdInfo;

    // the units of a block (shared by IsInside and GetFg)
    static const FieldRect& GetLocalUnitRect()
    {
        // 0x0098B124 (guard 0x0094F4D0)
        static FieldRect s_LocalUnitRect(Vector2i::Zero(), Vector2i(UNIT_BASE_NUM - 1, UNIT_BASE_NUM - 1));
        return s_LocalUnitRect;
    }

    /* 0x0 */ SvBgName m_Name;
    /* 0x4 */ SvFgBlock* m_pBlocks[MAX_LAYER_NUM];
};
ASSERT_SIZE(BgInfo, 0xC);

// The grass of a field, 0x4000 bytes (in the town the save data SvTownDataUnk5C::mapGrass; name is ours).
struct FieldGrass {
    // sets all of it to 0
    void Clear(); // 0x0061437C

    u8 data[0x4000];
};
ASSERT_SIZE(FieldGrass, 0x4000);

// The field of a stage: blocks with their ground, heights (JmpBlock) and items (layout and the names GetFg, PlaceItem
// after libgarden field/Field.hpp and the reference symbols; the other names are ours).
class Field
{
public:
    Field();  // 0x005824D4
    ~Field(); // 0x00582500

    // the blocks and their heights
    bool InitializeBase(sead::Heap* heap, s32 width, s32 height); // 0x00581FB8
    // ... and the grass
    bool Initialize(sead::Heap* heap, s32 width, s32 height); // 0x00581B38
    // ... and the items of the layers 0..maxLayer
    bool InitializeWithFg(sead::Heap* heap, s32 width, s32 height, u32 maxLayer); // 0x00581B94
    // ... both
    bool InitializeWithFgAndGrass(sead::Heap* heap, s32 width, s32 height, u32 maxLayer); // 0x00581DD4
    void Finalize(); // 0x005822D8

    const FieldRect& GetBlockBounds() const { return m_BlockBounds; }
    const FieldRect& GetUnitBounds() const { return m_UnitBounds; }
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetBlockWidth() const { return m_BlockBounds.GetWidth(); }   // 0x00748F70
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetBlockHeight() const { return m_BlockBounds.GetHeight(); } // 0x00748FC4
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetUnitWidth() const { return m_UnitBounds.GetWidth(); }     // 0x007539AC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetUnitHeight() const { return m_UnitBounds.GetHeight(); }   // 0x007539B4

    // the block (NULL outside of the field)
    BgInfo* GetBg(s32 blockX, s32 blockZ); // 0x00316F40
    JmpBlock* GetJmpBlock(s32 blockX, s32 blockZ); // 0x00753938

    // whether a position is inside of the field
    bool IsInside(const nn::math::VEC3& wpos) const; // 0x007539BC
    bool IsInside(s32 x, s32 z) const; // 0x007539F4
    bool IsInside(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ) const; // 0x00753A2C

    // the item of a unit (NULL if there is none; the original has the const and the non-const symbol at one address)
    const SvFgName* GetFg(s32 x, s32 z, u8 layer) const; // 0x002FEF9C | libgarden [tier A]
    const SvFgName* GetFg(const nn::math::VEC3& wpos, u8 layer) const; // 0x00753A9C
    const SvFgName* GetFg(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, u8 layer) const; // 0x00753B60

    // places an item (libgarden: stage::Field::PlaceItem(const SvFgName&, math::Vector<int, 2>, u8), the vector is
    // passed as x and z); false if it could not be placed
    bool PlaceItem(const SvFgName& fg, s32 x, s32 z, u8 layer); // 0x002FCA24
    bool PlaceItem(const SvFgName& fg, const nn::math::VEC3& wpos, u8 layer); // 0x00581E3C
    bool PlaceItem(const SvFgName& fg, s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, u8 layer); // 0x00581F10

    FieldGrass* GetGrass() const; // 0x0058229C

    // the fields by FieldName (name is ours; the field code clears 30, 31 and 32..41)
    static Field* s_pFields[42];

private:
    /* 0x00 */ FieldRect m_BlockBounds;
    /* 0x10 */ FieldRect m_UnitBounds;
    /* 0x20 */ BgInfo* m_pBgs;
    /* 0x24 */ JmpBlock* m_pJmpBlocks;
    /* 0x28 */ FieldGrass* m_pGrass;  // (libgarden: m_pGrass) the grass in use
    /* 0x2C */ FieldGrass* m_pGrass2; // (libgarden: m_pGrass2) the own buffer of the grass
    /* 0x30 */ sead::CheckedArray<SvFgBlock*, MAX_LAYER_NUM> m_pFgBlocks; // the items of all blocks per layer
};
ASSERT_SIZE(Field, 0x38);
