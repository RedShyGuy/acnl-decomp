#include "cfl/cfl_Database.h"
#include "cfl/cfl_Net.h"
#include "cfl/cfl_Model.h"
#include "nn/gr/CTR/CTR_Api.h"
#include "nn/gr/CTR/detail/gr_Command.h"
#include "nn/math/math_ARMv6.h"

#include <cstring>

namespace {
// a PICA command header (3dbrew "GPU/Internal Registers"): register, byte mask, extra parameters,
// consecutive registers (name is ours)
constexpr u32 Header(u32 reg, u32 mask = 0xF, u32 extraCount = 0, bool isConsecutive = false)
{
    return reg | (mask << 16) | (extraCount << 20) | (isConsecutive ? 0x80000000 : 0);
}

// the registers (names are ours, after 3dbrew)
const u32 REG_FRAGOP_ALPHA_TEST = 0x104;
const u32 REG_VSH_FLOATUNIFORM_INDEX = 0x2C0;
const u32 REG_VSH_FLOATUNIFORM_DATA = 0x2C1;

// the texture units (values of the OpenGL ES API)
const GLenum TEXTURE0 = 0x84C0;
const GLenum TEXTURE1 = 0x84C1;

// a float uniform index in 32-bit mode
const u32 FLOATUNIFORM_F32 = 0x80000000;

// pads a command to 8 bytes
// 0x008A3CB4
const u32 s_Padding = 0;
// 0x008A3CBC
const u32 s_Padding2 = 0;

// the parameters of nngxGetCmdlistParameteri (names are ours)
const GLenum CMDLIST_USED_BUFSIZE = 0x202;
const GLenum CMDLIST_CURRENT_BUFADDR = 0x208;

// the address the next command goes to (inline; name is ours)
inline u32* GetCommandBuffer()
{
    u32* buffer;
    nngxGetCmdlistParameteri(CMDLIST_CURRENT_BUFADDR, reinterpret_cast<GLint*>(&buffer));
    return buffer;
}
} // namespace

// 0x00AE5CEC
CFLProgramContext s_ProgramContext;

// the combiners that use only the primary color (stage 0) and that pass the previous result on (stages 1-5; names
// are ours)
// 0x008A4154
const u32 s_TexEnvNone[4] = {0x0EEE0EEE, Header(0x0C0, 0xF, 2, true), 0, 0};
// 0x008A4184
const u32 s_TexEnvPassThrough[20] = {
    0x0FFF0FFF, Header(0x0C8, 0xF, 2, true), 0, 0, 0x0FFF0FFF, Header(0x0D0, 0xF, 2, true), 0, 0,
    0x0FFF0FFF, Header(0x0D8, 0xF, 2, true), 0, 0, 0x0FFF0FFF, Header(0x0F0, 0xF, 2, true), 0, 0,
    0x0FFF0FFF, Header(0x0F8, 0xF, 2, true), 0, 0,
};
// the sources and the scales of all combiner stages (names are ours)
// 0x008A40B4
const u32 s_TexEnvSources[12] = {
    0x0EEE0EEE, Header(0x0C0), 0x0FFF0FFF, Header(0x0C8), 0x0FFF0FFF, Header(0x0D0),
    0x0FFF0FFF, Header(0x0D8), 0x0FFF0FFF, Header(0x0F0), 0x0FFF0FFF, Header(0x0F8),
};
// 0x008A40E4
const u32 s_TexEnvScales[12] = {
    0, Header(0x0C4), 0, Header(0x0CC), 0, Header(0x0D4), 0, Header(0x0DC), 0, Header(0x0F4), 0, Header(0x0FC),
};

extern "C" {
// 0x0015403C (name is ours, after the order of the functions of this part)
void CFL_SetShaderParameter(u32 projectionIndex, u32 modelViewIndex, u32 normalIndex, u32 index34, bool directUniforms,
                            CFLiShaderParam param)
{
    CFLProgramContext* context = &s_ProgramContext;
    context->_38 = 3;
    context->_48 = 0;
    context->_4C = 0;
    context->textureUnit = param.textureUnit;
    context->projectionIndex = projectionIndex;
    context->modelViewIndex = modelViewIndex;
    context->normalIndex = normalIndex;
    context->index34 = index34;
    context->directUniforms = directUniforms;
    context->attribute[0] = param.attribute[0];
    context->attribute[1] = param.attribute[1];
    context->attribute[2] = param.attribute[2];
    if (param.textureUnit == TEXTURE0) {
        context->textureUnitIndex = 3;
    } else if (param.textureUnit == TEXTURE1) {
        context->textureUnitIndex = 4;
    } else {
        context->textureUnitIndex = 5;
    }
}
} // extern "C"

// two vectors to the uniform at index34
// 0x001668D8 (name is ours)
DECOMP_NOINLINE void CFLi_SetUniformMtx24(const f32* values)
{
    u32 index = s_ProgramContext.index34 | FLOATUNIFORM_F32;
    const u32* v = reinterpret_cast<const u32*>(values);
    if (s_ProgramContext.directUniforms) {
        u32 command[12] = {index, Header(REG_VSH_FLOATUNIFORM_INDEX), v[0], Header(REG_VSH_FLOATUNIFORM_DATA, 0xF, 7),
                           v[1], v[2], v[3], v[4], v[5], v[6], v[7], 0};
        nngxAdd3DCommand(command, sizeof(command), true);
    } else {
        u32 command[12] = {index, Header(REG_VSH_FLOATUNIFORM_INDEX), v[3], Header(REG_VSH_FLOATUNIFORM_DATA, 0xF, 7),
                           v[2], v[1], v[0], v[7], v[6], v[5], v[4], 0};
        nngxAdd3DCommand(command, sizeof(command), true);
    }
}

// a matrix of three rows to a uniform
// 0x00171598 (name is ours)
DECOMP_NOINLINE void CFLi_SetUniformMtx34(u32 index, const nn::math::MTX34* mtx, bool direct)
{
    index |= FLOATUNIFORM_F32;
    const u32* m = reinterpret_cast<const u32*>(mtx->m);
    if (direct) {
        u32 command[4] = {index, Header(REG_VSH_FLOATUNIFORM_INDEX), m[0], Header(REG_VSH_FLOATUNIFORM_DATA, 0xF, 11)};
        nngxAdd3DCommand(command, sizeof(command), true);
        nngxAdd3DCommand(&m[1], 11 * sizeof(u32), true);
        nngxAdd3DCommand(&s_Padding, sizeof(s_Padding), true);
    } else {
        u32 command[16] = {index, Header(REG_VSH_FLOATUNIFORM_INDEX), m[3], Header(REG_VSH_FLOATUNIFORM_DATA, 0xF, 11),
                           m[2], m[1], m[0], m[7], m[6], m[5], m[4], m[11], m[10], m[9], m[8], 0};
        nngxAdd3DCommand(command, sizeof(command), true);
    }
}

// the projection matrix (four rows)
// 0x0017549C (name is ours)
DECOMP_NOINLINE void CFLi_SetUniformMtx44(const f32* mtx)
{
    u32 index = s_ProgramContext.projectionIndex | FLOATUNIFORM_F32;
    const u32* m = reinterpret_cast<const u32*>(mtx);
    if (s_ProgramContext.directUniforms) {
        u32 command[4] = {index, Header(REG_VSH_FLOATUNIFORM_INDEX), m[0], Header(REG_VSH_FLOATUNIFORM_DATA, 0xF, 15)};
        nngxAdd3DCommand(command, sizeof(command), true);
        nngxAdd3DCommand(&m[1], 15 * sizeof(u32), true);
        nngxAdd3DCommand(&s_Padding2, sizeof(s_Padding2), true);
    } else {
        u32 command[20] = {index, Header(REG_VSH_FLOATUNIFORM_INDEX), m[3], Header(REG_VSH_FLOATUNIFORM_DATA, 0xF, 15),
                           m[2], m[1], m[0], m[7], m[6], m[5], m[4], m[11], m[10], m[9], m[8], m[15], m[14], m[13],
                           m[12], 0};
        nngxAdd3DCommand(command, sizeof(command), true);
    }
}

// the model view matrix and the matrix of the normals (a uniform or a GL uniform of the program)
// 0x0016D118 (name is ours)
DECOMP_NOINLINE void CFLi_SetModelMatrices(const nn::math::MTX34* pos, const nn::math::MTX34* nrm, bool normalAsUniform)
{
    CFLProgramContext* context = &s_ProgramContext;
    CFLi_SetUniformMtx34(context->modelViewIndex, pos, context->directUniforms);
    if (nrm == NULL || context->normalIndex == context->modelViewIndex) {
        return;
    }
    if (normalAsUniform) {
        CFLi_SetUniformMtx34(context->normalIndex, nrm, context->directUniforms);
        return;
    }
    GLint location = context->normalLocation;
    if (context->directUniforms) {
        const f32* m = &nrm->m[0][0];
        f32 values[12] = {m[3], m[2], m[1], m[0], m[7], m[6], m[5], m[4], m[11], m[10], m[9], m[8]};
        glUniform4fv(location, 3, values);
    } else {
        glUniform4fv(location, 3, &nrm->m[0][0]);
    }
    glGetError();
}

// an orthographic projection of the size of the target with an identity model view
// 0x0016C448 (name is ours)
DECOMP_NOINLINE void CFLi_SetOrthoProjection(int width, int height)
{
    nn::math::MTX44 projection;
    nn::math::ARMv6::MTX44OrthoC_FAST(&projection, 0.0f, width, 0.0f, height, -200.0f, 200.0f);
    CFLi_SetUniformMtx44(&projection.m[0][0]);
    nn::math::MTX34 modelView(nn::math::MTX34::Identity());
    CFLi_SetModelMatrices(&modelView, &modelView, true);
}

// 0x001660C8 (name is ours)
DECOMP_NOINLINE void CFLi_PutAlphaTest(const CFLProgramContext*, bool enable)
{
    u32* buffer = GetCommandBuffer();
    if (enable) {
        // greater than 0
        const u32 command[2] = {0x61, Header(REG_FRAGOP_ALPHA_TEST, 0x3)};
        std::memcpy(buffer, command, sizeof(command));
    } else {
        const u32 command[2] = {0, Header(REG_FRAGOP_ALPHA_TEST, 0x1)};
        std::memcpy(buffer, command, sizeof(command));
    }
    nngxMoveCommandbufferPointer(8);
}

namespace {
// the registers of the render target (names are ours, after 3dbrew)
const u32 REG_VIEWPORT_WIDTH = 0x041;
const u32 REG_VIEWPORT_INVW = 0x042;
const u32 REG_VIEWPORT_HEIGHT = 0x043;
const u32 REG_VIEWPORT_INVH = 0x044;
const u32 REG_SCISSORTEST_MODE = 0x065;
const u32 REG_SCISSORTEST_POS = 0x066;
const u32 REG_SCISSORTEST_DIM = 0x067;
const u32 REG_VIEWPORT_XY = 0x068;
const u32 REG_RENDERBUF_DIM = 0x06E;
const u32 REG_COLORBUFFER_FORMAT = 0x117;
const u32 REG_COLORBUFFER_LOC = 0x11D;
const u32 REG_FRAMEBUFFER_DIM = 0x11E;
const u32 REG_TEXENV0_SOURCE = 0x0C0;
const u32 REG_TEXENV0_COLOR = 0x0C3;
const u32 REG_TEXENV1_SOURCE = 0x0C8;
const u32 REG_TEXENV2_SOURCE = 0x0D0;
const u32 REG_TEXENV3_SOURCE = 0x0D8;
const u32 REG_TEXENV4_SOURCE = 0x0F0;
const u32 REG_TEXENV5_SOURCE = 0x0F8;

// the internal formats of the OpenGL ES API (names after the API)
const GLenum RGBA4 = 0x8056;
const GLenum RGB5_A1 = 0x8057;
const GLenum RGBA8 = 0x8058;
} // namespace

// viewport and scissor of a target at (0, 0); returns the end of the commands
// 0x0017C278 (name is ours)
DECOMP_NOINLINE u32* CFLi_PutViewport(u32* command, int width, int height)
{
    u32 commands[16] = {
        0, Header(REG_VIEWPORT_WIDTH, 0x7), 0, Header(REG_VIEWPORT_HEIGHT, 0x7),
        0, Header(REG_VIEWPORT_INVW),       0, Header(REG_VIEWPORT_INVH),
        0, Header(REG_VIEWPORT_XY),         3, Header(REG_SCISSORTEST_MODE),
        0, Header(REG_SCISSORTEST_POS),     0, Header(REG_SCISSORTEST_DIM),
    };
    commands[0] = nn::gr::CTR::Float32ToFloat24(width * 0.5f);
    commands[2] = nn::gr::CTR::Float32ToFloat24(height * 0.5f);
    commands[4] = nn::gr::CTR::detail::Float32ToFloat31(2.0f / width) << 1;
    commands[6] = nn::gr::CTR::detail::Float32ToFloat31(2.0f / height) << 1;
    commands[14] = (width - 1) | ((height - 1) << 16);
    std::memcpy(command, commands, sizeof(commands));
    return command + 16;
}

// the color buffer of the target
// 0x00175350 (name is ours)
DECOMP_NOINLINE void CFLi_PutColorBuffer(uptr address, int width, int height, GLenum format)
{
    u32* buffer = GetCommandBuffer();
    u32 pixelSize = 0;
    u32 colorFormat;
    if (format == RGBA8) {
        pixelSize = 2;
        colorFormat = 0;
    } else if (format == RGBA4) {
        colorFormat = 4;
    } else if (format == RGB5_A1) {
        colorFormat = 2;
    } else {
        colorFormat = 3;
    }
    u32 dimension = 0x1000000 | ((height - 1) << 12) | width;
    u32 commands[8] = {
        pixelSize | (colorFormat << 16), Header(REG_COLORBUFFER_FORMAT),
        nngxGetPhysicalAddr(address) >> 3, Header(REG_COLORBUFFER_LOC),
        dimension, Header(REG_FRAMEBUFFER_DIM),
        dimension, Header(REG_RENDERBUF_DIM),
    };
    std::memcpy(buffer, commands, sizeof(commands));
    nngxMoveCommandbufferPointer(sizeof(commands));
}

