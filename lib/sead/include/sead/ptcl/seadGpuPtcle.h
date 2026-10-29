#pragma once

#include "decomp.h"
#include "nn/gr/CTR/gr_BindSymbol.h"
#include "nn/gr/CTR/gr_BindSymbolVSFloat.h"
#include "nn/gr/CTR/gr_Shader.h"
#include "nn/gr/CTR/gr_Vertex.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector4.h"

namespace sead {
namespace ptcl {
class PtclRenderer;

// The vertices of the particles of the GPU emitters: three sets of 1024 random points in the unit ball
// (w = 1023 - index) with two random vectors each, and the indices 0 to 1023 (names other than the one of
// the reference symbols are ours).
struct sGpuParticlePrimitive
{
    static const s32 NUM_SET = 3;
    static const s32 NUM_VERTEX = 1024;

    // the random points and vectors (rand of this file) and the indices, flushed for the GPU
    void SetupVtxStream(); // 0x0055C380 | mk7dlp:bytes [tier B]

    nn::math::VEC4 pos[NUM_SET][NUM_VERTEX];  // 0x00000
    nn::math::VEC4 uv0[NUM_SET][NUM_VERTEX];  // 0x0C000
    nn::math::VEC4 uv1[NUM_SET][NUM_VERTEX];  // 0x18000
    u16 index[NUM_VERTEX];                    // 0x24000
};
ASSERT_SIZE(sGpuParticlePrimitive, 0x24800);

// The particles that the vertex and geometry shader calculate from the frame (point sprites of the
// geometry shader; the class and function names are from the reference symbols, the others are ours).
class GpuPtcle
{
public:
    GpuPtcle(); // 0x0055CD98 | mk7dlp:bytes-fuzzy [tier B]

    void Init(PtclRenderer* renderer, const u8* shaderBinary, s32 vsIndex, s32 gsIndex); // 0x0055CBB0 | mk7dlp:bytes [tier B]
    void InitShader(const u8* shaderBinary, s32 vsIndex, s32 gsIndex); // 0x0055C5C4 | mk7dlp:bytes [tier B]
    void InitPrimitive(); // 0x0055C778 | mk7dlp:bytes [tier B]
    // the shader and the point settings (once per frame of the calculation)
    u32* PreDraw(u32* cmd); // 0x0055CCA0 | mk7dlp:bytes [tier B]
    // the matrix (uniform user), the 12 vectors of an emitter (uniforms c28 to c39) and the drawing of
    // numPtcl - first points of the vertex set
    u32* Draw(u32* cmd, nn::math::MTX34& mtx, nn::math::VEC4& v0, nn::math::VEC4& v1, nn::math::VEC4& v2,
              nn::math::VEC4& v3, nn::math::VEC4& v4, nn::math::VEC4& v5, nn::math::VEC4& v6, nn::math::VEC4& v7,
              nn::math::VEC4& v8, nn::math::VEC4& v9, nn::math::VEC4& v10, nn::math::VEC4& v11, s32 numPtcl, s32 first,
              s32 set); // 0x0055C8CC | mk7dlp:bytes [tier B]
    // the attributes disabled again
    u32* PostDraw(u32* cmd); // 0x0055CD64 | mk7dlp:bytes [tier B]

    PtclRenderer* mRenderer;                          // 0x00000
    nn::gr::CTR::Shader mShader;                      // 0x00004
    u8 _pad_0x128EC[0x70];
    nn::gr::CTR::BindSymbolVSFloat mUser;             // 0x1295C
    nn::gr::CTR::BindSymbolVSFloat mProj;             // 0x12964
    nn::gr::CTR::BindSymbolVSFloat mView;             // 0x1296C
    nn::gr::CTR::BindSymbolVSFloat mTime;             // 0x12974
    nn::gr::CTR::BindSymbolVSFloat mScaleParam;       // 0x1297C
    nn::gr::CTR::BindSymbolVSFloat mScaleAnm;         // 0x12984
    nn::gr::CTR::BindSymbolVSFloat mEmitterVel;       // 0x1298C
    nn::gr::CTR::BindSymbolVSFloat mSpreadVec;        // 0x12994
    nn::gr::CTR::BindSymbolVSFloat mGravity;          // 0x1299C
    nn::gr::CTR::BindSymbolVSFloat mColor;            // 0x129A4
    nn::gr::CTR::BindSymbolVSFloat mAlphaAnm;         // 0x129AC
    nn::gr::CTR::BindSymbolVSFloat mRotateZ;          // 0x129B4
    nn::gr::CTR::BindSymbolVSFloat mColorPk;          // 0x129BC
    nn::gr::CTR::BindSymbolVSFloat mColorEd;          // 0x129C4
    nn::gr::CTR::BindSymbolVSFloat mColorAnm;         // 0x129CC
    nn::gr::CTR::BindSymbol mPointViewport;           // 0x129D4, a float uniform of the geometry shader
    nn::gr::CTR::BindSymbol mPointDistanceAttenuation; // 0x129DC, a bool uniform of the geometry shader
    sGpuParticlePrimitive mPrimitive;                 // 0x129E4
    nn::gr::CTR::Vertex mVertex[sGpuParticlePrimitive::NUM_SET]; // 0x371E4
    nn::gr::CTR::Vertex::IndexStream mIndexStream;    // 0x38894
    u32 mShaderCmd[0x400];                            // 0x388A0
    u32 mShaderCmdSize;                               // 0x398A0, bytes
    u32 mPostCmd[0x80];                               // 0x398A4, the attributes disabled
    u32 mPostCmdSize;                                 // 0x39AA4, bytes
};
ASSERT_OFFSET(GpuPtcle, mUser, 0x1295C);
ASSERT_OFFSET(GpuPtcle, mPrimitive, 0x129E4);
ASSERT_OFFSET(GpuPtcle, mVertex, 0x371E4);
ASSERT_OFFSET(GpuPtcle, mIndexStream, 0x38894);
ASSERT_OFFSET(GpuPtcle, mShaderCmd, 0x388A0);
ASSERT_OFFSET(GpuPtcle, mPostCmdSize, 0x39AA4);
ASSERT_SIZE(GpuPtcle, 0x39AA8);
} // namespace ptcl
} // namespace sead
