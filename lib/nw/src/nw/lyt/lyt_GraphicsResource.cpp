#include "nw/lyt/lyt_GraphicsResource.h"

#include <cstring>

#include "nn/gx/gx_Api.h"
#include "nn/math/math_ARMv6.h"
#include "nw/lyt/internal/lyt_GlConstants.h"
#include "nw/lyt/lyt_Layout.h"

namespace nw {
namespace lyt {
namespace {
// the uniforms of the pane shader (the order of GraphicsResource::Uniform)
// 0x0097F4EC (name is ours)
const char* s_UniformNames[GraphicsResource::UNIFORM_MAX] = {
    "uProjection",
    "uModelView",
    "uTexMtx0",
    "uTexMtx1",
    "uTexMtx2",
    "uColor",
    "uTransform",
    "uFrameSpec",
    "uVertexColor",
    "uVertexTexCoord0",
    "uVertexTexCoord1",
    "uVertexTexCoord2",
    "uRcpTexSize0",
    "dmp_Texture[0].samplerType",
    "dmp_Texture[1].samplerType",
    "dmp_Texture[2].samplerType",
    "dmp_Texture[3].samplerType",
    "dmp_TexEnv[0].combineRgb",
    "dmp_TexEnv[1].combineRgb",
    "dmp_TexEnv[2].combineRgb",
    "dmp_TexEnv[3].combineRgb",
    "dmp_TexEnv[4].combineRgb",
    "dmp_TexEnv[5].combineRgb",
    "dmp_TexEnv[0].combineAlpha",
    "dmp_TexEnv[1].combineAlpha",
    "dmp_TexEnv[2].combineAlpha",
    "dmp_TexEnv[3].combineAlpha",
    "dmp_TexEnv[4].combineAlpha",
    "dmp_TexEnv[5].combineAlpha",
    "dmp_TexEnv[0].srcRgb",
    "dmp_TexEnv[1].srcRgb",
    "dmp_TexEnv[2].srcRgb",
    "dmp_TexEnv[3].srcRgb",
    "dmp_TexEnv[4].srcRgb",
    "dmp_TexEnv[5].srcRgb",
    "dmp_TexEnv[0].srcAlpha",
    "dmp_TexEnv[1].srcAlpha",
    "dmp_TexEnv[2].srcAlpha",
    "dmp_TexEnv[3].srcAlpha",
    "dmp_TexEnv[4].srcAlpha",
    "dmp_TexEnv[5].srcAlpha",
    "dmp_TexEnv[0].operandRgb",
    "dmp_TexEnv[1].operandRgb",
    "dmp_TexEnv[2].operandRgb",
    "dmp_TexEnv[3].operandRgb",
    "dmp_TexEnv[4].operandRgb",
    "dmp_TexEnv[5].operandRgb",
    "dmp_TexEnv[0].operandAlpha",
    "dmp_TexEnv[1].operandAlpha",
    "dmp_TexEnv[2].operandAlpha",
    "dmp_TexEnv[3].operandAlpha",
    "dmp_TexEnv[4].operandAlpha",
    "dmp_TexEnv[5].operandAlpha",
    "dmp_TexEnv[0].scaleRgb",
    "dmp_TexEnv[1].scaleRgb",
    "dmp_TexEnv[2].scaleRgb",
    "dmp_TexEnv[3].scaleRgb",
    "dmp_TexEnv[4].scaleRgb",
    "dmp_TexEnv[5].scaleRgb",
    "dmp_TexEnv[0].scaleAlpha",
    "dmp_TexEnv[1].scaleAlpha",
    "dmp_TexEnv[2].scaleAlpha",
    "dmp_TexEnv[3].scaleAlpha",
    "dmp_TexEnv[4].scaleAlpha",
    "dmp_TexEnv[5].scaleAlpha",
    "dmp_TexEnv[0].constRgba",
    "dmp_TexEnv[1].constRgba",
    "dmp_TexEnv[2].constRgba",
    "dmp_TexEnv[3].constRgba",
    "dmp_TexEnv[4].constRgba",
    "dmp_TexEnv[5].constRgba",
    "dmp_TexEnv[0].bufferColor",
    "dmp_TexEnv[1].bufferInput",
    "dmp_TexEnv[2].bufferInput",
    "dmp_TexEnv[3].bufferInput",
    "dmp_TexEnv[4].bufferInput",
    "dmp_FragOperation.enableAlphaTest",
    "dmp_FragOperation.alphaRefValue",
    "dmp_FragOperation.alphaTestFunc",
};

// the files of the resources (GetResourcePath)
// 0x0097F4D4 (name is ours)
bool s_IsResourcePathInitialized = false;
// 0x0097F4D8 (name is ours)
const wchar_t* s_ResourceRootPath = L"rom:";
// 0x0097F4DC (name is ours)
const wchar_t* s_ResourceFileNames[GraphicsResource::RESOURCE_MAX] = {
    L"/shader/nwfont_RectDrawerShader.shbin",
    L"/shader/nwfont_TextWriterShader.shbin",
    L"/shader/nwlyt_PaneShader.shbin",
    L"/shader/nwlyt_ConstColorShader.shbin",
};
// 0x00AEFF68 (name is ours)
wchar_t s_ResourcePaths[GraphicsResource::RESOURCE_MAX][256];

// the quad of the pane shader: the vertex indices and the corners (x, y)
// 0x008B4C5C (name is ours)
const u16 s_QuadIndices[6] = {1, 0, 3, 0, 2, 3};
// 0x008B4C68 (name is ours)
const s16 s_QuadVertices[4][2] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};

// the GL enums of the indices of materials (names after the OpenGL ES API where it has them)
// GL_NEAREST, GL_LINEAR
// 0x008B4C08 (name is ours)
const GLint s_TexFilters[2] = {0x2600, 0x2601};
// GL_REPLACE, GL_MODULATE, GL_ADD, GL_ADD_SIGNED, GL_INTERPOLATE, GL_SUBTRACT, then two combine
// modes of the gx library
// 0x008B4C10 (name is ours)
const GLint s_TexEnvCombines[8] = {0x1E01, 0x2100, 0x0104, 0x8574, 0x8575, 0x84E7, 0x6402, 0x6401};
// GL_TEXTURE0 - GL_TEXTURE3, GL_CONSTANT, GL_PRIMARY_COLOR, GL_PREVIOUS, the previous buffer
// 0x008B4C30 (name is ours)
const GLint s_TexEnvSources[8] = {0x84C0, 0x84C1, 0x84C2, 0x84C3, 0x8576, 0x8577, 0x8578, 0x8579};
// 0x008B4C50 (name is ours)
const f32 s_TexEnvScales[3] = {1.0f, 2.0f, 4.0f};
// GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, then the operands
// of single components of the gx library
// 0x008B4CB8 (name is ours)
const GLint s_TexEnvOperandsRgb[10] = {0x0300, 0x0301, 0x0302, 0x0303, 0x8580, 0x8583, 0x8581, 0x8584, 0x8582, 0x8585};
// GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, then the single components
// 0x008B4CE0 (name is ours)
const GLint s_TexEnvOperandsAlpha[8] = {0x0302, 0x0303, 0x8580, 0x8583, 0x8581, 0x8584, 0x8582, 0x8585};
// GL_NEVER, GL_LESS, GL_LEQUAL, GL_EQUAL, GL_NOTEQUAL, GL_GEQUAL, GL_GREATER, GL_ALWAYS
// 0x008B4D00 (name is ours)
const GLint s_AlphaTestFuncs[8] = {0x0200, 0x0201, 0x0203, 0x0202, 0x0205, 0x0206, 0x0204, 0x0207};

// the shader object of the fixed fragment shader of the gx library
const GLuint FRAGMENT_SHADER_FIXED = 0xFFFFFFFF;
} // namespace

// 0x004B5C80 (name is ours)
nw::lyt::GraphicsResource::GraphicsResource()
    : m_pRectDrawerShader(nullptr), m_RectDrawerShaderSize(0), m_GlProgram(0), m_GlProgramConstColor(0), m_Initialized(false)
{
}

// 0x004B5CB4 | nintendogs:callgraph [tier C]
nw::lyt::GraphicsResource::~GraphicsResource()
{
    if (m_Initialized) {
        m_Initialized = false;
        glUseProgram(0);
        glDeleteProgram(m_GlProgram);
        m_GlProgram = 0;
        glDeleteProgram(m_GlProgramConstColor);
        m_GlProgramConstColor = 0;
        glDeleteBuffers(2, m_VertexBufferObjects);
        if (m_pRectDrawerShader != nullptr) {
            Layout::FreeMemory(m_pRectDrawerShader);
        }
        m_pRectDrawerShader = nullptr;
        m_RectDrawerShaderSize = 0;
        m_TextWriter.m_pTextWriterResource = nullptr;
        m_TextWriterResource.Finalize();
    }
}

// 0x004B57E8 | nintendogs:bytes-fuzzy [tier A]
void nw::lyt::GraphicsResource::SetResource(int resType, void* pData, u32 size, bool isOwned)
{
    switch (resType) {
    case RESOURCE_FONT_RECTDRAWER_SHADER:
        m_pRectDrawerShader = Layout::AllocMemory(size);
        m_RectDrawerShaderSize = size;
        std::memcpy(m_pRectDrawerShader, pData, size);
        if (isOwned) {
            Layout::FreeMemory(pData);
        }
        break;
    case RESOURCE_FONT_TEXTWRITER_SHADER:
        m_TextWriterResource.InitResource(pData, size);
        m_TextWriter.m_pTextWriterResource = &m_TextWriterResource;
        if (isOwned) {
            Layout::FreeMemory(pData);
        }
        break;
    case RESOURCE_LYT_PANE_SHADER: {
        m_GlProgram = glCreateProgram();
        GLuint shader = glCreateShader(internal::gl::VERTEX_SHADER);
        glShaderBinary(1, &shader, internal::gl::SHADER_BINARY_FORMAT, pData, size);
        glAttachShader(m_GlProgram, shader);
        glAttachShader(m_GlProgram, FRAGMENT_SHADER_FIXED);
        glDeleteShader(shader);
        glBindAttribLocation(m_GlProgram, 0, "aVertexIndex");
        glLinkProgram(m_GlProgram);
        glUseProgram(m_GlProgram);
        glUniform1i(glGetUniformLocation(m_GlProgram, "dmp_FragOperation.mode"), internal::gl::FRAGMENT_MODE_DEFAULT);
        glUniform1i(glGetUniformLocation(m_GlProgram, "dmp_FragmentLighting.enabled"), 0);
        if (isOwned) {
            Layout::FreeMemory(pData);
        }
        break;
    }
    case RESOURCE_LYT_CONSTCOLOR_SHADER: {
        m_GlProgramConstColor = glCreateProgram();
        GLuint shader = glCreateShader(internal::gl::VERTEX_SHADER);
        glShaderBinary(1, &shader, internal::gl::SHADER_BINARY_FORMAT, pData, size);
        glAttachShader(m_GlProgramConstColor, shader);
        glAttachShader(m_GlProgramConstColor, FRAGMENT_SHADER_FIXED);
        glDeleteShader(shader);
        glBindAttribLocation(m_GlProgramConstColor, 0, "aPosition");
        glLinkProgram(m_GlProgramConstColor);
        if (isOwned) {
            Layout::FreeMemory(pData);
        }
        break;
    }
    }
}

// 0x004B5708 | nintendogs:callseq [tier A]
void nw::lyt::GraphicsResource::FinishSetup()
{
    nn::math::ARMv6::MTX34CopyAsm(&m_ModelViewMtx, &nn::math::MTX34::Identity());
    InitVBO();
    for (int i = 0; i < UNIFORM_MAX; ++i) {
        if (s_UniformNames[i] == nullptr) {
            m_UniformLocations[i] = 0;
        } else {
            m_UniformLocations[i] = glGetUniformLocation(m_GlProgram, s_UniformNames[i]);
        }
    }
    m_Initialized = true;
}

// 0x004B5A1C | libgarden [tier A]
void nw::lyt::GraphicsResource::ResetGlState()
{
    m_IsVertexAttribDirty = true;
}

// 0x004B5B84 | libgarden [tier A]
void nw::lyt::GraphicsResource::ResetGlProgramState()
{
    m_NumTexEnv = 6;
    m_IsTexEnvModeValid = false;
    m_TexEnvMode = 0;
}

// 0x004B5B10 | libgarden [tier A]
void nw::lyt::GraphicsResource::SetProjectionMtx(const nn::math::MTX44& mtx)
{
    glUseProgram(m_GlProgram);
    glUniformMatrix4fv(m_UniformLocations[UNIFORM_PROJECTION], 1, 1, mtx.m[0]);

    const GLuint programConstColor = m_GlProgramConstColor;
    glUseProgram(programConstColor);
    glUniformMatrix4fv(glGetUniformLocation(programConstColor, "uProjection"), 1, 1, mtx.m[0]);

    glUseProgram(m_TextWriterResource.m_ProgramId);
    m_TextWriterResource.SetProjectionMtx(mtx);
}

// 0x004B5A28 | nintendogs:bytes [tier A]
const wchar_t* nw::lyt::GraphicsResource::GetResourcePath(int resType)
{
    if (!s_IsResourcePathInitialized) {
        for (int i = 0; i < RESOURCE_MAX; ++i) {
            wchar_t* dst = s_ResourcePaths[i];
            const wchar_t* src = s_ResourceRootPath;
            int len = 0;
            while (src[len] != L'\0') {
                dst[len] = src[len];
                ++len;
            }
            dst[len] = src[len];

            dst += len;
            src = s_ResourceFileNames[i];
            len = 0;
            while (src[len] != L'\0') {
                dst[len] = src[len];
                ++len;
            }
            dst[len] = src[len];
        }
        s_IsResourcePathInitialized = true;
    }

    if (static_cast<u32>(resType) < RESOURCE_MAX) {
        return s_ResourcePaths[resType];
    }
    return nullptr;
}

// 0x004B5B9C | nintendogs:bytes [tier A]
void nw::lyt::GraphicsResource::InitVBO()
{
    glGenBuffers(2, m_VertexBufferObjects);

    glBindBuffer(internal::gl::ELEMENT_ARRAY_BUFFER, m_VertexBufferObjects[0]);
    glBufferData(internal::gl::ELEMENT_ARRAY_BUFFER, sizeof(s_QuadIndices), s_QuadIndices, internal::gl::STATIC_DRAW);
    glBindBuffer(internal::gl::ELEMENT_ARRAY_BUFFER, 0);

    glBindBuffer(internal::gl::ARRAY_BUFFER, m_VertexBufferObjects[1]);
    glBufferData(internal::gl::ARRAY_BUFFER, sizeof(s_QuadVertices), s_QuadVertices, internal::gl::STATIC_DRAW);
    glBindBuffer(internal::gl::ARRAY_BUFFER, 0);
}

// 0x004B5C24 (name is ours)
void nw::lyt::GraphicsResource::SetupVertexAttrib()
{
    glBindBuffer(internal::gl::ELEMENT_ARRAY_BUFFER, m_VertexBufferObjects[0]);
    glBindBuffer(internal::gl::ARRAY_BUFFER, m_VertexBufferObjects[1]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, internal::gl::SHORT, 0, 0, nullptr);
}

// 0x004BEE70 | libgarden [tier A]
void nw::lyt::GraphicsResource::FinalizeGraphics()
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
    glBindBuffer(internal::gl::ARRAY_BUFFER, 0);
    glBindBuffer(internal::gl::ELEMENT_ARRAY_BUFFER, 0);
}