// the combiners: 0 none, 1 texture, 2 texture with the alpha of the primary color, 3 the texture alpha; the
// other stages pass the previous result on when resetOthers
// 0x00179EBC (name is ours)
DECOMP_NOINLINE void CFLi_PutTexEnv(const CFLProgramContext* context, int mode, bool resetOthers)
{
    if (mode == 0) {
        nngxAdd3DCommand(s_TexEnvNone, sizeof(s_TexEnvNone), true);
    } else if (mode == 3) {
        u32 commands[4] = {0x0EE00EEE | (static_cast<u32>(context->textureUnitIndex) << 16), Header(REG_TEXENV0_SOURCE, 0xF, 2, true),
                           0, 0};
        nngxAdd3DCommand(commands, sizeof(commands), true);
    } else {
        u32 combine = mode == 2 ? 1 : 0;
        u32 unit = context->textureUnitIndex;
        u32 commands[4] = {0x0EE00EE0 | (unit << 16) | unit, Header(REG_TEXENV0_SOURCE, 0xF, 2, true), 0,
                           combine | (combine << 16)};
        nngxAdd3DCommand(commands, sizeof(commands), true);
    }
    if (resetOthers) {
        nngxAdd3DCommand(s_TexEnvPassThrough, sizeof(s_TexEnvPassThrough), true);
    }
}

namespace {
// the registers of the fragment operations and the vertex input (names are ours, after 3dbrew)
const u32 REG_FACECULLING_CONFIG = 0x040;
const u32 REG_EARLYDEPTH_TEST1 = 0x062;
const u32 REG_TEXUNIT_CONFIG = 0x080;
const u32 REG_COLOR_OPERATION = 0x100;
const u32 REG_BLEND_FUNC = 0x101;
const u32 REG_STENCIL_TEST = 0x105;
const u32 REG_DEPTH_COLOR_MASK = 0x107;
const u32 REG_FRAMEBUFFER_INVALIDATE = 0x110;
const u32 REG_FRAMEBUFFER_FLUSH = 0x111;
const u32 REG_COLORBUFFER_READING = 0x112;
const u32 REG_COLORBUFFER_WRITING = 0x113;
const u32 REG_DEPTHBUFFER_READING = 0x114;
const u32 REG_DEPTHBUFFER_WRITING = 0x115;
const u32 REG_118 = 0x118;
const u32 REG_126 = 0x126;
const u32 REG_229 = 0x229;
const u32 REG_RESTART_PRIMITIVE = 0x231;
const u32 REG_FIXEDATTRIB_INDEX = 0x232;
const u32 REG_FIXEDATTRIB_DATA = 0x233;
const u32 REG_VSH_NUM_ATTR = 0x242;
const u32 REG_245 = 0x245;
const u32 REG_253 = 0x253;
const u32 REG_25E = 0x25E;
const u32 REG_25F = 0x25F;
const u32 REG_VSH_INPUTBUFFER_CONFIG = 0x2B9;
const u32 REG_VSH_ENTRYPOINT = 0x2BA;
const u32 REG_VSH_ATTRIBUTES_PERMUTATION_LOW = 0x2BB;
const u32 REG_VSH_ATTRIBUTES_PERMUTATION_HIGH = 0x2BC;

const GLenum RGB565 = 0x8D62;

// the memory areas of nngx
const GLenum MEM_VRAMA = 0x20000;
const GLenum MEM_VRAMB = 0x30000;

// the components of the attributes position, normal and texture coordinate
// 0x008A44B4
const s32 s_AttributeComponents[3] = {3, 3, 2};
} // namespace

// the quad of CFLi_DrawQuad (names are ours)
// 0x009761CC
u8 s_QuadIndices[4] = {0, 1, 3, 2};
// 0x00976268
f32 s_QuadTexCoords[8] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};

// color and depth writes, blending
// 0x0017D5C0 (name is ours)
DECOMP_NOINLINE void CFLi_PutFragmentOperation(bool colorWrite, int, bool depth, bool readColor, u32 blendFunc)
{
    u32* buffer = GetCommandBuffer();
    u32 commands[26] = {
        0x100,      Header(REG_COLOR_OPERATION, 0x2),
        0,          Header(REG_BLEND_FUNC),
        0,          Header(REG_DEPTH_COLOR_MASK, 0x3),
        0x3000000,  Header(REG_126, 0x8),
        0,          Header(REG_STENCIL_TEST),
        0,          Header(REG_EARLYDEPTH_TEST1, 0x1),
        0,          Header(REG_118),
        1,          Header(REG_FRAMEBUFFER_FLUSH),
        1,          Header(REG_FRAMEBUFFER_INVALIDATE),
        0,          Header(REG_COLORBUFFER_READING),
        0xF,        Header(REG_COLORBUFFER_WRITING),
        0,          Header(REG_DEPTHBUFFER_READING),
        0,          Header(REG_DEPTHBUFFER_WRITING),
    };
    commands[2] = blendFunc;
    commands[4] = (colorWrite ? 0x100 : 0) | (colorWrite ? 0x200 : 0) | (colorWrite ? 0x400 : 0) |
                  (depth ? 0x1000 : 0) | 0x840;
    commands[18] = (!readColor && colorWrite) ? 0 : 0xF;
    commands[22] = depth ? 2 : 0;
    commands[24] = (depth ? 1 : 0) << 1;
    std::memcpy(buffer, commands, sizeof(commands));
    nngxMoveCommandbufferPointer(sizeof(commands));
}

// vertices as fixed attributes of the vertex shader (immediate mode); returns the end of the commands
// 0x00172950 (name is ours)
DECOMP_NOINLINE u32* CFLi_PutImmediateVertices(u32* command, const CFLProgramContext* context, int, int vertexCount,
                               const u8* indices, const f32* positions, const f32* normals, const f32* texCoords)
{
    const f32* attributes[3] = {positions, normals, texCoords};
    s32 components[3] = {s_AttributeComponents[0], s_AttributeComponents[1], s_AttributeComponents[2]};
    u32 permutation = 0;
    int count = 0;
    if (attributes[0] != NULL) {
        permutation = context->attribute[0];
        count = 1;
    }
    if (attributes[1] != NULL) {
        permutation |= context->attribute[1] << (count * 4);
        count++;
    }
    if (attributes[2] != NULL) {
        permutation |= context->attribute[2] << (count * 4);
        count++;
    }
    u32 header[22] = {
        0, Header(REG_VSH_INPUTBUFFER_CONFIG, 0xB),
        0, Header(REG_VSH_NUM_ATTR),
        0, Header(REG_VSH_ATTRIBUTES_PERMUTATION_LOW),
        0, Header(REG_VSH_ATTRIBUTES_PERMUTATION_HIGH),
        0, Header(REG_229, 0x2),
        0, Header(REG_253, 0x2),
        0, Header(REG_25E, 0x2),
        1, Header(REG_25F),
        0xF, Header(REG_FIXEDATTRIB_INDEX),
        1, Header(REG_253, 0x1),
        0, Header(REG_245, 0x1),
    };
    header[2] = (count - 1) & 15;
    header[12] = 0x100;
    header[0] = 0xA0000000 | ((count - 1) & 15);
    header[4] = permutation;
    header[6] = 0;
    std::memcpy(command, header, sizeof(header));
    command += 22;

    u32* data = static_cast<u32*>(CFLi_AllocateMemory(vertexCount * 3 * 16, 4));
    u32* p = data;
    for (int v = 0; v < vertexCount; v++) {
        u8 index = indices[v];
        for (int a = 0; a < 3; a++) {
            const f32* attribute = attributes[a];
            if (attribute == NULL) {
                continue;
            }
            const f32* value = &attribute[index * components[a]];
            f32 z = components[a] == 3 ? value[2] : 0.0f;
            u32 x24 = nn::gr::CTR::Float32ToFloat24(value[0]);
            u32 y24 = nn::gr::CTR::Float32ToFloat24(value[1]);
            u32 z24 = nn::gr::CTR::Float32ToFloat24(z);
            u32 w24 = nn::gr::CTR::Float32ToFloat24(1.0f);
            p[0] = (w24 << 8) | (z24 >> 16);
            p[1] = Header(REG_FIXEDATTRIB_DATA, 0xF, 2, true);
            p[2] = (z24 << 16) | (y24 >> 8);
            p[3] = x24 | (y24 << 24);
            p += 4;
        }
    }
    u32 size = reinterpret_cast<u8*>(p) - reinterpret_cast<u8*>(data);
    std::memcpy(command, data, size);
    command = reinterpret_cast<u32*>(reinterpret_cast<u8*>(command) + (size & ~3));
    CFLi_FreeMemory(data);

    u32 footer[14] = {
        1, Header(REG_245, 0x1),
        1, Header(REG_RESTART_PRIMITIVE),
        0, Header(REG_253, 0x1),
        1, Header(REG_FRAMEBUFFER_FLUSH),
        0, Header(REG_25E, 0x8),
        0, Header(REG_25E, 0x8),
        0x7FFF0000, Header(REG_VSH_ENTRYPOINT, 0xC),
    };
    std::memcpy(command, footer, sizeof(footer));
    return command + 14;
}

// a quad from (x0, y0) to (x1, y1) at z, into command or (NULL) into the command list
// 0x00188F70 (name is ours)
DECOMP_NOINLINE u32* CFLi_DrawQuad(u32* command, const CFLProgramContext* context, int, bool textured, f32 x0, f32 x1, f32 y0,
                   f32 y1, f32 z)
{
    f32 positions[12] = {x0, y0, z, x1, y0, z, x1, y1, z, x0, y1, z};
    if (command != NULL) {
        return CFLi_PutImmediateVertices(command, context, 5, 4, s_QuadIndices, positions, NULL,
                                         textured ? s_QuadTexCoords : NULL);
    }
    u32* buffer = GetCommandBuffer();
    u32* end = CFLi_PutImmediateVertices(buffer, context, 5, 4, s_QuadIndices, positions, NULL,
                                         textured ? s_QuadTexCoords : NULL);
    nngxMoveCommandbufferPointer((reinterpret_cast<u8*>(end) - reinterpret_cast<u8*>(buffer)) & ~3);
    return command;
}

// the start of a pass into a texture: projection, no textures, no culling, the combiners and a quad that clears
// with the color
// 0x00165984 (name is ours)
DECOMP_NOINLINE void CFLi_BeginRenderTexture(int width, int height, u32 color, bool depth)
{
    CFLi_SetOrthoProjection(width, height);
    u32* buffer = GetCommandBuffer();
    const u32 texUnits[4] = {0, Header(REG_TEXUNIT_CONFIG, 0x1), 0x10000, Header(REG_TEXUNIT_CONFIG, 0x4)};
    std::memcpy(buffer, texUnits, sizeof(texUnits));
    nngxMoveCommandbufferPointer(sizeof(texUnits));
    buffer = GetCommandBuffer();
    const u32 culling[2] = {0, Header(REG_FACECULLING_CONFIG)};
    std::memcpy(buffer, culling, sizeof(culling));
    nngxMoveCommandbufferPointer(sizeof(culling));
    CFLi_PutAlphaTest(&s_ProgramContext, false);
    buffer = GetCommandBuffer();
    std::memcpy(buffer, s_TexEnvSources, sizeof(s_TexEnvSources));
    nngxMoveCommandbufferPointer(sizeof(s_TexEnvSources));
    buffer = GetCommandBuffer();
    std::memcpy(buffer, s_TexEnvScales, sizeof(s_TexEnvScales));
    nngxMoveCommandbufferPointer(sizeof(s_TexEnvScales));
    CFLi_PutFragmentOperation(true, 0, depth, false, 0x1010000);
    const u32 commands[6] = {0x0EEE0EEE, Header(REG_TEXENV5_SOURCE, 0xF, 2, true), 0, 0, color, Header(0x0FB)};
    nngxAdd3DCommand(commands, sizeof(commands), true);
    CFLi_DrawQuad(NULL, &s_ProgramContext, 0, false, 0.0f, width, 0.0f, height, depth ? -198.0f : 0.0f);
}

// draws into a texture from now on
// 0x0017D074 (name is ours)
DECOMP_NOINLINE void CFLi_BindRenderTexture(CFLiTexHandle** texture, int width, int height, bool begin, u32 color, bool depth)
{
    CFLiTexHandle* handle = *texture;
    uptr address = handle->address;
    if (!(nngxGetVramStartAddr(MEM_VRAMA) <= address && address <= nngxGetVramEndAddr(MEM_VRAMA)) &&
        !(nngxGetVramStartAddr(MEM_VRAMB) <= address && address <= nngxGetVramEndAddr(MEM_VRAMB))) {
        int bits = s_TexFormatBits[handle->format];
        int w = handle->allocatedWidth;
        int h = handle->allocatedHeight;
        u32 size = 0;
        for (int i = 0; i < handle->mipLevels; i++) {
            size += w * h * bits / 8;
            w >>= 1;
            h >>= 1;
        }
        nngxUpdateBuffer(reinterpret_cast<const GLvoid*>(handle->address), size);
    }
    handle = *texture;
    GLenum format = RGB565;
    if (handle->format == 7) {
    } else if (handle->format == 9) {
        format = RGB5_A1;
    } else if (handle->format == 10) {
        format = RGBA4;
    } else if (handle->format == 11) {
        format = RGBA8;
    }
    CFLi_PutColorBuffer(handle->address, width, height, format);
    u32* buffer = GetCommandBuffer();
    u32* end = CFLi_PutViewport(buffer, width, height);
    nngxMoveCommandbufferPointer((reinterpret_cast<u8*>(end) - reinterpret_cast<u8*>(buffer)) & ~3);
    if (begin) {
        CFLi_BeginRenderTexture(width, height, color, depth);
    } else {
        CFLi_SetOrthoProjection(width, height);
    }
}

namespace {
// the registers of the texture units (names are ours, after 3dbrew)
const u32 REG_TEXUNIT0_BORDER_COLOR = 0x081;
const u32 REG_TEXUNIT0_TYPE = 0x08E;
const u32 REG_TEXUNIT1_BORDER_COLOR = 0x091;
const u32 REG_TEXUNIT1_TYPE = 0x096;
const u32 REG_TEXUNIT2_BORDER_COLOR = 0x099;
const u32 REG_TEXUNIT2_TYPE = 0x09E;

// the filters of the OpenGL ES API
const GLenum LINEAR = 0x2601;
const GLenum NEAREST_MIPMAP_LINEAR = 0x2702;
const GLenum LINEAR_MIPMAP_NEAREST = 0x2701;
const GLenum LINEAR_MIPMAP_LINEAR = 0x2703;
const GLenum TEXTURE2 = 0x84C2;
} // namespace

// the wrap modes of the GPU for CFLTexWrap (name is ours)
// 0x008A3CAC
const u8 s_WrapModes[4] = {0, 2, 3, 0};
// the formats of the GPU for CFLTexFmt (name is ours)
// 0x008A4970
const u8 s_TexFormats[16] = {0x0A, 0x07, 0x0B, 0x08, 0x09, 0x05, 0x06, 0x03, 0x01, 0x02, 0x04, 0x00, 0x0C, 0x0D, 0x04, 0x00};

