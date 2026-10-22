#pragma once

#include "decomp.h"
#include "nn/gx/gx_Types.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_MTX44.h"
#include "nw/font/font_TextWriterBase.h"
#include "nw/font/font_TextWriterResource.h"
#include "nw/ut/ut_Color8.h"

namespace nw {
namespace lyt {
// The GL resources of the layout drawing: the shader of the font RectDrawer (a copy), the text
// writer, the pane shader with its uniform locations, the shader of constant colors and the
// vertex buffers of a quad; plus a cache of the GL state the materials set. The class name and
// the names of FinishSetup, SetResource, ResetGlState, GetResourcePath, SetProjectionMtx,
// ResetGlProgramState, InitVBO and FinalizeGraphics are from the reference symbols; the others
// are ours.
class GraphicsResource
{
public:
    // the resources (SetResource / GetResourcePath)
    enum ResourceType
    {
        RESOURCE_FONT_RECTDRAWER_SHADER,
        RESOURCE_FONT_TEXTWRITER_SHADER,
        RESOURCE_LYT_PANE_SHADER,
        RESOURCE_LYT_CONSTCOLOR_SHADER,
        RESOURCE_MAX
    };
    // the uniforms of the pane shader (the order of s_UniformNames)
    enum Uniform
    {
        UNIFORM_PROJECTION,
        UNIFORM_MODELVIEW,
        UNIFORM_TEXMTX0,
        UNIFORM_TEXMTX1,
        UNIFORM_TEXMTX2,
        UNIFORM_COLOR,
        UNIFORM_TRANSFORM,
        UNIFORM_FRAMESPEC,
        UNIFORM_VERTEXCOLOR,
        UNIFORM_VERTEXTEXCOORD0,
        UNIFORM_VERTEXTEXCOORD1,
        UNIFORM_VERTEXTEXCOORD2,
        UNIFORM_RCPTEXSIZE0,
        UNIFORM_TEXTURE0_SAMPLERTYPE,       // 4 textures
        UNIFORM_TEXENV0_COMBINERGB = 17,    // 6 stages each
        UNIFORM_TEXENV0_COMBINEALPHA = 23,
        UNIFORM_TEXENV0_SRCRGB = 29,
        UNIFORM_TEXENV0_SRCALPHA = 35,
        UNIFORM_TEXENV0_OPERANDRGB = 41,
        UNIFORM_TEXENV0_OPERANDALPHA = 47,
        UNIFORM_TEXENV0_SCALERGB = 53,
        UNIFORM_TEXENV0_SCALEALPHA = 59,
        UNIFORM_TEXENV0_CONSTRGBA = 65,
        UNIFORM_TEXENV0_BUFFERCOLOR = 71,
        UNIFORM_TEXENV1_BUFFERINPUT = 72,   // stages 1 - 4
        UNIFORM_FRAGOP_ENABLEALPHATEST = 76,
        UNIFORM_FRAGOP_ALPHAREFVALUE = 77,
        UNIFORM_FRAGOP_ALPHATESTFUNC = 78,
        UNIFORM_MAX
    };
    // the texture coordinate sets of the vertices
    static const int VERTEX_TEXCOORD_MAX = 3;

    GraphicsResource(); // 0x004B5C80 (name is ours)
    ~GraphicsResource(); // 0x004B5CB4 | nintendogs:callgraph [tier C]

    // the resource of the type (data of size bytes; isOwned: freed with FreeMemory)
    void SetResource(int resType, void* pData, u32 size, bool isOwned); // 0x004B57E8 | nintendogs:bytes-fuzzy [tier A]
    // the uniform locations and the vertex buffers, after the resources are set
    void FinishSetup(); // 0x004B5708 | nintendogs:callseq [tier A]
    // the vertex attributes are set again at the next drawing
    void ResetGlState(); // 0x004B5A1C | libgarden [tier A]
    // the texture combiner state is unknown
    void ResetGlProgramState(); // 0x004B5B84 | libgarden [tier A]
    void SetProjectionMtx(const nn::math::MTX44& mtx); // 0x004B5B10 | libgarden [tier A]
    // the path of the file of the resource ("rom:/shader/...")
    static const wchar_t* GetResourcePath(int resType); // 0x004B5A28 | nintendogs:bytes [tier A]
    // the vertex attributes and buffers are unbound
    static void FinalizeGraphics(); // 0x004BEE70 | libgarden [tier A]