// 0x004BF034 (name is ours)
void nw::lyt::GraphicsResource::SetTexEnvSrcRgb(int stage, u32 src0, u32 src1, u32 src2)
{
    const GLint srcs[3] = {s_TexEnvSources[src0], s_TexEnvSources[src1], s_TexEnvSources[src2]};
    glUniform3iv(m_UniformLocations[UNIFORM_TEXENV0_SRCRGB + stage], 1, srcs);
}

// 0x004BF098 (name is ours)
void nw::lyt::GraphicsResource::SetTexEnvSrcAlpha(int stage, u32 src0, u32 src1, u32 src2)
{
    const GLint srcs[3] = {s_TexEnvSources[src0], s_TexEnvSources[src1], s_TexEnvSources[src2]};
    glUniform3iv(m_UniformLocations[UNIFORM_TEXENV0_SRCALPHA + stage], 1, srcs);
}

// 0x004BF240 (name is ours)
void nw::lyt::GraphicsResource::SetTexEnvOperandRgb(int stage, u32 op0, u32 op1, u32 op2)
{
    const GLint ops[3] = {s_TexEnvOperandsRgb[op0], s_TexEnvOperandsRgb[op1], s_TexEnvOperandsRgb[op2]};
    glUniform3iv(m_UniformLocations[UNIFORM_TEXENV0_OPERANDRGB + stage], 1, ops);
}