// the texture to the texture unit of the context (without one the units are switched off); the sizes of the
// commands go to the three counters
// 0x0071403C (name is ours)
DECOMP_NOINLINE DECOMP_NOIPA void CFLi_BindTexture(const CFLiTexHandle* texture, const CFLProgramContext* context, GLenum filter, u32* sizeA,
                      u32* sizeB, u32* sizeC)
{
    if (texture->self == NULL) {
        u32* buffer = GetCommandBuffer();
        const u32 commands[4] = {0, Header(REG_TEXUNIT_CONFIG, 0x1), 0x10000, Header(REG_TEXUNIT_CONFIG, 0x4)};
        std::memcpy(buffer, commands, sizeof(commands));
        nngxMoveCommandbufferPointer(sizeof(commands));
        return;
    }
    GLenum unit = context->textureUnit;
    u32 reg;
    if (unit == TEXTURE1) {
        reg = REG_TEXUNIT1_BORDER_COLOR;
    } else if (unit == TEXTURE2) {
        reg = REG_TEXUNIT2_BORDER_COLOR;
    } else {
        reg = REG_TEXUNIT0_BORDER_COLOR;
    }
    if (texture->mipLevels < 2) {
        filter = LINEAR;
    }
    u32 count = (unit == TEXTURE0 ? 5 : 0) + 5;
    uptr address = nngxGetPhysicalAddr(texture->address);
    u32 commands[6];
    commands[0] = 0;
    commands[1] = Header(reg, 0xF, count - 1, true);
    commands[2] = (texture->allocatedWidth << 16) | texture->allocatedHeight;
    u32 mipLinear = filter == NEAREST_MIPMAP_LINEAR || filter == LINEAR_MIPMAP_LINEAR;
    u32 parameter = (mipLinear << 24) | (s_WrapModes[texture->wrapS] << 12) | (s_WrapModes[texture->wrapT] << 8) |
                    ((texture->format == 12 ? 2 : 0) << 4);
    u32 linear = filter == LINEAR || filter == LINEAR_MIPMAP_NEAREST || filter == LINEAR_MIPMAP_LINEAR;
    commands[3] = parameter | (linear << 2) | 2;
    commands[5] = address >> 3;
    commands[4] = (texture->mipLevels >= 2 ? texture->mipLevels - 1 : 0) << 16;
    nngxAdd3DCommand(commands, sizeof(commands), true);
    if (sizeA != NULL) {
        *sizeA = 20;
    }
    if (sizeB != NULL) {
        *sizeB = 12;
    }
    if (context->textureUnit == TEXTURE0) {
        const u32 rest[6] = {0, 0, 0, 0, 0, 0};
        nngxAdd3DCommand(rest, sizeof(rest), true);
    }
    unit = context->textureUnit;
    u32 enable;
    if (unit == TEXTURE1) {
        enable = 2;
    } else if (unit == TEXTURE2) {
        enable = 4;
    } else {
        enable = 1;
    }
    u32 type[6] = {0, 0, 0, Header(REG_TEXUNIT_CONFIG, 0x1), 0x10000, Header(REG_TEXUNIT_CONFIG, 0x4)};
    type[0] = s_TexFormats[texture->format];
    unit = context->textureUnit;
    if (unit == TEXTURE1) {
        type[1] = Header(REG_TEXUNIT1_TYPE);
    } else if (unit == TEXTURE2) {
        type[1] = Header(REG_TEXUNIT2_TYPE);
    } else {
        type[1] = Header(REG_TEXUNIT0_TYPE);
    }
    type[2] = enable;
    nngxAdd3DCommand(type, sizeof(type), true);
    if (sizeC != NULL) {
        *sizeC = 32;
        if (context->textureUnit == TEXTURE0) {
            *sizeC = 56;
        }
    }
}

// the order of the vertices of a part (name is ours)
// 0x008A3CC0
const u8 s_PartIndices[4] = {2, 1, 3, 0};

// a part of the face (eye, eyebrow, mouth ...) as a textured quad: 0 drawn left of the position, 1 right of it
// mirrored, 2 like 0 shifted (name is ours)
// 0x00170674 (name is ours)
DECOMP_NOINLINE void CFLi_DrawFacePart(const CFLiFacePart* part)
{
    const CFLProgramContext* context = &s_ProgramContext;
    s32 flip = part->flip;
    f32 x = part->x;
    f32 y = part->y;
    nn::math::MTX34 mtx;
    nn::math::MTX34 rotate;
    nn::math::MTX34 aspect;
    nn::math::MTX34 translate;
    CFL_GetScaleMatrix(&mtx, part->scaleX, part->scaleY, 1.0f);
    CFL_GetRotateMatrixRadian(&rotate, 'z', part->rotate * 2.0f * 3.1415927f * (1.0f / 360.0f));
    nn::math::ARMv6::MTX34MultAsm(&mtx, &rotate, &mtx);
    CFL_GetScaleMatrix(&aspect, 0x1.c77b920000000p-1f, 0x1.daf73c0000000p-1f, 1.0f);
    nn::math::ARMv6::MTX34MultAsm(&mtx, &aspect, &mtx);
    CFL_GetTranslateMatrix(&translate, x, y, 0.0f);
    nn::math::ARMv6::MTX34MultAsm(&mtx, &translate, &mtx);
    CFLi_SetModelMatrices(&mtx, NULL, true);

    f32 left = 0.0f;
    f32 u0 = 0.0f;
    f32 u1 = 0.0f;
    if (flip == 0) {
        left = -0.5f;
        u0 = 1.0f;
    } else if (flip == 1) {
        u1 = 1.0f;
    } else if (flip == 2) {
        left = -1.0f;
        u0 = 1.0f;
    }
    f32 right = left + 1.0f;
    f32 positions[12] = {right, -0.5f, 0.0f, right, 0.5f, 0.0f, left, 0.5f, 0.0f, left, -0.5f, 0.0f};
    f32 su = static_cast<f32>(part->texture.width) / static_cast<f32>(part->texture.allocatedWidth);
    f32 sv = static_cast<f32>(part->texture.height) / static_cast<f32>(part->texture.allocatedHeight);
    f32 texCoords[8] = {u0 * su, 1.0f * sv, u0 * su, 0.0f * sv, u1 * su, 0.0f * sv, u1 * su, 1.0f * sv};
    CFLi_BindTexture(&part->texture, context, LINEAR_MIPMAP_NEAREST, NULL, NULL, NULL);
    u32* buffer = GetCommandBuffer();
    u32* end = CFLi_PutImmediateVertices(buffer, context, 5, 4, s_PartIndices, positions, NULL, texCoords);
    nngxMoveCommandbufferPointer((reinterpret_cast<u8*>(end) - reinterpret_cast<u8*>(buffer)) & ~3);
    glGetError();
}

namespace {
// the registers of the vertex arrays and the drawing (names are ours, after 3dbrew)
const u32 REG_ATTRIBBUFFERS_LOC = 0x200;
const u32 REG_INDEXBUFFER_CONFIG = 0x227;
const u32 REG_NUMVERTICES = 0x228;
const u32 REG_DRAWELEMENTS = 0x22F;

// the types of the OpenGL ES API
const GLenum SHORT = 0x1402;
const GLenum UNSIGNED_SHORT = 0x1403;
} // namespace

// draws the shape with the attributes of the context: arrays from the vertex buffer, the others constant
// 0x00712A0C | nintendogs:callseq [tier A]
DECOMP_NOINLINE void CFLiResShape::DirectDraw(const CFLProgramContext* context) const
{
    if (primitiveNum == 0) {
        return;
    }
    uptr vertexAddress = nngxGetPhysicalAddr(reinterpret_cast<uptr>(vertices.address));
    uptr indexAddress = nngxGetPhysicalAddr(reinterpret_cast<uptr>(indices.address));
    uptr base = indexAddress;
    if (vertices.name != 0 && indexAddress > vertexAddress) {
        base = vertexAddress;
    }
    base &= ~15;
    u32 registers[3] = {context->attribute[0], context->attribute[1], context->attribute[2]};
    u32 formats[2] = {0, 0};
    u32 permutation[2] = {context->_48, context->_4C};
    u32 order[2] = {0, 0};
    u32 offset = vertices.name != 0 ? vertexAddress - base : 0;
    int count = 0;
    for (int i = 0; i < 3; i++) {
        const Attribute* attribute = &attributes[i];
        if (attribute->index == -1) {
            continue;
        }
        u32 format;
        if (type == SHORT) {
            format = attribute->components == 2 ? 6 : 10;
        } else {
            format = attribute->components == 2 ? 7 : 11;
        }
        int shift = (count % 8) * 4;
        formats[count / 8] |= format << shift;
        permutation[count / 8] |= registers[i] << shift;
        order[count / 8] |= count << shift;
        count++;
    }
    if (count != 0) {
        order[1] |= (count << 28) | (stride << 16);
    }
    int fixedIndex = count;
    for (int i = 0; i < 3; i++) {
        if (attributes[i].index != -1) {
            continue;
        }
        formats[1] |= 1 << (count + 16);
        permutation[count / 8] |= registers[i] << ((count % 8) * 4);
        count++;
    }
    s32 total = context->_38;
    for (int i = 0; i < total - 3; i++) {
        formats[1] |= 1 << (count + 16);
        count++;
    }
    formats[1] |= (total - 1) << 28;

    u32 commands[48] = {};
    commands[1] = Header(REG_VSH_INPUTBUFFER_CONFIG, 0xB);
    commands[3] = Header(REG_VSH_NUM_ATTR);
    commands[5] = Header(REG_ATTRIBBUFFERS_LOC, 0xF, 38, true);
    commands[45] = Header(REG_VSH_ATTRIBUTES_PERMUTATION_LOW);
    commands[47] = Header(REG_VSH_ATTRIBUTES_PERMUTATION_HIGH);
    commands[0] = 0xA0000000 | ((total - 1) & 15);
    commands[4] = (base >> 4) << 1;
    commands[2] = (context->_38 - 1) & 15;
    commands[6] = formats[0];
    commands[7] = formats[1];
    commands[8] = offset;
    commands[9] = order[0];
    commands[10] = order[1];
    commands[44] = permutation[0];
    commands[46] = permutation[1];
    nngxAdd3DCommand(commands, sizeof(commands), true);

    for (int i = 0; i < 3; i++) {
        const Attribute* attribute = &attributes[i];
        if (attribute->index != -1) {
            continue;
        }
        u32 x24 = nn::gr::CTR::Float32ToFloat24(attribute->value[0]);
        u32 y24 = nn::gr::CTR::Float32ToFloat24(attribute->value[1]);
        u32 z24 = nn::gr::CTR::Float32ToFloat24(attribute->value[2]);
        u32 w24 = nn::gr::CTR::Float32ToFloat24(1.0f);
        u32 fixed[6] = {static_cast<u32>(fixedIndex), Header(REG_FIXEDATTRIB_INDEX, 0xF, 3, true),
                        (w24 << 8) | (z24 >> 16), (z24 << 16) | (y24 >> 8), x24 | (y24 << 24), 0};
        nngxAdd3DCommand(fixed, sizeof(fixed), true);
        fixedIndex++;
    }

    for (int i = 0; i < primitiveNum; i++) {
        const Primitive* primitive = &primitives[i];
        u32 mode;
        if (primitive->mode == 5) {
            mode = 1;
        } else if (primitive->mode == 6) {
            mode = 2;
        } else {
            mode = 3;
        }
        u32 draw[24] = {
            0, Header(REG_229, 0x2), 0, Header(REG_253, 0x2), 0, Header(REG_25E, 0x2), 1, Header(REG_25F),
            0, Header(REG_INDEXBUFFER_CONFIG), 0, Header(REG_NUMVERTICES), 0, Header(REG_245, 0x1),
            1, Header(REG_DRAWELEMENTS), 1, Header(REG_245, 0x1), 1, Header(REG_RESTART_PRIMITIVE),
            0, Header(REG_25E, 0x8), 0, Header(REG_25E, 0x8),
        };
        draw[0] = primitive->mode == 4 ? 0x100 : 0;
        draw[4] = mode << 8;
        draw[2] = primitive->mode == 4 ? 0x100 : 0;
        draw[8] = (primitive->offset + (indexAddress - base)) | (indexType == UNSIGNED_SHORT ? 0x80000000 : 0);
        draw[10] = primitive->count;
        nngxAdd3DCommand(draw, sizeof(draw), true);
    }
}

// binds a texture of the model while the command list is saved and remembers where its address and mip levels are
// in the list (CFL_SetExpression changes them)
// 0x00179778 (name is ours)
DECOMP_NOINLINE u32* CFLi_BindModelTexture(CFLCharModel* model, CFLiTexHandle* texture, u32 offset)
{
    CFLiCharModelData* data = model->data;
    const u32 marker[4] = {0, Header(REG_TEXUNIT_CONFIG, 0x0, 2), 0, 0};
    nngxAdd3DCommand(marker, sizeof(marker), true);
    GLint used;
    nngxGetCmdlistParameteri(CMDLIST_USED_BUFSIZE, &used);
    offset += used & ~3;
    s32 sizeA = -1;
    s32 sizeB = -1;
    s32 sizeC = -1;
    CFLi_BindTexture(texture, &s_ProgramContext, model->filter, reinterpret_cast<u32*>(&sizeA),
                     reinterpret_cast<u32*>(&sizeB), reinterpret_cast<u32*>(&sizeC));
    u32 addressOffset = 0;
    if (sizeA != -1) {
        addressOffset = (sizeA & ~3) + offset;
    }
    if (sizeB != -1 && texture->mipLevels >= 2) {
        data->lodCommands[data->lodCommandNum++] = reinterpret_cast<u32*>((sizeB & ~3) + offset);
    }
    if (sizeC != -1) {
        data->typeCommands[data->typeCommandNum++] = reinterpret_cast<u32*>((sizeC & ~3) + offset);
    }
    return reinterpret_cast<u32*>(addressOffset);
}

namespace {
const u32 REG_DEPTHMAP_SCALE = 0x04D;
const u32 REG_DEPTHMAP_OFFSET = 0x04E;
const u32 REG_DEPTHMAP_ENABLE = 0x06D;

// the target of glGetTexParameteriv (value of the API) and the parameter for the address of the data of the bound
// texture (name is ours)
const GLenum TEXTURE_2D = 0x0DE1;
const GLenum TEXTURE_DATA_ADDRESS = 0x6790;
} // namespace

