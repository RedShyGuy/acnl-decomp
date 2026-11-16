#include "field/dField.h"

#include "field/dFdInfo.h"
#include "field/dJmpBlock.h"
#include "sead/seadNew.h"

// 0x009B5A24
Field* Field::s_pFields[42];

// 0x005824D4 (name is ours)
Field::Field() : m_pBgs(NULL), m_pJmpBlocks(NULL), m_pGrass(NULL), m_pGrass2(NULL)
{
}

// 0x00582500 (name is ours)
Field::~Field()
{
    // (only the members are destroyed)
}

// 0x00581FB8 (name is ours)
bool Field::InitializeBase(sead::Heap* heap, s32 width, s32 height)
{
    m_BlockBounds = FieldRect(Vector2i::Zero(), Vector2i(width - 1, height - 1));
    m_UnitBounds = FieldRect(Vector2i::Zero(), Vector2i(width * UNIT_BASE_NUM - 1, height * UNIT_BASE_NUM - 1));

    s32 num = width * height;
    BgInfo* bgs = new (heap, 4) BgInfo[num];
    if (bgs == NULL) {
        return false;
    }
    FieldRect::iterator end = m_BlockBounds.end();
    for (FieldRect::iterator it = m_BlockBounds.begin(); it != end; ++it) {
        const Vector2i& pos = *it;
        if (!bgs[m_BlockBounds.ToIndex(pos.x, pos.y)].Initialize(pos.x, pos.y)) {
            Finalize();
            return false;
        }
    }

    JmpBlock* jmpBlocks = new (heap, 4) JmpBlock[num];
    if (jmpBlocks == NULL) {
        return false;
    }
    for (FieldRect::iterator it = m_BlockBounds.begin(); it != end; ++it) {
        const Vector2i& pos = *it;
        if (!jmpBlocks[m_BlockBounds.ToIndex(pos.x, pos.y)].Initialize(pos.x, pos.y)) {
            Finalize();
            return false;
        }
    }

    m_pFgBlocks.mBuffer[0] = NULL;
    m_pBgs = bgs;
    m_pJmpBlocks = jmpBlocks;
    m_pFgBlocks.mBuffer[1] = NULL;
    return true;
}

// 0x00581B38 (name is ours)
bool Field::Initialize(sead::Heap* heap, s32 width, s32 height)
{
    if (InitializeBase(heap, width, height)) {
        m_pGrass2 = new (heap, 128) FieldGrass;
        if (m_pGrass2 != NULL) {
            m_pGrass2->Clear();
            m_pGrass = m_pGrass2;
            return true;
        }
    }
    Finalize();
    return false;
}

// 0x00581B94 (name is ours)
bool Field::InitializeWithFg(sead::Heap* heap, s32 width, s32 height, u32 maxLayer)
{
    if (!InitializeBase(heap, width, height)) {
        Finalize();
        return false;
    }

    s32 num = width * height;
    FieldRect::iterator end = m_BlockBounds.end();
    for (u32 i = 0; i < maxLayer + 1; i++) {
        m_pFgBlocks[i] = new (heap, 4) SvFgBlock[num];
        if (m_pFgBlocks[i] == NULL) {
            return false;
        }
        for (FieldRect::iterator it = m_BlockBounds.begin(); it != end; ++it) {
            const Vector2i& pos = *it;
            m_pFgBlocks[i][m_BlockBounds.ToIndex(pos.x, pos.y)].Clear();
        }
    }

    SvFgBlock* layer0 = NULL;
    SvFgBlock* layer1 = NULL;
    for (FieldRect::iterator it = m_BlockBounds.begin(); it != end; ++it) {
        const Vector2i& pos = *it;
        s32 index = m_BlockBounds.ToIndex(pos.x, pos.y);
        if (m_pFgBlocks.mBuffer[0] != NULL) {
            layer0 = &m_pFgBlocks.mBuffer[0][index];
        }
        if (m_pFgBlocks.mBuffer[1] != NULL) {
            layer1 = &m_pFgBlocks.mBuffer[1][index];
        }
        m_pBgs[index].SetBlocks(layer0, layer1);
    }
    return true;
}

// 0x00581DD4 (name is ours)
bool Field::InitializeWithFgAndGrass(sead::Heap* heap, s32 width, s32 height, u32 maxLayer)
{
    if (InitializeWithFg(heap, width, height, maxLayer)) {
        m_pGrass2 = new (heap, 128) FieldGrass;
        if (m_pGrass2 != NULL) {
            m_pGrass2->Clear();
            m_pGrass = m_pGrass2;
            return true;
        }
    }
    Finalize();
    return false;
}

