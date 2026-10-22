#pragma once

#include "decomp.h"

namespace nw {
namespace gfx {
// What gfx needs of a shader binary (3dbrew "SHBIN": the DVLB with its DVLE and the DVLP) to build
// the PICA commands of its programs. The class and function names are from the reference symbols,
// the members are ours.
class ShaderBinaryInfo
{
public:
    class SafeBuffer;

    enum
    {
        SHADER_MAX = 32,
        SWIZZLE_MAX = 128,
    };

    // where a uniform is (names are ours)
    struct UniformLocation
    {
        // the kind of register
        enum Type : u8
        {
            TYPE_NONE = 0,
            TYPE_INPUT = 1,     // 0-15
            TYPE_FLOAT = 2,     // c0-c95 (16-111)
            TYPE_INTEGER = 3,   // i0-i3 (112-119)
            TYPE_BOOL = 4,      // b0-b15 (120-135)
        };

        s32 index; // 0x0 the register of the type (-1: none)
        Type type; // 0x4
    };

    // (inline)
    explicit ShaderBinaryInfo(const void* pBinary)
        : m_pBinary(pBinary), m_ShaderCount(0), m_pProgramCode(nullptr), m_ProgramSize(0), m_SwizzleCount(0), m_GeometryShaderCount(0)
    {
        for (int i = 0; i < SHADER_MAX; ++i) {
            m_pShaders[i] = nullptr;
        }
    }

    // the shaders, the program code and the swizzle patterns of the binary
    void AnalyzeBinary(); // 0x0049C448 | nintendogs:bytes [tier A]
    // commands that load count words to the register, at most 256 per command
    void PutLoadCommand(SafeBuffer& buffer, u32 reg, const u32* pData, u32 count) const; // 0x00739BC8 | nintendogs:bytes-fuzzy [tier A]
    // the commands of the binary (geometry mode, swizzles, program); returns their size in bytes
    u32 BuildCommonCommand(u32* pBuffer, u32 bufferSize) const; // 0x00739CD4 | nintendogs:bytes [tier B]
    UniformLocation SearchUniformIndex(int shaderIdx, const char* name) const; // 0x00739DD8 | mk7dlp:bytes [tier B]
    void BuildOutAttrCommand(SafeBuffer& buffer, int vertexShaderIdx, int geometryShaderIdx) const; // 0x00739ECC | nintendogs:bytes-fuzzy [tier A]
    void BuildProgramCommand(SafeBuffer& buffer) const; // 0x0073A2E8 | nintendogs:bytes [tier A]
    void BuildConstRegCommand(SafeBuffer& buffer, int shaderIdx) const; // 0x0073A3E4 | nintendogs:bytes [tier A]
    // the commands of a program (constants, output attributes); returns their size in bytes
    u32 BuildShaderProgramCommand(int vertexShaderIdx, int geometryShaderIdx, u32* pBuffer, u32 bufferSize) const; // 0x0073A684 | nintendogs:bytes [tier A]
    u32 GetShaderProgramCommandSize(int vertexShaderIdx, int geometryShaderIdx) const; // 0x0073A730 | nintendogs:bytes [tier A]
    // the size of BuildCommonCommand's commands (name is ours)
    u32 GetCommonCommandSize() const; // 0x0073A568 (name is ours)

    const void* m_pBinary;              // 0x000 the DVLB
    u32 m_ShaderCount;                  // 0x004
    const void* m_pShaders[SHADER_MAX]; // 0x008 the DVLE
    const u32* m_pProgramCode;          // 0x088
    u32 m_ProgramSize;                  // 0x08C words
    u32 m_Swizzles[SWIZZLE_MAX];        // 0x090 the operand descriptors
    u32 m_SwizzleCount;                 // 0x290
    s32 m_GeometryShaderCount;          // 0x294
};
ASSERT_SIZE(ShaderBinaryInfo, 0x298);
} // namespace gfx
} // namespace nw