// the favorite colors (RGBA8, background of icons; name is ours)
// 0x008A4600
const u32 s_FavoriteColors[12] = {0xFF141ED2, 0xFF196EFF, 0xFF20D8FF, 0xFF20D278, 0xFF307800, 0xFFB4480A,
                                  0xFFDEAA3C, 0xFF7D5AF5, 0xFFAD2873, 0xFF183848, 0xFFE0E0E0, 0xFF141818};

// the light of icons (two vectors; name is ours)
// 0x008A4434
const f32 s_IconLight[8] = {0x1p-13f, 0.0f, 0.0f, 0.0f, 0.0f, 0x1p-13f, 0.0f, 0.0f};

// the fragment operations and combiners of icons (names are ours)
// 0x008A3FD4
const u32 s_IconCommands[40] = {
    0x100, Header(REG_COLOR_OPERATION, 0x2), 0x71760000, Header(REG_BLEND_FUNC), 0x61, Header(REG_FRAGOP_ALPHA_TEST, 0x3),
    0x1F41, Header(REG_DEPTH_COLOR_MASK, 0x1), 0x3000000, Header(REG_126, 0x8), 0, Header(REG_STENCIL_TEST),
    0x0FFF0FFF, Header(REG_TEXENV1_SOURCE, 0xF, 2, true), 0, 0, 0x0FFF0FFF, Header(REG_TEXENV3_SOURCE, 0xF, 2, true), 0, 0,
    0x0FFF0FFF, Header(REG_TEXENV4_SOURCE, 0xF, 2, true), 0, 0, 0x0FFF0FFF, Header(REG_TEXENV5_SOURCE, 0xF, 2, true), 0, 0,
    0, Header(0x0C4), 0, Header(0x0CC), 0, Header(0x0D4), 0, Header(0x0DC), 0, Header(0x0F4), 0, Header(0x0FC),
};
// the combiner stage 2 of icons without and with the alpha of the texture
// 0x008A4074
const u32 s_IconTexEnv2[4] = {0x0FFF0FFF, Header(REG_TEXENV2_SOURCE, 0xF, 2, true), 0, 0};
// 0x008A4084
const u32 s_IconTexEnv2Alpha[4] = {0x0FFF021F, Header(REG_TEXENV2_SOURCE, 0xF, 2, true), 0, 8};

extern "C" {
// draws a model into the texture (the bound one for 0) as an icon
// 0x001539C8 (name is ours, after the order of the functions)
DECOMP_NOINLINE s32 CFL_MakeIconWithModel(GLuint texture, const CFLCharModel* model, int expression,
                                          const CFLIconParam* param)
{
    if (model == NULL || model->expressionTextures[expression] == NULL) {
        return CFL_RESULT_INVALID_ARGUMENT;
    }
    if (texture == 0) {
        GLint bound;
        glGetTexParameteriv(TEXTURE_2D, TEXTURE_DATA_ADDRESS, &bound);
        texture = bound;
    }
    CFLi_PutColorBuffer(texture, param->width, param->height, param->format);
    u32* buffer = GetCommandBuffer();
    u32* end = CFLi_PutViewport(buffer, param->width, param->height);
    nngxMoveCommandbufferPointer((reinterpret_cast<u8*>(end) - reinterpret_cast<u8*>(buffer)) & ~3);
    buffer = GetCommandBuffer();
    u32 depth[6] = {0, Header(REG_DEPTHMAP_SCALE), 0, Header(REG_DEPTHMAP_OFFSET), 1, Header(REG_DEPTHMAP_ENABLE)};
    depth[0] = nn::gr::CTR::Float32ToFloat24(-1.0f);
    depth[2] = nn::gr::CTR::Float32ToFloat24(0.0f);
    std::memcpy(buffer, depth, sizeof(depth));
    nngxMoveCommandbufferPointer(sizeof(depth));
    if (param->background != 2) {
        u32 color;
        if (param->background == 1) {
            color = param->color;
        } else {
            s32 favorite = model->data->info.favoriteColor;
            if (favorite > 11) {
                favorite = 11;
            }
            color = s_FavoriteColors[favorite];
        }
        CFLi_BeginRenderTexture(param->width, param->height, color & 0xFFFFFF, true);
    }
    nn::math::MTX44 projection;
    nn::math::MTX34 view;
    CFLi_GetIconMtx(&projection, &view, param->width, param->height);
    CFLi_SetUniformMtx44(&projection.m[0][0]);
    CFLCharModel icon;
    std::memcpy(static_cast<void*>(&icon), model, sizeof(icon));
    s32 expressionBefore = icon.expression;
    CFL_SetExpression(&icon, expression);
    if (s_ProgramContext.normalIndex != s_ProgramContext.modelViewIndex) {
        nn::math::MTX34 normal;
        nn::math::ARMv6::MTX34InverseAsm(&normal, &view);
        nn::math::ARMv6::MTX34TransposeAsm(&normal, &normal);
        CFL_SetMtxPosNrm(&icon, &view, &normal);
    } else {
        CFL_SetMtxPosNrm(&icon, &view, &view);
    }
    bool textureAlpha = param->textureAlpha;
    CFLi_SetUniformMtx24(s_IconLight);
    nngxAdd3DCommand(s_IconCommands, sizeof(s_IconCommands), true);
    if (!textureAlpha) {
        nngxAdd3DCommand(s_IconTexEnv2, sizeof(s_IconTexEnv2), true);
    } else {
        nngxAdd3DCommand(s_IconTexEnv2Alpha, sizeof(s_IconTexEnv2Alpha), true);
    }
    if (param->callback != NULL) {
        param->callback(param->callbackArg);
    }
    if (!param->skipOpa) {
        CFLi_SetModelMatrices(&icon.posMtx, &icon.nrmMtx, true);
        CFLiSavedCmdlist* list = &icon.data->opa;
        nngxUseSavedCmdlist(list->name, 0, list->size, list->requestOffset, list->requestCount, 0, list->_C);
        glGetError();
        glGetError();
    }
    CFLi_SetModelMatrices(&icon.posMtx, &icon.nrmMtx, true);
    CFLiSavedCmdlist* list = &icon.data->xlu;
    nngxUseSavedCmdlist(list->name, 0, list->size, list->requestOffset, list->requestCount, 0, list->_C);
    glGetError();
    glGetError();
    if (CFLi_GetManager()->iconDrawDoneCallback != NULL) {
        CFLi_GetManager()->iconDrawDoneCallback();
    }
    CFL_SetExpression(&icon, expressionBefore);
    return CFL_RESULT_SUCCESS;
}

// makes a model of the Mii in the size of the icon and draws it
// 0x001559F0 | nintendogs:callgraph [tier C]
DECOMP_NOINLINE void CFLi_MakeIcon(GLuint texture, const CFLiCharInfo* info, int expression, const CFLIconParam* param)
{
    if (texture == 0) {
        GLint bound;
        glGetTexParameteriv(TEXTURE_2D, TEXTURE_DATA_ADDRESS, &bound);
        texture = bound;
    }
    u32 mask = 1 << expression;
    s32 resolution;
    if ((param->width > 128 || param->height > 128) && CFLi_Get_C1()) {
        resolution = 256;
    } else {
        resolution = (param->width > 64 || param->height > 64) ? 128 : 64;
    }
    if (CFLi_IsHDModeEnabled()) {
        if ((param->width > 512 || param->height > 512) && CFLi_Get_C1()) {
            resolution = 1024;
        } else if (param->width > 256 || param->height > 256) {
            resolution = 512;
        }
    }
    void* buffer = CFLi_AllocateMemory(CFL_GetModelBufferSize(mask), 32);
    CFLCharModel model;
    CFLi_InitCharModel(&model, info, 0x10000, buffer, resolution, mask, 0x10000000);
    CFL_SetExpression(&model, expression);
    CFL_MakeIconWithModel(texture, &model, expression, param);
    nngxSplitDrawCmdlist();
    glGetError();
    nngxRunCmdlist();
    nngxWaitCmdlistDone();
    nngxClearCmdlist();
    glGetError();
    CFL_DeleteCharModel(&model);
    CFLi_FreeMemory(buffer);
}

// 0x00153948 | nintendogs:bytes [tier B]
s32 CFL_MakeIcon(GLuint texture, int source, const void* data, u16 index, int expression, const CFLIconParam* param)
{
    CFLiCharInfo info;
    s32 status = CFLi_PickupCharInfo(&info, source, data, index);
    if (status != CFL_RESULT_SUCCESS) {
        return status;
    }
    if (!CFL_IsResourceCached()) {
        return CFL_RESULT_NOT_AVAILABLE;
    }
    bool replaced = CFLi_ReplaceProhibitedCharInfo(&info);
    CFLi_MakeIcon(texture, &info, expression, param);
    if (!replaced && source != 0 && source != 2) {
        CFLi_AddRecentDBCharInfo(&info);
    }
    return status;
}
} // extern "C"

extern "C" {
// sets up the model of a Mii in the buffer: the textures of the expressions, the face and the command lists
// 0x00155060 | nintendogs:callgraph [tier C]
DECOMP_NOINLINE void CFLi_InitCharModel(CFLCharModel* model, const CFLiCharInfo* info, u32 area, void* buffer,
                                        s32 resolution, u32 expressionMask, u32 flags)
{
    u32* commands = GetCommandBuffer();
    std::memcpy(commands, s_TexEnvSources, sizeof(s_TexEnvSources));
    nngxMoveCommandbufferPointer(sizeof(s_TexEnvSources));
    commands = GetCommandBuffer();
    std::memcpy(commands, s_TexEnvScales, sizeof(s_TexEnvScales));
    nngxMoveCommandbufferPointer(sizeof(s_TexEnvScales));
    CFLiCharModelData* data = static_cast<CFLiCharModelData*>(buffer);
    model->_64 = 0;
    model->filter = LINEAR_MIPMAP_NEAREST;
    model->data = data;
    std::memcpy(&data->info, info, sizeof(CFLiCharInfo));
    data->_68C = flags;
    data->expressionMask = expressionMask;
    data->_67C = area;
    data->self = data;
    data->resolution = resolution;
    CFLiTexHandle* texture = reinterpret_cast<CFLiTexHandle*>(data + 1);
    for (int i = 0; i < CFL_EXPRESSION_MAX; i++) {
        if (expressionMask & (1 << i)) {
            model->expressionTextures[i] = texture++;
        } else {
            model->expressionTextures[i] = NULL;
        }
    }
    s32 size;
    switch (resolution) {
    case 64:
    case 96:
    default:
        size = 64;
        break;
    case 128:
    case 224:
        size = 128;
        break;
    case 256:
    case 480:
        size = 256;
        break;
    case 512:
        size = 512;
        break;
    case 1024:
        size = 1024;
        break;
    }
    bool first = false;
    for (int i = 0; i < CFL_EXPRESSION_MAX; i++) {
        if (!(expressionMask & (1 << i))) {
            continue;
        }
        model->expressionTextures[i]->Create(size, size, 1, CFL_TEX_WRAP_0, CFL_TEX_WRAP_0, static_cast<CFLTexFmt>(10),
                                             NULL, 0x10000, 0);
        if (!first) {
            first = true;
            model->expression = i;
        }
    }
    CFLiFaceParts parts;
    CFLi_MakeFaceTextures(&parts, info, model, resolution);
    CFLi_InitShapes(data, &parts);
    CFLi_MakeModelCmdlists(model);
}

// 0x001534BC | nintendogs:bytes [tier B]
s32 CFL_InitCharModel(CFLCharModel* model, int source, const void* data, u16 index, u32 area, void* buffer,
                      s32 resolution, u32 expressionMask, u32 flags)
{
    CFLiCharInfo info;
    s32 status = CFLi_PickupCharInfo(&info, source, data, index);
    if (status != CFL_RESULT_SUCCESS) {
        return status;
    }
    if (!CFL_IsResourceCached()) {
        return CFL_RESULT_NOT_AVAILABLE;
    }
    bool replaced = CFLi_ReplaceProhibitedCharInfo(&info);
    CFLi_InitCharModel(model, &info, area, buffer, resolution, expressionMask, flags);
    if (!replaced && source != 0 && source != 2) {
        CFLi_AddRecentDBCharInfo(&info);
    }
    return status;
}
} // extern "C"

// (names are ours)
// 0x008A46E0
const f32 s_ZeroVector[3] = {0.0f, 0.0f, 0.0f};
// 0x008A46EC
const f32 s_HairRotate64a[3] = {0x1.657184p-2f, 0.0f, 0.0f};
// 0x008A46F8
const f32 s_HairRotate64b[3] = {-0x1.becde6p-2f, 0.0f, 0.0f};

namespace {
// the types of the OpenGL ES API
const GLenum UNSIGNED_BYTE = 0x1401;
const GLenum FLOAT = 0x1406;

// the purpose of vertex buffers for the allocator of nngx (name is ours)
const GLenum MEM_VERTEXBUFFER = 0x102;

// the components of position, normal and texture coordinate in the archive (name is ours)
// 0x008A4704
const s32 s_ShapeComponents[3] = {3, 3, 2};

// copies a buffer to VRAM or FCRAM (inline; name is ours)
inline void UploadBuffer(CFLiVertexBuffer* buffer, GLenum area, const void* data, u32 size)
{
    buffer->allocated = 1;
    buffer->area = area;
    buffer->name = reinterpret_cast<GLuint>(buffer);
    GxAllocateFunction allocateFunction;
    nngxGetAllocator(&allocateFunction, NULL);
    buffer->address = allocateFunction(area, MEM_VERTEXBUFFER, buffer->name, size);
    if (data != NULL) {
        if (area != 0x10000) {
            nngxlowStoreDataCache(reinterpret_cast<uptr>(data), size);
            nngxAddVramDmaCommand(data, buffer->address, size);
            return;
        }
        std::memcpy(buffer->address, data, size);
        nngxlowStoreDataCache(reinterpret_cast<uptr>(buffer->address), size);
        return;
    }
    if (area == 0x10000) {
        nngxlowStoreDataCache(reinterpret_cast<uptr>(buffer->address), size);
    }
}

// runs the command list so that the copies to VRAM are done (inline; name is ours)
inline void FlushCommands()
{
    nngxSplitDrawCmdlist();
    glGetError();
    nngxRunCmdlist();
    nngxWaitCmdlistDone();
    nngxClearCmdlist();
    glGetError();
}
} // namespace

