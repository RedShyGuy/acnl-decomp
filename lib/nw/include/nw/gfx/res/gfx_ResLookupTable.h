#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"

namespace nw {
namespace gfx {
namespace res {
class ResGraphicsFile;

// A lookup table of the fragment lighting or the fog: an image (the PICA commands that load it) or
// a reference by name to one in a LUTS of the file (class names from the reference symbols, the
// rest is ours).
struct ResLookupTableData
{
    u32 typeInfo; // 0x0
};

struct ResImageLookupTableData : public ResLookupTableData
{
    u32 m_Unknown0x4[2];  // 0x4
    u32 commandSize;      // 0xC bytes
    ut::Offset commands;  // 0x10
};
ASSERT_OFFSET(ResImageLookupTableData, commands, 0x10);

struct ResReferenceLookupTableData : public ResLookupTableData
{
    ut::Offset path;      // 0x4 the name of the LUTS
    ut::Offset tableName; // 0x8 the name of the table in it
    ut::Offset target;    // 0xC the table, once resolved
};
ASSERT_SIZE(ResReferenceLookupTableData, 0x10);

// the binary resource of a lookup table
class ResLookupTable
{
public:
    enum Type
    {
        TYPE_IMAGE = 0x80000000,
        TYPE_REFERENCE = 0x40000000,
    };

    // (inline)
    explicit ResLookupTable(const void* pData = nullptr) : m_pData(static_cast<const ResLookupTableData*>(pData)) {}
    const ResLookupTableData* ptr() const { return m_pData; }
    bool IsValid() const { return m_pData != nullptr; }

    void Cleanup(); // 0x0013F108 | nintendogs:callgraph [tier A]
    u32 Setup(); // 0x004A6B98 | nintendogs:bytes [tier A]
    // the image of the table (a reference: its target; NULL: none)
    const ResImageLookupTableData* Dereference() const; // 0x004A6B5C | nintendogs:callseq

    const ResLookupTableData* m_pData; // 0x0
};
ASSERT_SIZE(ResLookupTable, 0x4);

class ResImageLookupTable : public ResLookupTable
{
public:
    // (inline)
    explicit ResImageLookupTable(const void* pData = nullptr) : ResLookupTable(pData) {}
    const ResImageLookupTableData* ptr() const { return static_cast<const ResImageLookupTableData*>(m_pData); }
};

class ResReferenceLookupTable : public ResLookupTable
{
public:
    // (inline)
    explicit ResReferenceLookupTable(const void* pData = nullptr) : ResLookupTable(pData) {}
    const ResReferenceLookupTableData* ptr() const { return static_cast<const ResReferenceLookupTableData*>(m_pData); }
    ResReferenceLookupTableData* ptr() { return const_cast<ResReferenceLookupTableData*>(static_cast<const ResReferenceLookupTableData*>(m_pData)); }
};

// a lookup table of the fragment lighting with its input settings
struct ResLightingLookupTableData
{
    u32 m_Unknown0x0[2];   // 0x0
    ut::Offset sampler;    // 0x8 the ResLookupTableData
};
ASSERT_OFFSET(ResLightingLookupTableData, sampler, 0x8);

class ResLightingLookupTable
{
public:
    // (inline)
    explicit ResLightingLookupTable(const void* pData = nullptr)
        : m_pData(static_cast<const ResLightingLookupTableData*>(pData))
    {
    }
    const ResLightingLookupTableData* ptr() const { return m_pData; }
    bool IsValid() const { return m_pData != nullptr; }
    ResLookupTable GetSampler() const { return ResLookupTable(m_pData->sampler.ToPtr()); }

    const ResLightingLookupTableData* m_pData; // 0x0
};
ASSERT_SIZE(ResLightingLookupTable, 0x4);

// the cleanup of an image table: it is no longer active in the GraphicsDevice (in its file; name is ours)
void CleanupImageLookupTable(ResLookupTableData* pData);

// a reference to a lookup table resolved by name (RESULT_LUT_NOT_FOUND: not in the file)
u32 SetupReferenceLut(ResReferenceLookupTable lut, ResGraphicsFile graphicsFile); // 0x004A7D9C | nintendogs:bytes [tier A]
ReferenceTarget<ResLookupTable> GetReferenceLutTarget(ResReferenceLookupTable lut, ResGraphicsFile graphicsFile); // 0x004A811C | nintendogs:bytes-fuzzy [tier A]
} // namespace res
} // namespace gfx
} // namespace nw
