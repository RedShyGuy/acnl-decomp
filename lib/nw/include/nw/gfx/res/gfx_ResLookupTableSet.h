#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/gfx/res/gfx_ResLookupTable.h"

namespace nw {
namespace gfx {
namespace res {
class ResGraphicsFile;

// A set of lookup tables (3dbrew "LUTS"; names are ours).
struct ResLookupTableSetData
{
    u32 m_Unknown0x0[6]; // 0x0
    ResDicInfo tables;   // 0x18
};
ASSERT_OFFSET(ResLookupTableSetData, tables, 0x18);

// the binary resource of a lookup table set (the class name is from the reference symbols)
class ResLookupTableSet
{
public:
    // (inline)
    explicit ResLookupTableSet(const void* pData = nullptr) : m_pData(static_cast<const ResLookupTableSetData*>(pData)) {}
    const ResLookupTableSetData* ptr() const { return m_pData; }
    bool IsValid() const { return m_pData != nullptr; }
    s32 GetTableCount() const { return m_pData->tables.count; }
    ResLookupTable GetTable(int index) const { return ResLookupTable(m_pData->tables.GetData(index)); }
    ResLookupTable GetTable(const char* name) const { return ResLookupTable(m_pData->tables.GetData(name)); }

    void Cleanup(); // 0x00137638 | nintendogs:bytes [tier A]
    u32 Setup(os::IAllocator* pAllocator, ResGraphicsFile graphicsFile); // 0x004A7C5C | nintendogs:bytes [tier A]

    const ResLookupTableSetData* m_pData; // 0x0
};
ASSERT_SIZE(ResLookupTableSet, 0x4);
} // namespace res
} // namespace gfx
} // namespace nw