// loads a shape of the archive into the model: positions scaled and moved (in 1/256), mirrored for flip; for the
// hair (6) only the triangles in the cells of the mask stay
// 0x0016E2E0 (name is ours)
DECOMP_NOINLINE void CFLi_LoadShape(CFLiCharModelData* data, int kind, int index, const f32* offset, f32 scaleX,
                                    f32 scaleY, bool flip, const CFLiFaceParts* mask)
{
    CFLiResShape* shape = &data->shapes[kind];
    u8* image = NULL;
    s32 imageSize = CFLi_ReadArchiveImage(NULL, kind, index);
    if (imageSize > 0) {
        image = static_cast<u8*>(CFLi_AllocateMemory(imageSize, 32));
        CFLi_ReadArchiveImage(image, kind, index);
    }
    if (imageSize <= 0 || image == NULL) {
        shape->primitiveNum = 0;
        return;
    }
    const u8* p = image;
    if (kind == 2) {
        for (int i = 0; i < 3; i++) {
            data->facelinePos[0][i] = reinterpret_cast<const f32*>(p)[i];
        }
        p += 12;
        for (int i = 0; i < 3; i++) {
            data->facelinePos[1][i] = reinterpret_cast<const f32*>(p)[i];
        }
        p += 12;
        for (int i = 0; i < 3; i++) {
            data->facelinePos[2][i] = reinterpret_cast<const f32*>(p)[i];
        }
        p += 12;
        std::memcpy(data->_508, data->facelinePos[0], sizeof(data->_508));
        std::memcpy(data->hairPos[1], s_ZeroVector, sizeof(s_ZeroVector));
        std::memcpy(data->hairPos[3], s_ZeroVector, sizeof(s_ZeroVector));
        std::memcpy(data->hairPos[5], s_ZeroVector, sizeof(s_ZeroVector));
        std::memcpy(data->hairPos[0], s_ZeroVector, sizeof(s_ZeroVector));
        std::memcpy(data->hairPos[2], s_ZeroVector, sizeof(s_ZeroVector));
        std::memcpy(data->hairPos[4], s_ZeroVector, sizeof(s_ZeroVector));
    } else if (kind == 5) {
        for (int i = 0; i < 18; i++) {
            (&data->hairPos[0][0])[i] = reinterpret_cast<const f32*>(p)[i];
        }
        p += 72;
        if (index == 64) {
            std::memcpy(data->hairPos[0], s_HairRotate64a, sizeof(s_HairRotate64a));
            std::memcpy(data->hairPos[4], s_HairRotate64b, sizeof(s_HairRotate64b));
        }
    }
    const u16* header = reinterpret_cast<const u16*>(p);
    u32 counts[3] = {header[0], header[1], header[2]};
    u16 flags = header[3];
    bool shortIndices = (flags & 0x8000) >> 15;
    shape->primitiveNum = flags & 0x7FFF;
    shape->indexType = shortIndices ? UNSIGNED_SHORT : UNSIGNED_BYTE;
    p += 8;
    if (shape->primitiveNum > 0) {
        f32 scaleZ = (scaleX + scaleY) * 0.5f;
        if ((kind == 8 || kind == 7) && (data->_68C & 8) && scaleZ > 1.1f) {
            scaleZ = 1.1f;
        }
        f32 ox;
        f32 oy;
        f32 oz;
        if (offset != NULL) {
            ox = offset[0] * 256.0f;
            oy = offset[1] * 256.0f;
            oz = offset[2] * 256.0f;
        } else {
            ox = 0.0f;
            oy = 0.0f;
            oz = 0.0f;
        }
        s32 components[3] = {s_ShapeComponents[0], s_ShapeComponents[1], s_ShapeComponents[2]};
        const u8* arrays[3] = {NULL, NULL, NULL};
        s32 offsets[3] = {0, 0, 0};
        const u8* vertexData = p;
        shape->type = SHORT;
        shape->stride = 0;
        if (counts[0] >= 2) {
            arrays[0] = p;
            shape->stride += components[0] * 2;
        }
        if (counts[1] >= 2) {
            arrays[1] = p;
            offsets[1] = shape->stride;
            shape->stride += components[1] * 2;
        }
        if (counts[2] >= 2) {
            arrays[2] = p;
            offsets[2] = shape->stride;
            shape->stride += components[2] * 2;
        }
        u32 vertexNum = counts[0] > counts[1] ? counts[0] : counts[1];
        if (counts[2] > vertexNum) {
            vertexNum = counts[2];
        }
        const u8* q = p + vertexNum * shape->stride;
        if (counts[0] == 1) {
            arrays[0] = q;
            q += components[0] * 2;
        }
        if (counts[1] == 1) {
            arrays[1] = q;
            q += components[1] * 2;
        }
        if (counts[2] == 1) {
            arrays[2] = q;
            q += components[2] * 2;
        }
        // positions
        s16* v = const_cast<s16*>(reinterpret_cast<const s16*>(arrays[0]));
        for (u32 i = 0; i < counts[0]; i++) {
            if (flip) {
                v[0] = -v[0];
            }
            s16 x = static_cast<s32>(ox + v[0] * scaleX);
            v[0] = x;
            s16 y = static_cast<s32>(oy + v[1] * scaleY);
            v[1] = y;
            s16 z = static_cast<s32>(oz + v[2] * scaleZ);
            v[2] = z;
            if (s_Axis[2]) {
                x = -x;
            }
            if (s_Axis[0]) {
                y = -y;
            }
            if (s_Axis[1]) {
                z = -z;
            }
            v[s_Axis[5]] = x;
            v[s_Axis[3]] = y;
            v[s_Axis[4]] = z;
            v = reinterpret_cast<s16*>(reinterpret_cast<u8*>(v) + (shape->stride & ~1));
        }
        // normals
        v = const_cast<s16*>(reinterpret_cast<const s16*>(arrays[1] + (offsets[1] & ~1)));
        for (u32 i = 0; i < counts[1]; i++) {
            if (flip) {
                v[0] = -v[0];
            }
            s16 x = s_Axis[2] ? -v[0] : v[0];
            s16 y = s_Axis[0] ? -v[1] : v[1];
            s16 z = v[2];
            if (s_Axis[1]) {
                z = -z;
            }
            v[s_Axis[5]] = x;
            v[s_Axis[3]] = y;
            v[s_Axis[4]] = z;
            v = reinterpret_cast<s16*>(reinterpret_cast<u8*>(v) + (shape->stride & ~1));
        }
        for (int a = 0; a < 3; a++) {
            if (shape->type == FLOAT) {
                offsets[a] *= 2;
            }
            CFLiResShape::Attribute* attribute = &shape->attributes[a];
            attribute->components = components[a];
            if (counts[a] >= 2) {
                attribute->index = offsets[a];
            } else {
                attribute->index = -1;
                attribute->value[0] = 0.0f;
                attribute->value[1] = 0.0f;
                attribute->value[2] = 0.0f;
                if (counts[a] == 1) {
                    const s16* value = reinterpret_cast<const s16*>(arrays[a]);
                    for (int j = 0; j < components[a]; j++) {
                        attribute->value[j] = value[j];
                    }
                }
            }
        }
        shape->vertices.name = 0;
        if (shape->stride != 0) {
            if (shape->type == FLOAT) {
                shape->stride *= 2;
                f32* floats = static_cast<f32*>(CFLi_AllocateMemory(shape->stride * vertexNum, 4));
                u32 floatNum = (shape->stride * vertexNum) >> 2;
                const s16* shorts = reinterpret_cast<const s16*>(vertexData);
                for (u32 i = 0; i < floatNum; i++) {
                    floats[i] = shorts[i];
                }
                vertexData = reinterpret_cast<const u8*>(floats);
            }
            UploadBuffer(&shape->vertices, data->_67C, vertexData, shape->stride * vertexNum);
            if (data->_67C != 0x10000) {
                FlushCommands();
            }
            if (shape->type == FLOAT) {
                CFLi_FreeMemory(const_cast<u8*>(vertexData));
            }
        }
        // the primitives and the indices
        const u16* primitive = reinterpret_cast<const u16*>(q);
        u32 indexNum = 0;
        for (int i = 0; i < shape->primitiveNum; i++) {
            shape->primitives[i].mode = primitive[0];
            shape->primitives[i].count = primitive[1];
            shape->primitives[i].offset = indexNum;
            indexNum += primitive[1];
            primitive += 2;
        }
        const u8* indices = reinterpret_cast<const u8*>(primitive);
        if (kind == 6 && shape->primitiveNum == 1 && mask != NULL && !(data->_68C & 0x10000000)) {
            u32 indexSize = shortIndices ? 2 : 1;
            u32 triangleSize = indexSize * 3;
            u32 triangleNum = shape->primitives[0].count / 3;
            const u8* cells = indices + shape->primitives[0].count * indexSize;
            const u8* read = indices;
            u8* write = const_cast<u8*>(indices);
            u32 kept = 0;
            for (u32 t = 0; t < triangleNum; t++) {
                u8 b0 = cells[t * 2];
                u8 b1 = cells[t * 2 + 1];
                for (int y = b1 >> 4; y < (b1 & 15) + 1; y++) {
                    for (int x = b0 >> 4; x < (b0 & 15) + 1; x++) {
                        if (mask->cells[y * 16 + x] != 0) {
                            if (write != read) {
                                std::memcpy(write, read, triangleSize);
                            }
                            kept += 3;
                            write += triangleSize;
                            goto next;
                        }
                    }
                }
            next:
                read += triangleSize;
            }
            if (kept == 0) {
                kept = 3;
            }
            indexNum = kept;
            shape->primitives[0].count = kept;
        }
        UploadBuffer(&shape->indices, data->_67C, indices, indexNum * (shortIndices ? 2 : 1));
        if (data->_67C != 0x10000) {
            FlushCommands();
        }
    }
    CFLi_FreeMemory(image);
}

// the colors of the hair and the beard (RGBA8; names are ours)
// 0x008A3F84
const u32 s_HairColors[8] = {0xFF181A1E, 0xFF102040, 0xFF0A185C, 0xFF143A7C,
                             0xFF807878, 0xFF103E4E, 0xFF185888, 0xFF4AA0D0};
// the color of the hair and beard when the flags of the model ask for it
// 0x009761C4
u32 s_SpecialHairColor = 0xFFFFFFFF;
// the color of the eyes when the flags of the model ask for it
// 0x009761C8
u32 s_SpecialEyeColor = 0xFF000000;

// the light of the faceline texture (name is ours)
// 0x008A43F4
const f32 s_FacelineLight[8] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};

// the shapes of the head and the faceline texture with makeup, wrinkles and beard
// 0x00175AF4 (name is ours)
DECOMP_NOINLINE void CFLi_InitShapes(CFLiCharModelData* data, const CFLiFaceParts* mask)
{
    const CFLiCharInfo* info = &data->info;
    u32 mode = data->_68C & 3;
    if (data->_68C & 0x20000000) {
        data->hairColor = s_SpecialHairColor;
    } else {
        data->hairColor = s_HairColors[info->hairColor];
    }
    for (int i = 0; i < CFL_SHAPE_NUM; i++) {
        data->shapes[i].primitiveNum = 0;
    }
    data->textures[0].self = NULL;
    data->textures[1].self = NULL;
    data->textures[3].self = NULL;
    data->textures[2].self = NULL;
    int hair = (mode == 1 ? 1 : 0) + info->hairType * 2;
    CFLi_LoadShape(data, 2, info->faceType, NULL, 1.0f, 1.0f, false, NULL);
    if (mode != 2) {
        CFLi_LoadShape(data, 1, hair, data->facelinePos[0], 1.0f, 1.0f, info->hairFlip & 0xFF, NULL);
        CFLi_LoadShape(data, 5, hair, data->facelinePos[0], 1.0f, 1.0f, info->hairFlip & 0xFF, NULL);
        CFLi_LoadShape(data, 3, hair, data->facelinePos[0], 1.0f, 1.0f, info->hairFlip & 0xFF, NULL);
    }
    if (info->beardType < 4) {
        CFLi_LoadShape(data, 0, info->beardType, data->facelinePos[2], 1.0f, 1.0f, false, NULL);
    }
    f32 nose[3];
    nose[0] = data->facelinePos[1][0];
    nose[1] = data->facelinePos[1][1] + (info->noseY - 8) * -1.5f;
    nose[2] = data->facelinePos[1][2];
    f32 noseScale = 0.4f + info->noseScale * 0.175f;
    CFLi_LoadShape(data, 8, info->noseType, nose, noseScale, noseScale, false, NULL);
    CFLi_LoadShape(data, 7, info->noseType, nose, noseScale, noseScale, false, NULL);
    CFLi_LoadShape(data, 6, info->faceType, NULL, 1.0f, 1.0f, false, mask);
    if (info->glassType != 0) {
        f32 glass[3];
        glass[0] = data->facelinePos[1][0];
        glass[1] = data->facelinePos[1][1] + (5.0f + (info->glassY - 11) * -1.5f);
        glass[2] = data->facelinePos[1][2] + 2.0f;
        f32 glassScale = 0.4f + info->glassScale * 0.15f;
        CFLi_LoadShape(data, 4, 0, glass, glassScale, glassScale, false, NULL);
    }
    if (data->shapes[1].primitiveNum != 0) {
        CFLi_LoadArchiveTexture(&data->textures[1], 9, info->hairType, data->_67C);
    }
    if (data->shapes[7].primitiveNum != 0) {
        CFLi_LoadArchiveTexture(&data->textures[2], 19, info->noseType, data->_67C);
    }
    if (info->glassType != 0) {
        CFLi_LoadArchiveTexture(&data->textures[3], 15, info->glassType, data->_67C);
    }

    // the faceline texture
    const CFLProgramContext* context = &s_ProgramContext;
    s32 width = 32;
    s32 height = 64;
    u32 format = 7;
    if (CFLi_IsHDModeEnabled()) {
        width = 256;
        height = 512;
        format = 11;
    }
    s32 faceColor = info->faceColor;
    if (faceColor > 5) {
        faceColor = 5;
    }
    u32 color = s_FacelineColors[faceColor];
    CFLiTexHandle* faceline = &data->textures[0];
    faceline->Create(width, height, 1, static_cast<CFLTexWrap>(2), CFL_TEX_WRAP_0, static_cast<CFLTexFmt>(format), NULL,
                     data->_67C, 0);
    CFLi_BindRenderTexture(&faceline, width, height, true, color, false);
    CFLi_SetUniformMtx24(s_FacelineLight);
    CFLiTexHandle wrinkle;
    CFLiTexHandle makeup;
    CFLiTexHandle beard;
    wrinkle.self = NULL;
    makeup.self = NULL;
    beard.self = NULL;
    if (info->faceTex != 0) {
        CFLi_LoadArchiveTexture(&wrinkle, 13, info->faceTex, 0x10000);
    }
    if (info->faceMake != 0) {
        CFLi_LoadArchiveTexture(&makeup, 14, info->faceMake, 0x10000);
    }
    if (info->beardType >= 4) {
        CFLi_LoadArchiveTexture(&beard, 12, info->beardType - 3, 0x10000);
    }
    CFLi_PutAlphaTest(context, true);
    CFLi_PutFragmentOperation(true, 0, false, true, 0x10760000);
    if (makeup.self != NULL) {
        CFLi_PutTexEnv(context, 1, true);
        CFLi_BindTexture(&makeup, context, LINEAR_MIPMAP_NEAREST, NULL, NULL, NULL);
        CFLi_DrawQuad(NULL, context, 0, true, 0.0f, width, 0.0f, height, 0.0f);
    }
    if (wrinkle.self != NULL) {
        CFLi_PutTexEnv(context, 3, true);
        CFLi_BindTexture(&wrinkle, context, LINEAR_MIPMAP_NEAREST, NULL, NULL, NULL);
        const u32 commands[2] = {0xFF000000, Header(REG_TEXENV0_COLOR)};
        nngxAdd3DCommand(commands, sizeof(commands), true);
        CFLi_DrawQuad(NULL, context, 0, true, 0.0f, width, 0.0f, height, 0.0f);
    }
    if (beard.self != NULL) {
        CFLi_PutTexEnv(context, 3, true);
        CFLi_BindTexture(&beard, context, LINEAR_MIPMAP_NEAREST, NULL, NULL, NULL);
        u32 beardColor;
        if (data->_68C >> 31) {
            beardColor = s_SpecialHairColor;
        } else {
            beardColor = s_HairColors[info->beardColor];
        }
        const u32 commands[2] = {beardColor, Header(REG_TEXENV0_COLOR)};
        nngxAdd3DCommand(commands, sizeof(commands), true);
        CFLi_DrawQuad(NULL, context, 0, true, 0.0f, width, 0.0f, height, 0.0f);
    }
    FlushCommands();
    glGetError();
    u32* buffer = GetCommandBuffer();
    const u32 texUnits[4] = {0, Header(REG_TEXUNIT_CONFIG, 0x1), 0x10000, Header(REG_TEXUNIT_CONFIG, 0x4)};
    std::memcpy(buffer, texUnits, sizeof(texUnits));
    nngxMoveCommandbufferPointer(sizeof(texUnits));
    beard.Delete();
    makeup.Delete();
    wrinkle.Delete();

    // the positions of the hair relative to the faceline
    for (int i = 0; i < 3; i++) {
        data->hairPos[1][i] += data->facelinePos[0][i];
        data->hairPos[3][i] += data->facelinePos[0][i];
        data->hairPos[5][i] += data->facelinePos[0][i];
    }
    CFLi_ApplyAxis(data->_508);
    for (int i = 0; i < 6; i++) {
        CFLi_ApplyAxis(data->hairPos[i]);
    }
}

