#include "nn/gx/gx_Api.h"
#include "nw/font/font_TextWriterResource.h"

namespace nw {
namespace font {
namespace {
// the values of the OpenGL ES API and its DMP extensions (names are ours)
const GLenum SHADER_VERTEX = 0x8B31;
const GLenum SHADER_BINARY_PLATFORM = 0x6000;
// the fixed fragment shader
const GLuint SHADER_FRAGMENT_FIXED = 0xFFFFFFFF;

} // namespace

// 0x004D993C (name is ours)
DECOMP_NOINLINE nw::font::TextWriterResource::Vertex::Vertex()
{
    // (only the color: white by the default constructor of Color8)
}

// 0x004D9178 (name is ours)
nw::font::TextWriterResource::TextWriterResource() : m_pLastTexture(nullptr), m_PositionZ(0.0f), m_IsInitialized(false)
{
}

// 0x004D91B4 | nintendogs:callgraph [tier C]
nw::font::TextWriterResource::~TextWriterResource()
{
    Finalize();
}

// 0x004D8A50 (name is ours)
void nw::font::TextWriterResource::DisableVertexAttribArrays()
{
    for (int i = 0; i < ATTRIBUTE_COUNT; i++) {
        glDisableVertexAttribArray(i);
    }
}

// 0x004D8A70 (name is ours)
void nw::font::TextWriterResource::InitResource(const void* pShaderBinary, u32 size)
{
    m_ProgramId = glCreateProgram();
    GLuint shader = glCreateShader(SHADER_VERTEX);
    glShaderBinary(1, &shader, SHADER_BINARY_PLATFORM, pShaderBinary, size);
    glAttachShader(m_ProgramId, shader);
    glDeleteShader(shader);
    glAttachShader(m_ProgramId, SHADER_FRAGMENT_FIXED);
    glBindAttribLocation(m_ProgramId, ATTRIBUTE_POSITION, "aPosition");
    glBindAttribLocation(m_ProgramId, ATTRIBUTE_POSITION_Z, "aPositionZ");
    glBindAttribLocation(m_ProgramId, ATTRIBUTE_COLOR, "aColor");
    glBindAttribLocation(m_ProgramId, ATTRIBUTE_TEXCOORD, "aTexCoord");
    glLinkProgram(m_ProgramId);
    glGenTextures(1, &m_TextureId);

    GLint* pLocations = m_UniformLocations;
    pLocations[UNIFORM_PROJECTION] = glGetUniformLocation(m_ProgramId, "uProjection");
    pLocations[UNIFORM_MODELVIEW] = glGetUniformLocation(m_ProgramId, "uModelView");
    pLocations[UNIFORM_FRAGMENT_LIGHTING] = glGetUniformLocation(m_ProgramId, "dmp_FragmentLighting.enabled");
    pLocations[UNIFORM_TEXTURE0_SAMPLER_TYPE] = glGetUniformLocation(m_ProgramId, "dmp_Texture[0].samplerType");
    pLocations[UNIFORM_FOG_MODE] = glGetUniformLocation(m_ProgramId, "dmp_Fog.mode");
    pLocations[UNIFORM_ALPHA_TEST_ENABLE] = glGetUniformLocation(m_ProgramId, "dmp_FragOperation.enableAlphaTest");
    pLocations[UNIFORM_ALPHA_TEST_REF] = glGetUniformLocation(m_ProgramId, "dmp_FragOperation.alphaRefValue");
    pLocations[UNIFORM_ALPHA_TEST_FUNC] = glGetUniformLocation(m_ProgramId, "dmp_FragOperation.alphaTestFunc");
    pLocations += UNIFORM_TEXENV3_SRC_RGB;
    pLocations[0] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[3].srcRgb");
    pLocations[1] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[3].srcAlpha");
    pLocations[2] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[3].operandRgb");
    pLocations[3] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[3].operandAlpha");
    pLocations[4] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[3].combineRgb");
    pLocations[5] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[3].combineAlpha");
    pLocations[6] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[3].scaleRgb");
    pLocations[7] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[3].scaleAlpha");
    pLocations[8] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[3].constRgba");
    pLocations[9] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[4].srcRgb");
    pLocations[10] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[4].srcAlpha");
    pLocations[11] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[4].operandRgb");
    pLocations[12] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[4].operandAlpha");
    pLocations[13] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[4].combineRgb");
    pLocations[14] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[4].combineAlpha");
    pLocations[15] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[4].scaleRgb");
    pLocations[16] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[4].scaleAlpha");
    pLocations[17] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[4].constRgba");
    pLocations[18] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[5].srcRgb");
    pLocations[19] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[5].srcAlpha");
    pLocations[20] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[5].operandRgb");
    pLocations[21] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[5].operandAlpha");
    pLocations[22] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[5].combineRgb");
    pLocations[23] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[5].combineAlpha");
    pLocations[24] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[5].scaleRgb");
    pLocations[25] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[5].scaleAlpha");
    pLocations[26] = glGetUniformLocation(m_ProgramId, "dmp_TexEnv[5].constRgba");
    m_IsInitialized = true;
}

// 0x004D9124 | nintendogs:callgraph [tier C]
void nw::font::TextWriterResource::Finalize()
{
    if (!m_IsInitialized) {
        return;
    }
    DisableVertexAttribArrays();
    glDeleteTextures(1, &m_TextureId);
    m_IsInitialized = false;
    glUseProgram(0);
    glDeleteProgram(m_ProgramId);
}

// 0x007452E0 (name is ours)
void nw::font::TextWriterResource::SetProjectionMtx(const nn::math::MTX44& mtx) const
{
    glUniform4fv(m_UniformLocations[UNIFORM_PROJECTION], 4, reinterpret_cast<const GLfloat*>(&mtx));
}

// (falls into glUniform4fv)
// 0x007ADEC8 | libgarden [tier A]
void nw::font::TextWriterResource::SetViewMtx(const nn::math::MTX34& mtx) const
{
    glUniform4fv(m_UniformLocations[UNIFORM_MODELVIEW], 3, reinterpret_cast<const GLfloat*>(&mtx));
}

// (falls into glVertexAttrib1f)
// 0x007AE450 (name is ours)
void nw::font::TextWriterResource::SetPositionZ(f32 z)
{
    m_PositionZ = z;
    glVertexAttrib1f(ATTRIBUTE_POSITION_Z, z);
}

} // namespace font
} // namespace nw