// 0x004BF33C (name is ours)
void nw::lyt::GraphicsResource::SetTexEnvOperandAlpha(int stage, u32 op0, u32 op1, u32 op2)
{
    const GLint ops[3] = {s_TexEnvOperandsAlpha[op0], s_TexEnvOperandsAlpha[op1], s_TexEnvOperandsAlpha[op2]};
    glUniform3iv(m_UniformLocations[UNIFORM_TEXENV0_OPERANDALPHA + stage], 1, ops);
}

// 0x004BF228 (name is ours)
void nw::lyt::GraphicsResource::SetTexEnvCombineRgb(int stage, u32 combine)
{
    glUniform1i(m_UniformLocations[UNIFORM_TEXENV0_COMBINERGB + stage], s_TexEnvCombines[combine]);
}

// 0x004BF324 (name is ours)
void nw::lyt::GraphicsResource::SetTexEnvCombineAlpha(int stage, u32 combine)
{
    glUniform1i(m_UniformLocations[UNIFORM_TEXENV0_COMBINEALPHA + stage], s_TexEnvCombines[combine]);
}

// 0x004BF07C (name is ours)
void nw::lyt::GraphicsResource::SetTexEnvScaleRgb(int stage, u32 scale)
{
    glUniform1f(m_UniformLocations[UNIFORM_TEXENV0_SCALERGB + stage], s_TexEnvScales[scale]);
}

