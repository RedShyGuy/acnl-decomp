#include "sead/ptcl/seadEmitterCalc.h"
#include "sead/ptcl/seadEmitterData.h"
#include "sead/ptcl/seadEmitterInstance.h"
#include "sead/ptcl/seadEmitterSet.h"
#include "sead/ptcl/seadPtclInstance.h"
#include "sead/ptcl/seadPtclRandom.h"
#include "sead/ptcl/seadPtclSystem.h"
#include "sead/ptcl/ptcl_Api.h"
#include "sead/seadMathCalcCommon.h"
#include "sead/seadNew.h"

#include <math.h>

namespace sead {
namespace ptcl {
// 0x0097F704
PtclSystem* EmitterCalc::s_pSystem = nullptr;
// 0x0097F708
PtclRenderer* EmitterCalc::s_pRenderer = nullptr;
// 0x0097F70C
TextureHandle* EmitterCalc::s_CurrentTexture = nullptr;
// 0x0097F710
EmitterCalc::CurrentState EmitterCalc::s_CurrentState = {0, 0, 0};
// 0x0097F764
f32* EmitterCalc::s_pFluctuationTbl = nullptr;
// 0x00AF4EB8
f32 EmitterCalc::s_QuadBlockIndex[8] = {14.0f, 23.0f, 32.0f, 41.0f, 50.0f, 59.0f, 68.0f, 77.0f};
// 0x00AF4ED8
f32 EmitterCalc::s_BillboardBlockIndex[4] = {0.0f, 5.0f, 5.0f, 0.0f};
// (the static initializer sets s_QuadBlockIndex and s_BillboardBlockIndex; GCC initializes them constantly)
// 0x0079D24C (static initializer)

// the directions of _emitSphereSameDivide: the vertices of polyhedra with 2 to 32 corners
// 0x008BBF24
extern const Vector3<f32> s_SphereDiv2[2] = {
    {0.0f, 0x1.0000000000000p+0f, 0.0f},
    {0.0f, -0x1.0000000000000p+0f, 0.0f},
};
// 0x008BBF3C
extern const Vector3<f32> s_SphereDiv3[3] = {
    {0.0f, 0x1.0000000000000p+0f, 0.0f},
    {-0x1.6a09ee0000000p-1f, -0x1.6a09ee0000000p-1f, 0.0f},
    {0x1.6a09ee0000000p-1f, -0x1.6a09ee0000000p-1f, 0.0f},
};
// 0x008BBF60
extern const Vector3<f32> s_SphereDiv4[4] = {
    {0.0f, -0x1.0000000000000p+0f, 0.0f},
    {0x1.e2b7ba0000000p-2f, 0x1.55553e0000000p-2f, 0x1.a20be60000000p-1f},
    {0x1.e2b7ba0000000p-2f, 0x1.55553e0000000p-2f, -0x1.a20be60000000p-1f},
    {-0x1.e2b7dc0000000p-1f, 0x1.55553e0000000p-2f, 0.0f},
};
// 0x008BBF90
extern const Vector3<f32> s_SphereDiv6[6] = {
    {0.0f, 0.0f, 0x1.0000000000000p+0f},
    {0.0f, 0x1.0000000000000p+0f, 0.0f},
    {0.0f, 0.0f, -0x1.0000000000000p+0f},
    {0.0f, -0x1.0000000000000p+0f, 0.0f},
    {0x1.0000000000000p+0f, 0.0f, 0.0f},
    {-0x1.0000000000000p+0f, 0.0f, 0.0f},
};
// 0x008BBFD8
extern const Vector3<f32> s_SphereDiv8[8] = {
    {0x1.279a6c0000000p-1f, 0x1.279a6c0000000p-1f, -0x1.279a6c0000000p-1f},
    {-0x1.279a6c0000000p-1f, 0x1.279a6c0000000p-1f, -0x1.279a6c0000000p-1f},
    {-0x1.279a6c0000000p-1f, 0x1.279a6c0000000p-1f, 0x1.279a6c0000000p-1f},
    {0x1.279a6c0000000p-1f, 0x1.279a6c0000000p-1f, 0x1.279a6c0000000p-1f},
    {0x1.279a6c0000000p-1f, -0x1.279a6c0000000p-1f, -0x1.279a6c0000000p-1f},
    {-0x1.279a6c0000000p-1f, -0x1.279a6c0000000p-1f, -0x1.279a6c0000000p-1f},
    {-0x1.279a6c0000000p-1f, -0x1.279a6c0000000p-1f, 0x1.279a6c0000000p-1f},
    {0x1.279a6c0000000p-1f, -0x1.279a6c0000000p-1f, 0x1.279a6c0000000p-1f},
};
// 0x008BC038
extern const Vector3<f32> s_SphereDiv12[12] = {
    {0x1.b388880000000p-1f, 0.0f, -0x1.0d2c9e0000000p-1f},
    {0x1.b388880000000p-1f, 0.0f, 0x1.0d2c9e0000000p-1f},
    {0x1.0d2c9e0000000p-1f, -0x1.b388880000000p-1f, 0.0f},
    {0.0f, -0x1.0d2c9e0000000p-1f, 0x1.b388880000000p-1f},
    {-0x1.0d2c9e0000000p-1f, -0x1.b388880000000p-1f, 0.0f},
    {0.0f, -0x1.0d2c9e0000000p-1f, -0x1.b388880000000p-1f},
    {0.0f, 0x1.0d2c9e0000000p-1f, -0x1.b388880000000p-1f},
    {0x1.0d2c9e0000000p-1f, 0x1.b388880000000p-1f, 0.0f},
    {0.0f, 0x1.0d2c9e0000000p-1f, 0x1.b388880000000p-1f},
    {-0x1.b388880000000p-1f, 0.0f, 0x1.0d2c9e0000000p-1f},
    {-0x1.0d2c9e0000000p-1f, 0x1.b388880000000p-1f, 0.0f},
    {-0x1.b388880000000p-1f, 0.0f, -0x1.0d2c9e0000000p-1f},
};
// 0x008BC0C8
extern const Vector3<f32> s_SphereDiv20[20] = {
    {0x1.de4bca0000000p-1f, -0x1.6d62c00000000p-2f, 0.0f},
    {0x1.de4bca0000000p-1f, 0x1.6d62c00000000p-2f, 0.0f},
    {0x1.6d62c00000000p-2f, 0.0f, -0x1.de4bca0000000p-1f},
    {0x1.279a6c0000000p-1f, 0x1.279a6c0000000p-1f, -0x1.279a6c0000000p-1f},
    {0x1.279a6c0000000p-1f, -0x1.279a6c0000000p-1f, -0x1.279a6c0000000p-1f},
    {0x1.6d62c00000000p-2f, 0.0f, 0x1.de4bca0000000p-1f},
    {0x1.279a6c0000000p-1f, -0x1.279a6c0000000p-1f, 0x1.279a6c0000000p-1f},
    {0x1.279a6c0000000p-1f, 0x1.279a6c0000000p-1f, 0x1.279a6c0000000p-1f},
    {-0x1.de4bca0000000p-1f, -0x1.6d62c00000000p-2f, 0.0f},
    {-0x1.de4bca0000000p-1f, 0x1.6d62c00000000p-2f, 0.0f},
    {-0x1.6d62c00000000p-2f, 0.0f, 0x1.de4bca0000000p-1f},
    {-0x1.279a6c0000000p-1f, -0x1.279a6c0000000p-1f, 0x1.279a6c0000000p-1f},
    {-0x1.279a6c0000000p-1f, 0x1.279a6c0000000p-1f, 0x1.279a6c0000000p-1f},
    {-0x1.6d62c00000000p-2f, 0.0f, -0x1.de4bca0000000p-1f},
    {-0x1.279a6c0000000p-1f, 0x1.279a6c0000000p-1f, -0x1.279a6c0000000p-1f},
    {-0x1.279a6c0000000p-1f, -0x1.279a6c0000000p-1f, -0x1.279a6c0000000p-1f},
    {0.0f, -0x1.de4bca0000000p-1f, 0x1.6d62c00000000p-2f},
    {0.0f, 0x1.de4bca0000000p-1f, 0x1.6d62c00000000p-2f},
    {0.0f, 0x1.de4bca0000000p-1f, -0x1.6d62c00000000p-2f},
    {0.0f, -0x1.de4bca0000000p-1f, -0x1.6d62c00000000p-2f},
};
// 0x008BC1B8
extern const Vector3<f32> s_SphereDiv32[32] = {
    {0x1.8208180000000p-3f, 0x1.d475600000000p-1f, -0x1.6d62c00000000p-2f},
    {-0x1.9a9b4a0000000p-2f, 0x1.d475600000000p-1f, 0x1.742bb60000000p-5f},
    {0x1.8c460e0000000p-3f, 0x1.e0e3480000000p-1f, 0x1.225aee0000000p-2f},
    {0x1.75be5e0000000p-1f, 0x1.5d2b2c0000000p-1f, 0x1.742bb60000000p-5f},
    {-0x1.8c460e0000000p-2f, 0x1.74434e0000000p-1f, -0x1.225aee0000000p-1f},
    {-0x1.ca63300000000p-3f, 0x1.5d2b2c0000000p-1f, 0x1.6480420000000p-1f},
    {0x1.4923c40000000p-1f, 0x1.07a3340000000p-1f, -0x1.225b100000000p-1f},
    {0x1.e68c260000000p-2f, 0x1.1371760000000p-1f, 0x1.6480420000000p-1f},
    {-0x1.b8fbca0000000p-1f, 0x1.cbc1260000000p-2f, -0x1.e72e280000000p-3f},
    {0x1.7adc0a0000000p-4f, 0x1.cbc1ac0000000p-2f, -0x1.c709100000000p-1f},
    {-0x1.865e020000000p-1f, 0x1.07a3340000000p-1f, 0x1.9142f60000000p-2f},
    {0x1.efef5a0000000p-1f, 0x1.26e5480000000p-4f, -0x1.e72d1a0000000p-3f},
    {0x1.cec8b40000000p-1f, 0x1.5f82b20000000p-3f, 0x1.9142700000000p-2f},
    {-0x1.1cd4880000000p-1f, 0x1.49b62c0000000p-3f, -0x1.a165f60000000p-1f},
    {0x1.21a9320000000p-5f, 0x1.5f84440000000p-3f, 0x1.f813aa0000000p-1f},
    {-0x1.26377a0000000p-1f, 0x1.26e6540000000p-4f, 0x1.a166160000000p-1f},
    {0x1.26377a0000000p-1f, -0x1.26e6540000000p-4f, -0x1.a166160000000p-1f},
    {0x1.1cd4880000000p-1f, -0x1.49b62c0000000p-3f, 0x1.a165f60000000p-1f},
    {-0x1.cec8b40000000p-1f, -0x1.5f82b20000000p-3f, -0x1.9142700000000p-2f},
    {-0x1.efef5a0000000p-1f, -0x1.26e5480000000p-4f, 0x1.e72d1a0000000p-3f},
    {-0x1.21a9320000000p-5f, -0x1.5f84440000000p-3f, -0x1.f813aa0000000p-1f},
    {0x1.865e020000000p-1f, -0x1.07a3340000000p-1f, -0x1.9142f60000000p-2f},
    {0x1.b8fbca0000000p-1f, -0x1.cbc1260000000p-2f, 0x1.e72e280000000p-3f},
    {-0x1.e68c260000000p-2f, -0x1.1371760000000p-1f, -0x1.6480420000000p-1f},
    {-0x1.7adc0a0000000p-4f, -0x1.cbc1ac0000000p-2f, 0x1.c709100000000p-1f},
    {-0x1.4923c40000000p-1f, -0x1.07a3340000000p-1f, 0x1.225b100000000p-1f},
    {0x1.ca63300000000p-3f, -0x1.5d2b2c0000000p-1f, -0x1.6480420000000p-1f},
    {0x1.8c460e0000000p-2f, -0x1.74434e0000000p-1f, 0x1.225aee0000000p-1f},
    {-0x1.75be5e0000000p-1f, -0x1.5d2b2c0000000p-1f, -0x1.742bb60000000p-5f},
    {0x1.9a9b4a0000000p-2f, -0x1.d475600000000p-1f, -0x1.742bb60000000p-5f},
    {-0x1.8c460e0000000p-3f, -0x1.e0e3480000000p-1f, -0x1.225aee0000000p-2f},
    {-0x1.8208180000000p-3f, -0x1.d475600000000p-1f, 0x1.6d62c00000000p-2f},
};

// 0x0097F83C
const Vector3<f32>* EmitterCalc::s_SphereDivTbl[8] = {s_SphereDiv2,  s_SphereDiv3,  s_SphereDiv4,  s_SphereDiv6,
                                                      s_SphereDiv8,  s_SphereDiv12, s_SphereDiv20, s_SphereDiv32};

namespace {
// the number of particles an emission of emitter makes
inline s32 getEmitNum_(const EmitterInstance* emitter)
{
    return (emitter->mRecord->emitRate * emitter->mRes->emitRate) >> 8;
}

// the start of a particle of an emission
inline void startPtcl_(PtclInstance* ptcl, SimpleEmitterData* res)
{
    ptcl->mRes = res;
    ptcl->mStripe = nullptr;
}

// the block of the vertex shader of a particle: per row the color component and the row of the rotation
// with the columns in the order z, y, x
inline u32* writeBlock_(u32* cmd, f32 r, f32 g, f32 b, f32 m00, f32 m01, f32 m02, f32 m10, f32 m11, f32 m12, f32 m20,
                        f32 m21, f32 m22)
{
    f32* out = reinterpret_cast<f32*>(cmd);
    out[0] = r;
    out[1] = m02;
    out[2] = m01;
    out[3] = m00;
    out[4] = g;
    out[5] = m12;
    out[6] = m11;
    out[7] = m10;
    out[8] = b;
    out[9] = m22;
    out[10] = m21;
    out[11] = m20;
    return cmd + 12;
}

// the blocks without rotation and with a rotation about one axis or all three (z * y * x)
inline u32* writeRotNone_(u32* cmd, f32 r, f32 g, f32 b)
{
    return writeBlock_(cmd, r, g, b, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

inline u32* writeRotX_(u32* cmd, f32 r, f32 g, f32 b, f32 s, f32 c)
{
    return writeBlock_(cmd, r, g, b, 1.0f, 0.0f, 0.0f, 0.0f, c, -s, 0.0f, s, c);
}

inline u32* writeRotY_(u32* cmd, f32 r, f32 g, f32 b, f32 s, f32 c)
{
    return writeBlock_(cmd, r, g, b, c, 0.0f, s, 0.0f, 1.0f, 0.0f, -s, 0.0f, c);
}

inline u32* writeRotZ_(u32* cmd, f32 r, f32 g, f32 b, f32 s, f32 c)
{
    return writeBlock_(cmd, r, g, b, c, -s, 0.0f, s, c, 0.0f, 0.0f, 0.0f, 1.0f);
}

inline u32* writeRotXYZ_(u32* cmd, f32 r, f32 g, f32 b, f32 sx, f32 cx, f32 sy, f32 cy, f32 sz, f32 cz)
{
    return writeBlock_(cmd, r, g, b, cy * cz, sx * sy * cz - cx * sz, sx * sz + cx * cz * sy, cy * sz,
                       cx * cz + sx * sy * sz, sy * (cx * sz) - sx * cz, -sy, sx * cy, cx * cy);
}

// the rotation of a particle in a frame
inline void addRot_(PtclInstance* ptcl, s32 axis)
{
    ptcl->mRot[axis] += ptcl->mRotVel[axis];
}

// the block with the rotation of all three axes, the sines and cosines from the table
inline u32* writeRotXYZInline_(u32* cmd, f32 r, f32 g, f32 b, PtclInstance* ptcl)
{
    u32 x = ptcl->mRot[0] + ptcl->mRotVel[0];
    u32 y = ptcl->mRot[1] + ptcl->mRotVel[1];
    u32 z = ptcl->mRot[2] + ptcl->mRotVel[2];
    ptcl->mRot[1] = y;
    ptcl->mRot[0] = x;
    ptcl->mRot[2] = z;
    return writeRotXYZ_(cmd, r, g, b, MathCommon<f32>::sinIdx(x), MathCommon<f32>::cosIdx(x), MathCommon<f32>::sinIdx(y),
                        MathCommon<f32>::cosIdx(y), MathCommon<f32>::sinIdx(z), MathCommon<f32>::cosIdx(z));
}
} // namespace

// 0x005517E4 (name is ours)
void sead::ptcl::EmitterCalc::initPtcl_(EmitterInstance* emitter, PtclInstance* ptcl)
{
    EmitterSet* set = emitter->mEmitterSet;
    SimpleEmitterData* res = emitter->mRes;

    f32 velRandom = 1.0f - res->initVelocityRandom * set->mRandomVelocityScale * emitter->mRandom.getF32();
    f32 dirVel = res->emitterVelocity * set->mDirectionalVelocityScale;
    Vector3<f32> vel;
    if (set->mIsSetDirectional) {
        vel.x = (ptcl->mVel.x + set->mDirectional.x * dirVel) * velRandom;
        vel.y = (ptcl->mVel.y + set->mDirectional.y * dirVel) * velRandom;
        vel.z = (ptcl->mVel.z + set->mDirectional.z * dirVel) * velRandom;
    } else {
        vel.x = (ptcl->mVel.x + res->emitterVelocityDir.x * dirVel) * velRandom;
        vel.y = (ptcl->mVel.y + res->emitterVelocityDir.y * dirVel) * velRandom;
        vel.z = (ptcl->mVel.z + res->emitterVelocityDir.z * dirVel) * velRandom;
    }
    const Vector3<f32>& spread = emitter->mRandom.getVec3();
    vel.x += spread.x * res->spreadVec.x;
    vel.y += spread.y * res->spreadVec.y;
    vel.z += spread.z * res->spreadVec.z;
    // the velocity added by the set, in its axes
    const Vector3<f32>& add = set->mAddVelocity;
    const Matrix34<f32>& rt = set->mRT;
    vel.x += add.x * rt.m[0][0] + add.y * rt.m[1][0] + add.z * rt.m[2][0];
    vel.y += add.x * rt.m[0][1] + add.y * rt.m[1][1] + add.z * rt.m[2][1];
    vel.z += add.x * rt.m[0][2] + add.y * rt.m[1][2] + add.z * rt.m[2][2];

    ptcl->mCnt = 0;
    ptcl->mRandom = emitter->mRandom.getU32();
    if (res->ptclLife == 0x7FFFFFFF) {
        ptcl->mLife = 0x7FFFFFFF;
    } else {
        ptcl->mLife = ((res->ptclLife - emitter->mRandom.getS32Range(res->ptclLifeRandom)) * emitter->mRecord->life) >> 8;
    }

    f32 scaleRandom = 1.0f - res->scaleRandom * emitter->mRandom.getF32();
    f32 scaleX = set->mPtclEmitScale.x * scaleRandom;
    f32 scaleY = set->mPtclEmitScale.y * scaleRandom;
    s32 life = ptcl->mLife - 1;
    if (life == 0) {
        ptcl->mAlphaSection1 = -1;
        ptcl->mAlphaSection2 = 0x7FFFFFFF;
        ptcl->mAlphaAdd1 = 0.0f;
        ptcl->mAlphaAdd2 = 0.0f;
        ptcl->mAlpha = res->alphaInit;
        ptcl->mScaleSection1 = -1;
        ptcl->mScaleSection2 = 0x7FFFFFFF;
        ptcl->mScaleAdd1 = Vector2<f32>::zero;
        ptcl->mScaleAdd2 = Vector2<f32>::zero;
        ptcl->mScale.x = res->scaleInit.x * scaleX;
        ptcl->mScale.y = res->scaleInit.y * scaleY;
    } else {
        ptcl->mAlphaSection1 = res->alphaSection1 * life / 100;
        ptcl->mAlphaSection2 = res->alphaSection2 * life / 100;
        ptcl->mScaleSection1 = res->scaleSection1 * life / 100;
        ptcl->mScaleSection2 = res->scaleSection2 * life / 100;
        ptcl->mAlphaAdd1 = res->alphaDiff1 / ptcl->mAlphaSection1;
        ptcl->mAlphaAdd2 = res->alphaDiff2 / (life - ptcl->mAlphaSection2);
        ptcl->mAlpha = res->alphaInit - ptcl->mAlphaAdd1;
        f32 inv1 = 1.0f / ptcl->mScaleSection1;
        f32 inv2 = 1.0f / (life - ptcl->mScaleSection2);
        ptcl->mScaleAdd1.x = res->scaleDiff1.x * (inv1 * scaleX);
        ptcl->mScaleAdd1.y = res->scaleDiff1.y * (inv1 * scaleY);
        ptcl->mScaleAdd2.x = res->scaleDiff2.x * (inv2 * scaleX);
        ptcl->mScaleAdd2.y = res->scaleDiff2.y * (inv2 * scaleY);
        ptcl->mScale.x = (res->scaleInit.x - ptcl->mScaleAdd1.x) * scaleX;
        ptcl->mScale.y = (res->scaleInit.y - ptcl->mScaleAdd1.y) * scaleY;
    }

    for (s32 i = 0; i < 3; i++) {
        ptcl->mRot[i] = res->rotInit[i] + (emitter->mRandom.getU32() >> 24) * res->rotInitRandom[i];
    }
    for (s32 i = 0; i < 3; i++) {
        ptcl->mRotVel[i] = res->rotVel[i] + (emitter->mRandom.getU32() >> 24) * res->rotVelRandom[i];
    }

    if (res->ptclFollowType == 0) {
        Matrix34CalcCtr<f32>::multiply(ptcl->mEmitterRT, set->mRT, res->emitterRT);
        Matrix34CalcCtr<f32>::multiply(ptcl->mEmitterSRT, set->mSRT, res->emitterSRT);
    }

    if (res->texPatNum <= 1) {
        ptcl->mTexOffset.x = 0.0f;
        ptcl->mTexOffset.y = 0.0f;
        ptcl->mTexScale = res->texPatScale;
    } else {
        s32 pattern = emitter->mRandom.getS32Range(res->texPatNum);
        s32 row = pattern / res->texPatDivX;
        s32 column = pattern % res->texPatDivX;
        ptcl->mTexScale = res->texPatScale;
        ptcl->mTexOffset.x = res->texPatScale.x * column;
        ptcl->mTexOffset.y = res->texPatScale.y * row;
    }

    ptcl->mVel = vel;

    PtclInstance* head = emitter->mPtclHead;
    emitter->mPtclHead = ptcl;
    if (head == nullptr) {
        ptcl->mNext = nullptr;
    } else {
        head->mPrev = ptcl;
        ptcl->mNext = head;
    }
    ptcl->mPrev = nullptr;
}

// 0x005515D4 | mk7dlp:bytes [tier B]
void sead::ptcl::EmitterCalc::_emitPoint(EmitterInstance* emitter)
{
    SimpleEmitterData* res = emitter->mRes;
    s32 num = getEmitNum_(emitter);
    if (num == 0) {
        return;
    }
    f32 figureVel = res->figureVelocity * emitter->mEmitterSet->mFigureVelocityScale;
    for (s32 i = 0; i < num; i++) {
        PtclInstance* ptcl = s_pSystem->allocPtcl();
        if (ptcl == nullptr) {
            continue;
        }
        ptcl->mRes = res;
        ptcl->mPos = Vector3<f32>::zero;
        ptcl->mStripe = nullptr;
        const Vector3<f32>& dir = emitter->mRandom.getNormalizedVec3();
        ptcl->mVel.x = dir.x * figureVel;
        ptcl->mVel.y = dir.y * figureVel;
        ptcl->mVel.z = dir.z * figureVel;
        initPtcl_(emitter, ptcl);
    }
}

// 0x005516C0 (name is ours)
void sead::ptcl::EmitterCalc::_emitCircle(EmitterInstance* emitter)
{
    SimpleEmitterData* res = emitter->mRes;
    s32 num = getEmitNum_(emitter);
    if (num == 0) {
        return;
    }
    EmitterSet* set = emitter->mEmitterSet;
    f32 figureVel = res->figureVelocity * set->mFigureVelocityScale;
    f32 scaleX = res->volumeScale.x * set->mEmitterVolumeScale.x;
    f32 scaleZ = res->volumeScale.z * set->mEmitterVolumeScale.z;
    for (s32 i = 0; i < num; i++) {
        PtclInstance* ptcl = s_pSystem->allocPtcl();
        if (ptcl == nullptr) {
            continue;
        }
        startPtcl_(ptcl, res);
        u32 angle = res->volumeArcStart + emitter->mRandom.getU32() % res->volumeArcWidth;
        f32 s;
        f32 c;
        MathCtr<f32>::sinCosIdx(&s, &c, angle);
        ptcl->mPos.x = s * scaleX;
        ptcl->mPos.y = 0.0f;
        ptcl->mPos.z = c * scaleZ;
        ptcl->mVel.z = c * figureVel;
        ptcl->mVel.y = 0.0f;
        ptcl->mVel.x = s * figureVel;
        initPtcl_(emitter, ptcl);
    }
}

// 0x00551D94 (name is ours)
void sead::ptcl::EmitterCalc::_emitSphere(EmitterInstance* emitter)
{
    SimpleEmitterData* res = emitter->mRes;
    s32 num = getEmitNum_(emitter);
    if (num == 0) {
        return;
    }
    EmitterSet* set = emitter->mEmitterSet;
    f32 figureVel = res->figureVelocity * set->mFigureVelocityScale;
    f32 scaleX = res->volumeScale.x * set->mEmitterVolumeScale.x;
    f32 scaleY = res->volumeScale.y * set->mEmitterVolumeScale.y;
    f32 scaleZ = res->volumeScale.z * set->mEmitterVolumeScale.z;
    for (s32 i = 0; i < num; i++) {
        PtclInstance* ptcl = s_pSystem->allocPtcl();
        if (ptcl == nullptr) {
            continue;
        }
        startPtcl_(ptcl, res);
        u32 angleY = res->volumeArcStart + emitter->mRandom.getU32() % res->volumeArcWidth;
        u32 angleX = (emitter->mRandom.getU32() >> 1) + 0xC0000001;
        f32 sinY;
        f32 cosY;
        f32 sinX;
        f32 cosX;
        MathCtr<f32>::sinCosIdx(&sinY, &cosY, angleY);
        MathCtr<f32>::sinCosIdx(&sinX, &cosX, angleX);
        f32 x = sinY * cosX;
        f32 y = -sinX;
        f32 z = cosY * cosX;
        ptcl->mVel.x = x * figureVel;
        ptcl->mVel.y = y * figureVel;
        ptcl->mVel.z = z * figureVel;
        ptcl->mPos.x = x * scaleX;
        ptcl->mPos.y = y * scaleY;
        ptcl->mPos.z = z * scaleZ;
        initPtcl_(emitter, ptcl);
    }
}

// 0x00551EF8 (name is ours)
void sead::ptcl::EmitterCalc::_emitFillBox(EmitterInstance* emitter)
{
    SimpleEmitterData* res = emitter->mRes;
    s32 num = getEmitNum_(emitter);
    if (num == 0) {
        return;
    }
    EmitterSet* set = emitter->mEmitterSet;
    f32 figureVel = res->figureVelocity * set->mFigureVelocityScale;
    f32 scaleX = res->volumeScale.x * set->mEmitterVolumeScale.x;
    f32 scaleY = res->volumeScale.y * set->mEmitterVolumeScale.y;
    f32 scaleZ = res->volumeScale.z * set->mEmitterVolumeScale.z;
    for (s32 i = 0; i < num; i++) {
        PtclInstance* ptcl = s_pSystem->allocPtcl();
        if (ptcl == nullptr) {
            continue;
        }
        ptcl->mRes = res;
        f32 rx = emitter->mRandom.getF32();
        f32 ry = emitter->mRandom.getF32();
        f32 rz = emitter->mRandom.getF32();
        ptcl->mStripe = nullptr;
        ptcl->mPos.x = (rx * 2.0f - 1.0f) * scaleX;
        ptcl->mPos.y = (ry * 2.0f - 1.0f) * scaleY;
        ptcl->mPos.z = (rz * 2.0f - 1.0f) * scaleZ;
        // (the direction keeps no value for a particle at the center, like the original)
        Vector3<f32> dir;
        f32 length = sqrtf(ptcl->mPos.x * ptcl->mPos.x + ptcl->mPos.y * ptcl->mPos.y + ptcl->mPos.z * ptcl->mPos.z);
        if (length > 0.0f) {
            f32 inv = 1.0f / length;
            dir.x = ptcl->mPos.x * inv;
            dir.y = ptcl->mPos.y * inv;
            dir.z = ptcl->mPos.z * inv;
        }
        ptcl->mVel.x = dir.x * figureVel;
        ptcl->mVel.y = dir.y * figureVel;
        ptcl->mVel.z = dir.z * figureVel;
        initPtcl_(emitter, ptcl);
    }
}

// 0x005520AC (name is ours)
void sead::ptcl::EmitterCalc::_emitCylinder(EmitterInstance* emitter)
{
    SimpleEmitterData* res = emitter->mRes;
    s32 num = getEmitNum_(emitter);
    if (num == 0) {
        return;
    }
    EmitterSet* set = emitter->mEmitterSet;
    f32 figureVel = res->figureVelocity * set->mFigureVelocityScale;
    f32 scaleX = res->volumeScale.x * set->mEmitterVolumeScale.x;
    f32 scaleY = res->volumeScale.y * set->mEmitterVolumeScale.y;
    f32 scaleZ = res->volumeScale.z * set->mEmitterVolumeScale.z;
    u32 arcStart = res->volumeArcStart;
    for (s32 i = 0; i < num; i++) {
        PtclInstance* ptcl = s_pSystem->allocPtcl();
        if (ptcl == nullptr) {
            continue;
        }
        startPtcl_(ptcl, res);
        u32 angle = emitter->mRandom.getU32() % res->volumeArcWidth + arcStart;
        f32 s;
        f32 c;
        MathCtr<f32>::sinCosIdx(&s, &c, angle);
        const Vector3<f32>& random = emitter->mRandom.getVec3();
        ptcl->mPos.x = s * scaleX;
        ptcl->mPos.y = random.y * scaleY;
        ptcl->mPos.z = c * scaleZ;
        ptcl->mVel.z = c * figureVel;
        ptcl->mVel.y = 0.0f;
        ptcl->mVel.x = s * figureVel;
        initPtcl_(emitter, ptcl);
    }
}

// 0x00552208 (name is ours)
void sead::ptcl::EmitterCalc::_emitRectangle(EmitterInstance* emitter)
{
    SimpleEmitterData* res = emitter->mRes;
    s32 num = getEmitNum_(emitter);
    if (num == 0) {
        return;
    }
    EmitterSet* set = emitter->mEmitterSet;
    f32 figureVel = res->figureVelocity * set->mFigureVelocityScale;
    f32 scaleX = res->volumeScale.x * set->mEmitterVolumeScale.x;
    f32 scaleZ = res->volumeScale.z * set->mEmitterVolumeScale.z;
    for (s32 i = 0; i < num; i++) {
        PtclInstance* ptcl = s_pSystem->allocPtcl();
        if (ptcl == nullptr) {
            continue;
        }
        startPtcl_(ptcl, res);
        bool onSideZ = emitter->mRandom.getU32() < 0x7FFFFFFF;
        bool positive = emitter->mRandom.getU32() < 0x7FFFFFFF;
        const Vector3<f32>& random = emitter->mRandom.getVec3();
        if (onSideZ) {
            ptcl->mPos.x = random.x * scaleX;
            ptcl->mPos.y = 0.0f;
            ptcl->mPos.z = positive ? scaleZ : -scaleZ;
        } else {
            ptcl->mPos.x = positive ? scaleX : -scaleX;
            ptcl->mPos.y = 0.0f;
            ptcl->mPos.z = random.z * scaleZ;
        }
        // (the direction keeps no value for a particle at the center, like the original)
        f32 dirX;
        f32 dirZ;
        f32 length = sqrtf(ptcl->mPos.x * ptcl->mPos.x + 0.0f * 0.0f + ptcl->mPos.z * ptcl->mPos.z);
        if (length > 0.0f) {
            f32 inv = 1.0f / length;
            dirZ = ptcl->mPos.z * inv;
            dirX = ptcl->mPos.x * inv;
        }
        ptcl->mVel.x = dirX * figureVel;
        ptcl->mVel.z = dirZ * figureVel;
        ptcl->mVel.y = 0.0f;
        initPtcl_(emitter, ptcl);
    }
}

// 0x0055257C (name is ours)
void sead::ptcl::EmitterCalc::_emitFillCircle(EmitterInstance* emitter)
{
    SimpleEmitterData* res = emitter->mRes;
    s32 num = getEmitNum_(emitter);
    if (num == 0) {
        return;
    }
    EmitterSet* set = emitter->mEmitterSet;
    f32 figureVel = res->figureVelocity * set->mFigureVelocityScale;
    f32 scaleX = res->volumeScale.x * set->mEmitterVolumeScale.x;
    f32 scaleZ = res->volumeScale.z * set->mEmitterVolumeScale.z;
    for (s32 i = 0; i < num; i++) {
        PtclInstance* ptcl = s_pSystem->allocPtcl();
        if (ptcl == nullptr) {
            continue;
        }
        startPtcl_(ptcl, res);
        u32 angle = res->volumeArcStart + emitter->mRandom.getU32() % res->volumeArcWidth;
        f32 s;
        f32 c;
        MathCtr<f32>::sinCosIdx(&s, &c, angle);
        f32 radius = emitter->mRandom.getF32();
        ptcl->mPos.x = s * radius * scaleX;
        ptcl->mPos.y = 0.0f;
        ptcl->mPos.z = c * radius * scaleZ;
        ptcl->mVel.z = c * figureVel;
        ptcl->mVel.y = 0.0f;
        ptcl->mVel.x = s * figureVel;
        initPtcl_(emitter, ptcl);
    }
}

// 0x005526D4 (name is ours)
void sead::ptcl::EmitterCalc::_emitFillSphere(EmitterInstance* emitter)
{
    SimpleEmitterData* res = emitter->mRes;
    s32 num = getEmitNum_(emitter);
    if (num == 0) {
        return;
    }
    EmitterSet* set = emitter->mEmitterSet;
    f32 figureVel = res->figureVelocity * set->mFigureVelocityScale;
    f32 scaleX = res->volumeScale.x * set->mEmitterVolumeScale.x;
    f32 scaleY = res->volumeScale.y * set->mEmitterVolumeScale.y;
    f32 scaleZ = res->volumeScale.z * set->mEmitterVolumeScale.z;
    u32 arcStart = res->volumeArcStart;
    for (s32 i = 0; i < num; i++) {
        PtclInstance* ptcl = s_pSystem->allocPtcl();
        if (ptcl == nullptr) {
            continue;
        }
        startPtcl_(ptcl, res);
        u32 angleY = emitter->mRandom.getU32() % res->volumeArcWidth + arcStart;
        u32 angleX = (emitter->mRandom.getU32() >> 1) + 0xC0000001;
        f32 sinY;
        f32 cosY;
        f32 sinX;
        f32 cosX;
        MathCtr<f32>::sinCosIdx(&sinY, &cosY, angleY);
        MathCtr<f32>::sinCosIdx(&sinX, &cosX, angleX);
        f32 radius = emitter->mRandom.getF32();
        f32 x = sinY * cosX;
        f32 y = -sinX;
        f32 z = cosY * cosX;
        ptcl->mVel.y = y * figureVel;
        ptcl->mPos.x = x * (radius * scaleX);
        ptcl->mPos.y = y * (radius * scaleY);
        ptcl->mPos.z = z * (radius * scaleZ);
        ptcl->mVel.x = x * figureVel;
        ptcl->mVel.z = z * figureVel;
        initPtcl_(emitter, ptcl);
    }
}

// 0x00552AE8 (name is ours)
void sead::ptcl::EmitterCalc::_emitFillCylinder(EmitterInstance* emitter)
{
    SimpleEmitterData* res = emitter->mRes;
    s32 num = getEmitNum_(emitter);
    if (num == 0) {
        return;
    }
    EmitterSet* set = emitter->mEmitterSet;
    f32 figureVel = res->figureVelocity * set->mFigureVelocityScale;
    f32 scaleX = res->volumeScale.x * set->mEmitterVolumeScale.x;
    f32 scaleY = res->volumeScale.y * set->mEmitterVolumeScale.y;
    f32 scaleZ = res->volumeScale.z * set->mEmitterVolumeScale.z;
    u32 arcStart = res->volumeArcStart;
    for (s32 i = 0; i < num; i++) {
        PtclInstance* ptcl = s_pSystem->allocPtcl();
        if (ptcl == nullptr) {
            continue;
        }
        startPtcl_(ptcl, res);
        u32 angle = emitter->mRandom.getU32() % res->volumeArcWidth + arcStart;
        f32 s;
        f32 c;
        MathCtr<f32>::sinCosIdx(&s, &c, angle);
        f32 radius = emitter->mRandom.getF32();
        const Vector3<f32>& random = emitter->mRandom.getVec3();
        ptcl->mPos.x = radius * s * scaleX;
        ptcl->mPos.y = random.y * scaleY;
        ptcl->mPos.z = radius * c * scaleZ;
        ptcl->mVel.z = c * figureVel;
        ptcl->mVel.y = 0.0f;
        ptcl->mVel.x = s * figureVel;
        initPtcl_(emitter, ptcl);
    }
}

// 0x00552EA4 | mk7dlp:bytes [tier B]
void sead::ptcl::EmitterCalc::_emitLineSameDivide(EmitterInstance* emitter)
{
    SimpleEmitterData* res = emitter->mRes;
    s32 num = getEmitNum_(emitter);
    if (num == 0) {
        return;
    }
    EmitterSet* set = emitter->mEmitterSet;
    f32 figureVel = res->figureVelocity * set->mFigureVelocityScale;
    f32 scaleZ = res->volumeScale.z * set->mEmitterVolumeScale.z;
    f32 pos = 0.0f;
    f32 step;
    if (num == 1) {
        step = 0.0f;
        pos = 0.5f;
    } else {
        step = 1.0f / (num - 1);
    }
    for (s32 i = 0; i < num; i++) {
        PtclInstance* ptcl = s_pSystem->allocPtcl();
        if (ptcl == nullptr) {
            continue;
        }
        f32 z = pos * scaleZ;
        pos += step;
        ptcl->mRes = res;
        ptcl->mStripe = nullptr;
        ptcl->mVel.z = figureVel;
        ptcl->mPos.x = 0.0f;
        ptcl->mPos.y = 0.0f;
        ptcl->mVel.x = 0.0f;
        ptcl->mVel.y = 0.0f;
        ptcl->mPos.z = z;
        initPtcl_(emitter, ptcl);
    }
}

// 0x00553104 (name is ours)
void sead::ptcl::EmitterCalc::_emitCircleSameDivide(EmitterInstance* emitter)
{
    SimpleEmitterData* res = emitter->mRes;
    s32 num = getEmitNum_(emitter);
    if (num == 0) {
        return;
    }
    EmitterSet* set = emitter->mEmitterSet;
    f32 figureVel = res->figureVelocity * set->mFigureVelocityScale;
    f32 scaleX = res->volumeScale.x * set->mEmitterVolumeScale.x;
    f32 scaleZ = res->volumeScale.z * set->mEmitterVolumeScale.z;
    u32 angle = res->volumeArcStart;
    u32 step = res->volumeArcWidth;
    for (s32 i = 0; i < num; i++) {
        PtclInstance* ptcl = s_pSystem->allocPtcl();
        if (ptcl == nullptr) {
            continue;
        }
        ptcl->mStripe = nullptr;
        ptcl->mRes = res;
        f32 s;
        f32 c;
        MathCtr<f32>::sinCosIdx(&s, &c, angle);
        angle += step;
        ptcl->mPos.x = s * scaleX;
        ptcl->mPos.y = 0.0f;
        ptcl->mPos.z = c * scaleZ;
        ptcl->mVel.z = c * figureVel;
        ptcl->mVel.y = 0.0f;
        ptcl->mVel.x = s * figureVel;
        initPtcl_(emitter, ptcl);
    }
}

// 0x00553204 | mk7dlp:bytes [tier B]
void sead::ptcl::EmitterCalc::_emitSphereSameDivide(EmitterInstance* emitter)
{
    SimpleEmitterData* res = emitter->mRes;
    s32 num = getEmitNum_(emitter);
    if (num == 0) {
        return;
    }
    EmitterSet* set = emitter->mEmitterSet;
    f32 figureVel = res->figureVelocity * set->mFigureVelocityScale;
    f32 scaleX = res->volumeScale.x * set->mEmitterVolumeScale.x;
    f32 scaleY = res->volumeScale.y * set->mEmitterVolumeScale.y;
    f32 scaleZ = res->volumeScale.z * set->mEmitterVolumeScale.z;
    const Vector3<f32>* dir = s_SphereDivTbl[res->sphereDivTblIndex];
    for (s32 i = 0; i < num; i++) {
        PtclInstance* ptcl = s_pSystem->allocPtcl();
        if (ptcl == nullptr) {
            continue;
        }
        startPtcl_(ptcl, res);
        ptcl->mVel.x = dir->x * figureVel;
        ptcl->mVel.y = dir->y * figureVel;
        ptcl->mVel.z = dir->z * figureVel;
        ptcl->mPos.x = dir->x * scaleX;
        ptcl->mPos.y = dir->y * scaleY;
        ptcl->mPos.z = dir->z * scaleZ;
        dir++;
        initPtcl_(emitter, ptcl);
    }
}

// 0x0055287C | mk7dlp
const void* sead::ptcl::EmitterCalc::_ptclField_Spin(EmitterInstance*, PtclInstance* ptcl, const void* data)
{
    const FieldSpinData* spin = static_cast<const FieldSpinData*>(data);
    f32 s;
    f32 c;
    MathCtr<f32>::sinCosIdx(&s, &c, spin->angle);
    switch (spin->axis) {
    case 0: {
        f32 y = ptcl->mPos.y * c + ptcl->mPos.z * s;
        f32 z = ptcl->mPos.z * c - ptcl->mPos.y * s;
        ptcl->mPos.y = y;
        ptcl->mPos.z = z;
        break;
    }
    case 1: {
        f32 z = ptcl->mPos.z * c + ptcl->mPos.x * s;
        f32 x = ptcl->mPos.x * c - ptcl->mPos.z * s;
        ptcl->mPos.z = z;
        ptcl->mPos.x = x;
        break;
    }
    case 2: {
        f32 x = ptcl->mPos.x * c + ptcl->mPos.y * s;
        f32 y = ptcl->mPos.y * c - ptcl->mPos.x * s;
        ptcl->mPos.x = x;
        ptcl->mPos.y = y;
        break;
    }
    }
    return spin + 1;
}

// 0x00552940 | mk7dlp
void sead::ptcl::EmitterCalc::calcFluctuation(EmitterInstance* emitter, PtclInstance* ptcl)
{
    const SimpleEmitterData* res = emitter->mRes;
    const FluctuationData* fluctuation =
        reinterpret_cast<const FluctuationData*>(reinterpret_cast<const u8*>(res) + res->fluctuationDataOffset);
    u32 index = static_cast<s32>(ptcl->mCnt * fluctuation->frequency) + ptcl->mRandom * fluctuation->phaseRandom;
    f32 value = 1.0f - s_pFluctuationTbl[index & 127] * fluctuation->amplitude;
    if (res->fluctuationFlag & 2) {
        ptcl->mFluctuationAlpha = value;
    }
    if (res->fluctuationFlag & 4) {
        ptcl->mFluctuationScale = value;
    }
}

// 0x005529C0 (name is ours)
u32* sead::ptcl::EmitterCalc::_childCalc_RotXYZ(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl)
{
    addRot_(ptcl, 1);
    addRot_(ptcl, 2);
    addRot_(ptcl, 0);
    f32 sx;
    f32 cx;
    f32 sy;
    f32 cy;
    f32 sz;
    f32 cz;
    MathCtr<f32>::sinCosIdx(&sx, &cx, ptcl->mRot[0]);
    MathCtr<f32>::sinCosIdx(&sy, &cy, ptcl->mRot[1]);
    MathCtr<f32>::sinCosIdx(&sz, &cz, ptcl->mRot[2]);
    const Color4f& color = emitter->mEmitterSet->mColor;
    return writeRotXYZ_(cmd, ptcl->mColor.r * color.r, ptcl->mColor.g * color.g, ptcl->mColor.b * color.b, sx, cx, sy,
                        cy, sz, cz);
}

// 0x00552C78 | mk7dlp
const void* sead::ptcl::EmitterCalc::_ptclField_Magnet(EmitterInstance*, PtclInstance* ptcl, const void* data)
{
    const FieldMagnetData* magnet = static_cast<const FieldMagnetData*>(data);
    if (magnet->flag & 1) {
        ptcl->mVel.x += (magnet->pos.x - ptcl->mPos.x - ptcl->mVel.x) * magnet->power;
    }
    if (magnet->flag & 2) {
        ptcl->mVel.y += (magnet->pos.y - ptcl->mPos.y - ptcl->mVel.y) * magnet->power;
    }
    if (magnet->flag & 4) {
        ptcl->mVel.z += (magnet->pos.z - ptcl->mPos.z - ptcl->mVel.z) * magnet->power;
    }
    return magnet + 1;
}

// 0x00552CFC | mk7dlp:bytes [tier B]
const void* sead::ptcl::EmitterCalc::_ptclField_PosAdd(EmitterInstance*, PtclInstance* ptcl, const void* data)
{
    const FieldPosAddData* posAdd = static_cast<const FieldPosAddData*>(data);
    ptcl->mPos.x += posAdd->pos.x;
    ptcl->mPos.y += posAdd->pos.y;
    ptcl->mPos.z += posAdd->pos.z;
    return posAdd + 1;
}

// 0x00552D34 | mk7dlp:bytes [tier B]
const void* sead::ptcl::EmitterCalc::_ptclField_Random(EmitterInstance* emitter, PtclInstance* ptcl, const void* data)
{
    const FieldRandomData* random = static_cast<const FieldRandomData*>(data);
    if (ptcl->mCnt % random->interval == 0) {
        const Vector3<f32>& v = emitter->mRandom.getVec3();
        ptcl->mVel.x += v.x * random->power.x;
        ptcl->mVel.y += v.y * random->power.y;
        ptcl->mVel.z += v.z * random->power.z;
    }
    return random + 1;
}

// 0x00552DB8 | mk7dlp:bytes [tier B]
void sead::ptcl::EmitterCalc::setupFluctuation_(Heap* heap)
{
    s_pFluctuationTbl = new (heap, 4) f32[128];
    for (s32 i = 0; i < 128; i++) {
        s_pFluctuationTbl[i] = cosf(i * (1.0f / 128) * 2.0f * 3.1415901f) * 0.5f + 0.5f;
    }
}

// 0x00552E40 | mk7dlp:bytes [tier B]
u32* sead::ptcl::EmitterCalc::_childCalc_RotNone(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl)
{
    const Color4f& color = emitter->mEmitterSet->mColor;
    return writeRotNone_(cmd, ptcl->mColor.r * color.r, ptcl->mColor.g * color.g, ptcl->mColor.b * color.b);
}

// 0x00552F98 | mk7dlp
const void* sead::ptcl::EmitterCalc::_ptclField_Collision(EmitterInstance* emitter, PtclInstance* ptcl, const void* data)
{
    const FieldCollisionData* collision = static_cast<const FieldCollisionData*>(data);
    f32 y = collision->y;
    if (collision->isWorld != 0) {
        const Matrix34<f32>& mtx = emitter->mRes->ptclFollowType != 0 ? emitter->mSRT : ptcl->mEmitterSRT;
        Vector3<f32> world;
        Vector3CalcCtr<f32>::mul(toVEC3(world), mtx, toVEC3(ptcl->mPos));
        if (collision->type == 0) {
            if (world.y < y) {
                world.y = y;
                ptcl->mCnt = ptcl->mLife - 1;
            }
        } else if (collision->type == 1) {
            if (world.y < y) {
                world.y = y + 0.0001f;
                Vector3<f32> vel;
                Vector3CalcCtr<f32>::rotate(toVEC3(vel), mtx, toVEC3(ptcl->mVel));
                vel.y = -(vel.y * collision->bounce);
                Matrix34<f32> inv;
                Matrix34CalcCtr<f32>::inverse(inv, mtx);
                Vector3CalcCtr<f32>::mul(toVEC3(ptcl->mPos), inv, toVEC3(world));
                Vector3CalcCtr<f32>::rotate(toVEC3(ptcl->mVel), inv, toVEC3(vel));
            }
        }
    } else {
        if (collision->type == 0) {
            if (ptcl->mPos.y < y) {
                ptcl->mPos.y = y;
                ptcl->mCnt = ptcl->mLife - 1;
            }
        } else if (collision->type == 1) {
            if (ptcl->mPos.y < y) {
                ptcl->mPos.y = y;
                ptcl->mVel.y = -(ptcl->mVel.y * collision->bounce);
            }
        }
    }
    return collision + 1;
}

// 0x005532F8 | mk7dlp
const void* sead::ptcl::EmitterCalc::_ptclField_Convergence(EmitterInstance*, PtclInstance* ptcl, const void* data)
{
    const FieldConvergenceData* convergence = static_cast<const FieldConvergenceData*>(data);
    ptcl->mPos.x += (convergence->pos.x - ptcl->mPos.x) * convergence->ratio;
    ptcl->mPos.y += (convergence->pos.y - ptcl->mPos.y) * convergence->ratio;
    ptcl->mPos.z += (convergence->pos.z - ptcl->mPos.z) * convergence->ratio;
    return convergence + 1;
}

// 0x005523A0 (name is ours)
u32* sead::ptcl::EmitterCalc::_childCalc_RotX(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl)
{
    addRot_(ptcl, 0);
    f32 s;
    f32 c;
    MathCtr<f32>::sinCosIdx(&s, &c, ptcl->mRot[0]);
    const Color4f& color = emitter->mEmitterSet->mColor;
    return writeRotX_(cmd, ptcl->mColor.r * color.r, ptcl->mColor.g * color.g, ptcl->mColor.b * color.b, s, c);
}

// 0x00552440 (name is ours)
u32* sead::ptcl::EmitterCalc::_childCalc_RotY(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl)
{
    addRot_(ptcl, 1);
    f32 s;
    f32 c;
    MathCtr<f32>::sinCosIdx(&s, &c, ptcl->mRot[1]);
    const Color4f& color = emitter->mEmitterSet->mColor;
    return writeRotY_(cmd, ptcl->mColor.r * color.r, ptcl->mColor.g * color.g, ptcl->mColor.b * color.b, s, c);
}

// 0x005524DC (name is ours)
u32* sead::ptcl::EmitterCalc::_childCalc_RotZ(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl)
{
    addRot_(ptcl, 2);
    f32 s;
    f32 c;
    MathCtr<f32>::sinCosIdx(&s, &c, ptcl->mRot[2]);
    const Color4f& color = emitter->mEmitterSet->mColor;
    return writeRotZ_(cmd, ptcl->mColor.r * color.r, ptcl->mColor.g * color.g, ptcl->mColor.b * color.b, s, c);
}

// 0x00553330 | mk7dlp:bytes [tier B]
u32* sead::ptcl::EmitterCalc::_ptclCalc_RotNone_PassColor1(u32* cmd, EmitterInstance* emitter, PtclInstance*)
{
    const Color4f& resColor = emitter->mRes->colors[0];
    const Color4f& color = emitter->mEmitterSet->mColor;
    return writeRotNone_(cmd, resColor.r * color.r, resColor.g * color.g, resColor.b * color.b);
}

// 0x00553398 | mk7dlp:bytes [tier B]
u32* sead::ptcl::EmitterCalc::_ptclCalc_RotXYZ__PassColor1(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl)
{
    const Color4f& resColor = emitter->mRes->colors[0];
    const Color4f& color = emitter->mEmitterSet->mColor;
    return writeRotXYZInline_(cmd, resColor.r * color.r, resColor.g * color.g, resColor.b * color.b, ptcl);
}

// 0x00553510 (name is ours)
u32* sead::ptcl::EmitterCalc::_ptclCalc_RotX___PassColor1(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl)
{
    addRot_(ptcl, 0);
    f32 s;
    f32 c;
    MathCtr<f32>::sinCosIdx(&s, &c, ptcl->mRot[0]);
    const Color4f& resColor = emitter->mRes->colors[0];
    const Color4f& color = emitter->mEmitterSet->mColor;
    return writeRotX_(cmd, resColor.r * color.r, resColor.g * color.g, resColor.b * color.b, s, c);
}

// 0x005535B8 (name is ours)
u32* sead::ptcl::EmitterCalc::_ptclCalc_RotY___PassColor1(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl)
{
    addRot_(ptcl, 1);
    f32 s;
    f32 c;
    MathCtr<f32>::sinCosIdx(&s, &c, ptcl->mRot[1]);
    const Color4f& resColor = emitter->mRes->colors[0];
    const Color4f& color = emitter->mEmitterSet->mColor;
    return writeRotY_(cmd, resColor.r * color.r, resColor.g * color.g, resColor.b * color.b, s, c);
}

// 0x00553660 (name is ours)
u32* sead::ptcl::EmitterCalc::_ptclCalc_RotZ___PassColor1(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl)
{
    addRot_(ptcl, 2);
    f32 s;
    f32 c;
    MathCtr<f32>::sinCosIdx(&s, &c, ptcl->mRot[2]);
    const Color4f& resColor = emitter->mRes->colors[0];
    const Color4f& color = emitter->mEmitterSet->mColor;
    return writeRotZ_(cmd, resColor.r * color.r, resColor.g * color.g, resColor.b * color.b, s, c);
}

// 0x00553708 | mk7dlp:bytes [tier B]
u32* sead::ptcl::EmitterCalc::_ptclCalc_RotNone_ColorNoWork(u32* cmd, EmitterInstance*, PtclInstance*)
{
    return writeRotNone_(cmd, 0.0f, 0.0f, 0.0f);
}

// 0x00553748 | mk7dlp:bytes [tier B]
u32* sead::ptcl::EmitterCalc::_ptclCalc_RotXYZ__ColorNoWork(u32* cmd, EmitterInstance*, PtclInstance* ptcl)
{
    return writeRotXYZInline_(cmd, 0.0f, 0.0f, 0.0f, ptcl);
}

// 0x00553888 (name is ours)
u32* sead::ptcl::EmitterCalc::_ptclCalc_RotX___ColorNoWork(u32* cmd, EmitterInstance*, PtclInstance* ptcl)
{
    addRot_(ptcl, 0);
    f32 s;
    f32 c;
    MathCtr<f32>::sinCosIdx(&s, &c, ptcl->mRot[0]);
    return writeRotX_(cmd, 0.0f, 0.0f, 0.0f, s, c);
}

// 0x00553904 (name is ours)
u32* sead::ptcl::EmitterCalc::_ptclCalc_RotY___ColorNoWork(u32* cmd, EmitterInstance*, PtclInstance* ptcl)
{
    addRot_(ptcl, 1);
    f32 s;
    f32 c;
    MathCtr<f32>::sinCosIdx(&s, &c, ptcl->mRot[1]);
    return writeRotY_(cmd, 0.0f, 0.0f, 0.0f, s, c);
}

// 0x00553980 (name is ours)
u32* sead::ptcl::EmitterCalc::_ptclCalc_RotZ___ColorNoWork(u32* cmd, EmitterInstance*, PtclInstance* ptcl)
{
    addRot_(ptcl, 2);
    f32 s;
    f32 c;
    MathCtr<f32>::sinCosIdx(&s, &c, ptcl->mRot[2]);
    return writeRotZ_(cmd, 0.0f, 0.0f, 0.0f, s, c);
}

namespace {
// the alpha of the particle in this frame
inline f32 stepAlpha_(const PtclInstance* ptcl)
{
    f32 alpha = ptcl->mAlpha;
    if (ptcl->mCnt <= ptcl->mAlphaSection1) {
        alpha += ptcl->mAlphaAdd1;
    } else if (ptcl->mCnt > ptcl->mAlphaSection2) {
        alpha += ptcl->mAlphaAdd2;
    }
    return alpha;
}

// the scale of the particle in this frame
inline void stepScale_(const PtclInstance* ptcl, f32* x, f32* y)
{
    if (ptcl->mCnt <= ptcl->mScaleSection1) {
        *x += ptcl->mScaleAdd1.x;
        *y += ptcl->mScaleAdd1.y;
    } else if (ptcl->mCnt > ptcl->mScaleSection2) {
        *x += ptcl->mScaleAdd2.x;
        *y += ptcl->mScaleAdd2.y;
    }
}

// the gravity in the axes of rt (gravity * rt without the translation)
inline void addGravity_(Vector3<f32>* vel, const Vector3<f32>& g, const Matrix34<f32>& rt)
{
    vel->x += g.x * rt.m[0][0] + g.y * rt.m[1][0] + g.z * rt.m[2][0];
    vel->y += g.x * rt.m[0][1] + g.y * rt.m[1][1] + g.z * rt.m[2][1];
    vel->z += g.x * rt.m[0][2] + g.y * rt.m[1][2] + g.z * rt.m[2][2];
}

// the movement of a frame: the position, the velocity with air resistance and gravity
inline void stepMove_(PtclInstance* ptcl, const SimpleEmitterData* res, const Matrix34<f32>& rt)
{
    Vector3<f32> pos(ptcl->mPos.x + ptcl->mVel.x, ptcl->mPos.y + ptcl->mVel.y, ptcl->mPos.z + ptcl->mVel.z);
    Vector3<f32> vel(res->airRegist * ptcl->mVel.x, res->airRegist * ptcl->mVel.y, res->airRegist * ptcl->mVel.z);
    if (res->isWorldGravity) {
        addGravity_(&vel, res->gravity, rt);
    } else {
        vel.x += res->gravity.x;
        vel.y += res->gravity.y;
        vel.z += res->gravity.z;
    }
    ptcl->mVel = vel;
    ptcl->mPos = pos;
}

// the row of the block with the direction (in the order z, y, x)
inline s32 writeDirection_(u32* cmd, const Matrix34<f32>& srt, const Vector3<f32>& v)
{
    Vector3<f32> dir = v;
    Vector3CalcCtr<f32>::rotate(toVEC3(dir), srt, toVEC3(dir));
    f32* out = reinterpret_cast<f32*>(cmd);
    cmd[27] = 0;
    out[28] = dir.z;
    out[29] = dir.y;
    out[30] = dir.x;
    return 1;
}

// the three rows of the rotation of the emitter (columns z, y, x)
inline s32 writeEmitterRT_(u32* cmd, const Matrix34<f32>& rt)
{
    f32* out = reinterpret_cast<f32*>(cmd);
    cmd[27] = 0;
    cmd[31] = 0;
    cmd[35] = 0;
    out[28] = rt.m[0][2];
    out[29] = rt.m[0][1];
    out[30] = rt.m[0][0];
    out[32] = rt.m[1][2];
    out[33] = rt.m[1][1];
    out[34] = rt.m[1][0];
    out[38] = rt.m[2][0];
    out[36] = rt.m[2][2];
    out[37] = rt.m[2][1];
    return 3;
}

// the fields of a complex emitter in the order of their bits
inline void calcField_(EmitterInstance* emitter, PtclInstance* ptcl, const SimpleEmitterData* res)
{
    if (res->fieldFlag == 0) {
        return;
    }
    const void* data = reinterpret_cast<const u8*>(res) + res->fieldDataOffset;
    if (res->fieldFlag & 1) {
        data = EmitterCalc::_ptclField_Random(emitter, ptcl, data);
    }
    if (res->fieldFlag & 2) {
        data = EmitterCalc::_ptclField_Magnet(emitter, ptcl, data);
    }
    if (res->fieldFlag & 4) {
        data = EmitterCalc::_ptclField_Spin(emitter, ptcl, data);
    }
    if (res->fieldFlag & 8) {
        data = EmitterCalc::_ptclField_Collision(emitter, ptcl, data);
    }
    if (res->fieldFlag & 16) {
        data = EmitterCalc::_ptclField_Convergence(emitter, ptcl, data);
    }
    if (res->fieldFlag & 32) {
        data = EmitterCalc::_ptclField_PosAdd(emitter, ptcl, data);
    }
}

// the movement of a frame of a complex emitter (with the fluctuation and the fields)
inline void stepMoveComplex_(EmitterInstance* emitter, PtclInstance* ptcl, const SimpleEmitterData* res,
                             const Matrix34<f32>& rt)
{
    Vector3<f32> pos(ptcl->mPos.x + ptcl->mVel.x, ptcl->mPos.y + ptcl->mVel.y, ptcl->mPos.z + ptcl->mVel.z);
    Vector3<f32> vel(res->airRegist * ptcl->mVel.x, res->airRegist * ptcl->mVel.y, res->airRegist * ptcl->mVel.z);
    if (res->isWorldGravity) {
        addGravity_(&vel, res->gravity, rt);
    } else {
        vel.x += res->gravity.x;
        vel.y += res->gravity.y;
        vel.z += res->gravity.z;
    }
    ptcl->mPos = pos;
    if (res->isTexPatAnim) {
        _calcTexPatAnim(res, ptcl);
    }
    if (res->fluctuationFlag & 1) {
        EmitterCalc::calcFluctuation(emitter, ptcl);
    }
    ptcl->mVel = vel;
    calcField_(emitter, ptcl, res);
}

// the follow types without the emitter: the translation of the emitter now (FollowPosOnly)
inline void followPos_(PtclInstance* ptcl, const EmitterInstance* emitter)
{
    ptcl->mEmitterSRT.m[0][3] = emitter->mSRT.m[0][3];
    ptcl->mEmitterSRT.m[1][3] = emitter->mSRT.m[1][3];
    ptcl->mEmitterSRT.m[2][3] = emitter->mSRT.m[2][3];
    ptcl->mEmitterRT.m[0][3] = emitter->mRT.m[0][3];
    ptcl->mEmitterRT.m[1][3] = emitter->mRT.m[1][3];
    ptcl->mEmitterRT.m[2][3] = emitter->mRT.m[2][3];
}

// the end of the frame of a simple emitter that keeps the matrices of the emission
inline s32 finishSimple_(u32* cmd, PtclInstance* ptcl, const SimpleEmitterData* res, f32 alpha, f32 scaleX, f32 scaleY)
{
    Vector3CalcCtr<f32>::mul(toVEC3(ptcl->mWorldPos), ptcl->mEmitterSRT, toVEC3(ptcl->mPos));
    stepScale_(ptcl, &scaleX, &scaleY);
    ptcl->mCnt++;
    if (res->isEmitterBillboard) {
        ptcl->mAlpha = alpha;
        ptcl->mScale.x = scaleX;
        ptcl->mScale.y = scaleY;
        return writeEmitterRT_(cmd, ptcl->mEmitterRT);
    }
    if (res->isDirectional) {
        s32 rows = writeDirection_(cmd, ptcl->mEmitterSRT, ptcl->mVel);
        ptcl->mAlpha = alpha;
        ptcl->mScale.x = scaleX;
        ptcl->mScale.y = scaleY;
        return rows;
    }
    ptcl->mAlpha = alpha;
    ptcl->mScale.x = scaleX;
    ptcl->mScale.y = scaleY;
    return 0;
}

// the end of the frame of a complex emitter that keeps the matrices of the emission
inline s32 finishComplex_(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl, const SimpleEmitterData* res,
                          const Vector3<f32>& oldPos, f32 alpha, f32 scaleX, f32 scaleY)
{
    Vector3CalcCtr<f32>::mul(toVEC3(ptcl->mWorldPos), ptcl->mEmitterSRT, toVEC3(ptcl->mPos));
    stepScale_(ptcl, &scaleX, &scaleY);
    ptcl->mAlpha = alpha;
    ptcl->mScale.x = scaleX;
    ptcl->mScale.y = scaleY;
    if (emitter->mRecord->callback != nullptr) {
        emitter->mRecord->callback(emitter, ptcl);
    }
    ptcl->mPosDiff.x = ptcl->mPos.x - oldPos.x;
    ptcl->mPosDiff.y = ptcl->mPos.y - oldPos.y;
    ptcl->mPosDiff.z = ptcl->mPos.z - oldPos.z;
    ptcl->mCnt++;
    if (res->isEmitterBillboard) {
        return writeEmitterRT_(cmd, ptcl->mEmitterRT);
    }
    if (res->isDirectional) {
        return writeDirection_(cmd, ptcl->mEmitterSRT, ptcl->mPosDiff);
    }
    return 0;
}
} // namespace

// 0x00554FBC (name is ours)
void sead::ptcl::EmitterCalc::_emitBox(EmitterInstance* emitter)
{
    SimpleEmitterData* res = emitter->mRes;
    s32 num = getEmitNum_(emitter);
    if (num == 0) {
        return;
    }
    EmitterSet* set = emitter->mEmitterSet;
    f32 figureVel = res->figureVelocity * set->mFigureVelocityScale;
    f32 scaleX = res->volumeScale.x * set->mEmitterVolumeScale.x;
    f32 scaleY = res->volumeScale.y * set->mEmitterVolumeScale.y;
    f32 scaleZ = res->volumeScale.z * set->mEmitterVolumeScale.z;
    for (s32 i = 0; i < num; i++) {
        PtclInstance* ptcl = s_pSystem->allocPtcl();
        if (ptcl == nullptr) {
            continue;
        }
        startPtcl_(ptcl, res);
        u32 face = emitter->mRandom.getU32();
        bool positive = emitter->mRandom.getU32() < 0x7FFFFFFF;
        const Vector3<f32>& random = emitter->mRandom.getVec3();
        if (face < 0x55555555) {
            ptcl->mPos.z = positive ? scaleZ : -scaleZ;
            ptcl->mPos.x = random.x * scaleX;
            ptcl->mPos.y = random.y * scaleY;
        } else if (face < 0xAAAAAAAA) {
            ptcl->mPos.y = positive ? scaleY : -scaleY;
            ptcl->mPos.x = random.x * scaleX;
            ptcl->mPos.z = random.z * scaleZ;
        } else {
            ptcl->mPos.x = positive ? scaleX : -scaleX;
            ptcl->mPos.y = random.y * scaleY;
            ptcl->mPos.z = random.z * scaleZ;
        }
        // (the direction keeps no value for a particle at the center, like the original)
        Vector3<f32> dir;
        f32 length = sqrtf(ptcl->mPos.x * ptcl->mPos.x + ptcl->mPos.y * ptcl->mPos.y + ptcl->mPos.z * ptcl->mPos.z);
        if (length > 0.0f) {
            f32 inv = 1.0f / length;
            dir.y = ptcl->mPos.y * inv;
            dir.z = ptcl->mPos.z * inv;
            dir.x = ptcl->mPos.x * inv;
        }
        ptcl->mVel.x = dir.x * figureVel;
        ptcl->mVel.y = dir.y * figureVel;
        ptcl->mVel.z = dir.z * figureVel;
        initPtcl_(emitter, ptcl);
    }
}

// 0x005551DC (name is ours)
void sead::ptcl::EmitterCalc::_emitLine(EmitterInstance* emitter)
{
    SimpleEmitterData* res = emitter->mRes;
    s32 num = getEmitNum_(emitter);
    if (num == 0) {
        return;
    }
    EmitterSet* set = emitter->mEmitterSet;
    f32 figureVel = res->figureVelocity * set->mFigureVelocityScale;
    f32 scaleZ = res->volumeScale.z * set->mEmitterVolumeScale.z;
    for (s32 i = 0; i < num; i++) {
        PtclInstance* ptcl = s_pSystem->allocPtcl();
        if (ptcl == nullptr) {
            continue;
        }
        startPtcl_(ptcl, res);
        ptcl->mVel.z = figureVel;
        ptcl->mPos.x = 0.0f;
        ptcl->mPos.y = 0.0f;
        ptcl->mVel.x = 0.0f;
        ptcl->mVel.y = 0.0f;
        ptcl->mPos.z = emitter->mRandom.getF32() * scaleZ;
        initPtcl_(emitter, ptcl);
    }
}

// 0x005552C8 (name is ours)
const sead::Matrix34<f32>* sead::ptcl::EmitterCalc::getPtclSRT(EmitterInstance* emitter, PtclInstance* ptcl)
{
    return emitter->mRes->ptclFollowType != 0 ? &emitter->mSRT : &ptcl->mEmitterSRT;
}

// 0x005539FC | mk7dlp:bytes [tier B]
s32 sead::ptcl::EmitterCalc::_ptclPreCalc_FollowAll_____Simple(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl)
{
    SimpleEmitterData* res = emitter->mRes;
    f32 scaleX = ptcl->mScale.x;
    f32 scaleY = ptcl->mScale.y;
    f32 alpha = stepAlpha_(ptcl);
    stepMove_(ptcl, res, emitter->mRT);
    if (res->isTexPatAnim) {
        _calcTexPatAnim(res, ptcl);
    }
    Vector3CalcCtr<f32>::mul(toVEC3(ptcl->mWorldPos), emitter->mSRT, toVEC3(ptcl->mPos));
    stepScale_(ptcl, &scaleX, &scaleY);
    ptcl->mCnt++;
    if (res->isDirectional) {
        s32 rows = writeDirection_(cmd, emitter->mSRT, ptcl->mVel);
        ptcl->mAlpha = alpha;
        ptcl->mScale.x = scaleX;
        ptcl->mScale.y = scaleY;
        return rows;
    }
    ptcl->mAlpha = alpha;
    ptcl->mScale.x = scaleX;
    ptcl->mScale.y = scaleY;
    return 0;
}

// 0x00553C08 | mk7dlp:bytes [tier B]
s32 sead::ptcl::EmitterCalc::_ptclPreCalc_FollowNone____Simple(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl)
{
    SimpleEmitterData* res = emitter->mRes;
    f32 scaleX = ptcl->mScale.x;
    f32 scaleY = ptcl->mScale.y;
    f32 alpha = stepAlpha_(ptcl);
    stepMove_(ptcl, res, ptcl->mEmitterRT);
    if (res->isTexPatAnim) {
        _calcTexPatAnim(res, ptcl);
    }
    return finishSimple_(cmd, ptcl, res, alpha, scaleX, scaleY);
}

// 0x00553E98 | mk7dlp:bytes [tier B]
s32 sead::ptcl::EmitterCalc::_ptclPreCalc_FollowPosOnly_Simple(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl)
{
    SimpleEmitterData* res = emitter->mRes;
    f32 scaleX = ptcl->mScale.x;
    f32 scaleY = ptcl->mScale.y;
    f32 alpha = stepAlpha_(ptcl);
    followPos_(ptcl, emitter);
    stepMove_(ptcl, res, ptcl->mEmitterRT);
    if (res->isTexPatAnim) {
        _calcTexPatAnim(res, ptcl);
    }
    return finishSimple_(cmd, ptcl, res, alpha, scaleX, scaleY);
}

// 0x00554160 | mk7dlp:bytes [tier B]
s32 sead::ptcl::EmitterCalc::_ptclPreCalc_FollowAll_____Complex(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl)
{
    SimpleEmitterData* res = emitter->mRes;
    Vector3<f32> oldPos = ptcl->mPos;
    f32 scaleX = ptcl->mScale.x;
    f32 scaleY = ptcl->mScale.y;
    f32 alpha = stepAlpha_(ptcl);
    stepMoveComplex_(emitter, ptcl, res, emitter->mRT);
    Vector3CalcCtr<f32>::mul(toVEC3(ptcl->mWorldPos), emitter->mSRT, toVEC3(ptcl->mPos));
    stepScale_(ptcl, &scaleX, &scaleY);
    ptcl->mAlpha = alpha;
    ptcl->mScale.x = scaleX;
    ptcl->mScale.y = scaleY;
    if (emitter->mRecord->callback != nullptr) {
        emitter->mRecord->callback(emitter, ptcl);
    }
    ptcl->mPosDiff.x = ptcl->mPos.x - oldPos.x;
    ptcl->mPosDiff.y = ptcl->mPos.y - oldPos.y;
    ptcl->mPosDiff.z = ptcl->mPos.z - oldPos.z;
    ptcl->mCnt++;
    if (res->isDirectional) {
        return writeDirection_(cmd, emitter->mSRT, ptcl->mPosDiff);
    }
    return 0;
}

// 0x00554470 | mk7dlp:bytes [tier B]
s32 sead::ptcl::EmitterCalc::_ptclPreCalc_FollowNone____Complex(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl)
{
    SimpleEmitterData* res = emitter->mRes;
    Vector3<f32> oldPos = ptcl->mPos;
    f32 scaleX = ptcl->mScale.x;
    f32 scaleY = ptcl->mScale.y;
    f32 alpha = stepAlpha_(ptcl);
    stepMoveComplex_(emitter, ptcl, res, ptcl->mEmitterRT);
    return finishComplex_(cmd, emitter, ptcl, res, oldPos, alpha, scaleX, scaleY);
}

// 0x005547EC | mk7dlp:bytes [tier B]
s32 sead::ptcl::EmitterCalc::_ptclPreCalc_FollowPosOnly_Complex(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl)
{
    SimpleEmitterData* res = emitter->mRes;
    Vector3<f32> oldPos = ptcl->mPos;
    f32 scaleX = ptcl->mScale.x;
    f32 scaleY = ptcl->mScale.y;
    f32 alpha = stepAlpha_(ptcl);
    followPos_(ptcl, emitter);
    stepMoveComplex_(emitter, ptcl, res, ptcl->mEmitterRT);
    return finishComplex_(cmd, emitter, ptcl, res, oldPos, alpha, scaleX, scaleY);
}

// 0x00554BA0 | mk7dlp:bytes [tier B]
u32* sead::ptcl::EmitterCalc::_ptclCalc_RotNone_Interpolate3Colors(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl)
{
    _interpolate3Colors(emitter->mRes, ptcl);
    const Color4f& color = emitter->mEmitterSet->mColor;
    return writeRotNone_(cmd, ptcl->mColor.r * color.r, ptcl->mColor.g * color.g, ptcl->mColor.b * color.b);
}

// 0x00554C18 | mk7dlp:bytes [tier B]
u32* sead::ptcl::EmitterCalc::_ptclCalc_RotXYZ__Interpolate3Colors(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl)
{
    u32 x = ptcl->mRot[0] + ptcl->mRotVel[0];
    u32 z = ptcl->mRot[2] + ptcl->mRotVel[2];
    u32 y = ptcl->mRot[1] + ptcl->mRotVel[1];
    f32 sx = MathCommon<f32>::sinIdx(x);
    f32 cx = MathCommon<f32>::cosIdx(x);
    ptcl->mRot[0] = x;
    f32 sy = MathCommon<f32>::sinIdx(y);
    f32 cy = MathCommon<f32>::cosIdx(y);
    ptcl->mRot[1] = y;
    f32 sz = MathCommon<f32>::sinIdx(z);
    f32 cz = MathCommon<f32>::cosIdx(z);
    ptcl->mRot[2] = z;
    _interpolate3Colors(emitter->mRes, ptcl);
    const Color4f& color = emitter->mEmitterSet->mColor;
    return writeRotXYZ_(cmd, ptcl->mColor.r * color.r, ptcl->mColor.g * color.g, ptcl->mColor.b * color.b, sx, cx, sy, cy,
                        sz, cz);
}

// 0x00554DA0 (name is ours)
u32* sead::ptcl::EmitterCalc::_ptclCalc_RotX___Interpolate3Colors(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl)
{
    addRot_(ptcl, 0);
    f32 s;
    f32 c;
    MathCtr<f32>::sinCosIdx(&s, &c, ptcl->mRot[0]);
    _interpolate3Colors(emitter->mRes, ptcl);
    const Color4f& color = emitter->mEmitterSet->mColor;
    return writeRotX_(cmd, ptcl->mColor.r * color.r, ptcl->mColor.g * color.g, ptcl->mColor.b * color.b, s, c);
}

// 0x00554E54 (name is ours)
u32* sead::ptcl::EmitterCalc::_ptclCalc_RotY___Interpolate3Colors(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl)
{
    addRot_(ptcl, 1);
    f32 s;
    f32 c;
    MathCtr<f32>::sinCosIdx(&s, &c, ptcl->mRot[1]);
    _interpolate3Colors(emitter->mRes, ptcl);
    const Color4f& color = emitter->mEmitterSet->mColor;
    return writeRotY_(cmd, ptcl->mColor.r * color.r, ptcl->mColor.g * color.g, ptcl->mColor.b * color.b, s, c);
}

// 0x00554F08 (name is ours)
u32* sead::ptcl::EmitterCalc::_ptclCalc_RotZ___Interpolate3Colors(u32* cmd, EmitterInstance* emitter, PtclInstance* ptcl)
{
    addRot_(ptcl, 2);
    f32 s;
    f32 c;
    MathCtr<f32>::sinCosIdx(&s, &c, ptcl->mRot[2]);
    _interpolate3Colors(emitter->mRes, ptcl);
    const Color4f& color = emitter->mEmitterSet->mColor;
    return writeRotZ_(cmd, ptcl->mColor.r * color.r, ptcl->mColor.g * color.g, ptcl->mColor.b * color.b, s, c);
}

// 0x00982D20
EmitterCalc::PtclCalcFunction EmitterCalc::s_ChildCalcFunctions[5] = {
    _childCalc_RotNone, _childCalc_RotX, _childCalc_RotY, _childCalc_RotZ, _childCalc_RotXYZ,
};
// 0x00982D34
EmitterCalc::PtclCalcFunction EmitterCalc::s_PtclCalcFunctions[15] = {
    _ptclCalc_RotNone_ColorNoWork,        _ptclCalc_RotX___ColorNoWork,         _ptclCalc_RotY___ColorNoWork,
    _ptclCalc_RotZ___ColorNoWork,         _ptclCalc_RotXYZ__ColorNoWork,        _ptclCalc_RotNone_PassColor1,
    _ptclCalc_RotX___PassColor1,          _ptclCalc_RotY___PassColor1,          _ptclCalc_RotZ___PassColor1,
    _ptclCalc_RotXYZ__PassColor1,         _ptclCalc_RotNone_Interpolate3Colors, _ptclCalc_RotX___Interpolate3Colors,
    _ptclCalc_RotY___Interpolate3Colors,  _ptclCalc_RotZ___Interpolate3Colors, _ptclCalc_RotXYZ__Interpolate3Colors,
};
// 0x00982D70
EmitterCalc::PreCalcFunction EmitterCalc::s_PreCalcFunctions[6] = {
    _ptclPreCalc_FollowAll_____Simple,  _ptclPreCalc_FollowNone____Simple,  _ptclPreCalc_FollowPosOnly_Simple,
    _ptclPreCalc_FollowAll_____Complex, _ptclPreCalc_FollowNone____Complex, _ptclPreCalc_FollowPosOnly_Complex,
};

// the emit functions by SimpleEmitterData::volumeType
// 0x0097F804
EmitterCalc::EmitFunction EmitterCalc::s_EmitFunctions[14] = {
    _emitPoint,  _emitCircle,  _emitCircleSameDivide, _emitFillCircle,       _emitSphere, _emitSphereSameDivide,
    _emitFillSphere, _emitCylinder, _emitFillCylinder, _emitBox, _emitFillBox, _emitLine, _emitLineSameDivide,
    _emitRectangle,
};
} // namespace ptcl
} // namespace sead
