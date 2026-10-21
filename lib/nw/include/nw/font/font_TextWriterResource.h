#pragma once

#include "decomp.h"
#include "nn/gx/gx_Types.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_MTX44.h"
#include "nw/ut/ut_Color8.h"

namespace nw {
namespace font {
// The GL objects of the text drawing: the shader program, a texture, the uniform locations and
// the vertices of one character (CharWriter::PrintGlyph draws them as a triangle fan). Member,
// struct and enum names are ours; the uniform names are the strings of InitResource.
class TextWriterResource
{
public:
    // the uniforms of the program (the order of m_UniformLocations)
    enum Uniform
    {
        UNIFORM_PROJECTION,              // uProjection
        UNIFORM_MODELVIEW,               // uModelView
        UNIFORM_FRAGMENT_LIGHTING,       // dmp_FragmentLighting.enabled
        UNIFORM_TEXTURE0_SAMPLER_TYPE,   // dmp_Texture[0].samplerType
        UNIFORM_FOG_MODE,                // dmp_Fog.mode
        UNIFORM_ALPHA_TEST_ENABLE,       // dmp_FragOperation.enableAlphaTest
        UNIFORM_ALPHA_TEST_REF,          // dmp_FragOperation.alphaRefValue
        UNIFORM_ALPHA_TEST_FUNC,         // dmp_FragOperation.alphaTestFunc
        UNIFORM_TEXENV3_SRC_RGB,         // dmp_TexEnv[3..5]: srcRgb ... constRgba
        UNIFORM_TEXENV_COUNT = 3,
        UNIFORM_TEXENV_PARAMS = 9,
        UNIFORM_COUNT = UNIFORM_TEXENV3_SRC_RGB + UNIFORM_TEXENV_COUNT * UNIFORM_TEXENV_PARAMS
    };
    // the parameters of a dmp_TexEnv stage (offsets in the uniforms of the stage)
    enum TexEnvParam
    {
        TEXENV_SRC_RGB,
        TEXENV_SRC_ALPHA,
        TEXENV_OPERAND_RGB,
        TEXENV_OPERAND_ALPHA,
        TEXENV_COMBINE_RGB,
        TEXENV_COMBINE_ALPHA,
        TEXENV_SCALE_RGB,
        TEXENV_SCALE_ALPHA,
        TEXENV_CONST_RGBA
    };
    // the attributes of the vertices
    enum Attribute
    {
        ATTRIBUTE_POSITION,    // aPosition
        ATTRIBUTE_POSITION_Z,  // aPositionZ
        ATTRIBUTE_COLOR,       // aColor
        ATTRIBUTE_TEXCOORD,    // aTexCoord
        ATTRIBUTE_COUNT
    };
    // a vertex of the character: right top, left top, left bottom, right bottom
    struct Vertex
    {
        Vertex(); // 0x004D993C (name is ours)

        f32 x;            // 0x00
        f32 y;            // 0x04
        ut::Color8 color; // 0x08
        f32 u;            // 0x0C
        f32 v;            // 0x10
    };
    static const int VERTEX_COUNT = 4;

    TextWriterResource(); // 0x004D9178 (name is ours)
    ~TextWriterResource(); // 0x004D91B4 | nintendogs:callgraph [tier C]

    // the program from the vertex shader binary (with the fixed fragment shader)
    void InitResource(const void* pShaderBinary, u32 size); // 0x004D8A70 (name is ours)
    void Finalize(); // 0x004D9124 | nintendogs:callgraph [tier C]

    // (name is ours)
    void SetProjectionMtx(const nn::math::MTX44& mtx) const; // 0x007452E0 (name is ours)
    void SetViewMtx(const nn::math::MTX34& mtx) const; // 0x007ADEC8 | libgarden [tier A]
    // the z of the characters (a constant attribute)
    void SetPositionZ(f32 z); // 0x007AE450 (name is ours)

    // (name is ours)
    static void DisableVertexAttribArrays(); // 0x004D8A50 (name is ours)

    GLint GetUniformLocation(int uniform) const { return m_UniformLocations[uniform]; }
    GLint GetTexEnvUniformLocation(int stage, int param) const
    {
        return m_UniformLocations[UNIFORM_TEXENV3_SRC_RGB + (stage - 3) * UNIFORM_TEXENV_PARAMS + param];
    }

    GLuint m_ProgramId;                        // 0x000
    GLuint m_TextureId;                        // 0x004
    Vertex m_Vertices[VERTEX_COUNT];           // 0x008
    GLint m_UniformLocations[UNIFORM_COUNT];   // 0x058
    const void* m_pLastTexture;                // 0x0E4 the image in m_TextureId
    f32 m_PositionZ;                           // 0x0E8
    bool m_IsInitialized;                      // 0x0EC
};
ASSERT_OFFSET(TextWriterResource, m_UniformLocations, 0x58);
ASSERT_OFFSET(TextWriterResource, m_pLastTexture, 0xE4);
ASSERT_SIZE(TextWriterResource, 0xF0);
} // namespace font
} // namespace nw