// 0x004BF288 (name is ours)
void nw::lyt::GraphicsResource::SetTexEnvScaleAlpha(int stage, u32 scale)
{
    glUniform1f(m_UniformLocations[UNIFORM_TEXENV0_SCALEALPHA + stage], s_TexEnvScales[scale]);
}

// 0x004BF0E0 (name is ours)
void nw::lyt::GraphicsResource::SetTexEnvConstColor(int stage, ut::Color8 color)
{
    const f32 values[4] = {color.r * (1.0f / 255.0f), color.g * (1.0f / 255.0f), color.b * (1.0f / 255.0f), color.a * (1.0f / 255.0f)};
    glUniform4fv(m_UniformLocations[UNIFORM_TEXENV0_CONSTRGBA + stage], 1, values);
}

// 0x004BF15C (name is ours)
void nw::lyt::GraphicsResource::SetTexEnvConstColor(int stage, ut::Color8 rgb, ut::Color8 alpha)
{
    const f32 values[4] = {rgb.r * (1.0f / 255.0f), rgb.g * (1.0f / 255.0f), rgb.b * (1.0f / 255.0f), alpha.a * (1.0f / 255.0f)};
    glUniform4fv(m_UniformLocations[UNIFORM_TEXENV0_CONSTRGBA + stage], 1, values);
}

