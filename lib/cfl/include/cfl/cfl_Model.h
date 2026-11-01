#pragma once

// The model of a Mii: a buffer with the shapes and the command lists of the program (CFLiCharModelData) and the
// textures of the expressions. CFLiTexHandle, CFLiTexHeader, CFLiResShape, CFLProgramContext, CFLTexWrap and
// CFLTexFmt are named in the reference symbols; all members and the other names are ours.

#include "cfl/cfl_Api.h"
#include "nn/gx/gx_Api.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_MTX44.h"

#define CFL_EXPRESSION_MAX 19

enum CFLTexWrap
{
    CFL_TEX_WRAP_0 = 0,
};

enum CFLTexFmt
{
    CFL_TEX_FMT_0 = 0,
};

// the header of a texture of the resource archive; the image follows
struct CFLiTexHeader
{
    u16 width;      // 0x0
    u16 height;     // 0x2
    u8 mipLevels;   // 0x4
    u8 format;      // 0x5
    u8 wrapS;       // 0x6
    u8 wrapT;       // 0x7
};
ASSERT_SIZE(CFLiTexHeader, 0x8);

// the uniforms of the shader of CFL (one global, set by CFL_SetShaderParameter; members are ours)
struct CFLProgramContext
{
    u32 _0;                    // 0x00
    u32 _4;                    // 0x04
    u32 _8;                    // 0x08
    GLint normalLocation;      // 0x0C
    u32 _10;                   // 0x10
    bool directUniforms;       // 0x14 the float uniforms in the order of the matrices (else w, z, y, x)
    u32 _18;                   // 0x18
    u32 _1C;                   // 0x1C
    u32 _20;                   // 0x20
    GLenum textureUnit;        // 0x24
    u32 projectionIndex;       // 0x28
    u32 modelViewIndex;        // 0x2C
    u32 normalIndex;           // 0x30
    u32 index34;               // 0x34
    s32 _38;                   // 0x38 the number of input attributes of the shader
    u32 attribute[3];          // 0x3C the input registers of position, normal and texture coordinate
    u32 _48;                   // 0x48
    u32 _4C;                   // 0x4C
    s32 textureUnitIndex;      // 0x50 3 for texture 0, 4 for texture 1, else 5
};

// (passed by value to CFL_SetShaderParameter; name is ours)
struct CFLiShaderParam
{
    u32 attribute[3];
    GLenum textureUnit;
};

extern CFLProgramContext s_ProgramContext;

// a texture in VRAM or in the buffer of the model
class CFLiTexHandle
{
public:
    void CreateFromRes(const CFLiTexHeader* header, unsigned size); // 0x0022BD8C | nintendogs:bytes [tier B]
    void Create(int width, int height, int mipLevels, CFLTexWrap wrapS, CFLTexWrap wrapT, CFLTexFmt format,
                const void* data, unsigned size, unsigned char flags); // 0x0022BDDC | nintendogs:bytes [tier B]
    void Delete(); // 0x0022BFA4 | nintendogs:bytes [tier B]

    CFLiTexHandle* self;    // 0x00 NULL when there is no texture
    s32 width;              // 0x04
    s32 height;             // 0x08
    s32 allocatedWidth;     // 0x0C rounded up to a power of two
    s32 allocatedHeight;    // 0x10
    s32 mipLevels;          // 0x14
    u32 wrapS;              // 0x18 CFLTexWrap
    u32 wrapT;              // 0x1C CFLTexWrap
    const void* data;       // 0x20
    u32 format;             // 0x24 CFLTexFmt
    uptr address;           // 0x28
    GLenum area;            // 0x2C
    u32 allocated;          // 0x30
};
ASSERT_SIZE(CFLiTexHandle, 0x34);

// a vertex or index buffer of a shape
struct CFLiVertexBuffer
{
    GLuint name;      // 0x0
    void* address;    // 0x4
    GLenum area;      // 0x8
    u32 allocated;    // 0xC
};
ASSERT_SIZE(CFLiVertexBuffer, 0x10);

// a shape of the resource archive (in the buffer of the model)
class CFLiResShape
{
public:
    void DirectDraw(const CFLProgramContext* context) const; // 0x00712A0C | nintendogs:callseq [tier A]

    // an attribute: from the vertex buffer, or constant (index -1)
    struct Attribute
    {
        s32 components;  // 0x00
        s32 index;       // 0x04
        f32 value[3];    // 0x08
    };

    struct Primitive
    {
        u16 mode;        // 0x0 4 triangles, 5 strip, 6 fan
        u16 count;       // 0x2
        u16 offset;      // 0x4 into the index buffer
        u16 _6;          // 0x6
    };

    GLenum type;                 // 0x00 of the vertex arrays
    CFLiVertexBuffer vertices;   // 0x04 (name 0: none)
    s32 stride;                  // 0x14
    Attribute attributes[3];     // 0x18 position, normal, texture coordinate
    CFLiVertexBuffer indices;    // 0x54
    GLenum indexType;            // 0x64
    s32 primitiveNum;            // 0x68
    Primitive primitives[1];     // 0x6C
};
ASSERT_SIZE(CFLiResShape, 0x74);

