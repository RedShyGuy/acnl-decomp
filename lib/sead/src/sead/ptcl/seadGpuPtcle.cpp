#include "sead/ptcl/seadGpuPtcle.h"
#include "nn/gr/CTR/CTR_Api.h"
#include "nn/gx/gx_Api.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "sead/ptcl/seadPtclRenderer.h"

#include <math.h>
#include <string.h>

namespace sead {
namespace ptcl {
namespace {
// the format of an attribute of four floats ((4 - 1) << 2 | 3)
const PicaDataVertexAttrType ATTR_FLOAT4 = static_cast<PicaDataVertexAttrType>(15);

// the seed of rand_
// 0x00982D1C (name is ours)
u32 s_RandomSeed = 0x50000;

// a random number in [0, 1] (a linear congruential generator of 28 bits)
f32 rand_()
{
    s_RandomSeed = s_RandomSeed * 0xFD43FD + 0xC39EC3;
    f32 value = static_cast<s32>(s_RandomSeed & 0x0FFFFFFF) * (1.0f / 0x10000000);
    while (value < 0.0f) {
        value += 1.0f;
    }
    while (value > 1.0f) {
        value -= 1.0f;
    }
    return value;
}

// the writing of a vector to a float uniform: the register, then the components w, z, y, x
inline u32* putUniform_(u32* cmd, u32 reg, const nn::math::VEC4& v)
{
    f32* out = reinterpret_cast<f32*>(cmd);
    cmd[0] = 0x80000000 | reg;
    cmd[1] = 0x804F02C0;
    out[2] = v.w;
    out[3] = v.z;
    out[4] = v.y;
    out[5] = v.x;
    return cmd + 6;
}
} // namespace

// the scale of the screen for the point sprites of the geometry shader (1 / 240, 1 / 400)
// 0x00AF63CC (name is ours)
nn::math::VEC4 s_PointViewport(1.0f / 240, 1.0f / 400, 0.0f, 0.0f);
// 0x0079EDEC (static initializer)

// 0x0055C380 | mk7dlp:bytes [tier B]
void sead::ptcl::sGpuParticlePrimitive::SetupVtxStream()
{
    for (u32 set = 0; set < NUM_SET; set++) {
        for (u32 i = 0; i < NUM_VERTEX; i++) {
            nn::math::VEC4& p = pos[set][i];
            p.x = rand_() * 2.0f - 1.0f;
            p.y = rand_() * 2.0f - 1.0f;
            p.z = rand_() * 2.0f - 1.0f;
            p.w = static_cast<f32>(NUM_VERTEX - 1 - i);
            while (sqrtf(p.x * p.x + p.y * p.y + p.z * p.z) > 1.0f) {
                p.x = rand_() * 2.0f - 1.0f;
                p.y = rand_() * 2.0f - 1.0f;
                p.z = rand_() * 2.0f - 1.0f;
            }
            nn::math::VEC4& u0 = uv0[set][i];
            u0.x = rand_() * 2.0f - 1.0f;
            u0.y = rand_() * 2.0f - 1.0f;
            u0.z = rand_() * 2.0f - 1.0f;
            u0.w = rand_();
            nn::math::VEC4& u1 = uv1[set][i];
            u1.x = rand_();
            u1.y = rand_();
            u1.z = rand_();
            u1.w = rand_();
        }
    }
    for (u32 i = 0; i < NUM_VERTEX; i++) {
        index[i] = i;
    }
    nngxUpdateBuffer(this, sizeof(sGpuParticlePrimitive));
}

// 0x0055C5C4 | mk7dlp:bytes [tier B]
void sead::ptcl::GpuPtcle::InitShader(const u8* shaderBinary, s32 vsIndex, s32 gsIndex)
{
    mShader.SetupBinary(shaderBinary, vsIndex, gsIndex);
    reinterpret_cast<u8*>(&mShader)[664] = 3;
    mShader.SearchBindSymbol(&mUser, "user");
    mShader.SearchBindSymbol(&mProj, "proj");
    mShader.SearchBindSymbol(&mView, "view");
    mShader.SearchBindSymbol(&mTime, "time");
    mShader.SearchBindSymbol(&mScaleParam, "scaleParam");
    mShader.SearchBindSymbol(&mScaleAnm, "scaleAnm");
    mShader.SearchBindSymbol(&mEmitterVel, "emitterVel");
    mShader.SearchBindSymbol(&mSpreadVec, "spreadVec");
    mShader.SearchBindSymbol(&mGravity, "gravity");
    mShader.SearchBindSymbol(&mColor, "color");
    mShader.SearchBindSymbol(&mAlphaAnm, "alphaAnm");
    mShader.SearchBindSymbol(&mRotateZ, "rotateZ");
    mShader.SearchBindSymbol(&mColorPk, "colorPk");
    mShader.SearchBindSymbol(&mColorEd, "colorEd");
    mShader.SearchBindSymbol(&mColorAnm, "colorAnm");
    mShader.SearchBindSymbol(&mPointViewport, "dmp_Point.viewport");
    mShader.SearchBindSymbol(&mPointDistanceAttenuation, "dmp_Point.distanceAttenuation");
}

// 0x0055C778 | mk7dlp:bytes [tier B]
void sead::ptcl::GpuPtcle::InitPrimitive()
{
    nn::gr::CTR::BindSymbolVSInput position;
    nn::gr::CTR::BindSymbolVSInput uv0;
    nn::gr::CTR::BindSymbolVSInput uv1;
    mPrimitive.SetupVtxStream();
    for (u32 set = 0; set < sGpuParticlePrimitive::NUM_SET; set++) {
        mShader.SearchBindSymbol(&position, "Position");
        mVertex[set].EnableAttrAsArray(position, nngxGetPhysicalAddr(reinterpret_cast<uptr>(mPrimitive.pos[set])),
                                       ATTR_FLOAT4);
        mShader.SearchBindSymbol(&uv0, "Uv0");
        mVertex[set].EnableAttrAsArray(uv0, nngxGetPhysicalAddr(reinterpret_cast<uptr>(mPrimitive.uv0[set])),
                                       ATTR_FLOAT4);
        mShader.SearchBindSymbol(&uv1, "Uv1");
        mVertex[set].EnableAttrAsArray(uv1, nngxGetPhysicalAddr(reinterpret_cast<uptr>(mPrimitive.uv1[set])),
                                       ATTR_FLOAT4);
    }
    mIndexStream.m_PhysicalAddr = nngxGetPhysicalAddr(reinterpret_cast<uptr>(mPrimitive.index));
    mIndexStream.m_DrawVertexCount = sGpuParticlePrimitive::NUM_VERTEX;
}

// 0x0055C8CC | mk7dlp:bytes [tier B]
u32* sead::ptcl::GpuPtcle::Draw(u32* cmd, nn::math::MTX34& mtx, nn::math::VEC4& v0, nn::math::VEC4& v1,
                                nn::math::VEC4& v2, nn::math::VEC4& v3, nn::math::VEC4& v4, nn::math::VEC4& v5,
                                nn::math::VEC4& v6, nn::math::VEC4& v7, nn::math::VEC4& v8, nn::math::VEC4& v9,
                                nn::math::VEC4& v10, nn::math::VEC4& v11, s32 numPtcl, s32 first, s32 set)
{
    cmd[0] = 0x80000000 | mUser.m_Start;
    cmd[1] = 0xF02C0;
    nn::gr::CTR::CopyMtx34WithHeader(reinterpret_cast<f32*>(cmd + 2), &mtx, 0xBF02C1);
    cmd += 16;
    cmd = putUniform_(cmd, 28, v0);
    cmd = putUniform_(cmd, 29, v1);
    cmd = putUniform_(cmd, 30, v2);
    cmd = putUniform_(cmd, 31, v3);
    cmd = putUniform_(cmd, 32, v4);
    cmd = putUniform_(cmd, 33, v5);
    cmd = putUniform_(cmd, 34, v6);
    cmd = putUniform_(cmd, 35, v10);
    cmd = putUniform_(cmd, 36, v11);
    cmd = putUniform_(cmd, 37, v7);
    cmd = putUniform_(cmd, 38, v8);
    cmd = putUniform_(cmd, 39, v9);

    nn::gr::CTR::Vertex* vertex = &mVertex[set];
    mIndexStream.m_DrawVertexCount = numPtcl - first;
    mIndexStream.m_PhysicalAddr =
        nngxGetPhysicalAddr(reinterpret_cast<uptr>(&mPrimitive.index[sGpuParticlePrimitive::NUM_VERTEX - numPtcl]));
    if (mIndexStream.m_DrawVertexCount >= sGpuParticlePrimitive::NUM_VERTEX) {
        mIndexStream.m_DrawVertexCount = sGpuParticlePrimitive::NUM_VERTEX - 1;
    }
    cmd = vertex->MakeEnableAttrCommand(cmd);
    return vertex->MakeDrawCommand(cmd, mIndexStream);
}

// 0x0055CBB0 | mk7dlp:bytes [tier B]
void sead::ptcl::GpuPtcle::Init(PtclRenderer* renderer, const u8* shaderBinary, s32 vsIndex, s32 gsIndex)
{
    mRenderer = renderer;
    InitShader(shaderBinary, vsIndex, gsIndex);
    InitPrimitive();
    mShaderCmdSize = reinterpret_cast<uptr>(mShader.MakeFullCommand(mShaderCmd)) - reinterpret_cast<uptr>(mShaderCmd);

    // the attributes disabled: none fixed, then the fixed attributes off
    u32* cmd = mPostCmd;
    *cmd++ = 0;
    *cmd++ = 0xF0201;
    *cmd++ = 0;
    *cmd++ = 0xF0202;
    memset(cmd, 0, 152);
    cmd[1] = 0x823F0203;
    cmd += 38;
    for (u32 i = 0; i < nn::gr::CTR::Vertex::ATTRIBUTE_COUNT; i++) {
        cmd[1] = 0x803F0232;
        cmd[2] = 0;
        cmd[3] = 0;
        cmd[4] = 0;
        cmd[0] = i;
        cmd[5] = 0;
        cmd += 6;
    }
    mPostCmdSize = reinterpret_cast<uptr>(cmd) - reinterpret_cast<uptr>(mPostCmd);
}

// 0x0055CCA0 | mk7dlp:bytes [tier B]
u32* sead::ptcl::GpuPtcle::PreDraw(u32* cmd)
{
    if (mRenderer->mCalcState == PtclRenderer::CALC_STATE_GPU) {
        return cmd;
    }
    mRenderer->mCalcState = PtclRenderer::CALC_STATE_GPU;
    f32* out = reinterpret_cast<f32*>(cmd);
    cmd[0] = 0x80000000 | mPointViewport.m_Start;
    cmd[1] = 0x804F0290;
    out[2] = s_PointViewport.w;
    out[3] = s_PointViewport.z;
    out[4] = s_PointViewport.y;
    out[5] = s_PointViewport.x;
    cmd += 6;
    memcpy(cmd, mShaderCmd, mShaderCmdSize);
    cmd = reinterpret_cast<u32*>(reinterpret_cast<u8*>(cmd) + (mShaderCmdSize & ~3));
    memcpy(cmd, mPostCmd, mPostCmdSize);
    return reinterpret_cast<u32*>(reinterpret_cast<u8*>(cmd) + (mPostCmdSize & ~3));
}

// 0x0055CD64 | mk7dlp:bytes [tier B]
u32* sead::ptcl::GpuPtcle::PostDraw(u32* cmd)
{
    memcpy(cmd, mPostCmd, mPostCmdSize);
    return reinterpret_cast<u32*>(reinterpret_cast<u8*>(cmd) + (mPostCmdSize & ~3));
}

// 0x0055CD98 | mk7dlp:bytes-fuzzy [tier B]
sead::ptcl::GpuPtcle::GpuPtcle()
    : mShader(), mUser(), mProj(), mView(), mTime(), mScaleParam(), mScaleAnm(), mEmitterVel(), mSpreadVec(),
      mGravity(), mColor(), mAlphaAnm(), mRotateZ(), mColorPk(), mColorEd(), mColorAnm(), mPointViewport(1, 2),
      mPointDistanceAttenuation(1, 4), mIndexStream()
{
}
} // namespace ptcl
} // namespace sead