// 0x004BF2A4 (name is ours)
void nw::lyt::GraphicsResource::SetTexEnvBufferColor(ut::Color8 color)
{
    glUniform4f(m_UniformLocations[UNIFORM_TEXENV0_BUFFERCOLOR], color.r * (1.0f / 255.0f), color.g * (1.0f / 255.0f), color.b * (1.0f / 255.0f),
                color.a * (1.0f / 255.0f));
}

// 0x004BF2F4 (name is ours)
void nw::lyt::GraphicsResource::SetTexEnvBufferInput(int stage, bool isRgbPrevious, bool isAlphaPrevious)
{
    glUniform2i(m_UniformLocations[UNIFORM_TEXENV0_BUFFERCOLOR + stage], isRgbPrevious ? internal::gl::PREVIOUS : internal::gl::PREVIOUS_BUFFER,
                isAlphaPrevious ? internal::gl::PREVIOUS : internal::gl::PREVIOUS_BUFFER);
}

// 0x004BF214 (name is ours)
void nw::lyt::GraphicsResource::SetAlphaTestFunc(u32 func)
{
    glUniform1i(m_UniformLocations[UNIFORM_FRAGOP_ALPHATESTFUNC], s_AlphaTestFuncs[func]);
}

// 0x007ADE4C (name is ours)
void nw::lyt::GraphicsResource::SetAlphaTestEnable(bool isEnable)
{
    glUniform1i(m_UniformLocations[UNIFORM_FRAGOP_ENABLEALPHATEST], isEnable);
}

// 0x007ADE14 (name is ours)
void nw::lyt::GraphicsResource::SetAlphaRefValue(f32 ref)
{
    glUniform1f(m_UniformLocations[UNIFORM_FRAGOP_ALPHAREFVALUE], ref);
}

// 0x004BF384 (name is ours)
void nw::lyt::GraphicsResource::SetTextureSamplerType(int idx, GLint type)
{
    glUniform1i(m_UniformLocations[UNIFORM_TEXTURE0_SAMPLERTYPE + idx], type);
}

// 0x004BF1D8 (name is ours)
void nw::lyt::GraphicsResource::SetTexMagFilter(u32 filter)
{
    glTexParameteri(internal::gl::TEXTURE_2D, internal::gl::TEXTURE_MAG_FILTER, s_TexFilters[filter]);
}

// 0x004BF1F4 (name is ours)
void nw::lyt::GraphicsResource::SetTexMinFilter(u32 filter)
{
    glTexParameteri(internal::gl::TEXTURE_2D, internal::gl::TEXTURE_MIN_FILTER, s_TexFilters[filter]);
}

} // namespace lyt
} // namespace nw