// the saved state around the recording of the command lists of a model (names are ours)
// 0x009761D0
GLint s_SavedCmdlist = 0;
// 0x009761D4
GLenum s_SavedCommandMode = 0;

// the fragment operations at the start of the opaque and the translucent list and the end of both (names are ours)
// 0x009761E0
u32 s_OpaBegin[14] = {
    0x1F41, Header(REG_DEPTH_COLOR_MASK, 0x2), 1, Header(REG_FRAMEBUFFER_FLUSH), 1, Header(REG_FRAMEBUFFER_INVALIDATE),
    0xF, Header(REG_COLORBUFFER_READING), 0xF, Header(REG_COLORBUFFER_WRITING), 2, Header(REG_DEPTHBUFFER_READING),
    2, Header(REG_DEPTHBUFFER_WRITING),
};
// 0x00976218
u32 s_XluBegin[14] = {
    0x0F41, Header(REG_DEPTH_COLOR_MASK, 0x2), 1, Header(REG_FRAMEBUFFER_FLUSH), 1, Header(REG_FRAMEBUFFER_INVALIDATE),
    0xF, Header(REG_COLORBUFFER_READING), 0xF, Header(REG_COLORBUFFER_WRITING), 2, Header(REG_DEPTHBUFFER_READING),
    0, Header(REG_DEPTHBUFFER_WRITING),
};
// 0x00976250
u32 s_ModelEnd[6] = {0, Header(REG_229, 0x2), 0, Header(REG_253, 0x2), 0, Header(REG_25E, 0x2)};

// the colors of the glasses (RGBA8; name is ours)
// 0x008A3FBC
const u32 s_GlassColors[6] = {0xFF181818, 0xFF103860, 0xFF0810A8, 0xFF683020, 0xFF0060A8, 0xFF687078};

namespace {
// the parameters of nngxGetCmdlistParameteri (names are ours)
const GLenum CMDLIST_IS_RUNNING = 0x201;
const GLenum CMDLIST_TOP_BUFADDR = 0x206;
const GLenum CMDLIST_BINDING = 0x207;
const GLenum COMMAND_GENERATION_DEFERRED = 0x801;

// the offset of the next command in the saved list (inline; name is ours)
inline u32* GetSavedOffset(GLint top)
{
    GLint used;
    nngxGetCmdlistParameteri(CMDLIST_USED_BUFSIZE, &used);
    return reinterpret_cast<u32*>((used & ~3) + top);
}

// a color as RGB565 and back (inline; name is ours)
inline u32 QuantizeColor(u32 color)
{
    u32 r = (color << 24) >> 27;
    u32 g = (color << 16) >> 26;
    u32 b = ((color << 8) >> 24) >> 3;
    r = (r << 3) | (r >> 2);
    g = (g << 2) | (g >> 4);
    b = (b << 3) | (b >> 2);
    return r | (g << 8) | (b << 16) | 0xFF000000;
}

// starts saving into a new command list of the model (inline; name is ours)
inline void BeginSave(CFLiSavedCmdlist* list, GLsizei storage)
{
    GLint bound;
    nngxGetCmdlistParameteri(CMDLIST_BINDING, &bound);
    nngxGenCmdlists(1, &list->name);
    nngxBindCmdlist(list->name);
    nngxCmdlistStorage(storage, 32);
    nngxGetCmdlistParameteri(CMDLIST_TOP_BUFADDR, reinterpret_cast<GLint*>(&list->_4));
    nngxBindCmdlist(bound);
    nngxGetCmdlistParameteri(CMDLIST_BINDING, &s_SavedCmdlist);
    nngxBindCmdlist(list->name);
    if (list->size > 0) {
        nngxClearCmdlist();
        list->size = 0;
    }
    nngxStartCmdlistSave();
    glGetError();
    nngxGetCommandGenerationMode(&s_SavedCommandMode);
    nngxSetCommandGenerationMode(COMMAND_GENERATION_DEFERRED);
}

// ends the saving and returns to the list of the program (inline; name is ours)
inline void EndSave(CFLiSavedCmdlist* list)
{
    nngxAdd3DCommand(s_ModelEnd, sizeof(s_ModelEnd), true);
    list->_C = true;
    GLuint offset;
    nngxStopCmdlistSave(&offset, &list->size, reinterpret_cast<GLsizei*>(&list->requestOffset), &list->requestCount);
    glGetError();
    nngxBindCmdlist(s_SavedCmdlist);
    nngxSetCommandGenerationMode(s_SavedCommandMode);
    glGetError();
}

// a combiner that takes the texture of the unit of the context (inline; name is ours)
inline void PutTextureCombiner(const CFLProgramContext* context)
{
    u32 unit = context->textureUnitIndex;
    const u32 commands[4] = {0x0EE00EE0 | (unit << 16) | unit, Header(REG_TEXENV0_SOURCE, 0xF, 2, true), 0, 0};
    nngxAdd3DCommand(commands, sizeof(commands), true);
}
} // namespace

// records the opaque and the translucent command list of the model
// 0x00172BA0 (name is ours)
DECOMP_NOINLINE void CFLi_MakeModelCmdlists(CFLCharModel* model)
{
    CFLiCharModelData* data = model->data;
    const CFLProgramContext* context = &s_ProgramContext;
    data->cullCommandNum = 0;
    data->flipCommandNum = 0;
    data->lodCommandNum = 0;
    data->expressionCommand = NULL;
    data->typeCommandNum = 0;
    data->unitCommandNum = 0;
    data->hairColorCommand = NULL;
    GLint running;
    nngxGetCmdlistParameteri(CMDLIST_IS_RUNNING, &running);
    if (running) {
        nngxStopCmdlist();
    }

    // opaque: faceline, beard, nose, hair
    BeginSave(&data->opa, 0x1000);
    data = model->data;
    bool special = data->_68C >> 31;
    GLint top;
    nngxGetCmdlistParameteri(CMDLIST_TOP_BUFADDR, &top);
    nngxAdd3DCommand(s_OpaBegin, sizeof(s_OpaBegin), true);
    data->cullCommands[data->cullCommandNum++] = GetSavedOffset(top);
    u32 cull[2] = {2, Header(REG_FACECULLING_CONFIG)};
    nngxAdd3DCommand(cull, sizeof(cull), true);
    PutTextureCombiner(context);
    CFLi_BindModelTexture(model, &data->textures[0], top);
    data->shapes[2].DirectDraw(context);
    const u32 marker[4] = {0, Header(REG_TEXUNIT_CONFIG, 0x0, 2), 0, 0};
    nngxAdd3DCommand(marker, sizeof(marker), true);
    data->unitCommands[data->unitCommandNum++] = GetSavedOffset(top);
    u32* buffer = GetCommandBuffer();
    const u32 texUnits[4] = {0, Header(REG_TEXUNIT_CONFIG, 0x1), 0x10000, Header(REG_TEXUNIT_CONFIG, 0x4)};
    std::memcpy(buffer, texUnits, sizeof(texUnits));
    nngxMoveCommandbufferPointer(sizeof(texUnits));
    u32 beard[6] = {0x0EEE0EEE, Header(REG_TEXENV0_SOURCE, 0xF, 2, true), 0, 0, 0, Header(REG_TEXENV0_COLOR)};
    beard[4] = special ? s_SpecialHairColor : s_HairColors[data->info.beardColor];
    nngxAdd3DCommand(beard, sizeof(beard), true);
    data->shapes[0].DirectDraw(context);
    s32 faceColor = data->info.faceColor;
    if (faceColor > 5) {
        faceColor = 5;
    }
    u32 color = s_FacelineColors[faceColor];
    if (!CFLi_IsHDModeEnabled()) {
        color = QuantizeColor(color);
    }
    const u32 nose[2] = {color, Header(REG_TEXENV0_COLOR)};
    nngxAdd3DCommand(nose, sizeof(nose), true);
    data->shapes[8].DirectDraw(context);
    if (data->info.hairFlip) {
        data->flipCommands[data->flipCommandNum++] = GetSavedOffset(top);
        const u32 flip[2] = {1, Header(REG_FACECULLING_CONFIG)};
        nngxAdd3DCommand(flip, sizeof(flip), true);
    }
    data->shapes[3].DirectDraw(context);
    data->hairColorCommand = GetSavedOffset(top);
    const u32 hair[2] = {data->hairColor, Header(REG_TEXENV0_COLOR)};
    nngxAdd3DCommand(hair, sizeof(hair), true);
    data->shapes[5].DirectDraw(context);
    if (data->textures[1].self != NULL) {
        s32 favorite = data->info.favoriteColor;
        if (favorite > 11) {
            favorite = 11;
        }
        u32 c = s_FavoriteColors[favorite];
        u32 half = (c & 0xFF000000) | (((c << 24) >> 25) | (((c << 16) >> 25) << 8)) | (((c << 8) >> 25) << 16);
        u32 cap[6] = {0x0EEE0EE0, Header(REG_TEXENV0_SOURCE, 0xF, 2, true), 0, 8, 0, Header(REG_TEXENV0_COLOR)};
        cap[0] |= context->textureUnitIndex;
        cap[4] = half;
        nngxAdd3DCommand(cap, sizeof(cap), true);
        CFLi_BindModelTexture(model, &data->textures[1], top);
        data->shapes[1].DirectDraw(context);
    }
    if (data->info.hairFlip) {
        data->cullCommands[data->cullCommandNum++] = GetSavedOffset(top);
        const u32 back[2] = {2, Header(REG_FACECULLING_CONFIG)};
        nngxAdd3DCommand(back, sizeof(back), true);
    }
    EndSave(&data->opa);

    // translucent: mask with the expression, nose line, glasses
    BeginSave(&data->xlu, 0x800);
    data = model->data;
    nngxGetCmdlistParameteri(CMDLIST_TOP_BUFADDR, &top);
    nngxAdd3DCommand(s_XluBegin, sizeof(s_XluBegin), true);
    data->cullCommands[data->cullCommandNum++] = GetSavedOffset(top);
    const u32 cullBack[2] = {2, Header(REG_FACECULLING_CONFIG)};
    nngxAdd3DCommand(cullBack, sizeof(cullBack), true);
    PutTextureCombiner(context);
    CFLiTexHandle* expression = model->expressionTextures[model->expression];
    if (expression != NULL) {
        data->expressionCommand = CFLi_BindModelTexture(model, expression, top);
        data->shapes[6].DirectDraw(context);
    }
    CFLi_BindModelTexture(model, &data->textures[2], top);
    data->shapes[7].DirectDraw(context);
    if (data->textures[3].self != NULL) {
        u32 glass[6] = {0, Header(REG_FACECULLING_CONFIG), 0x10001, Header(0x0C2), 0, Header(REG_TEXENV0_COLOR)};
        glass[4] = s_GlassColors[data->info.glassColor];
        nngxAdd3DCommand(glass, sizeof(glass), true);
        CFLi_BindModelTexture(model, &data->textures[3], top);
        data->shapes[4].DirectDraw(context);
    }
    EndSave(&data->xlu);
    if (running) {
        nngxRunCmdlist();
    }
}

// how an expression changes the parts of the face (types -1: unchanged; names are ours)
struct CFLiExpressionParts
{
    s32 eyeType;
    s32 mouthType;
    s32 eyeRotate;
    s32 eyebrowRotate;
    s32 eyebrowY;
};

// 0x008A4768
const CFLiExpressionParts s_ExpressionParts[CFL_EXPRESSION_MAX] = {
    {-1, -1, 0, 0, 0},  {60, -1, 0, 0, 0},  {-1, 10, 2, 2, 0},  {-1, 12, -2, -2, 0}, {61, -1, 0, 0, -2},
    {26, -1, 0, 0, 0},  {-1, 36, 0, 0, 0},  {60, 36, 0, 0, 0},  {-1, 36, 2, 2, 0},   {-1, 36, -2, -2, 0},
    {61, 36, 0, 0, -2}, {26, 36, 0, 0, 0},  {47, -1, 0, 0, 0},  {47, -1, 0, 0, 0},   {47, 36, 0, 0, 0},
    {47, 36, 0, 0, 0},  {47, 19, 0, 0, 0},  {47, 19, 0, 0, 0},  {47, 12, 0, 0, 0},
};

