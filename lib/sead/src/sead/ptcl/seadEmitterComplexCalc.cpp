#include "sead/ptcl/seadEmitterComplexCalc.h"
#include "sead/ptcl/seadEmitterData.h"
#include "sead/ptcl/seadEmitterInstance.h"
#include "sead/ptcl/seadEmitterSet.h"
#include "sead/ptcl/seadPtclInstance.h"
#include "sead/ptcl/seadPtclRenderer.h"
#include "sead/ptcl/seadPtclStripe.h"
#include "sead/ptcl/seadPtclSystem.h"
#include "sead/seadMathCalcCommon.h"

namespace sead {
namespace ptcl {
namespace {
// the draw state by blend type and by depth/alpha test type (each source file of the original has its own
// copy of these tables; names are ours)
// 0x0097F718
u32 s_ComplexBlendFuncTbl[4] = {0x76760000, 0x16160000, 0x16160202, 0x01010200};
// 0x0097F728
u32 s_ComplexDepthFuncTbl[3] = {0x00000F41, 0x00000F10, 0x00001F41};
// 0x0097F734
u32 s_ComplexAlphaTestTbl[3] = {0x00000061, 0x00000061, 0x00008061};
// 0x0097F740
u32 s_ComplexDepthReadTbl[3] = {2, 0, 2};
// 0x0097F74C
u32 s_ComplexDepthWriteTbl[3] = {0, 0, 2};
// 0x0097F758
u32 s_ComplexColorReadTbl[3] = {0xF, 0xF, 0};

// the bits of SimpleEmitterData::childFlag (names are ours)
enum ChildFlag
{
    CHILD_FLAG_ENABLE = 0x1,               // the particles emit children
    CHILD_FLAG_INHERIT_COLOR = 0x2,        // the color of the parent
    CHILD_FLAG_INHERIT_ALPHA = 0x4,        // the alpha of the parent
    CHILD_FLAG_INHERIT_SCALE = 0x8,        // the scale of the parent
    CHILD_FLAG_INHERIT_ROT = 0x10,         // the rotation of the parent
    CHILD_FLAG_INHERIT_VEL = 0x20,         // the movement of the parent
    CHILD_FLAG_FOLLOW_EMITTER = 0x40,      // the children take the matrices of the emitter each frame
    CHILD_FLAG_DRAW_PARENT = 0x80,         // the parents are drawn too
    CHILD_FLAG_WORLD_GRAVITY = 0x100,      // the gravity is in the space of the emitter
    CHILD_FLAG_EMITTER_BILLBOARD = 0x200,  // the children turn with the emitter
    CHILD_FLAG_ROTATE_EMITTER = 0x400,     // the billboard block has the rotation of the emitter
    CHILD_FLAG_FIELD = 0x800,              // the fields move the children too
    CHILD_FLAG_DRAW_BEFORE_PARENT = 0x1000,
};

// the gravity in the axes of rt (gravity * rt without the translation)
inline void addGravity_(Vector3<f32>* vel, const Vector3<f32>& g, const Matrix34<f32>& rt)
{
    vel->x += g.x * rt.m[0][0] + g.y * rt.m[1][0] + g.z * rt.m[2][0];
    vel->y += g.x * rt.m[0][1] + g.y * rt.m[1][1] + g.z * rt.m[2][1];
    vel->z += g.x * rt.m[0][2] + g.y * rt.m[1][2] + g.z * rt.m[2][2];
}

// the drawing of numIndex indices (six per quad)
inline u32* drawQuads_(u32* cmd, u32 numIndex)
{
    cmd[0] = numIndex;
    cmd[1] = 0xF0228;
    cmd[2] = 0;
    cmd[3] = 0xF022F;
    cmd[4] = 1;
    cmd[5] = 0xF0231;
    cmd[6] = 0;
    cmd[7] = 0x8025E;
    cmd[8] = 0;
    cmd[9] = 0x8025E;
    return cmd + 10;
}

inline const StripeData* getStripeData_(const SimpleEmitterData* res)
{
    return reinterpret_cast<const StripeData*>(reinterpret_cast<const u8*>(res) + res->stripeDataOffset);
}
} // namespace

// 0x00558990 | mk7dlp:bytes-fuzzy [tier B]
u32* sead::ptcl::EmitterComplexCalc::calcChild_(u32* cmd, EmitterInstance* emitter, s32* numPtcl)
{
    SimpleEmitterData* res = emitter->mRes;
    EmitterSet* set = emitter->mEmitterSet;
    const ChildData& child = res->child;
    cmd = childDrawSetting_(cmd, emitter);
    s32 count = 0;
    s32 quad = 0;
    PtclInstance* ptcl = emitter->mChildHead;
    while (ptcl != nullptr) {
        if (ptcl->mCnt >= ptcl->mLife) {
            // the child dies
            ptcl->mRes = nullptr;
            if (emitter->mChildHead == ptcl) {
                emitter->mChildHead = ptcl->mNext;
                if (ptcl->mNext != nullptr) {
                    ptcl->mNext->mPrev = nullptr;
                }
            } else {
                if (ptcl->mNext != nullptr) {
                    ptcl->mNext->mPrev = ptcl->mPrev;
                }
                ptcl->mPrev->mNext = ptcl->mNext;
            }
            ptcl = ptcl->mNext;
            continue;
        }

        if (res->childFlag & CHILD_FLAG_FOLLOW_EMITTER) {
            ptcl->mEmitterSRT = emitter->mSRT;
            ptcl->mEmitterRT = emitter->mRT;
        }

        // the movement
        Vector3<f32> prevPos = ptcl->mPos;
        if (res->childFlag & CHILD_FLAG_WORLD_GRAVITY) {
            addGravity_(&ptcl->mVel, child.gravity, ptcl->mEmitterRT);
        } else {
            ptcl->mVel.x += child.gravity.x;
            ptcl->mVel.y += child.gravity.y;
            ptcl->mVel.z += child.gravity.z;
        }
        Vector3CalcCtr<f32>::multScalar(toVEC3(ptcl->mVel), toVEC3(ptcl->mVel), child.airRegist);
        Vector3CalcCtr<f32>::add(toVEC3(ptcl->mPos), toVEC3(ptcl->mPos), toVEC3(ptcl->mVel));
        if (res->childFlag & CHILD_FLAG_FIELD) {
            const void* data = reinterpret_cast<const u8*>(res) + res->fieldDataOffset;
            if (res->fieldFlag & 1) {
                data = _ptclField_Random(emitter, ptcl, data);
            }
            if (res->fieldFlag & 2) {
                data = _ptclField_Magnet(emitter, ptcl, data);
            }
            if (res->fieldFlag & 4) {
                data = _ptclField_Spin(emitter, ptcl, data);
            }
            if (res->fieldFlag & 8) {
                data = _ptclField_Collision(emitter, ptcl, data);
            }
            if (res->fieldFlag & 16) {
                data = _ptclField_Convergence(emitter, ptcl, data);
            }
            if (res->fieldFlag & 32) {
                _ptclField_PosAdd(emitter, ptcl, data);
            }
        }
        Vector3CalcCtr<f32>::mul(toVEC3(ptcl->mWorldPos), ptcl->mEmitterSRT, toVEC3(ptcl->mPos));

        // the alpha and the scale
        if (ptcl->mCnt < child.alphaSection1) {
            ptcl->mAlpha += ptcl->mAlphaAdd1;
        } else if (ptcl->mCnt >= child.alphaSection2) {
            ptcl->mAlpha += ptcl->mAlphaAdd2;
        }
        if (ptcl->mCnt >= child.scaleSection) {
            ptcl->mScale.x += ptcl->mScaleAdd1.x;
            ptcl->mScale.y += ptcl->mScaleAdd1.y;
        }
        if (emitter->mRecord->childCallback != nullptr) {
            emitter->mRecord->childCallback(emitter, ptcl);
        }
        ptcl->mCnt++;

        // the block of the child (as the particles of EmitterSimpleCalc::calcCTR)
        f32 alpha = ptcl->mAlpha * set->mColor.a;
        f32 scaleX = ptcl->mScale.x * set->mPtclScaleXY.x;
        f32 scaleY = ptcl->mScale.y * set->mPtclScaleXY.y;
        alpha = alpha * emitter->mFadeAlpha;
        f32* out = reinterpret_cast<f32*>(cmd);
        cmd[0] = 0x80000000 | (quad * 9 + 8);
        cmd[1] = 0xF02C0;
        out[4] = ptcl->mWorldPos.z;
        out[5] = ptcl->mWorldPos.y;
        out[6] = ptcl->mWorldPos.x;
        out[7] = scaleY;
        out[8] = scaleX;
        out[9] = alpha;
        out[11] = ptcl->mTexScale.y;
        out[12] = ptcl->mTexScale.x;
        out[13] = ptcl->mTexOffset.y;
        out[14] = ptcl->mTexOffset.x;
        s_ChildCalcFunctions[child.ptclCalcType](cmd + 15, emitter, ptcl);
        if (res->childFlag & CHILD_FLAG_EMITTER_BILLBOARD) {
            // the three rows of the rotation of the emitter (columns z, y, x)
            const Matrix34<f32>& rt = ptcl->mEmitterRT;
            cmd[27] = 0;
            out[28] = rt.m[0][2];
            out[29] = rt.m[0][1];
            out[30] = rt.m[0][0];
            cmd[31] = 0;
            out[32] = rt.m[1][2];
            out[33] = rt.m[1][1];
            out[34] = rt.m[1][0];
            cmd[35] = 0;
            out[36] = rt.m[2][2];
            out[37] = rt.m[2][1];
            out[38] = rt.m[2][0];
            out[2] = s_QuadBlockIndex[quad];
            cmd[3] = 0x23F02C1;
            cmd += 40;
        } else if (child.billboardType == 3 || child.billboardType == 4) {
            // the direction of the movement of this frame
            // 0x00AF4EE8 (guard 0x0097F714)
            static Vector3<f32> s_ChildDir;
            s_ChildDir.x = ptcl->mPos.x - prevPos.x;
            s_ChildDir.y = ptcl->mPos.y - prevPos.y;
            s_ChildDir.z = ptcl->mPos.z - prevPos.z;
            Vector3CalcCtr<f32>::rotate(toVEC3(s_ChildDir), ptcl->mEmitterSRT, toVEC3(s_ChildDir));
            cmd[27] = 0;
            out[28] = s_ChildDir.z;
            out[29] = s_ChildDir.y;
            out[30] = s_ChildDir.x;
            out[2] = s_QuadBlockIndex[quad];
            cmd[3] = 0x1BF02C1;
            cmd += 32;
        } else {
            out[2] = s_BillboardBlockIndex[child.billboardType];
            cmd[3] = 0x17F02C1;
            cmd += 28;
        }

        quad++;
        if (quad == 8) {
            cmd = drawQuads_(cmd, 48);
            quad = 0;
        }
        count++;
        ptcl = ptcl->mNext;
    }
    if (quad != 0) {
        cmd = drawQuads_(cmd, quad * 6);
    }
    *numPtcl = count;
    return cmd;
}

// 0x00558F84 | mk7dlp
void sead::ptcl::EmitterComplexCalc::calcComplex_(EmitterInstance* emitter, PtclInstance* ptcl)
{
    s32 frame = ptcl->mCnt - 1;
    SimpleEmitterData* res = emitter->mRes;
    const ChildData& child = res->child;

    // the stripe: a new entry of its history at the particle
    if (res->billboardType == 5) {
        const StripeData* stripeData = getStripeData_(res);
        if (frame == 0) {
            ptcl->mStripe = s_pSystem->allocAndConnectStripe(emitter, ptcl);
        }
        PtclStripe* stripe = ptcl->mStripe;
        if (stripe != nullptr) {
            StripeHistory* hist = &stripe->mHistory[stripe->mHistoryPos];
            if (res->stripeFlag & 1) {
                hist->pos = ptcl->mPos;
                hist->mtx = Matrix34<f32>::ident;
            } else {
                hist->pos = ptcl->mWorldPos;
                hist->mtx = emitter->mSRT;
            }
            hist->scale = ptcl->mScale.x * emitter->mEmitterSet->mPtclScaleXY.x;
            if (stripe->mHistoryPos != stripe->mHistoryStart) {
                s32 prevIx = stripe->mHistoryPos - 1;
                if (prevIx < 0) {
                    prevIx = stripeData->numHistoryMax - 1;
                }
                StripeHistory* prev = &stripe->mHistory[prevIx];
                if (frame < 2) {
                    Vector3<f32> dir;
                    Vector3CalcCtr<f32>::sub(toVEC3(dir), toVEC3(hist->pos), toVEC3(prev->pos));
                    stripe->mDir = dir;
                    Vector3CalcCtr<f32>::normalize(toVEC3(stripe->mDir));
                } else {
                    // the direction turns toward the movement
                    Vector3<f32> dir;
                    Vector3CalcCtr<f32>::sub(toVEC3(dir), toVEC3(hist->pos), toVEC3(prev->pos));
                    Vector3<f32> move = dir;
                    Vector3CalcCtr<f32>::normalize(toVEC3(move));
                    Vector3CalcCtr<f32>::sub(toVEC3(dir), toVEC3(move), toVEC3(stripe->mDir));
                    Vector3<f32> diff = dir;
                    Vector3CalcCtr<f32>::multScalar(toVEC3(dir), toVEC3(diff), stripeData->dirInterpolate);
                    Vector3<f32> add = dir;
                    Vector3CalcCtr<f32>::add(toVEC3(stripe->mDir), toVEC3(stripe->mDir), toVEC3(add));
                    Vector3CalcCtr<f32>::normalize(toVEC3(stripe->mDir));
                }
                if (stripeData->type == 2) {
                    hist->dir.x = hist->mtx.m[0][1] * prev->scale;
                    hist->dir.y = hist->mtx.m[1][1] * prev->scale;
                    hist->dir.z = hist->mtx.m[2][1] * prev->scale;
                } else if (stripeData->type == 1) {
                    Vector3<f32> axisY(hist->mtx.m[0][1], hist->mtx.m[1][1], hist->mtx.m[2][1]);
                    Vector3<f32> dir = stripe->mDir;
                    Vector3<f32> cross;
                    Vector3CalcCtr<f32>::cross(toVEC3(cross), toVEC3(axisY), toVEC3(dir));
                    Vector3CalcCtr<f32>::normalize(toVEC3(cross));
                    Vector3CalcCtr<f32>::multScalar(toVEC3(cross), toVEC3(cross), prev->scale);
                    hist->dir = cross;
                } else {
                    hist->dir = stripe->mDir;
                }
            }

            stripe->mHistoryPos++;
            if (stripe->mHistoryPos >= stripeData->numHistoryMax) {
                stripe->mHistoryPos = 0;
            }
            if (stripe->mHistoryPos == stripe->mHistoryStart) {
                stripe->mHistoryStart++;
                if (stripe->mHistoryStart >= stripeData->numHistoryMax) {
                    stripe->mHistoryStart = 0;
                }
            }
            stripe->mNumHistory++;
            if (stripe->mNumHistory > stripeData->numHistoryMax) {
                stripe->mNumHistory = stripeData->numHistoryMax;
            }
            stripe->mPtclTexOffsetU = ptcl->mTexOffset.x;
            stripe->mPtclTexOffsetV = ptcl->mTexOffset.y;
            stripe->mPtclTexScaleU = ptcl->mTexScale.x;
            stripe->mPtclTexScaleV = ptcl->mTexScale.y;
            const Color4f& color = emitter->mEmitterSet->mColor;
            stripe->mColor.r = color.r;
            stripe->mColor.g = color.g;
            stripe->mColor.b = color.b;
            stripe->mColor.a = ptcl->mAlpha * color.a * emitter->mFadeAlpha;
            stripe->mEmitterMtx = emitter->mSRT;
        }
    }

    // the children
    if (!(res->childFlag & CHILD_FLAG_ENABLE)) {
        return;
    }
    if (frame == 0) {
        ptcl->mChildEmitCnt = 0x7FFFFFFF;
    }
    if (frame < (ptcl->mLife - 1) * child.emitStartPercent / 100) {
        return;
    }
    if (child.emitInterval > ptcl->mChildEmitCnt) {
        ptcl->mChildEmitCnt++;
        return;
    }
    ptcl->mChildEmitCnt = 0;
    for (s32 i = 0; i < child.emitRate; i++) {
        PtclInstance* c = s_pSystem->allocPtcl();
        if (c == nullptr) {
            continue;
        }
        c->mRes = res;
        c->mStripe = nullptr;
        s32 life = child.life;
        c->mLife = life;
        if (res->childFlag & CHILD_FLAG_INHERIT_VEL) {
            c->mVel.x = ptcl->mPosDiff.x * child.velInheritRate;
            c->mVel.y = ptcl->mPosDiff.y * child.velInheritRate;
            c->mVel.z = ptcl->mPosDiff.z * child.velInheritRate;
        } else {
            c->mVel = Vector3<f32>::zero;
        }
        f32 alpha;
        if (res->childFlag & CHILD_FLAG_INHERIT_ALPHA) {
            alpha = ptcl->mAlpha * ptcl->mFluctuationAlpha;
        } else {
            alpha = child.alpha;
        }
        if (res->childFlag & CHILD_FLAG_INHERIT_SCALE) {
            c->mScale.x = ptcl->mScale.x * child.scaleInheritRate * ptcl->mFluctuationScale;
            c->mScale.y = ptcl->mScale.y * child.scaleInheritRate * ptcl->mFluctuationScale;
        } else {
            c->mScale = child.scale;
        }
        if (res->childFlag & CHILD_FLAG_INHERIT_ROT) {
            c->mRot[0] = ptcl->mRot[0];
            c->mRot[1] = ptcl->mRot[1];
            c->mRot[2] = ptcl->mRot[2];
        } else {
            c->mRot[0] = child.rotInit[0] + (emitter->mRandom.getU32() >> 24) * child.rotInitRandom[0];
            c->mRot[1] = child.rotInit[1] + (emitter->mRandom.getU32() >> 24) * child.rotInitRandom[1];
            c->mRot[2] = child.rotInit[2] + (emitter->mRandom.getU32() >> 24) * child.rotInitRandom[2];
        }
        if (res->childFlag & CHILD_FLAG_INHERIT_COLOR) {
            c->mColor = ptcl->mColor;
        } else {
            c->mColor = child.color;
        }
        c->mRotVel[0] = child.rotVel[0] + (emitter->mRandom.getU32() >> 24) * child.rotVelRandom[0];
        c->mRotVel[1] = child.rotVel[1] + (emitter->mRandom.getU32() >> 24) * child.rotVelRandom[1];
        c->mRotVel[2] = child.rotVel[2] + (emitter->mRandom.getU32() >> 24) * child.rotVelRandom[2];

        // the alpha: from alphaInit to alpha in the first section, to alphaTarget after the second
        if (child.alphaSection1 == 0) {
            c->mAlpha = alpha;
            c->mAlphaAdd1 = 0.0f;
        } else {
            c->mAlpha = child.alphaInit;
            c->mAlphaAdd1 = (alpha - child.alphaInit) / static_cast<f32>(child.alphaSection1);
        }
        c->mAlphaAdd2 = (child.alphaTarget - alpha) * (1.0f / static_cast<f32>(life - child.alphaSection2));
        c->mTexOffset.x = 0.0f;
        c->mTexOffset.y = 0.0f;
        c->mTexScale = child.texScale;
        f32 scaleRate = 1.0f / static_cast<f32>(life - child.scaleSection);
        c->mScaleAdd1.x = c->mScale.x * ((child.scaleTarget.x - 1.0f) * scaleRate);
        c->mScaleAdd1.y = c->mScale.y * ((child.scaleTarget.y - 1.0f) * scaleRate);

        // the position around the parent and the velocity
        const Vector3<f32>& dir = emitter->mRandom.getNormalizedVec3();
        const Vector3<f32>& rnd = emitter->mRandom.getVec3();
        c->mRandom = emitter->mRandom.getU32();
        c->mCnt = 0;
        c->mVel.x += dir.x * child.figureVelocity + rnd.x * child.randomVelocity.x;
        c->mVel.y += dir.y * child.figureVelocity + rnd.y * child.randomVelocity.y;
        c->mVel.z += dir.z * child.figureVelocity + rnd.z * child.randomVelocity.z;
        c->mPos.x = ptcl->mPos.x + dir.x * child.initPosRandom;
        c->mPos.y = ptcl->mPos.y + dir.y * child.initPosRandom;
        c->mPos.z = ptcl->mPos.z + dir.z * child.initPosRandom;
        if (!(res->childFlag & CHILD_FLAG_FOLLOW_EMITTER)) {
            if (res->ptclFollowType != 0) {
                c->mEmitterRT = emitter->mRT;
                c->mEmitterSRT = emitter->mSRT;
            } else {
                c->mEmitterRT = ptcl->mEmitterRT;
                c->mEmitterSRT = ptcl->mEmitterSRT;
            }
        }

        // in front of the list of the children
        PtclInstance* head = emitter->mChildHead;
        if (head != nullptr) {
            head->mPrev = c;
            c->mNext = head;
        }
        emitter->mChildHead = c;
        if (head == nullptr) {
            c->mNext = nullptr;
        }
        c->mPrev = nullptr;
    }
}

// 0x00559850 | mk7dlp
u32* sead::ptcl::EmitterComplexCalc::calcEmitter_(u32* cmd, EmitterInstance* emitter, s32* numPtcl)
{
    EmitterSet* set = emitter->mEmitterSet;
    SimpleEmitterData* res = emitter->mRes;
    cmd = emitterDrawSetting_(cmd, emitter);
    if (emitter->mRecord->emitterCallback != nullptr) {
        emitter->mRecord->emitterCallback(emitter);
    }
    // the stripes draw their particles themselves
    bool isDraw = (res->childFlag & CHILD_FLAG_DRAW_PARENT) && res->billboardType != 5;
    s32 count = 0;
    s32 quad = 0;
    PtclInstance* ptcl = emitter->mPtclHead;
    while (ptcl != nullptr) {
        count++;
        if (ptcl->mCnt >= ptcl->mLife) {
            PtclStripe* stripe = ptcl->mStripe;
            if (stripe != nullptr) {
                const StripeData* stripeData = getStripeData_(res);
                if (stripe->mHistoryStart != stripe->mHistoryPos) {
                    // the stripe shrinks to the dead particle first
                    stripe->mHistoryStart++;
                    if (stripeData->numHistoryMax <= stripe->mHistoryStart) {
                        stripe->mHistoryStart = 0;
                    }
                    stripe->mNumHistory--;
                    stripe->mColor.r = emitter->mEmitterSet->mColor.r;
                    stripe->mColor.g = emitter->mEmitterSet->mColor.g;
                    stripe->mColor.b = emitter->mEmitterSet->mColor.b;
                    stripe->mColor.a = ptcl->mAlpha * emitter->mEmitterSet->mColor.a;
                    stripe->mEmitterMtx = emitter->mSRT;
                    ptcl = ptcl->mNext;
                    continue;
                }
                s_pSystem->removeStripe(stripe);
            }
            // the particle dies
            ptcl->mRes = nullptr;
            if (emitter->mPtclHead == ptcl) {
                emitter->mPtclHead = ptcl->mNext;
                if (ptcl->mNext != nullptr) {
                    ptcl->mNext->mPrev = nullptr;
                }
            } else {
                if (ptcl->mNext != nullptr) {
                    ptcl->mNext->mPrev = ptcl->mPrev;
                }
                if (ptcl->mPrev != nullptr) {
                    ptcl->mPrev->mNext = ptcl->mNext;
                }
            }
            ptcl = ptcl->mNext;
            continue;
        }

        if (ptcl->mCnt == 0) {
            ptcl->mFluctuationAlpha = 1.0f;
            ptcl->mFluctuationScale = 1.0f;
        }
        cmd[0] = 0x80000000 | (quad * 9 + 8);
        cmd[1] = 0xF02C0;
        s32 rows = s_PreCalcFunctions[res->preCalcType + 3](cmd, emitter, ptcl);
        if (isDraw) {
            // the block of the particle (as in EmitterSimpleCalc::calcCTR, with the fluctuation)
            f32 alpha = ptcl->mAlpha * set->mColor.a * (ptcl->mFluctuationAlpha * emitter->mFadeAlpha);
            f32 scaleX = ptcl->mScale.x * set->mPtclScaleXY.x * ptcl->mFluctuationScale;
            f32 scaleY = ptcl->mScale.y * set->mPtclScaleXY.y * ptcl->mFluctuationScale;
            f32* out = reinterpret_cast<f32*>(cmd);
            if (rows > 0) {
                out[2] = s_QuadBlockIndex[quad];
                cmd[3] = 0xF02C1 | ((rows << 22) + 0x1700000);
            } else {
                out[2] = s_BillboardBlockIndex[res->billboardType];
                cmd[3] = 0x17F02C1;
            }
            out[4] = ptcl->mWorldPos.z;
            out[5] = ptcl->mWorldPos.y;
            out[6] = ptcl->mWorldPos.x;
            out[7] = scaleY;
            out[8] = scaleX;
            out[9] = alpha;
            out[11] = ptcl->mTexScale.y;
            out[12] = ptcl->mTexScale.x;
            out[13] = ptcl->mTexOffset.y;
            out[14] = ptcl->mTexOffset.x;
            cmd = s_PtclCalcFunctions[res->ptclCalcType](cmd + 15, emitter, ptcl) + rows * 4 + 1;

            quad++;
            if (quad == 8) {
                cmd = drawQuads_(cmd, 48);
                quad = 0;
            }
        } else {
            // only the rotation and the color move on
            s_PtclCalcFunctions[res->ptclCalcType](cmd + 15, emitter, ptcl);
        }
        calcComplex_(emitter, ptcl);
        ptcl = ptcl->mNext;
    }
    if (quad != 0) {
        cmd = drawQuads_(cmd, quad * 6);
    }
    *numPtcl = count;
    return cmd;
}

// 0x00559CF0 (name is ours)
u32* sead::ptcl::EmitterComplexCalc::childDrawSetting_(u32* cmd, EmitterInstance* emitter)
{
    SimpleEmitterData* res = emitter->mRes;
    EmitterSet* set = emitter->mEmitterSet;
    const ChildData& child = res->child;
    f32 colorR = child.combinerColor.x * set->mColor.r;
    f32 colorG = child.combinerColor.y * set->mColor.g;
    f32 colorB = child.combinerColor.z * set->mColor.b;
    f32 billboardType = static_cast<f32>(child.billboardType);

    // the billboard of the children (uniforms from c4)
    f32* out = reinterpret_cast<f32*>(cmd);
    if (res->childFlag & CHILD_FLAG_ROTATE_EMITTER) {
        cmd[0] = 0x80000004;
        cmd[1] = 0xF02C0;
        cmd[2] = 0;
        cmd[3] = 0xFF02C1;
        out[4] = billboardType;
        out[5] = child.billboardParam.y;
        out[6] = child.billboardParam.x;
        cmd[7] = 0;
        out[8] = emitter->mRT.m[0][2];
        out[9] = emitter->mRT.m[0][1];
        out[10] = emitter->mRT.m[0][0];
        cmd[11] = 0;
        out[12] = emitter->mRT.m[1][2];
        out[13] = emitter->mRT.m[1][1];
        out[14] = emitter->mRT.m[1][0];
        cmd[15] = 0;
        out[16] = emitter->mRT.m[2][2];
        out[17] = emitter->mRT.m[2][1];
        out[18] = emitter->mRT.m[2][0];
        cmd += 20;
    } else {
        cmd[0] = 0x80000004;
        cmd[1] = 0xF02C0;
        cmd[2] = 0;
        out[4] = billboardType;
        out[5] = child.billboardParam.y;
        out[6] = child.billboardParam.x;
        if (child.billboardType == 3) {
            out[7] = 3.0f;
            cmd[3] = 0x7F02C1;
            cmd += 12;
        } else if (child.billboardType == 4) {
            out[7] = 5.0f;
            cmd[3] = 0x7F02C1;
            out[8] = emitter->mRT.m[2][1];
            out[9] = emitter->mRT.m[1][1];
            out[10] = emitter->mRT.m[0][1];
            cmd += 12;
        } else {
            cmd[3] = 0x3F02C1;
            cmd += 8;
        }
    }

    // the flush of the frame buffer cache
    cmd[0] = 1;
    cmd[1] = 0xF0111;
    cmd[2] = 1;
    cmd[3] = 0xF0110;
    cmd += 4;

    TextureHandle* texture = child.texture;
    if (texture != s_CurrentTexture) {
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
        s_CurrentTexture = texture;
    }

    u32 r = static_cast<u32>(colorR) & 0xFF;
    u32 g = static_cast<u32>(colorG) & 0xFF;
    u32 b = static_cast<u32>(colorB) & 0xFF;
    if (child.blendType != s_CurrentState.blendType) {
        *cmd++ = s_ComplexBlendFuncTbl[child.blendType];
        *cmd++ = 0xF0101;
        s_CurrentState.blendType = child.blendType;
    }
    u32 color = r | (g << 8) | (b << 16);
    if (child.depthAlphaType != s_CurrentState.depthAlphaType) {
        cmd[0] = s_ComplexDepthFuncTbl[child.depthAlphaType];
        cmd[1] = 0xF0107;
        cmd[2] = s_ComplexAlphaTestTbl[child.depthAlphaType];
        cmd[3] = 0x30104;
        cmd[4] = s_ComplexColorReadTbl[child.depthAlphaType];
        cmd[5] = 0x10112;
        cmd[6] = s_ComplexDepthReadTbl[child.depthAlphaType];
        cmd[7] = 0x10114;
        cmd[8] = s_ComplexDepthWriteTbl[child.depthAlphaType];
        cmd[9] = 0x10115;
        cmd += 10;
        s_CurrentState.depthAlphaType = child.depthAlphaType;
    }

    cmd[0] = 1;
    cmd[1] = 0xF0111;
    cmd[2] = 1;
    cmd[3] = 0xF0110;
    cmd += 4;

    if (child.combinerType != s_CurrentState.combinerType) {
        const u32* combiner = s_CombinerTbl[child.combinerType];
        cmd[0] = combiner[0];
        cmd[1] = combiner[1];
        cmd[2] = combiner[2];
        cmd[4] = color;
        cmd[3] = combiner[3];
        cmd[5] = combiner[5];
        cmd[6] = combiner[6];
        cmd[7] = combiner[7];
        cmd[8] = combiner[8];
        cmd[9] = combiner[9];
        cmd += 10;
        s_CurrentState.combinerType = child.combinerType;
    } else {
        *cmd++ = color;
        *cmd++ = 0xF00C3;
    }

    *cmd++ = (res->flag & 0x80) ? s_DrawStateValue : 0;
    *cmd++ = 0x500E0;
    return cmd;
}

// 0x0055A0D0 slot 0x00 (name after EmitterSimpleGPUCalc::calcCTR)
u32* sead::ptcl::EmitterComplexCalc::calcCTR(u32* cmd, EmitterInstance* emitter, s32* numPtcl)
{
    SimpleEmitterData* res = emitter->mRes;
    if (!calcEmission_(emitter, true)) {
        *numPtcl = 0;
        return cmd;
    }

    cmd = s_pRenderer->beginCalc(cmd);
    emitter->mCnt++;
    s32 numChild = 0;
    s32 numParent = 0;
    if (res->childFlag & CHILD_FLAG_ENABLE) {
        if (res->childFlag & CHILD_FLAG_DRAW_BEFORE_PARENT) {
            cmd = calcChild_(cmd, emitter, &numChild);
            cmd = calcEmitter_(cmd, emitter, &numParent);
        } else {
            cmd = calcEmitter_(cmd, emitter, &numParent);
            cmd = calcChild_(cmd, emitter, &numChild);
        }
    } else {
        cmd = calcEmitter_(cmd, emitter, &numParent);
    }
    *numPtcl = numChild + numParent;
    return cmd;
}
} // namespace ptcl
} // namespace sead
