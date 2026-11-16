#include "field/dFieldBuilderUtil.h"
#include "field/dJmpBlock.h"
#include "Other/dSaveProcess.h"
#include "sead/seadSafeStringBase.h"
#include "Sv/dSvFgName.h"
#include "Sv/dSvShopZakkaExtern.h"
#include "field/field_Extern.h"
#include <cstring>

// 0x0012D3C4 (name is ours)
FgDataLoader::FgDataLoader()
{
    // (only the members are constructed)
}

// 0x00201028 (name is ours)
bool FgDataLoader::Load(const char* path, sead::Heap* heap)
{
    if (path == NULL || heap == NULL) {
        return true;
    }
    return m_File.Read(sead::SafeStringBase<char>(path), heap, 0);
}

// 0x0020106C (name is ours)
void FgDataLoader::Unload()
{
    m_File.Reset();
}

// 0x00201070 (name is ours)
u32 FgDataLoader::GetItems(FgDataItem* out, u32 size, u32 bg) const
{
    if (m_File.m_pData == NULL) {
        return 0;
    }
    const u8* data = static_cast<const u8*>(m_File.m_pData);
    const u32 num = data[bg + 2];
    const u32 start = *reinterpret_cast<const u16*>(data) + 2;
    u32 index = 0;
    for (u32 i = 0; i < bg; i++) {
        index += data[i + 2];
    }
    if (size < num * sizeof(FgDataItem)) {
        return 0;
    }
    std::memcpy(out, data + start + index * sizeof(FgDataItem), num * sizeof(FgDataItem));
    return num;
}

// 0x00201120 (name is ours)
FgDataLoader::~FgDataLoader()
{
    m_File.Reset();
}

// 0x00201158 (name is ours)
void BlockItemsSetter::operator()(const Vector2i& block)
{
    if (m_pField == NULL) {
        return;
    }
    BgInfo* bg = m_pField->GetBg(block.x, block.y);
    if (!m_pLayout->IsLoaded()) {
        return;
    }
    const u32 num = m_pLayout->GetItems(m_Items, sizeof(m_Items), bg->GetName());
    for (u32 i = 0; i < num; i++) {
        const FgDataItem& entry = m_Items[i];
        SvFgName item(static_cast<SvFgName::ID>(entry.id));
        if (Func30B658(&item)) {
            Func311860(&item, SvFgName(static_cast<SvFgName::ID>(entry.id)), entry.flags >> 14);
        }
        bg->SetFg(item, entry.x, entry.z, 0);
    }
}

// 0x0020125C (name is ours)
void BlockHeightsSetter::operator()(const Vector2i& block)
{
    if (m_pField == NULL) {
        return;
    }
    BgInfo* bg = m_pField->GetBg(block.x, block.y);
    JmpBlock* jmpBlock = m_pField->GetJmpBlock(block.x, block.y);
    if (jmpBlock == NULL) {
        return;
    }
    const JmpUnit* units = Func1C2588(Func3044DC(), bg->GetName());
    if (units == NULL) {
        return;
    }
    jmpBlock->SetUnits(units, JmpBlock::DATA_SIZE);
}

// 0x0020245C (name is ours)
void BlockItemsCleaner::operator()(const Vector2i& block)
{
    if (m_pField == NULL) {
        return;
    }
    BgInfo* bg = m_pField->GetBg(block.x, block.y);
    if (bg == NULL) {
        return;
    }
    const FieldRect& units = BgInfo::GetLocalUnitRect();
    for (FieldRect::iterator it = units.begin(); it != units.end(); ++it) {
        bg->SetFg(SvFgName::s_Empty, (*it).x, (*it).y, 0);
        bg->SetFg(SvFgName::s_Empty, (*it).x, (*it).y, 1);
    }
}

// 0x00811C98 (name is ours)
template <>
sead::FixedObjArray<Vector2i, 50>::FixedObjArray()
{
    this->mFreeList.init(mWork, sizeof(Vector2i), 50);
    this->setBuffer(50, mPtrWork);
}

// 0x0080E4CC
template BlockItemsSetter ForEachBlock<BlockItemsSetter>(const FieldRect& rect, BlockItemsSetter function);
// 0x0080E69C
template BlockHeightsSetter ForEachBlock<BlockHeightsSetter>(const FieldRect& rect, BlockHeightsSetter function);
// 0x0080E78C
template BlockItemsCleaner ForEachBlock<BlockItemsCleaner>(const FieldRect& rect, BlockItemsCleaner function);
