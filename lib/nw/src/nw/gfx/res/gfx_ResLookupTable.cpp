#include "nw/gfx/res/gfx_ResLookupTable.h"

#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResLookupTableSet.h"

namespace nw {
namespace gfx {
namespace res {
namespace {
// the functions per type of lookup table (image, reference; names are ours)
typedef u32 (*SetupFunc)(ResLookupTableData* pData);
typedef void (*CleanupFunc)(ResLookupTableData* pData);

// 0x004A7C54 (name is ours)
u32 SetupImageLookupTable(ResLookupTableData* /*pData*/)
{
    return RESULT_SUCCESS;
}

// 0x004A8574 (name is ours)
u32 SetupReferenceLookupTable(ResLookupTableData* /*pData*/)
{
    // (resolved by SetupReferenceLut)
    return RESULT_SUCCESS;
}

// 0x004A8740 (name is ours)
void CleanupReferenceLookupTable(ResLookupTableData* pData)
{
    static_cast<ResReferenceLookupTableData*>(pData)->target.m_Offset = 0;
}
} // namespace

// 0x009760FC (name is ours)
SetupFunc s_LookupTableSetupFuncs[] = {SetupImageLookupTable, SetupReferenceLookupTable};
// 0x00976104 (name is ours)
CleanupFunc s_LookupTableCleanupFuncs[] = {CleanupImageLookupTable, CleanupReferenceLookupTable};

// 0x0013F108 | nintendogs:callgraph [tier A]
void nw::gfx::res::ResLookupTable::Cleanup()
{
    ResLookupTableData* pData = const_cast<ResLookupTableData*>(m_pData);
    switch (pData->typeInfo) {
    case TYPE_IMAGE:
        s_LookupTableCleanupFuncs[0](pData);
        break;
    case TYPE_REFERENCE:
        s_LookupTableCleanupFuncs[1](pData);
        break;
    default:
        break;
    }
}

// 0x004A6B5C | nintendogs:callseq
const nw::gfx::res::ResImageLookupTableData* nw::gfx::res::ResLookupTable::Dereference() const
{
    switch (m_pData->typeInfo) {
    case TYPE_IMAGE:
        return static_cast<const ResImageLookupTableData*>(m_pData);
    case TYPE_REFERENCE:
        return static_cast<const ResReferenceLookupTableData*>(m_pData)->target.To<ResImageLookupTableData>();
    default:
        return nullptr;
    }
}

// 0x004A6B98 | nintendogs:bytes [tier A]
u32 nw::gfx::res::ResLookupTable::Setup()
{
    u32 result = RESULT_SUCCESS;
    ResLookupTableData* pData = const_cast<ResLookupTableData*>(m_pData);
    switch (pData->typeInfo) {
    case TYPE_IMAGE:
        result |= s_LookupTableSetupFuncs[0](pData);
        break;
    case TYPE_REFERENCE:
        result |= s_LookupTableSetupFuncs[1](pData);
        break;
    default:
        break;
    }
    return result;
}

// 0x004A7D9C | nintendogs:bytes [tier A]
u32 SetupReferenceLut(ResReferenceLookupTable lut, ResGraphicsFile graphicsFile)
{
    u32 result = RESULT_SUCCESS;
    if (lut.IsValid()) {
        // not resolved yet
        if (DynamicCastData<ResLookupTableData>(lut.ptr()->target.ToPtr(), ResLookupTable::TYPE_IMAGE) == nullptr) {
            ReferenceTarget<ResLookupTable> target = GetReferenceLutTarget(lut, graphicsFile);
            if (!target.isFound) {
                result |= RESULT_LUT_NOT_FOUND;
            }
        }
    }
    return result;
}

// 0x004A811C | nintendogs:bytes-fuzzy [tier A]
ReferenceTarget<ResLookupTable> GetReferenceLutTarget(ResReferenceLookupTable lut, ResGraphicsFile graphicsFile)
{
    bool isFound = false;
    const char* pPath = lut.ptr()->path.To<char>();
    const char* pTableName = lut.ptr()->tableName.To<char>();
    ResLookupTableSet tableSet(graphicsFile.ptr()->luts.GetData(pPath));
    if (tableSet.IsValid()) {
        const void* pTable = DynamicCastData<ResLookupTableData>(tableSet.GetTable(pTableName).ptr(), ResLookupTable::TYPE_IMAGE);
        if (pTable != nullptr) {
            lut.ptr()->target.Set(pTable);
            isFound = true;
        }
    }
    ReferenceTarget<ResLookupTable> target;
    target.target = ResLookupTable(lut.ptr()->target.ToPtr());
    target.isFound = isFound;
    return target;
}

} // namespace res
} // namespace gfx
} // namespace nw