// the rotation the images of the eyes and eyebrows already have (in 1/32 turns; names are ours)
// 0x008A4710
const u8 s_EyeRotate[62] = {
    3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4, 4, 3, 3, 4, 4, 4, 3, 3, 4, 3, 4, 3, 3, 4, 3, 4, 4, 3, 4,
    4, 4, 3, 3, 3, 4, 4, 3, 3, 3, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 3, 4, 4, 3, 4, 4,
};
// 0x008A474E
const u8 s_EyebrowRotate[24] = {6, 6, 5, 7, 6, 7, 6, 7, 4, 7, 6, 8, 5, 5, 6, 6, 7, 7, 6, 6, 5, 6, 7, 5};

// the colors of the mouth and of its edge, of the eyes (RGBA8; names are ours)
// 0x008A45D8
const u32 s_MouthColors[5] = {0xFF0852D8, 0xFF080CF0, 0xFF4848F5, 0xFF749AF0, 0xFF40508C};
// 0x008A45EC
const u32 s_MouthEdgeColors[5] = {0xFF183082, 0xFF0C0C78, 0xFF282088, 0xFF5078DC, 0xFF0A1E46};
// 0x008A3FA4
const u32 s_EyeColors[6] = {0xFF000000, 0xFF70706C, 0xFF2C3C66, 0xFF305E60, 0xFFA85446, 0xFF587038};

// the format and the type of glTexImage2D for the texture formats (names are ours)
struct CFLiGlTexFormat
{
    GLenum format;
    GLenum type;
};

// 0x008A3D04
const CFLiGlTexFormat s_GlTexFormats[12] = {
    {0x6757, 0x6761}, {0x6757, 0x1401}, {0x6756, 0x6761}, {0x6756, 0x1401}, {0x6758, 0x6760}, {0x6758, 0x1401},
    {0x6759, 0x1401}, {0x6754, 0x8363}, {0x6754, 0x1401}, {0x6752, 0x8034}, {0x6752, 0x8033}, {0x6752, 0x1401},
};

namespace {
const u32 REG_TEXENV1_COLOR = 0x0CB;
const u32 REG_TEXENV2_COLOR = 0x0D3;

// the formats of glCopyTexImage2D and the native RGB format of the table (values of the API; names are ours)
const GLenum RGB = 0x1907;
const GLenum RGBA = 0x1908;
const GLenum FORMAT_NATIVE_RGB = 0x6754;
const GLenum MEM_FCRAM = 0x10000;

// the format of the textures of the expressions (RGBA4)
const int EXPRESSION_FORMAT = 10;

// the image of a part from the archive, NULL when there is none (inline; name is ours)
inline u8* ReadPartImage(int section, int index)
{
    s32 size = CFLi_ReadArchiveImage(NULL, section, index);
    if (size <= 0) {
        return NULL;
    }
    u8* image = static_cast<u8*>(CFLi_AllocateMemory(size, 32));
    CFLi_ReadArchiveImage(image, section, index);
    return image;
}

// the combiners of the mouth and the eyes: texture 0 picks the three colors (inline; name is ours)
inline void PutPartCombiners(const CFLProgramContext* context, u32 color0, u32 color1, u32 color2)
{
    u32 unit = context->textureUnitIndex;
    u32 source = unit | 0x0FFF0FE0;
    const u32 commands[18] = {
        0x0EE00EE0 | (unit << 16) | unit, Header(REG_TEXENV0_SOURCE, 0xF, 2, true), 4, 1, color0, Header(REG_TEXENV0_COLOR),
        source, Header(REG_TEXENV1_SOURCE, 0xF, 2, true), 8, 8, color1, Header(REG_TEXENV1_COLOR),
        source, Header(REG_TEXENV2_SOURCE, 0xF, 2, true), 0xC, 8, color2, Header(REG_TEXENV2_COLOR),
    };
    nngxAdd3DCommand(commands, sizeof(commands), true);
}

// a color command of combiner stage 0 (inline; name is ours)
inline void PutTexEnvColor(u32 color)
{
    const u32 commands[2] = {color, Header(REG_TEXENV0_COLOR)};
    nngxAdd3DCommand(commands, sizeof(commands), true);
}

// whether an address is in VRAM A or B (inline; name is ours)
inline bool IsVramAddress(uptr address)
{
    return (nngxGetVramStartAddr(MEM_VRAMA) <= address && address <= nngxGetVramEndAddr(MEM_VRAMA)) ||
           (nngxGetVramStartAddr(MEM_VRAMB) <= address && address <= nngxGetVramEndAddr(MEM_VRAMB));
}

// copies a texture the GPU rendered into memory; from VRAM over a texture in FCRAM (inline; name is ours)
DECOMP_ALWAYS_INLINE void ReadRenderTexture(void* dst, const CFLiTexHandle* texture, int size)
{
    FlushCommands();
    uptr address = texture->address;
    u32 bytes = size * size * 2;
    if (IsVramAddress(address)) {
        GLenum format = s_GlTexFormats[EXPRESSION_FORMAT].format == FORMAT_NATIVE_RGB ? RGB : RGBA;
        GLuint copy;
        glGenTextures(1, &copy);
        glBindTexture(TEXTURE_2D, copy);
        glCopyTexImage2D(TEXTURE_2D | MEM_FCRAM, 0, format, 0, 0, size, size, 0);
        glGetError();
        FlushCommands();
        GLint copyAddress;
        glGetTexParameteriv(TEXTURE_2D, TEXTURE_DATA_ADDRESS, &copyAddress);
        std::memcpy(dst, reinterpret_cast<const void*>(copyAddress), bytes);
        glBindTexture(TEXTURE_2D, 0);
        glDeleteTextures(1, &copy);
    } else {
        std::memcpy(dst, reinterpret_cast<const void*>(address), bytes);
    }
    FlushCommands();
}

// gives the texture of an expression its mip levels: each level is the previous one drawn at half the size, the
// levels go one after the other into the buffer and from there into a new texture in the area (inline; name is ours)
inline void MakeExpressionMipmaps(CFLiTexHandle* texture, int levels, u32 area, bool keepBuffer, u8* buffer)
{
    const CFLProgramContext* context = &s_ProgramContext;
    u32* commands = GetCommandBuffer();
    std::memcpy(commands, s_TexEnvSources, sizeof(s_TexEnvSources));
    nngxMoveCommandbufferPointer(sizeof(s_TexEnvSources));
    commands = GetCommandBuffer();
    std::memcpy(commands, s_TexEnvScales, sizeof(s_TexEnvScales));
    nngxMoveCommandbufferPointer(sizeof(s_TexEnvScales));
    CFLi_PutTexEnv(context, 1, true);
    CFLi_PutAlphaTest(context, false);
    commands = GetCommandBuffer();
    const u32 cull[2] = {0, Header(REG_FACECULLING_CONFIG)};
    std::memcpy(commands, cull, sizeof(cull));
    nngxMoveCommandbufferPointer(sizeof(cull));
    CFLiTexHandle* source = texture;
    int width = texture->width;
    int height = texture->height;
    int size = width;
    u8* dst = buffer;
    CFLiTexHandle levelTextures[3];
    int n = 0;
    for (int level = 0; level < levels; level++) {
        if (level >= 1) {
            CFLiTexHandle* target = &levelTextures[n];
            target->Create(size, size, 1, CFL_TEX_WRAP_0, CFL_TEX_WRAP_0, static_cast<CFLTexFmt>(EXPRESSION_FORMAT), NULL,
                           MEM_FCRAM, 0);
            CFLiTexHandle* bound = target;
            CFLi_BindRenderTexture(&bound, size, size, false, 0, false);
            CFLi_PutFragmentOperation(true, 0, false, false, 0x01010000);
            CFLi_BindTexture(source, context, LINEAR_MIPMAP_NEAREST, NULL, NULL, NULL);
            CFLi_DrawQuad(NULL, context, 0, true, 0.0f, size, 0.0f, size, 0.0f);
            ReadRenderTexture(dst, bound, size);
            FlushCommands();
            glGetError();
            commands = GetCommandBuffer();
            const u32 texUnits[4] = {0, Header(REG_TEXUNIT_CONFIG, 0x1), 0x10000, Header(REG_TEXUNIT_CONFIG, 0x4)};
            std::memcpy(commands, texUnits, sizeof(texUnits));
            nngxMoveCommandbufferPointer(sizeof(texUnits));
            source->Delete();
            source = target;
            n++;
        }
        dst += size * size * 2;
        size >>= 1;
    }
    commands = GetCommandBuffer();
    const u32 texUnits[4] = {0, Header(REG_TEXUNIT_CONFIG, 0x1), 0x10000, Header(REG_TEXUNIT_CONFIG, 0x4)};
    std::memcpy(commands, texUnits, sizeof(texUnits));
    nngxMoveCommandbufferPointer(sizeof(texUnits));
    source->Delete();
    texture->Create(width, height, levels, CFL_TEX_WRAP_0, CFL_TEX_WRAP_0, static_cast<CFLTexFmt>(EXPRESSION_FORMAT),
                    buffer, area, 0);
    if (area != MEM_FCRAM) {
        FlushCommands();
    }
    if (!keepBuffer) {
        CFLi_FreeMemory(buffer);
    }
}
} // namespace

