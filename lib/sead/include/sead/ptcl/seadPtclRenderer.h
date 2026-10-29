#pragma once

#include "decomp.h"
#include "nn/gr/CTR/gr_BindSymbol.h"
#include "nn/gr/CTR/gr_BindSymbolVSFloat.h"
#include "nn/gr/CTR/gr_Shader.h"
#include "nn/gr/CTR/gr_Vertex.h"
#include "nn/math/math_Vector4.h"
#include "sead/ptcl/seadGpuPtcle.h"
#include "sead/seadMatrix.h"
#include "sead/seadVector3.h"

namespace sead {
class Heap;

namespace ptcl {
class PtclSystem;
struct PtclStripe;
struct TextureHandle;

// The drawing of the particles: the quads of the CPU particles (one shader with up to eight particles per
// draw, their vertex blocks as uniforms), the stripes (a shader with position, texture coordinate and
// direction per vertex) and the GPU particles of the base class. The class and the names of the reference
// symbols are kept; the other names are ours.
class PtclRenderer : public GpuPtcle
{
public:
    // what the command buffer was last set up for
    enum CalcState
    {
        CALC_STATE_NONE = 0,
        CALC_STATE_CPU = 1,
        CALC_STATE_GPU = 2,
    };

    PtclRenderer(Heap* heap, s32 numPtcl, PtclSystem* system, const void* config); // 0x00556CD8

    // the state of the frame and the matrices (projection, view, billboard)
    u32* beginRender(u32* cmd, const Matrix44<f32>& proj, const Matrix34<f32>& view, const Matrix34<f32>& billboard); // 0x0055630C | mk7dlp:bytes [tier B]
    // the state of the frame and the billboard matrix only
    u32* beginRenderBillboard(u32* cmd, const Matrix34<f32>& billboard); // 0x00556A78
    // the attributes of the quads disabled again
    u32* endRender(u32* cmd); // 0x00556C18 | mk7dlp:bytes [tier B]
    // the state of the CPU particles (once until the GPU particles change it)
    u32* beginCalc(u32* cmd); // 0x00556B7C | mk7dlp:bytes [tier B]
    u32* endCalc(u32* cmd); // 0x00556B40 | mk7dlp:bytes [tier B]
    // the shader and the vertex arrays of the stripes
    u32* beginStripeCalc(u32* cmd); // 0x00556884 | mk7dlp:bytes [tier B]
    // the texture, blending, combiner and uniforms of a stripe and the drawing of its numVertex vertices
    u32* renderStripe(u32* cmd, PtclStripe* stripe, s32 numVertex); // 0x0055642C | mk7dlp

    Heap* mHeap;                                        // 0x39AA8
    nn::gr::CTR::Vertex mVertex;                        // 0x39AAC, the quads
    nn::gr::CTR::Vertex::IndexStream mIndexStream;      // 0x3A23C
    u32 _pad_0x3A248;
    nn::gr::CTR::Shader mShader;                        // 0x3A24C
    nn::gr::CTR::BindSymbolVSFloat mProj;               // 0x4CB34
    nn::gr::CTR::BindSymbolVSFloat mView;               // 0x4CB3C
    PtclSystem* mSystem;                                // 0x4CB44
    nn::gr::CTR::BindSymbolVSInput mStripePosition;     // 0x4CB48
    nn::gr::CTR::BindSymbolVSInput mStripeTexCoord;     // 0x4CB50
    nn::gr::CTR::BindSymbolVSInput mStripeDir;          // 0x4CB58
    nn::gr::CTR::Shader mStripeShader;                  // 0x4CB60
    s32 mNumStripeVtxUsed;                              // 0x5F448
    s32 mNumStripeVtxMax;                               // 0x5F44C
    s32 mNumStripeIndexUsed;                            // 0x5F450
    u16* mStripeIndex;                                  // 0x5F454
    Vector3<f32>* mStripePos;                           // 0x5F458
    Vector3<f32>* mStripeTex;                           // 0x5F45C
    nn::math::VEC4* mStripeDirection;                   // 0x5F460
    nn::gr::CTR::Vertex mStripeVertex;                  // 0x5F464
    nn::gr::CTR::Vertex::IndexStream mStripeIndexStream; // 0x5FBF4
    u32 mBeginCalcCmd[0x258];                           // 0x5FC00
    s32 mNumBeginCalcCmd;                               // 0x60560, words
    u32 mEndCalcCmd[0x80];                              // 0x60564
    s32 mNumEndCalcCmd;                                 // 0x60764, words
    s32 mCalcState;                                     // 0x60768, CalcState
    u32 _pad_0x6076C;
    void* mQuadBuffer;                                  // 0x60770, the vertices and indices of eight quads
    TextureHandle* mCurrentTexture;                     // 0x60774
    u8 mCurrentCombiner;                                // 0x60778
    u8 mCurrentBlend;                                   // 0x60779
    u8 mCurrentDepthAlpha;                              // 0x6077A
};
ASSERT_OFFSET(PtclRenderer, mHeap, 0x39AA8);
ASSERT_OFFSET(PtclRenderer, mShader, 0x3A24C);
ASSERT_OFFSET(PtclRenderer, mSystem, 0x4CB44);
ASSERT_OFFSET(PtclRenderer, mStripeShader, 0x4CB60);
ASSERT_OFFSET(PtclRenderer, mNumStripeVtxUsed, 0x5F448);
ASSERT_OFFSET(PtclRenderer, mStripeVertex, 0x5F464);
ASSERT_OFFSET(PtclRenderer, mBeginCalcCmd, 0x5FC00);
ASSERT_OFFSET(PtclRenderer, mCalcState, 0x60768);
ASSERT_OFFSET(PtclRenderer, mCurrentTexture, 0x60774);
ASSERT_SIZE(PtclRenderer, 0x6077C);

// the value of the gas/fog mode bit of the emitters with flag 0x80
extern u32 s_DrawStateValue;
// the commands of the texture combiners by combiner type (word 4 is the constant color)
extern u32 s_CombinerTbl[8][10];
} // namespace ptcl
} // namespace sead
