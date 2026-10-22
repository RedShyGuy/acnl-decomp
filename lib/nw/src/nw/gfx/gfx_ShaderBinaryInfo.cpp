#include <cstring>

#include "nn/gx/detail/gx_Shader.h"
#include "nw/gfx/gfx_ShaderBinaryInfo.h"
#include "nw/gfx/gfx_ShaderBinaryInfo_SafeBuffer.h"

namespace nw {
namespace gfx {
namespace {
using nn::gx::detail::DvlbHeader;
using nn::gx::detail::DvleConstant;
using nn::gx::detail::DvleHeader;
using nn::gx::detail::DvleOutput;
using nn::gx::detail::DvleUniform;
using nn::gx::detail::DvlpHeader;

// a PICA command header (3dbrew "GPU/Internal Registers"): register, byte mask, extra parameters,
// consecutive registers (name is ours)
constexpr u32 Header(u32 reg, u32 mask = 0xF, u32 extraCount = 0, bool isConsecutive = false)
{
    return reg | (mask << 16) | (extraCount << 20) | (isConsecutive ? 0x80000000 : 0);
}

// the registers (names are ours, after 3dbrew)
const u32 REG_SH_OUTMAP_TOTAL = 0x04F;
const u32 REG_SH_OUTMAP_O0 = 0x050;
const u32 REG_SH_OUTATTR_MODE = 0x064;
const u32 REG_SH_OUTATTR_CLOCK = 0x06F;
const u32 REG_GEOSTAGE_CONFIG = 0x229;
const u32 REG_VSH_COM_MODE = 0x244;
const u32 REG_VSH_OUTMAP_TOTAL1 = 0x24A;
const u32 REG_VSH_OUTMAP_TOTAL2 = 0x251;
const u32 REG_GSH_MISC0 = 0x252;
const u32 REG_GSH_MISC1 = 0x254;
const u32 REG_PRIMITIVE_CONFIG = 0x25E;
const u32 REG_GSH_INPUTBUFFER_CONFIG = 0x289;
const u32 REG_GSH_ENTRYPOINT = 0x28A;
const u32 REG_GSH_OUTMAP_MASK = 0x28D;
const u32 REG_GSH_CODETRANSFER_END = 0x28F;
const u32 REG_GSH_FLOATUNIFORM_INDEX = 0x290;
const u32 REG_GSH_CODETRANSFER_INDEX = 0x29B;
const u32 REG_GSH_CODETRANSFER_DATA = 0x29C;
const u32 REG_GSH_OPDESCS_INDEX = 0x2A5;
const u32 REG_GSH_OPDESCS_DATA = 0x2A6;
const u32 REG_VSH_ENTRYPOINT = 0x2BA;
const u32 REG_VSH_OUTMAP_MASK = 0x2BD;
const u32 REG_VSH_CODETRANSFER_END = 0x2BF;
const u32 REG_VSH_FLOATUNIFORM_INDEX = 0x2C0;
const u32 REG_VSH_CODETRANSFER_INDEX = 0x2CB;
const u32 REG_VSH_CODETRANSFER_DATA = 0x2CC;
const u32 REG_VSH_OPDESCS_INDEX = 0x2D5;
const u32 REG_VSH_OPDESCS_DATA = 0x2D6;
// the integer uniforms are 15 registers below the float uniform index
const u32 INTUNIFORM_DISTANCE = REG_VSH_FLOATUNIFORM_INDEX - 0x2B1;

// the words of one load command, the code that fits the vertex shader
const u32 LOAD_COMMAND_MAX = 256;
const u32 VERTEX_PROGRAM_MAX = 512;

// the output types of a DVLE (3dbrew "SHBIN"; names are ours)
enum OutputType
{
    OUTPUT_POSITION = 0,
    OUTPUT_NORMAL_QUATERNION = 1,
    OUTPUT_COLOR = 2,
    OUTPUT_TEXCOORD0 = 3,
    OUTPUT_TEXCOORD0_W = 4,
    OUTPUT_TEXCOORD1 = 5,
    OUTPUT_TEXCOORD2 = 6,
    OUTPUT_VIEW = 8,
};

// the semantics of SH_OUTMAP_O* (3dbrew "GPU/Internal Registers"; names are ours)
enum OutputAttribute
{
    OUTATTR_POSITION_X = 0x00,
    OUTATTR_QUATERNION_X = 0x04,
    OUTATTR_COLOR_R = 0x08,
    OUTATTR_TEXCOORD0_U = 0x0C,
    OUTATTR_TEXCOORD1_U = 0x0E,
    OUTATTR_TEXCOORD0_W = 0x10,
    OUTATTR_VIEW_X = 0x12,
    OUTATTR_TEXCOORD2_U = 0x16,
    OUTATTR_UNUSED = 0x1F,
};

// the uniform registers of a DVLE (3dbrew "SHBIN")
const u16 UNIFORM_FLOAT_FIRST = 0x10;
const u16 UNIFORM_INTEGER_FIRST = 0x70;
const u16 UNIFORM_BOOL_FIRST = 0x78;
const u16 UNIFORM_END = 0x88;

// the constant types of a DVLE (3dbrew "SHBIN")
const u16 CONSTANT_INTEGER = 1;
const u16 CONSTANT_FLOAT = 2;

// the geometry shader types of a DVLE (3dbrew "SHBIN")
const u8 GEOMETRY_VARIABLE_PRIMITIVE = 1;
const u8 GEOMETRY_FIXED_PRIMITIVE = 2;

// (inline) a value at an offset from the start of a header
template <typename T>
inline const T* AddOffset(const void* pBase, u32 offset)
{
    return reinterpret_cast<const T*>(static_cast<const u8*>(pBase) + offset);
}

// (inline) the number of output registers of a shader and their mask (name is ours)
inline void GetOutputRegisters(const DvleHeader* pShader, int* pCount, u32* pMask)
{
    const u32 mask = pShader->m_OutputMask;
    int count = 0;
    for (int i = 0; i < 16; ++i) {
        if (mask & (1 << i)) {
            ++count;
        }
    }
    if (pCount != nullptr) {
        *pCount = count;
    }
    if (pMask != nullptr) {
        *pMask = mask;
    }
}

// (inline) the size of the commands of PutLoadCommand (name is ours)
inline u32 GetLoadCommandSize(u32 count)
{
    u32 size = 0;
    do {
        const u32 n = count > LOAD_COMMAND_MAX ? LOAD_COMMAND_MAX : count;
        size += 2 * sizeof(u32) + (n - 1) * sizeof(u32);
        if ((n & 1) == 0) {
            size += sizeof(u32);
        }
        count -= n;
    } while (count != 0);
    return size;
}

// (inline) the size of the commands of BuildConstRegCommand (name is ours)
inline u32 GetConstRegCommandSize(const DvleHeader* pShader)
{
    const DvleConstant* pConstants = AddOffset<DvleConstant>(pShader, pShader->m_ConstantOffset);
    u32 size = 0;
    for (u32 i = 0; i < pShader->m_ConstantCount; ++i) {
        switch (pConstants[i].m_Type) {
        case CONSTANT_INTEGER:
            size += 2 * sizeof(u32);
            break;
        case CONSTANT_FLOAT:
            size += 6 * sizeof(u32);
            break;
        default:
            break;
        }
    }
    return size;
}
} // namespace

// 0x0049C448 | nintendogs:bytes [tier A]
void nw::gfx::ShaderBinaryInfo::AnalyzeBinary()
{
    const DvlbHeader* pDvlb = static_cast<const DvlbHeader*>(m_pBinary);
    m_ShaderCount = pDvlb->m_DvleCount;
    const u32* pOffset = pDvlb->m_DvleOffsets;
    for (u32 i = 0; i < m_ShaderCount; ++i) {
        const DvleHeader* pShader = AddOffset<DvleHeader>(m_pBinary, *pOffset);
        m_pShaders[i] = pShader;
        if (pShader->m_ShaderType != 0) {
            ++m_GeometryShaderCount;
        }
        ++pOffset;
    }

    // the DVLP follows the offsets of the DVLE
    const DvlpHeader* pDvlp = reinterpret_cast<const DvlpHeader*>(pOffset);
    m_pProgramCode = AddOffset<u32>(pDvlp, pDvlp->m_CodeOffset);
    m_ProgramSize = pDvlp->m_CodeSize;
    const u32* pDescriptor = AddOffset<u32>(pDvlp, pDvlp->m_OperandDescriptorOffset);
    m_SwizzleCount = pDvlp->m_OperandDescriptorCount;
    for (u32 i = 0; i < m_SwizzleCount; ++i) {
        // two words per descriptor, the second is unused
        m_Swizzles[i] = *pDescriptor;
        pDescriptor += 2;
    }
}

// 0x00739BC8 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::ShaderBinaryInfo::PutLoadCommand(SafeBuffer& buffer, u32 reg, const u32* pData, u32 count) const
{
    u32 i = 0;
    do {
        const u32 n = count > LOAD_COMMAND_MAX ? LOAD_COMMAND_MAX : count;

        // the first word goes with the header, the others follow
        const u32 command[] = { pData[i], Header(static_cast<u16>(reg), 0xF, (n - 1) & 0xFF) };
        buffer.Write(command, sizeof(command));
        buffer.Write(&pData[i + 1], (n - 1) * sizeof(u32));
        if ((n & 1) == 0) {
            // commands are 8-byte aligned
            const u32 padding = 0;
            buffer.Write(&padding, sizeof(padding));
        }

        count -= n;
        i += n;
    } while (count != 0);
}

// 0x00739CD4 | nintendogs:bytes [tier B]
u32 nw::gfx::ShaderBinaryInfo::BuildCommonCommand(u32* pBuffer, u32 bufferSize) const
{
    SafeBuffer buffer(pBuffer, bufferSize);

    const u32 modeCommand[] = { m_GeometryShaderCount > 0 ? 1u : 0u, Header(REG_VSH_COM_MODE, 0x1) };
    buffer.Write(modeCommand, sizeof(modeCommand));

    const u32 vertexSwizzleCommand[] = { 0, Header(REG_VSH_OPDESCS_INDEX) };
    buffer.Write(vertexSwizzleCommand, sizeof(vertexSwizzleCommand));
    PutLoadCommand(buffer, REG_VSH_OPDESCS_DATA, m_Swizzles, m_SwizzleCount);

    if (m_GeometryShaderCount > 0) {
        const u32 geometrySwizzleCommand[] = { 0, Header(REG_GSH_OPDESCS_INDEX) };
        buffer.Write(geometrySwizzleCommand, sizeof(geometrySwizzleCommand));
        PutLoadCommand(buffer, REG_GSH_OPDESCS_DATA, m_Swizzles, m_SwizzleCount);
    }

    BuildProgramCommand(buffer);
    return buffer.GetSize();
}

// 0x00739DD8 | mk7dlp:bytes [tier B]
nw::gfx::ShaderBinaryInfo::UniformLocation nw::gfx::ShaderBinaryInfo::SearchUniformIndex(int shaderIdx, const char* name) const
{
    const DvleHeader* pShader = static_cast<const DvleHeader*>(m_pShaders[shaderIdx]);
    const char* pSymbols = AddOffset<char>(pShader, pShader->m_SymbolOffset);
    const DvleUniform* pUniforms = AddOffset<DvleUniform>(pShader, pShader->m_UniformOffset);
    const u32 length = std::strlen(name);

    UniformLocation location;
    location.index = -1;
    location.type = UniformLocation::TYPE_NONE;
    for (int i = 0; i < static_cast<int>(pShader->m_UniformCount); ++i) {
        const DvleUniform& uniform = pUniforms[i];
        const char* pSymbol = pSymbols + uniform.m_SymbolOffset;
        // the name itself or a member of it
        if (std::strncmp(name, pSymbol, length) == 0 && (pSymbol[length] == '\0' || pSymbol[length] == '.')) {
            const int reg = uniform.m_StartRegister;
            if (reg >= UNIFORM_END) {
                location.index = -1;
            } else if (reg >= UNIFORM_BOOL_FIRST) {
                location.type = UniformLocation::TYPE_BOOL;
                location.index = reg - UNIFORM_BOOL_FIRST;
            } else if (reg >= UNIFORM_INTEGER_FIRST) {
                location.type = UniformLocation::TYPE_INTEGER;
                location.index = reg - UNIFORM_INTEGER_FIRST;
            } else if (reg >= UNIFORM_FLOAT_FIRST) {
                location.type = UniformLocation::TYPE_FLOAT;
                location.index = reg - UNIFORM_FLOAT_FIRST;
            } else {
                location.type = UniformLocation::TYPE_INPUT;
                location.index = reg;
            }
            break;
        }
    }
    return location;
}

// 0x00739ECC | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::ShaderBinaryInfo::BuildOutAttrCommand(SafeBuffer& buffer, int vertexShaderIdx, int geometryShaderIdx) const
{
    const u32 outAttrClock = 0x01030703;
    const bool hasGeometry = geometryShaderIdx >= 0;

    int vertexOutputCount;
    u32 vertexOutputMask;
    GetOutputRegisters(static_cast<const DvleHeader*>(m_pShaders[vertexShaderIdx]), &vertexOutputCount, &vertexOutputMask);

    // the outputs of the last stage
    const DvleHeader* pLast = static_cast<const DvleHeader*>(m_pShaders[hasGeometry ? geometryShaderIdx : vertexShaderIdx]);
    int outputCount;
    u32 outputMask;
    GetOutputRegisters(pLast, &outputCount, &outputMask);

    // the semantics of the components of each output register
    const DvleOutput* pOutputs = AddOffset<DvleOutput>(pLast, pLast->m_OutputOffset);
    u32 outmaps[7];
    for (int i = 0; i < 7; ++i) {
        outmaps[i] = OUTATTR_UNUSED * 0x01010101;
        for (int j = 0; j < static_cast<int>(pLast->m_OutputCount); ++j) {
            const DvleOutput& output = pOutputs[j];
            int n = 0;
            for (int c = 0; output.m_Register == i && c < 4; ++c) {
                if ((output.m_Mask & (1 << c)) == 0) {
                    continue;
                }
                u32 attribute = OUTATTR_UNUSED;
                switch (output.m_Type) {
                case OUTPUT_POSITION:
                    attribute = OUTATTR_POSITION_X + n++;
                    break;
                case OUTPUT_NORMAL_QUATERNION:
                    attribute = OUTATTR_QUATERNION_X + n++;
                    break;
                case OUTPUT_COLOR:
                    attribute = OUTATTR_COLOR_R + n++;
                    break;
                case OUTPUT_TEXCOORD0:
                    if (n < 2) {
                        attribute = OUTATTR_TEXCOORD0_U + n++;
                    }
                    break;
                case OUTPUT_TEXCOORD0_W:
                    attribute = OUTATTR_TEXCOORD0_W;
                    break;
                case OUTPUT_TEXCOORD1:
                    if (n < 2) {
                        attribute = OUTATTR_TEXCOORD1_U + n++;
                    }
                    break;
                case OUTPUT_TEXCOORD2:
                    if (n < 2) {
                        attribute = OUTATTR_TEXCOORD2_U + n++;
                    }
                    break;
                case OUTPUT_VIEW:
                    if (n < 3) {
                        attribute = OUTATTR_VIEW_X + n++;
                    }
                    break;
                default:
                    break;
                }
                outmaps[i] = (outmaps[i] & ~(0xFF << (c * 8))) | (attribute << (c * 8));
            }
        }
    }

    // the geometry stage (without a geometry shader: the first three commands, all off)
    u32 geometryCommand[] = {
        0, Header(REG_GSH_MISC0),
        0, Header(REG_GSH_MISC1, 0x1),
        0, Header(REG_GEOSTAGE_CONFIG, 0x8),
        0, Header(REG_GSH_INPUTBUFFER_CONFIG, 0x2),
    };
    const u32 vertexMain = static_cast<const DvleHeader*>(m_pShaders[vertexShaderIdx])->m_MainOffset;
    int geometryInputCount;
    u32 geometryMain;
    if (hasGeometry) {
        geometryInputCount = vertexOutputCount;
        const DvleHeader* pGeometry = static_cast<const DvleHeader*>(m_pShaders[geometryShaderIdx]);
        geometryMain = pGeometry->m_MainOffset;
        const u8 type = pGeometry->m_GeometryType;
        geometryCommand[0] |= type & 3;
        switch (type) {
        case GEOMETRY_VARIABLE_PRIMITIVE:
            geometryCommand[4] |= 0x80000000;
            geometryCommand[6] |= 0x100;
            geometryCommand[2] |= pGeometry->m_FullVertexCount;
            break;
        case GEOMETRY_FIXED_PRIMITIVE:
            geometryCommand[6] |= 0x100;
            geometryCommand[0] |= ((pGeometry->m_VariableVertexCount - 1) << 8) | ((vertexOutputCount - 1) << 12) |
                                  (pGeometry->m_StartFloatRegister << 16) | 0x01000000;
            break;
        default:
            break;
        }
    }

    const u32 outputCommand[] = {
        vertexOutputMask, Header(REG_VSH_OUTMAP_MASK),
        static_cast<u32>(vertexOutputCount - 1), Header(REG_VSH_OUTMAP_TOTAL2),
        static_cast<u32>(vertexOutputCount - 1), Header(REG_VSH_OUTMAP_TOTAL1),
        static_cast<u32>(outputCount - 1), Header(REG_PRIMITIVE_CONFIG, 0x1),
        static_cast<u32>(outputCount), Header(REG_SH_OUTMAP_TOTAL),
        outmaps[0], Header(REG_SH_OUTMAP_O0, 0xF, 6, true),
        outmaps[1], outmaps[2], outmaps[3], outmaps[4], outmaps[5], outmaps[6],
        1, Header(REG_SH_OUTATTR_MODE),
        outAttrClock, Header(REG_SH_OUTATTR_CLOCK),
        vertexMain | 0x7FFF0000, Header(REG_VSH_ENTRYPOINT),
    };

    buffer.Write(geometryCommand, hasGeometry ? sizeof(geometryCommand) : sizeof(geometryCommand) - 2 * sizeof(u32));
    buffer.Write(outputCommand, sizeof(outputCommand));
    if (hasGeometry) {
        const u32 geometryShaderCommand[] = {
            outputMask, Header(REG_GSH_OUTMAP_MASK),
            static_cast<u32>(geometryInputCount - 1) | 0x08000000, Header(REG_GSH_INPUTBUFFER_CONFIG, 0x9),
            geometryMain | 0x7FFF0000, Header(REG_GSH_ENTRYPOINT),
        };
        buffer.Write(geometryShaderCommand, sizeof(geometryShaderCommand));
    }
}

// 0x0073A2E8 | nintendogs:bytes [tier A]
void nw::gfx::ShaderBinaryInfo::BuildProgramCommand(SafeBuffer& buffer) const
{
    const u32 vertexStartCommand[] = { 0, Header(REG_VSH_CODETRANSFER_INDEX) };
    buffer.Write(vertexStartCommand, sizeof(vertexStartCommand));
    PutLoadCommand(buffer, REG_VSH_CODETRANSFER_DATA, m_pProgramCode,
                   m_ProgramSize > VERTEX_PROGRAM_MAX ? VERTEX_PROGRAM_MAX : m_ProgramSize);
    const u32 vertexEndCommand[] = { 1, Header(REG_VSH_CODETRANSFER_END) };
    buffer.Write(vertexEndCommand, sizeof(vertexEndCommand));

    if (m_GeometryShaderCount > 0) {
        const u32 geometryStartCommand[] = { 0, Header(REG_GSH_CODETRANSFER_INDEX) };
        buffer.Write(geometryStartCommand, sizeof(geometryStartCommand));
        PutLoadCommand(buffer, REG_GSH_CODETRANSFER_DATA, m_pProgramCode, m_ProgramSize);
        const u32 geometryEndCommand[] = { 1, Header(REG_GSH_CODETRANSFER_END) };
        buffer.Write(geometryEndCommand, sizeof(geometryEndCommand));
    }
}

// 0x0073A3E4 | nintendogs:bytes [tier A]
void nw::gfx::ShaderBinaryInfo::BuildConstRegCommand(SafeBuffer& buffer, int shaderIdx) const
{
    const DvleHeader* pShader = static_cast<const DvleHeader*>(m_pShaders[shaderIdx]);
    const u32 floatIndexReg = pShader->m_ShaderType != 0 ? REG_GSH_FLOATUNIFORM_INDEX : REG_VSH_FLOATUNIFORM_INDEX;
    const u32 intReg = floatIndexReg - INTUNIFORM_DISTANCE;
    const DvleConstant* pConstants = AddOffset<DvleConstant>(pShader, pShader->m_ConstantOffset);
    for (u32 i = 0; i < pShader->m_ConstantCount; ++i) {
        const DvleConstant& constant = pConstants[i];
        switch (constant.m_Type) {
        case CONSTANT_INTEGER: {
            const u32 command[] = { constant.m_Values[0], Header(static_cast<u16>(constant.m_Id + intReg)) };
            buffer.Write(command, sizeof(command));
            break;
        }
        case CONSTANT_FLOAT: {
            // the index (float24 mode), then the four float24 values packed into three words
            const u32* pValues = constant.m_Values;
            u32 command[6] = {};
            command[0] = constant.m_Id;
            command[1] = Header(static_cast<u16>(floatIndexReg), 0xF, 3, true);
            command[2] = ((pValues[2] << 8) >> 24) | (pValues[3] << 8);
            command[3] = ((pValues[1] >> 8) & 0xFFFF) | (pValues[2] << 16);
            command[4] = (pValues[0] & 0x00FFFFFF) | (pValues[1] << 24);
            buffer.Write(command, sizeof(command));
            break;
        }
        default:
            break;
        }
    }
}

// 0x0073A568 (name is ours)
u32 nw::gfx::ShaderBinaryInfo::GetCommonCommandSize() const
{
    u32 size = 2 * sizeof(u32);
    size += 2 * sizeof(u32) + GetLoadCommandSize(m_SwizzleCount);
    if (m_GeometryShaderCount > 0) {
        size += 2 * sizeof(u32) + GetLoadCommandSize(m_SwizzleCount);
    }
    size += 2 * sizeof(u32) + GetLoadCommandSize(m_ProgramSize > VERTEX_PROGRAM_MAX ? VERTEX_PROGRAM_MAX : m_ProgramSize) +
            2 * sizeof(u32);
    if (m_GeometryShaderCount > 0) {
        size += 2 * sizeof(u32) + GetLoadCommandSize(m_ProgramSize) + 2 * sizeof(u32);
    }
    return size;
}

// 0x0073A684 | nintendogs:bytes [tier A]
u32 nw::gfx::ShaderBinaryInfo::BuildShaderProgramCommand(int vertexShaderIdx, int geometryShaderIdx, u32* pBuffer, u32 bufferSize) const
{
    SafeBuffer buffer(pBuffer, bufferSize);
    const u32 modeCommand[] = { geometryShaderIdx >= 0 ? 1u : 0u, Header(REG_VSH_COM_MODE, 0x1) };
    buffer.Write(modeCommand, sizeof(modeCommand));
    BuildConstRegCommand(buffer, vertexShaderIdx);
    if (geometryShaderIdx >= 0) {
        BuildConstRegCommand(buffer, geometryShaderIdx);
    }
    BuildOutAttrCommand(buffer, vertexShaderIdx, geometryShaderIdx);
    return buffer.GetSize();
}

// 0x0073A730 | nintendogs:bytes [tier A]
u32 nw::gfx::ShaderBinaryInfo::GetShaderProgramCommandSize(int vertexShaderIdx, int geometryShaderIdx) const
{
    u32 size = 2 * sizeof(u32) + GetConstRegCommandSize(static_cast<const DvleHeader*>(m_pShaders[vertexShaderIdx]));
    if (geometryShaderIdx >= 0) {
        size += GetConstRegCommandSize(static_cast<const DvleHeader*>(m_pShaders[geometryShaderIdx]));
    }
    // the commands of BuildOutAttrCommand
    const bool hasGeometry = geometryShaderIdx >= 0;
    u32 outAttrSize = (8 - (hasGeometry ? 0 : 2)) * sizeof(u32) + 24 * sizeof(u32);
    if (hasGeometry) {
        outAttrSize += 6 * sizeof(u32);
    }
    return size + outAttrSize;
}

} // namespace gfx
} // namespace nw