// draws the eyes, eyebrows, mouth, mustache and mole into the texture of each expression of the model (positions in
// a face of 64 units), notes the cells of the face texture they cover (for the mask shape) and makes mip levels or
// moves the textures into the area of the model
// 0x0016A148 (name is ours)
DECOMP_NOINLINE void CFLi_MakeFaceTextures(CFLiFaceParts* parts, const CFLiCharInfo* info, CFLCharModel* model,
                                           s32 resolution)
{
    u8* eyeImage0 = NULL;
    u8* eyeImage1 = NULL;
    u8* eyeImage2 = NULL;
    u8* eyeImage3 = NULL;
    u8* eyeImage4 = NULL;
    u8* eyeImage5 = NULL;
    u8* mouthImage0 = NULL;
    u8* mouthImage1 = NULL;
    u8* mouthImage2 = NULL;
    u8* mouthImage3 = NULL;
    u8* mouthImage4 = NULL;
    u8* eyebrowImage = NULL;
    u8* mustacheImage = NULL;
    u8* moleImage = NULL;
    // the images of the eyes and the mouth each expression uses
    u8** const images[CFL_EXPRESSION_MAX][2] = {
        {&eyeImage0, &mouthImage0}, {&eyeImage1, &mouthImage0}, {&eyeImage0, &mouthImage1}, {&eyeImage2, &mouthImage2},
        {&eyeImage3, &mouthImage0}, {&eyeImage4, &mouthImage0}, {&eyeImage0, &mouthImage3}, {&eyeImage1, &mouthImage3},
        {&eyeImage0, &mouthImage3}, {&eyeImage2, &mouthImage3}, {&eyeImage3, &mouthImage3}, {&eyeImage4, &mouthImage3},
        {&eyeImage5, &mouthImage0}, {&eyeImage5, &mouthImage0}, {&eyeImage5, &mouthImage3}, {&eyeImage5, &mouthImage3},
        {&eyeImage5, &mouthImage4}, {&eyeImage5, &mouthImage4}, {&eyeImage5, &mouthImage2},
    };
    CFLiCharModelData* data = model->data;
    s32 size;
    switch (resolution) {
    case 64:
    case 96:
    default:
        size = 64;
        break;
    case 128:
    case 224:
        size = 128;
        break;
    case 256:
    case 480:
        size = 256;
        break;
    case 512:
        size = 512;
        break;
    case 1024:
        size = 1024;
        break;
    }
    int levels;
    bool mipmaps;
    switch (resolution) {
    case 96:
        levels = 2;
        mipmaps = true;
        break;
    case 224:
        levels = 3;
        mipmaps = true;
        break;
    case 480:
        levels = 4;
        mipmaps = true;
        break;
    default:
        levels = 1;
        mipmaps = false;
        break;
    }
    u32 flags = data->_68C;
    bool makeMask = !(flags & 0x10000000);
    bool specialEyebrowColor = (flags & 0x40000000) >> 30;
    bool specialEyeColor = (flags & 0x08000000) >> 27;
    bool specialMustacheColor = flags >> 31;
    bool readBack = makeMask || mipmaps;
    // the buffer of the image and its mip levels
    u32 bufferSize = 0;
    if (resolution & 32) {
        bufferSize = 32 * 32 * 2;
    }
    if (resolution & 64) {
        bufferSize += 64 * 64 * 2;
    }
    if (resolution & 128) {
        bufferSize += 128 * 128 * 2;
    }
    if (resolution & 256) {
        bufferSize += 256 * 256 * 2;
    }
    if (resolution & 512) {
        bufferSize += 512 * 512 * 2;
    }
    if (resolution & 1024) {
        bufferSize += 1024 * 1024 * 2;
    }
    u8* buffer = static_cast<u8*>(CFLi_AllocateMemory(bufferSize, 16));
    std::memset(parts, 0, sizeof(*parts));
    CFLi_SetUniformMtx24(s_FacelineLight);
    for (int i = 0; i < CFL_EXPRESSION_MAX; i++) {
        CFLiTexHandle* texture = model->expressionTextures[i];
        if (texture == NULL) {
            continue;
        }
        CFLiCharInfo local = *info;
        const CFLiExpressionParts* expression = &s_ExpressionParts[i];
        s32 eyeRotate = expression->eyeRotate;
        s32 eyebrowRotate = expression->eyebrowRotate;
        local.eyebrowY += expression->eyebrowY;
        if (expression->mouthType >= 0) {
            local.mouthType = expression->mouthType;
        }
        if (expression->eyeType >= 0 && expression->eyeType != local.eyeType) {
            eyeRotate += (32 - s_EyeRotate[local.eyeType]) - (32 - s_EyeRotate[expression->eyeType]);
            local.eyeType = expression->eyeType;
        }
        if (eyeRotate != 0) {
            s32 rotate = local.eyeRotate + eyeRotate;
            if (rotate < 0) {
                rotate = 0;
            } else if (rotate > 7) {
                rotate = 7;
            }
            local.eyeRotate = rotate;
        }
        if (eyebrowRotate != 0) {
            s32 rotate = local.eyebrowRotate + eyebrowRotate;
            if (rotate < 0) {
                rotate = 0;
            } else if (rotate > 11) {
                rotate = 11;
            }
            local.eyebrowRotate = rotate;
        }

        // the images (each loaded once)
        u8** eyeSlot = images[i][0];
        u8** mouthSlot = images[i][1];
        if (*eyeSlot == NULL) {
            *eyeSlot = ReadPartImage(10, local.eyeType);
        }
        // winks: one eye stays open
        if ((i == 12 || i == 13 || i == 14 || i == 15 || i == 16 || i == 17) && eyeImage0 == NULL) {
            eyeImage0 = ReadPartImage(10, info->eyeType);
        }
        if (*mouthSlot == NULL) {
            *mouthSlot = ReadPartImage(17, local.mouthType);
        }
        if (eyebrowImage == NULL) {
            eyebrowImage = ReadPartImage(11, local.eyebrowType);
        }
        if (mustacheImage == NULL) {
            mustacheImage = ReadPartImage(18, local.mustacheType);
        }
        if (moleImage == NULL) {
            moleImage = ReadPartImage(16, local.moleType);
        }
        u8* leftEye = *eyeSlot;
        u8* rightEye = leftEye;
        s32 leftEyeType = local.eyeType;
        s32 rightEyeType = local.eyeType;
        if (i == 12 || i == 14 || i == 16) {
            leftEye = eyeImage0;
            leftEyeType = info->eyeType;
        } else if (i == 13 || i == 15 || i == 17) {
            rightEye = eyeImage0;
            rightEyeType = info->eyeType;
        }

        // the parts: eyes, eyebrows, mouth, mustache (two halves), mole
        CFLiFacePart part[8];
        part[0].texture.CreateFromRes(reinterpret_cast<const CFLiTexHeader*>(leftEye), MEM_FCRAM);
        part[1].texture.CreateFromRes(reinterpret_cast<const CFLiTexHeader*>(rightEye), MEM_FCRAM);
        part[2].texture.CreateFromRes(reinterpret_cast<const CFLiTexHeader*>(eyebrowImage), MEM_FCRAM);
        part[3].texture.CreateFromRes(reinterpret_cast<const CFLiTexHeader*>(eyebrowImage), MEM_FCRAM);
        part[4].texture.CreateFromRes(reinterpret_cast<const CFLiTexHeader*>(*mouthSlot), MEM_FCRAM);
        part[5].texture.CreateFromRes(reinterpret_cast<const CFLiTexHeader*>(mustacheImage), MEM_FCRAM);
        part[6].texture.CreateFromRes(reinterpret_cast<const CFLiTexHeader*>(mustacheImage), MEM_FCRAM);
        part[7].texture.CreateFromRes(reinterpret_cast<const CFLiTexHeader*>(moleImage), MEM_FCRAM);
        CFLiTexHandle* target = texture;
        CFLi_BindRenderTexture(&target, size, size, true, 0, false);
        if (texture->area == MEM_FCRAM && static_cast<u32>(size) >= 1024) {
            std::memset(reinterpret_cast<void*>(texture->address), 0, size * size * 2);
            nngxlowStoreDataCache(texture->address, size * size * 2);
        }

        f32 scale = size * (1.0f / 64);
        f32 eyeScale = 1.0f + local.eyeScale * 0.4f;
        f32 eyeX = local.eyeX * 0.88961464f;
        f32 eyeY = 18.451523f + local.eyeY * 1.0760943f;
        f32 eyeWidth = eyeScale * 5.34375f;
        f32 eyeHeight = eyeScale * 4.5f * (0.64f + local.eyeAspect * 0.12f);
        f32 eyeAngle = ((32 - s_EyeRotate[local.eyeType] + local.eyeRotate) % 32) * 11.25f;
        f32 eyebrowScale = 1.0f + local.eyebrowScale * 0.4f;
        f32 eyebrowX = local.eyebrowX * 0.88961464f;
        f32 eyebrowY = 16.549807f + local.eyebrowY * 1.0760943f;
        f32 eyebrowWidth = eyebrowScale * 5.0625f;
        f32 eyebrowHeight = eyebrowScale * 4.5f * (0.64f + local.eyebrowAspect * 0.12f);
        f32 eyebrowAngle = ((32 - s_EyebrowRotate[local.eyebrowType] + local.eyebrowRotate) % 32) * 11.25f;
        f32 mouthScale = 1.0f + local.mouthScale * 0.4f;
        f32 mouthY = 29.25885f + local.mouthY * 1.0760943f;
        f32 mouthWidth = mouthScale * 6.1875f;
        f32 mouthHeight = mouthScale * 4.5f * (0.64f + local.mouthAspect * 0.12f);
        f32 mustacheScale = 1.0f + local.mustacheScale * 0.4f;
        f32 mustacheY = 31.763554f + local.mustacheY * 1.0760943f;
        f32 moleScale = 1.0f + local.moleScale * 0.4f;
        f32 moleX = 17.766165f + local.moleX * 1.7792293f;
        f32 moleY = 17.959862f + local.moleY * 1.0760943f;

        part[0].x = (32.0f - eyeX) * scale;
        part[0].y = eyeY * scale;
        part[0].scaleX = eyeWidth * scale;
        f32 height = eyeHeight * scale;
        if ((leftEyeType == 14 || leftEyeType == 26) && height < 12.0f) {
            part[0].scaleY = 12.0f;
        } else {
            part[0].scaleY = height;
        }
        part[0].rotate = eyeAngle;
        part[0].flip = 2;
        part[1].x = (eyeX + 32.0f) * scale;
        part[1].y = eyeY * scale;
        part[1].scaleX = eyeWidth * scale;
        if ((rightEyeType == 14 || rightEyeType == 26) && height < 12.0f) {
            height = 12.0f;
        }
        part[1].scaleY = height;
        part[1].rotate = 360.0f - eyeAngle;
        part[1].flip = 1;
        part[2].x = (32.0f - eyebrowX) * scale;
        part[2].y = eyebrowY * scale;
        part[2].scaleX = eyebrowWidth * scale;
        part[2].scaleY = eyebrowHeight * scale;
        part[2].rotate = eyebrowAngle;
        part[2].flip = 2;
        part[3].x = (eyebrowX + 32.0f) * scale;
        part[3].y = eyebrowY * scale;
        part[3].scaleX = eyebrowWidth * scale;
        part[3].scaleY = eyebrowHeight * scale;
        part[3].rotate = 360.0f - eyebrowAngle;
        part[3].flip = 1;
        part[4].x = scale * 32.0f;
        part[4].y = mouthY * scale;
        part[4].scaleX = mouthWidth * scale;
        height = mouthHeight * scale;
        s32 mouthType = local.mouthType;
        if ((mouthType == 3 || mouthType == 15 || mouthType == 19 || mouthType == 20 || mouthType == 21 ||
             mouthType == 23 || mouthType == 25) &&
            height < 12.0f) {
            height = 12.0f;
        }
        part[4].scaleY = height;
        part[4].rotate = 0.0f;
        part[4].flip = 0;
        part[5].x = scale * 32.0f;
        part[5].y = mustacheY * scale;
        part[5].scaleX = mustacheScale * 4.5f * scale;
        part[5].scaleY = mustacheScale * 9.0f * scale;
        part[5].rotate = 0.0f;
        part[5].flip = 2;
        part[6].x = scale * 32.0f;
        part[6].y = mustacheY * scale;
        part[6].scaleX = mustacheScale * 4.5f * scale;
        part[6].scaleY = mustacheScale * 9.0f * scale;
        part[6].rotate = 0.0f;
        part[6].flip = 1;
        part[7].x = moleX * scale;
        part[7].y = moleY * scale;
        part[7].scaleX = moleScale * scale;
        part[7].scaleY = moleScale * scale;
        part[7].rotate = 0.0f;
        part[7].flip = 0;

        // the colors, upside down
        nn::math::MTX44 projection;
        nn::math::ARMv6::MTX44OrthoC_FAST(&projection, 0.0f, size, size, 0.0f, -200.0f, 200.0f);
        CFLi_SetUniformMtx44(&projection.m[0][0]);
        const CFLProgramContext* context = &s_ProgramContext;
        u32* commands = GetCommandBuffer();
        const u32 cull[2] = {0, Header(REG_FACECULLING_CONFIG)};
        std::memcpy(commands, cull, sizeof(cull));
        nngxMoveCommandbufferPointer(sizeof(cull));
        CFLi_PutAlphaTest(context, true);
        CFLi_PutFragmentOperation(true, 0, false, true, 0x89890000);
        PutTexEnvColor(specialMustacheColor ? s_SpecialHairColor : s_HairColors[local.beardColor]);
        CFLi_PutTexEnv(context, 3, true);
        CFLi_DrawFacePart(&part[5]);
        CFLi_DrawFacePart(&part[6]);
        PutPartCombiners(context, s_MouthColors[local.mouthColor], s_MouthEdgeColors[local.mouthColor], 0xFFFFFFFF);
        CFLi_DrawFacePart(&part[4]);
        PutTexEnvColor(specialEyebrowColor ? s_SpecialHairColor : s_HairColors[local.eyebrowColor]);
        CFLi_PutTexEnv(context, 3, true);
        CFLi_DrawFacePart(&part[2]);
        CFLi_DrawFacePart(&part[3]);
        PutPartCombiners(context, 0xFFFFFF00, 0xFFFFFFFF,
                         specialEyeColor ? s_SpecialEyeColor : s_EyeColors[local.eyeColor]);
        CFLi_DrawFacePart(&part[0]);
        CFLi_DrawFacePart(&part[1]);
        PutTexEnvColor(0xFF0F0F12);
        CFLi_PutTexEnv(context, 3, true);
        CFLi_DrawFacePart(&part[7]);

        // the alpha: cleared, then the parts again
        CFLi_PutAlphaTest(context, false);
        CFLi_PutFragmentOperation(false, 0, false, false, 0x01010000);
        CFLi_PutTexEnv(context, 0, true);
        PutTexEnvColor(0x00FFFFFF);
        nn::math::MTX34 modelView(nn::math::MTX34::Identity());
        CFLi_SetModelMatrices(&modelView, &modelView, true);
        CFLi_DrawQuad(NULL, context, 0, false, 0.0f, size, 0.0f, size, 0.0f);
        CFLi_PutAlphaTest(context, true);
        CFLi_PutFragmentOperation(false, 0, false, true, 0x19190000);
        CFLi_PutTexEnv(context, 1, false);
        CFLi_DrawFacePart(&part[5]);
        CFLi_DrawFacePart(&part[6]);
        CFLi_DrawFacePart(&part[4]);
        CFLi_DrawFacePart(&part[2]);
        CFLi_DrawFacePart(&part[3]);
        CFLi_DrawFacePart(&part[0]);
        CFLi_DrawFacePart(&part[1]);
        CFLi_DrawFacePart(&part[7]);
        CFLi_PutFragmentOperation(true, 0, false, true, 0x01010000);

        if (readBack) {
            ReadRenderTexture(buffer, target, size);
        }
        // the cells (16 x 16) of the face the parts cover: a pixel with alpha
        if (makeMask) {
            s32 cell = size / 16;
            if (size >= 128) {
                // straight from the tiles (8 x 8 pixels, rows from the bottom)
                s32 y0 = 0;
                for (int cy = 0; cy < 16; cy++) {
                    s32 y1 = y0 + cell;
                    s32 x0 = 0;
                    for (int cx = 0; cx < 16; cx++) {
                        u8* covered = &parts->cells[cy * 16 + cx];
                        if (*covered == 0) {
                            s32 x1 = x0 + cell;
                            for (s32 y = y0; y < y1 && *covered == 0; y += 8) {
                                const u8* p = buffer + (((size - y - 8) / 8) * (size / 8) + x0 / 8) * 128;
                                for (s32 x = x0; x < x1 && *covered == 0; x += 8) {
                                    for (int k = 0; k < 64; k++) {
                                        if (*p & 15) {
                                            *covered = 1;
                                            break;
                                        }
                                        p += 2;
                                    }
                                }
                            }
                        }
                        x0 += cell;
                    }
                    y0 = y1;
                }
            } else {
                u8* image = static_cast<u8*>(CFLi_AllocateMemory(size * size * 2, 16));
                CFLi_UntileImage(image, buffer, size, size, 1, EXPRESSION_FORMAT);
                s32 y0 = 0;
                for (int cy = 0; cy < 16; cy++) {
                    s32 y1 = y0 + cell;
                    s32 x0 = 0;
                    for (int cx = 0; cx < 16; cx++) {
                        u8* covered = &parts->cells[cy * 16 + cx];
                        if (*covered == 0) {
                            for (s32 y = y0; y < y1; y++) {
                                const u8* p = image + (y * size + x0) * 2;
                                for (int k = 0; k < cell; k++) {
                                    if (*p & 15) {
                                        *covered = 1;
                                        break;
                                    }
                                    p += 2;
                                }
                                if (*covered != 0) {
                                    break;
                                }
                            }
                        }
                        x0 += cell;
                    }
                    y0 = y1;
                }
                CFLi_FreeMemory(image);
            }
        }
        FlushCommands();
        glGetError();
        commands = GetCommandBuffer();
        const u32 texUnits[4] = {0, Header(REG_TEXUNIT_CONFIG, 0x1), 0x10000, Header(REG_TEXUNIT_CONFIG, 0x4)};
        std::memcpy(commands, texUnits, sizeof(texUnits));
        nngxMoveCommandbufferPointer(sizeof(texUnits));
        for (int k = 0; k < 8; k++) {
            part[k].texture.Delete();
        }

        if (mipmaps) {
            MakeExpressionMipmaps(texture, levels, data->_67C, true, buffer);
        } else if (data->_67C != MEM_FCRAM) {
            // into the area of the model
            CFLiTexHandle moved;
            moved.Create(texture->width, texture->height, levels, CFL_TEX_WRAP_0, CFL_TEX_WRAP_0,
                         static_cast<CFLTexFmt>(EXPRESSION_FORMAT), reinterpret_cast<const void*>(texture->address),
                         data->_67C, 0);
            if (data->_67C != MEM_FCRAM) {
                FlushCommands();
            }
            texture->Delete();
            *texture = moved;
        }
    }
    CFLi_FreeMemory(eyeImage0);
    CFLi_FreeMemory(eyeImage1);
    CFLi_FreeMemory(eyeImage2);
    CFLi_FreeMemory(eyeImage3);
    CFLi_FreeMemory(eyeImage4);
    CFLi_FreeMemory(eyeImage5);
    CFLi_FreeMemory(mouthImage0);
    CFLi_FreeMemory(mouthImage1);
    CFLi_FreeMemory(mouthImage2);
    CFLi_FreeMemory(mouthImage3);
    CFLi_FreeMemory(mouthImage4);
    CFLi_FreeMemory(eyebrowImage);
    CFLi_FreeMemory(mustacheImage);
    CFLi_FreeMemory(moleImage);
    CFLi_FreeMemory(buffer);
}