    // the uniforms of the texture combiner (the values are indices of tables of GL enums)
    void SetTexEnvSrcRgb(int stage, u32 src0, u32 src1, u32 src2); // 0x004BF034 (name is ours)
    void SetTexEnvSrcAlpha(int stage, u32 src0, u32 src1, u32 src2); // 0x004BF098 (name is ours)
    void SetTexEnvOperandRgb(int stage, u32 op0, u32 op1, u32 op2); // 0x004BF240 (name is ours)
    void SetTexEnvOperandAlpha(int stage, u32 op0, u32 op1, u32 op2); // 0x004BF33C (name is ours)
    void SetTexEnvCombineRgb(int stage, u32 combine); // 0x004BF228 (name is ours)
    void SetTexEnvCombineAlpha(int stage, u32 combine); // 0x004BF324 (name is ours)
    void SetTexEnvScaleRgb(int stage, u32 scale); // 0x004BF07C (name is ours)
    void SetTexEnvScaleAlpha(int stage, u32 scale); // 0x004BF288 (name is ours)
    void SetTexEnvConstColor(int stage, ut::Color8 color); // 0x004BF0E0 (name is ours)
    // the RGB of one color, the alpha of the other
    void SetTexEnvConstColor(int stage, ut::Color8 rgb, ut::Color8 alpha); // 0x004BF15C (name is ours)
    void SetTexEnvBufferColor(ut::Color8 color); // 0x004BF2A4 (name is ours)
    // the combiner buffer of the stage takes the previous result (true) or the previous buffer
    void SetTexEnvBufferInput(int stage, bool isRgbPrevious, bool isAlphaPrevious); // 0x004BF2F4 (name is ours)
    void SetAlphaTestFunc(u32 func); // 0x004BF214 (name is ours)
    // (inline, copies out of line next to glUniform1i / glUniform1f)
    void SetAlphaTestEnable(bool isEnable); // 0x007ADE4C (name is ours)
    void SetAlphaRefValue(f32 ref); // 0x007ADE14 (name is ours)
    void SetTextureSamplerType(int idx, GLint type); // 0x004BF384 (name is ours)
    // the filters of the bound texture
    static void SetTexMagFilter(u32 filter); // 0x004BF1D8 (name is ours)
    static void SetTexMinFilter(u32 filter); // 0x004BF1F4 (name is ours)

    // the vertex attributes of the quad
    void SetupVertexAttrib(); // 0x004B5C24 (name is ours)

    // (inline)
    GLint GetUniformLocation(int uniform) const { return m_UniformLocations[uniform]; }

private:
    void InitVBO(); // 0x004B5B9C | nintendogs:bytes [tier A]

public:
    void* m_pRectDrawerShader;                          // 0x000
    u32 m_RectDrawerShaderSize;                         // 0x004
    font::TextWriterResource m_TextWriterResource;      // 0x008
    font::TextWriterBase<wchar_t> m_TextWriter;         // 0x0F8
    GLuint m_GlProgram;                                 // 0x15C the pane shader
    GLuint m_GlProgramConstColor;                       // 0x160
    nn::math::MTX34 m_ModelViewMtx;                     // 0x164
    GLuint m_VertexBufferObjects[2];                    // 0x194 the vertices, the indices
    GLint m_UniformLocations[UNIFORM_MAX];              // 0x19C
    s8 m_VertexTexCoordSrc[VERTEX_TEXCOORD_MAX];        // 0x2D8 the texture coordinates of the
                                                        //       vertex sets (-1: none)
    u8 m_NumTexEnv;                                     // 0x2DB the combiner stages set
    u8 m_TexEnvMode;                                    // 0x2DC
    bool m_IsTexEnvModeValid;                           // 0x2DD
    bool m_IsModelViewMtxLoaded;                        // 0x2DE
    bool m_IsVertexAttribDirty;                         // 0x2DF
    bool m_Initialized;                                 // 0x2E0
};
ASSERT_OFFSET(GraphicsResource, m_TextWriter, 0xF8);
ASSERT_OFFSET(GraphicsResource, m_GlProgram, 0x15C);
ASSERT_OFFSET(GraphicsResource, m_UniformLocations, 0x19C);
ASSERT_OFFSET(GraphicsResource, m_Initialized, 0x2E0);
ASSERT_SIZE(GraphicsResource, 0x2E4);
} // namespace lyt
} // namespace nw
