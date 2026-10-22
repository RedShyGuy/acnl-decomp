#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"

namespace nw {
namespace gfx {
class ShaderBinaryInfo;

namespace res {
// A shader (CGFX "SHDR"; the layout is not on 3dbrew, all field names are ours). A binary shader
// holds a shader binary (3dbrew "SHBIN") and the program descriptions that pair its executables; a
// reference shader names a shader of the file. Setup builds the PICA commands of the binary and of
// each program.
struct ResShaderData
{
    u32 typeInfo;          // 0x00
    u32 signature;         // 0x04 "SHDR"
    u32 m_Unknown0x8;      // 0x08
    ut::Offset name;       // 0x0C
    u32 m_Unknown0x10[2];  // 0x10
};
ASSERT_SIZE(ResShaderData, 0x18);

// a uniform of a program used by a material, with the register it has in the program
struct ResShaderSymbolData
{
    ut::Offset name;    // 0x0
    u8 m_Unknown0x4;    // 0x4
    bool isGeometry;    // 0x5 the register is one of the geometry shader
    u8 m_Unknown0x6[2]; // 0x6
    s32 index;          // 0x8 the register (-1: not looked up yet, or not found)
};
ASSERT_SIZE(ResShaderSymbolData, 0xC);

// a program: a vertex shader and an optional geometry shader of the binary
struct ResShaderProgramDescriptionData
{
    u32 typeInfo;                 // 0x00
    const void* pVertexShader;    // 0x04 the binary shader of the vertex shader (set by Setup)
    const void* pGeometryShader;  // 0x08 the binary shader of the geometry shader (NULL: none)
    s32 vertexShaderIndex;        // 0x0C the executable of the binary
    s32 geometryShaderIndex;      // 0x10 (-1: none)
    ResArrayInfo symbols;         // 0x14 ResShaderSymbolData
    u8 m_Unknown0x1C[0x30];       // 0x1C
    s8 attributeRegisters[0x28];  // 0x4C per vertex attribute usage: the input register of the vertex shader
    u32 m_Unknown0x74;            // 0x74 (cleared by Cleanup)
    u32 m_Unknown0x78;            // 0x78
    u32* pCommand;                // 0x7C the commands of the program (memory of the allocator of the binary)
    u32 commandSize;              // 0x80 bytes
    ut::Offset owner;             // 0x84 ResBinaryShaderData
};
ASSERT_SIZE(ResShaderProgramDescriptionData, 0x88);

struct ResBinaryShaderData : public ResShaderData
{
    u32 m_Unknown0x18;              // 0x18
    ut::Offset binary;              // 0x1C the shader binary (DVLB)
    s32 shaderCount;                // 0x20 the executables of the binary
    u32 m_Unknown0x24;              // 0x24
    ResArrayInfo descriptions;      // 0x28 ResShaderProgramDescriptionData
    s32 shaderSlotCount;            // 0x30
    ut::Offset shaderSlots;         // 0x34 per executable: the binary shader once set up (const void*)
    u32* pCommonCommand;            // 0x38 the commands of the binary (ShaderBinaryInfo::BuildCommonCommand)
    u32 commonCommandSize;          // 0x3C bytes
    ShaderBinaryInfo* pBinaryInfo;  // 0x40
    os::IAllocator* pAllocator;     // 0x44 the allocator of the memory above
};
ASSERT_SIZE(ResBinaryShaderData, 0x48);

struct ResReferenceShaderData : public ResShaderData
{
    ut::Offset path;   // 0x18 the name of the shader
    ut::Offset target; // 0x1C the shader, once resolved
};
ASSERT_SIZE(ResReferenceShaderData, 0x20);

// the binary resource of a shader (the class name and the functions are from the reference symbols)
class ResShader
{
public:
    enum Type
    {
        TYPE_REFERENCE = 0x80000001,
        TYPE_BINARY = 0x80000002,
    };

    // (inline)
    explicit ResShader(const void* pData = nullptr) : m_pData(static_cast<const ResShaderData*>(pData)) {}
    const ResShaderData* ptr() const { return m_pData; }
    bool IsValid() const { return m_pData != nullptr; }
    u32 GetTypeInfo() const { return m_pData->typeInfo; }

    // the memory of the binary freed, the program registers forgotten
    void Cleanup(); // 0x001378BC | nintendogs:bytes-fuzzy [tier A]
    // the binary shader (a reference: the shader it refers to; NULL: neither)
    ResShader Dereference(); // 0x004AA6C8 | nintendogs:bytes [tier A]
    u32 Setup(os::IAllocator* pAllocator, ResGraphicsFile graphicsFile); // 0x004AA708 | nintendogs:bytes [tier B]

    const ResShaderData* m_pData; // 0x0
};
ASSERT_SIZE(ResShader, 0x4);

// a shader of the file referenced by name (the class name is from the reference symbols)
class ResReferenceShader : public ResShader
{
public:
    // (inline)
    explicit ResReferenceShader(const void* pData = nullptr) : ResShader(pData) {}
    const ResReferenceShaderData* ptr() const { return static_cast<const ResReferenceShaderData*>(m_pData); }
    ResReferenceShaderData* ptr() { return const_cast<ResReferenceShaderData*>(static_cast<const ResReferenceShaderData*>(m_pData)); }
};

// a shader with its binary (class name is ours)
class ResBinaryShader : public ResShader
{
public:
    // (inline)
    explicit ResBinaryShader(const void* pData = nullptr) : ResShader(pData) {}
    const ResBinaryShaderData* ptr() const { return static_cast<const ResBinaryShaderData*>(m_pData); }
    ResBinaryShaderData* ptr() { return const_cast<ResBinaryShaderData*>(static_cast<const ResBinaryShaderData*>(m_pData)); }
    // (inline) a program description
    ResShaderProgramDescriptionData* GetDescription(int index) const
    {
        return static_cast<ResShaderProgramDescriptionData*>(ptr()->descriptions.GetData(index));
    }
};

// the binary resource of a uniform of a program (the class name is from the reference symbols)
class ResShaderSymbol
{
public:
    // (inline)
    explicit ResShaderSymbol(const void* pData = nullptr) : m_pData(static_cast<const ResShaderSymbolData*>(pData)) {}
    const ResShaderSymbolData* ptr() const { return m_pData; }
    const char* GetName() const { return m_pData->name.To<char>(); }

    const ResShaderSymbolData* m_pData; // 0x0
};

// the binary resource of a program (the class name is from the reference symbols)
class ResShaderProgramDescription
{
public:
    // (inline)
    explicit ResShaderProgramDescription(const void* pData = nullptr)
        : m_pData(static_cast<const ResShaderProgramDescriptionData*>(pData))
    {
    }
    const ResShaderProgramDescriptionData* ptr() const { return m_pData; }
    ResShaderProgramDescriptionData* ptr() { return const_cast<ResShaderProgramDescriptionData*>(m_pData); }

    // the registers of the symbols looked up, the commands of the program built (name is ours)
    u32 Setup(os::IAllocator* pAllocator); // 0x004A97BC (name is ours)

    const ResShaderProgramDescriptionData* m_pData; // 0x0
};
ASSERT_SIZE(ResShaderProgramDescription, 0x4);

// the result bit of a symbol that is in neither shader of its program (name is ours)
const u32 RESULT_UNIFORM_NOT_FOUND = 0x10;
} // namespace res
} // namespace gfx
} // namespace nw
