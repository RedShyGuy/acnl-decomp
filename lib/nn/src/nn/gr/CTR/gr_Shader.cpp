#include "nn/gr/CTR/gr_Shader.h"
#include <string.h>
#include "nn/gr/CTR/detail/gr_Command.h"
#include "nn/gr/CTR/gr_BindSymbol.h"

namespace nn {
namespace gr {
namespace CTR {
using detail::CommandHeader;
using detail::DUMMY_COMMAND;

namespace {
// the 7 output registers (and 16 vertex shader outputs into a geometry shader) are mapped to
// semantics per component; 0x1F marks an unused component
const u32 UNUSED_OUTPUT_MAP = 0x1F1F1F1F;
const s32 OUTPUT_REGISTER_COUNT = 7;
const s32 GS_INPUT_REGISTER_COUNT = 16;
const u32 LOAD_COMMAND_MAX = 128;

const u8 DRAW_MODE_GEOMETRY_PRIMITIVE = 3;

inline bool IsValidOutput(u16 type)
{
    return type < 9 && type != 7;
}
} // namespace

// 0x00349E64 | nintendogs:callseq [confirmed by fefates, mk7dlp] [tier A]
void nn::gr::CTR::Shader::SetupBinary(const void* binary, int vsIndex, int gsIndex)
{
    const u32* header = static_cast<const u32*>(binary);
    m_ExeCount = header[1];
    m_VsBoolMask = 0;
    m_GsBoolMask = 0;
    const u32* offsets = header + 2;
    for (s32 i = 0; i < m_ExeCount; ++i) {
        m_Exes[i] = At<ExeHeader>(binary, *offsets++);
    }

    // the program follows the executable offsets
    const ProgramHeader* program = reinterpret_cast<const ProgramHeader*>(offsets);
    m_Instructions = At<u32>(program, program->codeOffset);
    m_InstructionCount = program->codeCount;
    const u32* swizzle = At<u32>(program, program->swizzleOffset);
    m_SwizzleCount = program->swizzleCount;
    for (u32 i = 0; i < m_SwizzleCount; ++i) {
        m_Swizzles[i] = *swizzle;
        swizzle += 2;
    }

    const PicaDataDrawMode drawMode = m_DrawMode;
    MakeShaderConstCommandCache_();
    m_GsIndex = gsIndex;
    m_VsIndex = vsIndex;
    if (gsIndex >= 0) {
        m_DrawMode = static_cast<PicaDataDrawMode>(DRAW_MODE_GEOMETRY_PRIMITIVE);
    }
    m_OutAttrCommandCount = MakeOutAttrCommand_(m_OutAttrCommand, vsIndex, gsIndex) - m_OutAttrCommand;
    if (m_GsIndex < 0) {
        m_DrawMode = drawMode;
    }
}

// 0x00349F50 | nintendogs:callseq [confirmed by fefates] [tier A]
u32* nn::gr::CTR::Shader::MakeOutAttrCommand_(u32* command, int vsIndex, int gsIndex)
{
    bool useGs = false;
    int index = vsIndex;
    if (m_GsIndex >= 0) {
        index = gsIndex;
        useGs = true;
    }

    s32 outCount = 0;
    u32 clock = 0;
    bool useTexcoord = false;
    u32 outMask = 0;
    const ExeHeader* exe = m_Exes[index];

    OutputEntry outputs[64];
    s32 outputNum = 0;
    if (useGs && (exe->mergeOutmaps & 1)) {
        // the outputs of both shaders, the common ones first
        const ExeHeader* vsExe = m_Exes[vsIndex];
        const OutputEntry* gsTable = At<OutputEntry>(exe, exe->outputTableOffset);
        const OutputEntry* vsTable = At<OutputEntry>(vsExe, vsExe->outputTableOffset);
        u32 gsMerged = 0;
        u32 vsMerged = 0;
        for (u32 i = 0; i < exe->outputCount; ++i) {
            if (!IsValidOutput(gsTable[i].type)) {
                continue;
            }
            for (u32 j = 0; j < vsExe->outputCount; ++j) {
                // (the original tests the vertex shader output i here, not j)
                if (IsValidOutput(vsTable[i].type) && gsTable[i].type == vsTable[j].type) {
                    vsMerged |= 1 << j;
                    outputs[outputNum].type = gsTable[i].type;
                    outputs[outputNum].registerIndex = outputNum;
                    outputs[outputNum].mask = gsTable[i].mask;
                    gsMerged |= 1 << i;
                    ++outputNum;
                }
            }
        }
        for (u32 i = 0; i < exe->outputCount; ++i) {
            if (!(gsMerged & (1 << i)) && IsValidOutput(gsTable[i].type)) {
                outputs[outputNum].type = gsTable[i].type;
                outputs[outputNum].registerIndex = outputNum;
                outputs[outputNum].mask = gsTable[i].mask;
                ++outputNum;
            }
        }
        for (u32 j = 0; j < vsExe->outputCount; ++j) {
            if (!(vsMerged & (1 << j)) && IsValidOutput(vsTable[j].type)) {
                outputs[outputNum].type = vsTable[j].type;
                outputs[outputNum].registerIndex = outputNum;
                outputs[outputNum].mask = vsTable[j].mask;
                ++outputNum;
            }
        }
    } else {
        const OutputEntry* table = At<OutputEntry>(exe, exe->outputTableOffset);
        for (u32 i = 0; i < exe->outputCount; ++i) {
            outputs[i] = table[i];
        }
        outputNum = exe->outputCount;
    }

    u32 outMap[OUTPUT_REGISTER_COUNT];
    for (s32 reg = 0; reg < OUTPUT_REGISTER_COUNT; ++reg) {
        outMap[reg] = UNUSED_OUTPUT_MAP;
        for (s32 k = 0; k < outputNum; ++k) {
            const u16 mask = outputs[k].mask;
            const u16 type = outputs[k].type;
            const u16 registerIndex = outputs[k].registerIndex;
            u32 component = 0;
            for (s32 c = 0; registerIndex == reg && c < 4; ++c) {
                if (!(mask & (1 << c))) {
                    continue;
                }
                u32 semantic = 0x1F;
                switch (type) {
                case 0: // position
                    semantic = component++;
                    if (component == 2) {
                        clock |= 0x1;
                    }
                    break;
                case 1: // quaternion
                    clock |= 0x1000000;
                    semantic = 4 + component++;
                    break;
                case 2: // color
                    clock |= 0x2;
                    semantic = 8 + component++;
                    break;
                case 3: // texcoord0
                    if (component < 2) {
                        semantic = 12 + component++;
                    }
                    clock |= 0x100;
                    useTexcoord = true;
                    break;
                case 4: // texcoord0 w
                    semantic = 16;
                    clock |= 0x10000;
                    useTexcoord = true;
                    break;
                case 5: // texcoord1
                    if (component < 2) {
                        semantic = 14 + component++;
                    }
                    clock |= 0x200;
                    useTexcoord = true;
                    break;
                case 6: // texcoord2
                    if (component < 2) {
                        semantic = 22 + component++;
                    }
                    clock |= 0x400;
                    useTexcoord = true;
                    break;
                case 7:
                    break;
                case 8: // view
                    if (component < 3) {
                        semantic = 18 + component++;
                    }
                    clock |= 0x1000000;
                    break;
                }
                outMap[reg] = (outMap[reg] & ~(0xFF << (c * 8))) | (semantic << (c * 8));
            }
        }
        if (outMap[reg] != UNUSED_OUTPUT_MAP) {
            ++outCount;
            outMask |= 1 << reg;
        }
    }

    if (useGs) {
        // the vertex shader outputs that feed the geometry shader
        s32 vsOutCount = 0;
        u32 vsOutMask = 0;
        const ExeHeader* vsExe = m_Exes[vsIndex];
        const OutputEntry* table = At<OutputEntry>(vsExe, vsExe->outputTableOffset);
        const u32 tableCount = vsExe->outputCount;
        u32 vsMap[GS_INPUT_REGISTER_COUNT];
        for (s32 reg = 0; reg < GS_INPUT_REGISTER_COUNT; ++reg) {
            vsMap[reg] = UNUSED_OUTPUT_MAP;
            for (u32 k = 0; k < tableCount; ++k) {
                const u16 registerIndex = table[k].registerIndex;
                u32 component = 0;
                for (s32 c = 0; registerIndex == reg && c < 4; ++c) {
                    if (!(table[k].mask & (1 << c))) {
                        continue;
                    }
                    u32 semantic = 0x1F;
                    switch (table[k].type) {
                    case 0:
                        semantic = component++;
                        break;
                    case 1:
                        semantic = 4 + component++;
                        break;
                    case 2:
                        semantic = 8 + component++;
                        break;
                    case 3:
                        if (component < 2) {
                            semantic = 12 + component++;
                        }
                        break;
                    case 4:
                        semantic = 16;
                        break;
                    case 5:
                        if (component < 2) {
                            semantic = 14 + component++;
                        }
                        break;
                    case 6:
                        if (component < 2) {
                            semantic = 22 + component++;
                        }
                        break;
                    case 7:
                        break;
                    case 8:
                        if (component < 3) {
                            semantic = 18 + component++;
                        }
                        break;
                    case 9:
                        semantic = 0xFF;
                        break;
                    }
                    vsMap[reg] = (vsMap[reg] & ~(0xFF << (c * 8))) | (semantic << (c * 8));
                }
            }
            if (vsMap[reg] != UNUSED_OUTPUT_MAP) {
                ++vsOutCount;
                vsOutMask |= 1 << reg;
            }
        }

        const ExeHeader* gsExe = m_Exes[gsIndex];
        const u8 geometryType = gsExe->geometryType;
        *command++ = (geometryType == 1) ? 0x80000000 : 0;
        *command++ = CommandHeader(0x229, 0xA);
        *command++ = 0;
        *command++ = CommandHeader(0x253, 0x3);
        *command++ = (geometryType != 0 ? 0x100 : 0) | (vsOutCount - 1) | 0x08000000;
        *command++ = CommandHeader(0x289, 0xB);
        *command++ = m_Exes[gsIndex]->mainOffset | 0x7FFF0000;
        *command++ = CommandHeader(0x28A);
        *command++ = outMask;
        *command++ = CommandHeader(0x28D);
        *command++ = m_Exes[vsIndex]->mainOffset | 0x7FFF0000;
        *command++ = CommandHeader(0x2BA);
        *command++ = vsOutMask;
        *command++ = CommandHeader(0x2BD);
        *command++ = vsOutCount - 1;
        *command++ = CommandHeader(0x251);
        *command++ = 0x76543210;
        *command++ = CommandHeader(0x28B);
        *command++ = 0xFEDCBA98;
        *command++ = CommandHeader(0x28C);

        u32 geometryConfig = geometryType;
        if (geometryType == 1) {
            const u8 fullVertexCount = m_Exes[gsIndex]->geometryFullVertexCount;
            if (fullVertexCount != 0) {
                *command++ = fullVertexCount - 1;
                *command++ = CommandHeader(0x254, 0x1);
            }
        } else if (geometryType == 2) {
            const ExeHeader* gs = m_Exes[gsIndex];
            geometryConfig = 0x01000000 | (gs->geometryStartRegister << 16) | ((vsOutCount - 1) << 12)
                | ((gs->geometryVertexCount - 1) << 8) | 2;
        }
        *command++ = geometryConfig;
        *command++ = CommandHeader(0x252);
        *command++ = vsOutCount - 1;
        *command++ = CommandHeader(0x24A);
    } else {
        *command++ = 0;
        *command++ = CommandHeader(0x229, 0x8);
        *command++ = 0;
        *command++ = CommandHeader(0x253, 0x1);
        *command++ = 0xA0000000;
        *command++ = CommandHeader(0x289, 0xB);
        *command++ = m_Exes[vsIndex]->mainOffset | 0x7FFF0000;
        *command++ = CommandHeader(0x2BA);
        *command++ = outMask;
        *command++ = CommandHeader(0x2BD);
        *command++ = outCount - 1;
        *command++ = CommandHeader(0x251);
        *command++ = 0;
        *command++ = CommandHeader(0x252);
        *command++ = outCount - 1;
        *command++ = CommandHeader(0x24A);
    }

    *command++ = outCount - 1;
    *command++ = CommandHeader(0x25E, 0x1);
    *command++ = outCount;
    *command++ = CommandHeader(0x4F);
    s32 written = 0;
    for (s32 reg = 0; reg < OUTPUT_REGISTER_COUNT; ++reg) {
        if (outMap[reg] != UNUSED_OUTPUT_MAP) {
            *command++ = outMap[reg];
            *command++ = CommandHeader(0x50 + written);
            ++written;
        }
    }
    // (the remaining registers get the maps at their own index)
    for (; written < OUTPUT_REGISTER_COUNT; ++written) {
        *command++ = outMap[written];
        *command++ = CommandHeader(0x50 + written);
    }
    *command++ = useTexcoord;
    *command++ = CommandHeader(0x64);
    *command++ = clock;
    *command++ = CommandHeader(0x6F);
    if (useGs) {
        *command++ = 0;
        *command++ = CommandHeader(0x25E, 0x8);
    }
    return command;
}

// 0x0034A930 (name is ours)
u32* nn::gr::CTR::Shader::MakeDisableCommand(u32* command)
{
    return MakeShaderModeCommand_(command, false, static_cast<PicaDataDrawMode>(DRAW_MODE_GEOMETRY_PRIMITIVE));
}

// 0x0034A93C | nintendogs:bytes [confirmed by fefates] [tier A]
u32* nn::gr::CTR::Shader::MakeShaderModeCommand_(u32* command, bool isGeometryShader, PicaDataDrawMode drawMode)
{
    *command++ = isGeometryShader ? 0x300 : (drawMode << 8);
    *command++ = CommandHeader(0x25E, 0x2);
    *command++ = 0;
    *command++ = CommandHeader(0x251, 0x0, 9);
    for (s32 i = 0; i < 10; ++i) {
        *command++ = DUMMY_COMMAND;
    }
    *command++ = 0;
    *command++ = CommandHeader(0x200, 0x0, 29);
    for (s32 i = 0; i < 30; ++i) {
        *command++ = DUMMY_COMMAND;
    }
    *command++ = isGeometryShader ? 2 : 0;
    *command++ = CommandHeader(0x229, 0x1);
    *command++ = 0;
    *command++ = CommandHeader(0x200, 0x0, 29);
    for (s32 i = 0; i < 30; ++i) {
        *command++ = DUMMY_COMMAND;
    }
    *command++ = isGeometryShader;
    *command++ = CommandHeader(0x244, 0x1);
    return command;
}

// 0x0034AA2C | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::gr::CTR::Shader::MakeShaderConstCommandCache_()
{
    for (s32 i = 0; i < m_ExeCount; ++i) {
        u32* command = m_ConstCommand[i];
        const ExeHeader* exe = m_Exes[i];
        u32 floatRegister = 0x2C0;
        u32 intRegister = 0x2B1;
        u32* boolMask = &m_VsBoolMask;
        if (exe->shaderType != 0) {
            floatRegister = 0x290;
            intRegister = 0x281;
            boolMask = &m_GsBoolMask;
        }
        const ConstEntry* consts = At<ConstEntry>(exe, exe->constTableOffset);
        const u32 floatHeader = CommandHeader(floatRegister, 0xF, 3, true);
        for (u32 j = 0; j < exe->constCount; ++j) {
            const ConstEntry& entry = consts[j];
            switch (entry.type) {
            case 0:
                *boolMask |= (entry.value[0] & 1) << entry.registerIndex;
                break;
            case 1:
                *command++ = entry.value[0] | (entry.value[1] << 8) | (entry.value[2] << 16) | (entry.value[3] << 24);
                *command++ = CommandHeader(intRegister + entry.registerIndex);
                break;
            case 2:
                // four float24 values in three words, w first
                *command++ = entry.registerIndex;
                *command++ = floatHeader;
                *command++ = ((entry.value[2] >> 16) & 0xFF) | (entry.value[3] << 8);
                *command++ = ((entry.value[1] >> 8) & 0xFFFF) | (entry.value[2] << 16);
                *command++ = (entry.value[0] & 0xFFFFFF) | (entry.value[1] << 24);
                *command++ = DUMMY_COMMAND;
                break;
            }
        }
        m_ConstCommandCount[i] = command - m_ConstCommand[i];
    }
}

// 0x0034ABD0 | fefates:bytes [tier B]
nn::gr::CTR::Shader::Shader()
    : m_VsIndex(0), m_GsIndex(-1), m_ExeCount(0), m_InstructionCount(0), m_SwizzleCount(0),
      m_DrawMode(static_cast<PicaDataDrawMode>(DRAW_MODE_GEOMETRY_PRIMITIVE)), m_VsBoolMask(0), m_GsBoolMask(0),
      m_OutAttrCommandCount(0)
{
    memset(m_ConstCommandCount, 0, sizeof(m_ConstCommandCount));
}

// 0x0072770C | nintendogs:bytes-fuzzy [confirmed by fefates, mk7dlp] [tier A]
u32* nn::gr::CTR::Shader::MakeFullCommand(u32* command) const
{
    command = MakeShaderModeCommand_(command, m_GsIndex >= 0, m_DrawMode);
    if (m_GsIndex >= 0) {
        *command++ = 0;
        *command++ = CommandHeader(0x29B);
        command = MakeLoadCommand_(command, 0x29C, m_Instructions, m_InstructionCount);
        *command++ = 1;
        *command++ = CommandHeader(0x28F);
        *command++ = 0;
        *command++ = CommandHeader(0x2A5);
        command = MakeLoadCommand_(command, 0x2A6, m_Swizzles, m_SwizzleCount);
        memcpy(command, m_ConstCommand[m_GsIndex], m_ConstCommandCount[m_GsIndex] * sizeof(u32));
        command += m_ConstCommandCount[m_GsIndex];
        *command++ = m_GsBoolMask | 0x7FFF0000;
        *command++ = CommandHeader(0x280);
    }
    *command++ = 0;
    *command++ = CommandHeader(0x2CB);
    // the vertex shader program memory has 512 words
    command = MakeLoadCommand_(command, 0x2CC, m_Instructions, m_InstructionCount < 512 ? m_InstructionCount : 512);
    *command++ = 1;
    *command++ = CommandHeader(0x2BF);
    *command++ = 0;
    *command++ = CommandHeader(0x2D5);
    command = MakeLoadCommand_(command, 0x2D6, m_Swizzles, m_SwizzleCount);
    memcpy(command, m_ConstCommand[m_VsIndex], m_ConstCommandCount[m_VsIndex] * sizeof(u32));
    command += m_ConstCommandCount[m_VsIndex];
    *command++ = m_VsBoolMask | 0x7FFF0000;
    *command++ = CommandHeader(0x2B0);
    *command++ = (m_DrawMode == DRAW_MODE_GEOMETRY_PRIMITIVE) ? 0x100 : 0;
    *command++ = CommandHeader(0x229, 0x2);
    *command++ = (m_DrawMode == DRAW_MODE_GEOMETRY_PRIMITIVE) ? 0x100 : 0;
    *command++ = CommandHeader(0x253, 0x2);
    memcpy(command, m_OutAttrCommand, m_OutAttrCommandCount * sizeof(u32));
    return command + m_OutAttrCommandCount;
}

// 0x00727944 | nintendogs:bytes [confirmed by fefates] [tier A]
u32* nn::gr::CTR::Shader::MakeLoadCommand_(u32* command, u32 reg, const u32* data, u32 count) const
{
    while (count > LOAD_COMMAND_MAX) {
        *command++ = data[0];
        *command++ = CommandHeader(reg, 0xF, LOAD_COMMAND_MAX - 1);
        memcpy(command, data + 1, (LOAD_COMMAND_MAX - 1) * sizeof(u32));
        command += LOAD_COMMAND_MAX - 1;
        *command++ = DUMMY_COMMAND;
        data += LOAD_COMMAND_MAX;
        count -= LOAD_COMMAND_MAX;
    }
    *command++ = data[0];
    *command++ = CommandHeader(reg, 0xF, count - 1);
    memcpy(command, data + 1, (count - 1) * sizeof(u32));
    command += count - 1;
    if ((count & 1) == 0) {
        *command++ = DUMMY_COMMAND;
    }
    return command;
}

// 0x007279E4 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
bool nn::gr::CTR::Shader::SearchBindSymbol(BindSymbol* symbol, const char* name) const
{
    const ExeHeader* exe = m_Exes[symbol->m_ShaderType == 1 ? m_GsIndex : m_VsIndex];
    const char* symbols = At<char>(exe, exe->symbolTableOffset);
    const UniformEntry* uniforms = At<UniformEntry>(exe, exe->uniformTableOffset);
    const size_t length = strlen(name);
    for (u32 i = 0; i < exe->uniformCount; ++i) {
        const UniformEntry& uniform = uniforms[i];
        if (strncmp(name, symbols + uniform.symbolOffset, length) != 0) {
            continue;
        }
        // the name may continue with a member ("name.member")
        const char next = symbols[uniform.symbolOffset + length];
        if (next != '\0' && next != '.') {
            continue;
        }

        symbol->m_Name = symbols + uniform.symbolOffset;
        symbol->m_Start = static_cast<u8>(uniform.startRegister);
        symbol->m_End = static_cast<u8>(uniform.endRegister);
        if (symbol->m_Start >= 0x88) {
            return false;
        }
        if (symbol->m_Start >= 0x78) {
            symbol->m_Start -= 0x78;
            symbol->m_End -= 0x78;
            return symbol->m_SymbolType == 4;
        }
        if (symbol->m_Start >= 0x70) {
            symbol->m_Start -= 0x70;
            symbol->m_End -= 0x70;
            return symbol->m_SymbolType == 3;
        }
        if (symbol->m_Start >= 0x10) {
            symbol->m_Start -= 0x10;
            symbol->m_End -= 0x10;
            return symbol->m_SymbolType == 2;
        }
        return symbol->m_SymbolType == 1;
    }
    return false;
}

} // namespace CTR
} // namespace gr
} // namespace nn