// a part of the face drawn into the face texture (name is ours)
struct CFLiFacePart
{
    f32 x;                  // 0x00
    f32 y;                  // 0x04
    f32 scaleX;             // 0x08
    f32 scaleY;             // 0x0C
    f32 rotate;             // 0x10 degrees
    s32 flip;               // 0x14
    CFLiTexHandle texture;  // 0x18
};
ASSERT_SIZE(CFLiFacePart, 0x4C);

// a command list the model saved (nngxStartCmdlistSave)
struct CFLiSavedCmdlist
{
    GLuint name;           // 0x00
    u32 _4;                // 0x04
    GLsizei size;          // 0x08
    bool _C;               // 0x0C (the last argument of nngxUseSavedCmdlist)
    GLuint requestOffset;  // 0x10
    GLsizei requestCount;  // 0x14
};
ASSERT_SIZE(CFLiSavedCmdlist, 0x18);

#define CFL_SHAPE_NUM 9

// the buffer of the model (CFL_GetModelBufferSize; the textures follow it)
struct CFLiCharModelData
{
    CFLiResShape shapes[CFL_SHAPE_NUM];   // 0x000
    CFLiTexHandle textures[4];       // 0x414
    f32 facelinePos[3][3];           // 0x4E4 (from the faceline shape)
    f32 _508[3];                     // 0x508
    f32 hairPos[6][3];               // 0x514 (from the hair shape)
    CFLiCharInfo info;               // 0x55C
    u32 _67C;                        // 0x67C
    CFLiCharModelData* self;         // 0x680
    s32 resolution;                  // 0x684
    u32 expressionMask;              // 0x688
    u32 _68C;                        // 0x68C
    u32 _690;                        // 0x690
    CFLiSavedCmdlist opa;            // 0x694
    u32 _6AC;                        // 0x6AC
    CFLiSavedCmdlist xlu;            // 0x6B0
    s32 cullCommandNum;              // 0x6C8
    s32 flipCommandNum;              // 0x6CC
    u32* cullCommands[8];            // 0x6D0 offsets of the face culling commands in the saved lists
    u32* flipCommands[8];            // 0x6F0
    s32 lodCommandNum;               // 0x710
    u32* lodCommands[8];             // 0x714 offsets of the mip level commands in the saved list
    u32* expressionCommand;          // 0x734 the address of the texture of the expression in the command list
    s32 typeCommandNum;              // 0x738
    u32* typeCommands[8];            // 0x73C offsets of the texture type commands in the saved list
    s32 unitCommandNum;              // 0x75C
    u32* unitCommands[4];            // 0x760
    u32 hairColor;                   // 0x770 RGBA8
    u32* hairColorCommand;           // 0x774
    u32 _778;                        // 0x778
    u32 _77C;                        // 0x77C
};
ASSERT_SIZE(CFLiCharModelData, 0x780);

struct CFLCharModel
{
    nn::math::MTX34 posMtx;          // 0x00
    nn::math::MTX34 nrmMtx;          // 0x30
    s32 expression;                  // 0x60
    u32 _64;                         // 0x64
    GLenum filter;                   // 0x68
    CFLiCharModelData* data;         // 0x6C
    CFLiTexHandle* expressionTextures[CFL_EXPRESSION_MAX];  // 0x70
};
ASSERT_SIZE(CFLCharModel, 0xBC);

extern "C" {
void CFL_GetRotateMatrixRadian(nn::math::MTX34* mtx, char axis, f32 radian);
void CFL_GetScaleMatrix(nn::math::MTX34* mtx, f32 x, f32 y, f32 z);
int CFLi_GetExpressionCount(u32 mask);
u32 CFL_GetModelBufferSize(u32 expressionMask);
void CFL_SetMtxPosNrm(CFLCharModel* model, const nn::math::MTX34* pos, const nn::math::MTX34* nrm);
void CFL_SetExpression(CFLCharModel* model, int expression);
void CFL_SetShaderParameter(u32 projectionIndex, u32 modelViewIndex, u32 normalIndex, u32 index34, bool directUniforms,
                            CFLiShaderParam param);
} // extern "C"

// the uniforms (names are ours)
void CFLi_SetUniformMtx24(const f32* values);                                     // 0x001668D8
void CFLi_SetUniformMtx34(u32 index, const nn::math::MTX34* mtx, bool direct);    // 0x00171598
void CFLi_SetUniformMtx44(const f32* mtx);                                        // 0x0017549C
void CFLi_SetModelMatrices(const nn::math::MTX34* pos, const nn::math::MTX34* nrm, bool normalAsUniform);  // 0x0016D118
void CFLi_SetOrthoProjection(int width, int height);                              // 0x0016C448
void CFLi_PutAlphaTest(const CFLProgramContext* context, bool enable);                                              // 0x001660C8

