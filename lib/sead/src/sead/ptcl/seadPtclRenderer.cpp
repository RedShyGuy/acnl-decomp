#include "sead/ptcl/seadPtclRenderer.h"
#include "nn/gr/CTR/CTR_Api.h"
#include "nn/gr/CTR/gr_Combiner.h"
#include "nn/gr/CTR/gr_RenderState.h"
#include "nn/gx/detail/gx_Context.h"
#include "nn/gx/gx_Api.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "sead/ptcl/seadEmitterCalc.h"
#include "sead/ptcl/seadEmitterData.h"
#include "sead/ptcl/seadPtclStripe.h"
#include "sead/ptcl/seadPtclSystem.h"
#include "sead/seadHeapMgr.h"
#include "sead/seadNew.h"

#include <string.h>

namespace sead {
namespace ptcl {
// the shaders of the quads, of the stripes and of the GPU particles (DVLB after 3dbrew SHBIN; picasso sources
// seadPtclShader.v.pica, seadPtclStripeShader.v.pica and seadGpuPtclShader.v/g.pica, built into these arrays;
// at 0x8BAE80, 0x8BB280 and 0x8BB500 in the original)
extern const u8 s_PtclShaderBinary[];
extern const u8 s_StripeShaderBinary[];
extern const u8 s_GpuPtclShaderBinary[];

namespace {
// the formats of the attributes: three and four floats ((n - 1) << 2 | 3)
const PicaDataVertexAttrType ATTR_FLOAT3 = static_cast<PicaDataVertexAttrType>(11);
const PicaDataVertexAttrType ATTR_FLOAT4 = static_cast<PicaDataVertexAttrType>(15);

// the start of the frame: the vertex shader output map and attribute settings (all zero but the headers)
// 0x008BBDDC
const u32 s_RenderBeginCmd[82] = {
    0x00000000, 0x00900251, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01D00200, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00010229, 0x00000000, 0x01D00200,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00010244, 0x00000001, 0x000F0245,
};

// the billboard matrix as the float uniforms c0 to c3 (rows in the order w, z, y, x)
inline u32* putBillboard_(u32* cmd, const Matrix34<f32>& billboard)
{
    f32* out = reinterpret_cast<f32*>(cmd);
    cmd[0] = 0x80000000;
    cmd[1] = 0xF02C0;
    cmd[2] = 0;
    cmd[3] = 0xFF02C1;
    out[4] = billboard.m[0][2];
    out[5] = billboard.m[0][1];
    out[6] = billboard.m[0][0];
    cmd[7] = 0;
    out[8] = billboard.m[1][2];
    out[9] = billboard.m[1][1];
    out[10] = billboard.m[1][0];
    cmd[11] = 0;
    out[12] = billboard.m[2][2];
    out[13] = billboard.m[2][1];
    out[14] = billboard.m[2][0];
    cmd[15] = 0;
    out[16] = billboard.m[2][2];
    out[17] = billboard.m[1][2];
    out[18] = billboard.m[0][2];
    return cmd + 20;
}

// the disabling of the fixed attributes of a vertex shader input (twelve attributes)
inline u32* putDisableFixedAttr_(u32* cmd)
{
    *cmd++ = 0;
    *cmd++ = 0xF0201;
    *cmd++ = 0;
    *cmd++ = 0xF0202;
    memset(cmd, 0, 152);
    cmd[1] = 0x823F0203;
    cmd += 38;
    for (u32 i = 0; i < nn::gr::CTR::Vertex::ATTRIBUTE_COUNT; i++) {
        cmd[0] = i;
        cmd[1] = 0x803F0232;
        cmd[2] = 0;
        cmd[3] = 0;
        cmd[4] = 0;
        cmd[5] = 0;
        cmd += 6;
    }
    return cmd;
}

// the flush and invalidation of the frame buffer cache
inline u32* putFlush_(u32* cmd)
{
    cmd[0] = 1;
    cmd[1] = 0xF0111;
    cmd[2] = 1;
    cmd[3] = 0xF0110;
    return cmd + 4;
}
} // namespace

// the value of the gas/fog mode bit of the stripes with flag 0x80 (the constructor sets 5)
// 0x0097F6B4
u32 s_DrawStateValue = 0;
namespace {
// the draw state by blend type and by depth/alpha test type (each source file of the original has its own
// copy of these tables; names are ours)
// 0x0097F6B8
u32 s_BlendFuncTbl[4] = {0x76760000, 0x16160000, 0x16160202, 0x01010200};
// 0x0097F6C8
u32 s_DepthFuncTbl[3] = {0x00000F41, 0x00000F10, 0x00001F41};
// 0x0097F6D4
u32 s_AlphaTestTbl[3] = {0x00000061, 0x00000061, 0x00008061};
// 0x0097F6E0
u32 s_DepthReadTbl[3] = {2, 0, 2};
// 0x0097F6EC
u32 s_DepthWriteTbl[3] = {0, 0, 2};
// 0x0097F6F8
u32 s_ColorReadTbl[3] = {0xF, 0xF, 0};
} // namespace

// 0x009828BC
u32 s_CombinerTbl[8][10] = {
    {0x0F300F3E, 0x804F00C0, 0x00000000, 0x00010001, 0x00000000, 0x00000000, 0x0FFF0FFF, 0x802F00C8, 0x00000000, 0x00000000},
    {0x0F300F30, 0x804F00C0, 0x00000000, 0x00010001, 0x00000000, 0x00000000, 0x0FFF0FFF, 0x802F00C8, 0x00000000, 0x00000000},
    {0x0F300F3E, 0x804F00C0, 0x00000000, 0x00010001, 0x00000000, 0x00000000, 0x0FFF0F30, 0x802F00C8, 0x00000010, 0x00000008},
    {0x0F300F3E, 0x804F00C0, 0x00000010, 0x00010001, 0x00000000, 0x00000000, 0x0FFF0F30, 0x802F00C8, 0x00000000, 0x00000008},
    {0x0F300FFE, 0x804F00C0, 0x00000000, 0x00010000, 0x00000000, 0x00000000, 0x0FFF0FFF, 0x802F00C8, 0x00000000, 0x00000000},
    {0x0F300FF0, 0x804F00C0, 0x00000000, 0x00010000, 0x00000000, 0x00000000, 0x0FFF0FFF, 0x802F00C8, 0x00000000, 0x00000000},
    {0x0F30003E, 0x804F00C0, 0x00000000, 0x00010008, 0x00000000, 0x00000000, 0x0FFF0FFF, 0x802F00C8, 0x00000000, 0x00000000},
    {0x0F030F3E, 0x804F00C0, 0x00010000, 0x00050001, 0x00000000, 0x00010000, 0x0FFF0FFF, 0x802F00C8, 0x00000000, 0x00000000},
};

// 0x0055630C | mk7dlp:bytes [tier B]
u32* sead::ptcl::PtclRenderer::beginRender(u32* cmd, const Matrix44<f32>& proj, const Matrix34<f32>& view,
                                           const Matrix34<f32>& billboard)
{
    memcpy(cmd, s_RenderBeginCmd, sizeof(s_RenderBeginCmd));
    cmd += 82;
    cmd[0] = 0x80000000 | mProj.m_Start;
    cmd[1] = 0xF02C0;
    nn::gr::CTR::CopyMtx44WithHeader(reinterpret_cast<f32*>(cmd + 2), &proj, 0xFF02C1);
    cmd += 20;
    cmd[0] = 0x80000000 | mView.m_Start;
    cmd[1] = 0xF02C0;
    nn::gr::CTR::CopyMtx34WithHeader(reinterpret_cast<f32*>(cmd + 2), &view, 0xBF02C1);
    cmd += 16;
    return putBillboard_(cmd, billboard);
}

// 0x0055642C | mk7dlp
u32* sead::ptcl::PtclRenderer::renderStripe(u32* cmd, PtclStripe* stripe, s32 numVertex)
{
    SimpleEmitterData* res = stripe->mRes;
    const StripeData* stripeData =
        reinterpret_cast<const StripeData*>(reinterpret_cast<const u8*>(res) + res->stripeDataOffset);
    cmd = putFlush_(cmd);

    TextureHandle* texture = res->texture;
    if (mCurrentTexture != texture) {
        *cmd++ = 0x11001;
        *cmd++ = 0xF0080;
        *cmd++ = texture->size;
        *cmd++ = 0xF0082;
        *cmd++ = texture->param;
        *cmd++ = 0xF0083;
        *cmd++ = texture->lod;
        *cmd++ = 0xF0084;
        *cmd++ = texture->texture.m_Unit0.m_PhysicalAddr >> 3;
        *cmd++ = 0xF0085;
        *cmd++ = texture->texture.m_Unit0.m_Format;
        *cmd++ = 0xF008E;
        mCurrentTexture = res->texture;
    }

    if (mCurrentBlend != res->blendType) {
        *cmd++ = s_BlendFuncTbl[res->blendType];
        *cmd++ = 0xF0101;
        mCurrentBlend = res->blendType;
    }

    if (mCurrentDepthAlpha != res->depthAlphaType) {
        cmd[0] = s_DepthFuncTbl[res->depthAlphaType];
        cmd[1] = 0xF0107;
        cmd[2] = s_AlphaTestTbl[res->depthAlphaType];
        cmd[3] = 0x30104;
        cmd[4] = s_ColorReadTbl[res->depthAlphaType];
        cmd[5] = 0x10112;
        cmd[6] = s_DepthReadTbl[res->depthAlphaType];
        cmd[7] = 0x10114;
        cmd[8] = s_DepthWriteTbl[res->depthAlphaType];
        cmd[9] = 0x10115;
        cmd += 10;
        mCurrentDepthAlpha = res->depthAlphaType;
    }

    cmd = putFlush_(cmd);

    u32 r = static_cast<u32>(res->combinerColor.x * stripe->mColor.r) & 0xFF;
    u32 g = static_cast<u32>(res->combinerColor.y * stripe->mColor.g) & 0xFF;
    u32 b = static_cast<u32>(res->combinerColor.z * stripe->mColor.b) & 0xFF;
    if (mCurrentCombiner != res->combinerType) {
        const u32* combiner = s_CombinerTbl[res->combinerType];
        cmd[0] = combiner[0];
        cmd[1] = combiner[1];
        cmd[2] = combiner[2];
        cmd[3] = combiner[3];
        cmd[4] = r | (g << 8) | (b << 16);
        cmd[5] = combiner[5];
        cmd[6] = combiner[6];
        cmd[7] = combiner[7];
        cmd[8] = combiner[8];
        cmd[9] = combiner[9];
        cmd += 10;
        mCurrentCombiner = res->combinerType;
    } else {
        *cmd++ = r | (g << 8) | (b << 16);
        *cmd++ = 0xF00C3;
    }

    if (res->flag & 0x80) {
        cmd[0] = s_DrawStateValue;
    } else {
        cmd[0] = 0;
    }
    cmd[1] = 0x500E0;
    cmd += 2;

    // the texture scroll and the color of the stripe (uniforms c12 and c13)
    f32* out = reinterpret_cast<f32*>(cmd);
    cmd[0] = 0x8000000C;
    cmd[1] = 0xF02C0;
    out[2] = stripe->mPtclTexOffsetV + stripe->mTexScrollV;
    cmd[3] = 0x7F02C1;
    out[4] = stripe->mPtclTexOffsetU + stripe->mTexScrollU;
    out[5] = stripe->mPtclTexScaleV;
    out[6] = stripe->mPtclTexScaleU;
    out[7] = static_cast<f32>(static_cast<u32>(stripeData->type));
    out[8] = res->colors[0].b * stripe->mColor.b;
    out[9] = res->colors[0].g * stripe->mColor.g;
    out[10] = res->colors[0].r * stripe->mColor.r;
    cmd += 12;

    mStripeIndexStream.m_DrawVertexCount = (numVertex / 2 - 1) * 6;
    mStripeIndexStream.m_PhysicalAddr = nngxGetPhysicalAddr(reinterpret_cast<uptr>(mStripeIndex + mNumStripeIndexUsed));
    cmd[0] = 1;
    cmd[1] = 0xF0231;
    return mStripeVertex.MakeDrawCommand(cmd + 2, mStripeIndexStream);
}

// 0x00556884 | mk7dlp:bytes [tier B]
u32* sead::ptcl::PtclRenderer::beginStripeCalc(u32* cmd)
{
    cmd = mStripeShader.MakeFullCommand(cmd);
    mStripeVertex.Reset();
    mStripeVertex.EnableAttrAsArray(mStripePosition, nngxGetPhysicalAddr(reinterpret_cast<uptr>(mStripePos)), ATTR_FLOAT3);
    mStripeVertex.EnableAttrAsArray(mStripeTexCoord, nngxGetPhysicalAddr(reinterpret_cast<uptr>(mStripeTex)), ATTR_FLOAT3);
    mStripeVertex.EnableAttrAsArray(mStripeDir, nngxGetPhysicalAddr(reinterpret_cast<uptr>(mStripeDirection)),
                                    ATTR_FLOAT4);
    cmd = putDisableFixedAttr_(cmd);
    cmd = mStripeVertex.MakeEnableAttrCommand(cmd);
    mCurrentTexture = nullptr;
    mCurrentCombiner = 0xFF;
    mCurrentBlend = 0xFF;
    mCurrentDepthAlpha = 0xFF;
    return cmd;
}

// 0x00556A78 (name is ours)
u32* sead::ptcl::PtclRenderer::beginRenderBillboard(u32* cmd, const Matrix34<f32>& billboard)
{
    memcpy(cmd, s_RenderBeginCmd, sizeof(s_RenderBeginCmd));
    return putBillboard_(cmd + 82, billboard);
}

// 0x00556B40 | mk7dlp:bytes [tier B]
u32* sead::ptcl::PtclRenderer::endCalc(u32* cmd)
{
    memcpy(cmd, mEndCalcCmd, mNumEndCalcCmd * sizeof(u32));
    return cmd + mNumEndCalcCmd;
}

// 0x00556B7C | mk7dlp:bytes [tier B]
u32* sead::ptcl::PtclRenderer::beginCalc(u32* cmd)
{
    if (mCalcState == CALC_STATE_CPU) {
        return cmd;
    }
    mCalcState = CALC_STATE_CPU;
    memcpy(cmd, mBeginCalcCmd, mNumBeginCalcCmd * sizeof(u32));
    EmitterCalc::s_CurrentState.combinerType = 28;
    EmitterCalc::s_CurrentState.blendType = 4;
    EmitterCalc::s_CurrentState.depthAlphaType = 0;
    EmitterCalc::s_CurrentTexture = nullptr;
    return cmd + mNumBeginCalcCmd;
}

// 0x00556C18 | mk7dlp:bytes [tier B]
u32* sead::ptcl::PtclRenderer::endRender(u32* cmd)
{
    for (u32 i = 0; i < nn::gr::CTR::Vertex::ATTRIBUTE_COUNT; i++) {
        if (mVertex.m_LoadArrays[i].m_PhysicalAddr != 0) {
            *cmd++ = 0;
            *cmd++ = 0x80300000 | (0x203 + i * 3);
            *cmd++ = 0;
            *cmd++ = 0;
        } else {
            *cmd++ = 0;
            *cmd++ = 0xF0000 | (0x205 + i * 3);
        }
        if (mVertex.m_ConstAttrs[i].m_Dimension != 0) {
            *cmd++ = i;
            *cmd++ = 0x803F0232;
            *cmd++ = 0;
            *cmd++ = 0;
            *cmd++ = 0;
            *cmd++ = 0;
        }
    }
    *cmd++ = 1;
    *cmd++ = 0xF0231;
    return cmd;
}

// 0x00556CD8 (name is ours)
sead::ptcl::PtclRenderer::PtclRenderer(Heap* heap, s32, PtclSystem* system, const void* config)
    : GpuPtcle(), mVertex(), mIndexStream(), mShader(), mProj(), mView(), mStripePosition(), mStripeTexCoord(),
      mStripeDir(), mStripeShader(), mStripeVertex(), mStripeIndexStream()
{
    mSystem = system;
    ScopedCurrentHeapSetter setter(heap);
    mHeap = heap;
    s_DrawStateValue = 5;

    mShader.SetupBinary(s_PtclShaderBinary, 0, -1);
    mShader.SearchBindSymbol(&mProj, "proj");
    mShader.SearchBindSymbol(&mView, "view");
    mStripeShader.SetupBinary(s_StripeShaderBinary, 0, -1);
    mStripeShader.SearchBindSymbol(&mStripePosition, "Position");
    mStripeShader.SearchBindSymbol(&mStripeTexCoord, "TexCoord0");
    mStripeShader.SearchBindSymbol(&mStripeDir, "Dir");
    mNumStripeVtxMax = 0;
    mNumStripeVtxUsed = 0;
    mStripePos = nullptr;
    mStripeTex = nullptr;
    mStripeDirection = nullptr;
    mStripeIndex = nullptr;
    Init(this, s_GpuPtclShaderBinary, 0, 1);

    // eight quads: the positions, the texture coordinates with the index of the vertex block (9 uniforms per
    // quad) and the indices (bytes)
    nn::gr::CTR::BindSymbolVSInput symbol;
    u8* buffer = new (heap, 128) u8[816];
    Vector3<f32>* pos = reinterpret_cast<Vector3<f32>*>(buffer);
    Vector3<f32>* tex = reinterpret_cast<Vector3<f32>*>(buffer + 384);
    u8* index = buffer + 768;
    for (s32 i = 0; i < 8; i++) {
        f32 block = static_cast<f32>(i * 9);
        pos[i * 4 + 0] = Vector3<f32>(-0.5f, 0.5f, 0.0f);
        pos[i * 4 + 1] = Vector3<f32>(0.5f, 0.5f, 0.0f);
        pos[i * 4 + 2] = Vector3<f32>(-0.5f, -0.5f, 0.0f);
        pos[i * 4 + 3] = Vector3<f32>(0.5f, -0.5f, 0.0f);
        tex[i * 4 + 0] = Vector3<f32>(0.0f, 0.0f, block);
        tex[i * 4 + 1] = Vector3<f32>(1.0f, 0.0f, block);
        tex[i * 4 + 2] = Vector3<f32>(0.0f, 1.0f, block);
        tex[i * 4 + 3] = Vector3<f32>(1.0f, 1.0f, block);
        index[i * 6 + 0] = i * 4 + 2;
        index[i * 6 + 1] = i * 4 + 1;
        index[i * 6 + 2] = i * 4;
        index[i * 6 + 3] = i * 4 + 2;
        index[i * 6 + 4] = i * 4 + 3;
        index[i * 6 + 5] = i * 4 + 1;
    }
    nngxUpdateBuffer(buffer, 816);
    u8 location = static_cast<const PtclSystem::Config*>(config)->bufferLocation;
    if (location == 1 || location == 2) {
        GxAllocateFunction allocate;
        GxFreeFunction free;
        nngxGetAllocator(&allocate, &free);
        GLenum area = location == 1 ? nn::gx::detail::MEMORY_AREA_VRAM_A : nn::gx::detail::MEMORY_AREA_VRAM_B;
        void* vram = allocate(area, 0x102, 0, 816);
        nngxAddVramDmaCommand(buffer, vram, 816);
        buffer = static_cast<u8*>(vram);
    }
    mQuadBuffer = buffer;
    mShader.SearchBindSymbol(&symbol, "Position");
    mVertex.EnableAttrAsArray(symbol, nngxGetPhysicalAddr(reinterpret_cast<uptr>(mQuadBuffer)), ATTR_FLOAT3);
    mShader.SearchBindSymbol(&symbol, "TexCoord0");
    mVertex.EnableAttrAsArray(symbol, nngxGetPhysicalAddr(reinterpret_cast<uptr>(mQuadBuffer) + 384), ATTR_FLOAT3);
    mIndexStream.m_PhysicalAddr = nngxGetPhysicalAddr(reinterpret_cast<uptr>(mQuadBuffer) + 768);
    mIndexStream.m_DrawVertexCount = 48;
    mIndexStream.m_IsUnsignedByte = true;

    // the start of the CPU particles: the combiner, the shader, the quads and the render state
    u32* cmd = nn::gr::CTR::MakeDisableAllCommand(mBeginCalcCmd);
    nn::gr::CTR::Combiner combiner;
    combiner.m_Stages[0].SetModulatePrimaryColorTexture0();
    cmd = combiner.MakeCommand(cmd);
    *cmd++ = 1;
    *cmd++ = 0xF0245;
    cmd = mShader.MakeFullCommand(cmd);
    cmd = mVertex.MakeEnableAttrCommand(cmd);
    {
        nn::gr::CTR::RenderState state;
        state.m_Blend.m_IsEnable = true;
        state.m_Blend.m_EquationRgb = 0;
        state.m_Blend.m_SrcRgb = 6;
        state.m_Blend.m_DstRgb = 7;
        state.m_Blend.m_SrcAlpha = 7;
        state.m_Blend.m_EquationAlpha = 0;
        state.m_Blend.m_DstAlpha = 1;
        cmd = state.m_Blend.MakeCommand(cmd, false);
        state.m_DepthTest.m_IsEnable = true;
        state.m_DepthTest.m_IsEnableWrite = false;
        cmd = state.m_DepthTest.MakeCommand(cmd, false);
        state.m_AlphaTest.m_IsEnable = true;
        state.m_AlphaTest.m_Func = 6;
        cmd = state.m_AlphaTest.MakeCommand(cmd, false);
        state.m_Culling.m_IsEnable = false;
        cmd = state.m_Culling.MakeCommand(cmd, false);
        cmd = state.m_FBAccess.MakeCommand(cmd, true);
    }
    *cmd++ = 0;
    *cmd++ = 0xF0081;
    uptr vramBase = (nngxGetPhysicalAddr(nngxGetVramStartAddr(nn::gx::detail::MEMORY_AREA_VRAM_A)) >> 3) << 3;
    *cmd++ = (mIndexStream.m_PhysicalAddr - vramBase) | (mIndexStream.m_IsUnsignedByte ? 0 : 0x80000000);
    *cmd++ = 0xF0227;
    *cmd++ = 0;
    *cmd++ = 0xF0231;
    mNumBeginCalcCmd = cmd - mBeginCalcCmd;

    // the end of the CPU particles: blending off
    cmd = mEndCalcCmd;
    {
        nn::gr::CTR::RenderState state;
        state.m_Blend.m_IsEnable = false;
        cmd = state.m_Blend.MakeCommand(cmd, true);
        cmd = state.m_FBAccess.MakeCommand(cmd, true);
    }
    *cmd++ = 0;
    *cmd++ = 0x500E0;
    mNumEndCalcCmd = cmd - mEndCalcCmd;
}
} // namespace ptcl
} // namespace sead