// 0x005822D8 (name is ours)
void Field::Finalize()
{
    FieldRect::iterator end = m_BlockBounds.end();
    if (m_pBgs != NULL) {
        for (FieldRect::iterator it = m_BlockBounds.begin(); it != end; ++it) {
            const Vector2i& pos = *it;
            m_pBgs[m_BlockBounds.ToIndex(pos.x, pos.y)].Finalize();
        }
        delete[] m_pBgs;
        m_pBgs = NULL;
    }
    if (m_pJmpBlocks != NULL) {
        for (FieldRect::iterator it = m_BlockBounds.begin(); it != end; ++it) {
            const Vector2i& pos = *it;
            m_pJmpBlocks[m_BlockBounds.ToIndex(pos.x, pos.y)].Finalize();
        }
        delete[] m_pJmpBlocks;
        m_pJmpBlocks = NULL;
    }
    for (s32 i = 0; i < static_cast<s32>(MAX_LAYER_NUM); i++) {
        if (m_pFgBlocks[i] != NULL) {
            delete[] m_pFgBlocks[i];
            m_pFgBlocks[i] = NULL;
        }
    }
    if (m_pGrass2 != NULL) {
        delete m_pGrass2;
        m_pGrass2 = NULL;
    }
    m_pGrass = NULL;
}

// 0x0058229C (name is ours)
FieldGrass* Field::GetGrass() const
{
    return m_pGrass;
}

// 0x00316F40 (name is ours)
BgInfo* Field::GetBg(s32 blockX, s32 blockZ)
{
    Vector2i pos(blockX, blockZ);
    if (m_BlockBounds.Contains(pos) && m_pBgs != NULL) {
        return &m_pBgs[m_BlockBounds.ToIndex(blockX, blockZ)];
    }
    return NULL;
}

// 0x00753938 (name is ours)
JmpBlock* Field::GetJmpBlock(s32 blockX, s32 blockZ)
{
    Vector2i pos(blockX, blockZ);
    if (m_BlockBounds.Contains(pos) && m_pJmpBlocks != NULL) {
        return &m_pJmpBlocks[m_BlockBounds.ToIndex(blockX, blockZ)];
    }
    return NULL;
}

// 0x007539BC (name is ours)
bool Field::IsInside(const nn::math::VEC3& wpos) const
{
    Vector2i unit = FdInfo::ToUnit(wpos);
    return m_UnitBounds.Contains(unit);
}

// 0x007539F4 (name is ours)
bool Field::IsInside(s32 x, s32 z) const
{
    Vector2i unit(x, z);
    return m_UnitBounds.Contains(unit);
}

// 0x00753A2C (name is ours)
bool Field::IsInside(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ) const
{
    Vector2i unit = FdInfo::ToUnit(Vector2i(blockX, blockZ), Vector2i(unitX, unitZ));
    return m_UnitBounds.Contains(unit);
}

// 0x002FEF9C | libgarden [tier A]
const SvFgName* Field::GetFg(s32 x, s32 z, u8 layer) const
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    FdInfo::ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, x, z);
    const BgInfo* bg = const_cast<Field*>(this)->GetBg(blockX, blockZ);
    if (bg == NULL) {
        return NULL;
    }
    return bg->GetFg(unitX, unitZ, layer);
}

// 0x00753A9C (name is ours)
const SvFgName* Field::GetFg(const nn::math::VEC3& wpos, u8 layer) const
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    FdInfo::ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, wpos);
    const BgInfo* bg = const_cast<Field*>(this)->GetBg(blockX, blockZ);
    if (bg == NULL) {
        return NULL;
    }
    return bg->GetFg(unitX, unitZ, layer);
}

// 0x00753B60 (name is ours)
const SvFgName* Field::GetFg(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, u8 layer) const
{
    const BgInfo* bg = const_cast<Field*>(this)->GetBg(blockX, blockZ);
    if (bg == NULL) {
        return NULL;
    }
    return bg->GetFg(unitX, unitZ, layer);
}

// 0x002FCA24 | libgarden [tier A]
bool Field::PlaceItem(const SvFgName& fg, s32 x, s32 z, u8 layer)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    FdInfo::ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, x, z);
    BgInfo* bg = GetBg(blockX, blockZ);
    if (bg == NULL) {
        return false;
    }
    return bg->SetFg(fg, unitX, unitZ, layer);
}

// 0x00581E3C (name is ours)
bool Field::PlaceItem(const SvFgName& fg, const nn::math::VEC3& wpos, u8 layer)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    FdInfo::ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, wpos);
    BgInfo* bg = GetBg(blockX, blockZ);
    if (bg == NULL) {
        return false;
    }
    return bg->SetFg(fg, unitX, unitZ, layer);
}

// 0x00581F10 (name is ours)
bool Field::PlaceItem(const SvFgName& fg, s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, u8 layer)
{
    BgInfo* bg = GetBg(blockX, blockZ);
    if (bg == NULL) {
        return false;
    }
    return bg->SetFg(fg, unitX, unitZ, layer);
}