void CFL_GetTranslateMatrix(nn::math::MTX34* mtx, f32 x, f32 y, f32 z);  // 0x00153424 (name is ours)
void CFLi_UntileImage(u8* dst, const u8* src, int width, int height, int unused, int format);  // 0x001765E4 (name is ours)
u32* CFLi_PutViewport(u32* command, int width, int height);                      // 0x0017C278 (name is ours)
void CFLi_PutColorBuffer(uptr address, int width, int height, GLenum format);  // 0x00175350 (name is ours)
void CFLi_PutTexEnv(const CFLProgramContext* context, int mode, bool resetOthers);  // 0x00179EBC (name is ours)

extern const u32 s_TexEnvNone[4];
extern const u32 s_TexEnvPassThrough[20];
extern const u32 s_TexEnvSources[12];
extern const u32 s_TexEnvScales[12];
extern const s32 s_TexFormatBits[12];
void CFLi_PutFragmentOperation(bool colorWrite, int unused, bool depth, bool readColor, u32 blendFunc);  // 0x0017D5C0
u32* CFLi_PutImmediateVertices(u32* command, const CFLProgramContext* context, int unused, int vertexCount,
                               const u8* indices, const f32* positions, const f32* normals,
                               const f32* texCoords);  // 0x00172950
u32* CFLi_DrawQuad(u32* command, const CFLProgramContext* context, int unused, bool textured, f32 x0, f32 x1, f32 y0,
                   f32 y1, f32 z);  // 0x00188F70
void CFLi_BeginRenderTexture(int width, int height, u32 color, bool depth);  // 0x00165984
void CFLi_BindRenderTexture(CFLiTexHandle** texture, int width, int height, bool begin, u32 color,
                            bool depth);  // 0x0017D074
void* CFLi_LoadArchiveTexture(CFLiTexHandle* texture, int section, int index, unsigned area);  // 0x0013DE14 (name is ours)

extern "C" {
void CFL_DeleteCharModel(CFLCharModel* model);  // 0x00152D98 (name is ours)
void CFLi_GetIconMtx(nn::math::MTX44* projection, nn::math::MTX34* view, int width, int height);
} // extern "C"
void CFLi_BindTexture(const CFLiTexHandle* texture, const CFLProgramContext* context, GLenum filter, u32* sizeA,
                      u32* sizeB, u32* sizeC);  // 0x0071403C (name is ours)
void CFLi_DrawFacePart(const CFLiFacePart* part);  // 0x00170674 (name is ours)

extern "C" {
void CFL_DrawOpa(CFLCharModel* model);
void CFL_DrawXlu(CFLCharModel* model);
} // extern "C"
u32* CFLi_BindModelTexture(CFLCharModel* model, CFLiTexHandle* texture, u32 offset);  // 0x00179778 (name is ours)

// how an icon is drawn (members are ours)
struct CFLIconParam
{
    u16 width;                     // 0x00
    u16 height;                    // 0x02
    GLenum format;                 // 0x04 of the texture
    s32 background;                // 0x08 0 the favorite color, 1 color, 2 none
    u32 color;                     // 0x0C
    bool textureAlpha;             // 0x10
    bool skipOpa;                  // 0x11
    void (*callback)(void* arg);   // 0x14 before the model is drawn
    void* callbackArg;             // 0x18
};
ASSERT_SIZE(CFLIconParam, 0x1C);

// what CFLi_MakeFaceTextures leaves for the shapes (name is ours)
struct CFLiFaceParts
{
    u8 cells[16 * 16];   // the cells of the face texture that eyes, mouth etc. cover (for the mask shape)
};

void CFLi_MakeFaceTextures(CFLiFaceParts* parts, const CFLiCharInfo* info, CFLCharModel* model,
                           s32 resolution);  // 0x0016A148 (name is ours)
void CFLi_InitShapes(CFLiCharModelData* data, const CFLiFaceParts* parts);  // 0x00175AF4 (name is ours)
void CFLi_MakeModelCmdlists(CFLCharModel* model);  // 0x00172BA0 (name is ours)

extern "C" {
s32 CFL_MakeIconWithModel(GLuint texture, const CFLCharModel* model, int expression,
                          const CFLIconParam* param);  // 0x001539C8 (name is ours)
void CFLi_MakeIcon(GLuint texture, const CFLiCharInfo* info, int expression, const CFLIconParam* param);
s32 CFL_MakeIcon(GLuint texture, int source, const void* data, u16 index, int expression, const CFLIconParam* param);
void CFLi_InitCharModel(CFLCharModel* model, const CFLiCharInfo* info, u32 area, void* buffer, s32 resolution,
                        u32 expressionMask, u32 flags);
s32 CFL_InitCharModel(CFLCharModel* model, int source, const void* data, u16 index, u32 area, void* buffer,
                      s32 resolution, u32 expressionMask, u32 flags);
} // extern "C"
void CFLi_LoadShape(CFLiCharModelData* data, int kind, int index, const f32* offset, f32 scaleX, f32 scaleY, bool flip,
                    const CFLiFaceParts* mask);  // 0x0016E2E0 (name is ours)
extern const u32 s_FacelineColors[6];
