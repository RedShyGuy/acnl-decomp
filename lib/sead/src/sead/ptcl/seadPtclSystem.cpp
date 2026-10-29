#include "sead/ptcl/seadPtclSystem.h"
#include "nn/gx/gx_Api.h"
#include "sead/ptcl/seadEmitterCalc.h"
#include "sead/ptcl/seadEmitterComplexCalc.h"
#include "sead/ptcl/seadEmitterInstance.h"
#include "sead/ptcl/seadEmitterSet.h"
#include "sead/ptcl/seadEmitterSimpleCalc.h"
#include "sead/ptcl/seadEmitterSimpleGPUCalc.h"
#include "sead/ptcl/seadEmitterData.h"
#include "sead/ptcl/seadPtclEditor.h"
#include "sead/ptcl/seadPtclInstance.h"
#include "sead/ptcl/seadPtclRandom.h"
#include "sead/ptcl/seadPtclRenderer.h"
#include "sead/ptcl/seadPtclResource.h"
#include "sead/ptcl/seadPtclStripe.h"
#include "sead/seadGlobalRandom.h"
#include "sead/seadGraphics.h"
#include "sead/seadHeapMgr.h"
#include "sead/seadMathCalcCommon.h"
#include "sead/seadNew.h"
#include "sead/seadRandom.h"
#include "sead/seadTickSpan.h"

#include <math.h>
#include <string.h>


namespace sead {
namespace ptcl {
// (the static initializer builds s_WarningEvent and registers its destructor)
// 0x00AE5E34
DelegateEvent<const char*> PtclSystem::s_WarningEvent;
// 0x00798D8C (static initializer)

namespace {
// the emitter that follows the round robin index in the pool (NULL when all are in use)
inline EmitterInstance* allocEmitter_(PtclSystem* system)
{
    for (s32 i = 0; i < system->mNumEmitter; i++) {
        system->mEmitterIx = (system->mEmitterIx + 1) & system->mEmitterMask;
        if (system->mEmitters[system->mEmitterIx].mCalc == nullptr) {
            return &system->mEmitters[system->mEmitterIx];
        }
    }
    return nullptr;
}

// the start of an emitter: its random numbers, its calculator and the frame it starts emitting
inline void initEmitter_(PtclSystem* system, EmitterInstance* emitter, SimpleEmitterData* res, EmitterSet* set,
                         u8 groupId, Random* random)
{
    emitter->mCnt = 0;
    emitter->mRes = res;
    emitter->mGpuStopFrame = -1;
    emitter->mGpuLoopState = 0;
    emitter->mGpuFadeCnt = 0;
    emitter->mGpuCnt = 0;
    if (res->randomSeed != 0) {
        emitter->mRandom.init(res->randomSeed);
    } else {
        emitter->mRandom.init(random->getU32());
    }
    if (res->ptclLife == 0x7FFFFFFF) {
        set->mIsInfinite = true;
    }
    emitter->mPtclHead = nullptr;
    emitter->mChildHead = nullptr;
    emitter->mGroupID = groupId;
    emitter->mCalc = system->mEmitterCalc[emitter->mRes->type];
    emitter->mEmitInterval = emitter->mRes->emitInterval - emitter->mRandom.getS32Range(emitter->mRes->emitIntervalRandom);
    emitter->mEmitCnt = 0x7FFFFFFF;
}

// puts emitter at the head of the list of its group
inline void linkEmitter_(PtclSystem* system, EmitterInstance* emitter, u8 groupId)
{
    EmitterInstance*& head = system->mEmitterGroupHead[groupId];
    if (head != nullptr) {
        head->mPrev = emitter;
        emitter->mNext = head;
        head = emitter;
    } else {
        head = emitter;
        emitter->mNext = nullptr;
    }
    emitter->mPrev = nullptr;
}
} // namespace

// 0x0055104C (name is ours)
sead::ptcl::PtclSystem::PtclSystem(const Config& config) : hostio::Node(), mLastWarnTick()
{
    mHeap = config.heap;
    ScopedCurrentHeapSetter setter(mHeap);

    mNumResource = config.numResourceMax + 1;
    mResources = new (std::nothrow) PtclResource*[mNumResource];
    memset(mResources, 0, mNumResource * sizeof(PtclResource*));

    mNumEmitter = 1 << config.numEmitterLog2;
    mNumPtcl = 1 << config.numPtclLog2;
    mNumEmitterSet = 1 << config.numEmitterSetLog2;
    mEmitterSetMask = mNumEmitterSet - 1;
    mNumStripe = 1 << config.numStripeLog2;
    mEmitterMask = mNumEmitter - 1;
    mPtclMask = mNumPtcl - 1;
    mStripeMask = mNumStripe - 1;

    PtclRandom::CreateRandomTbl(mHeap);
    memset(mEmitterGroupHead, 0, sizeof(mEmitterGroupHead));
    memset(mStripeGroupHead, 0, sizeof(mStripeGroupHead));

    mEmitters = new (std::nothrow) EmitterInstance[mNumEmitter];
    memset(mEmitters, 0, mNumEmitter * sizeof(EmitterInstance));
    mPtcls = new (std::nothrow) PtclInstance[mNumPtcl];
    memset(mPtcls, 0, mNumPtcl * sizeof(PtclInstance));
    mStripes = new (std::nothrow) PtclStripe[mNumStripe];
    memset(mStripes, 0, mNumStripe * sizeof(PtclStripe));
    mEmitterSets = new (std::nothrow) EmitterSet[mNumEmitterSet];

    mRenderer = new (mHeap, 4) PtclRenderer(mHeap, mNumPtcl, this, &config);

    mEmitterCalc[0] = new (std::nothrow) EmitterSimpleCalc(this);
    mEmitterCalc[1] = new (std::nothrow) EmitterComplexCalc(this);
    mEmitterCalc[2] = new (std::nothrow) EmitterSimpleGPUCalc(this);

    for (s32 i = 0; i < mNumEmitterSet; i++) {
        mEmitterSets[i].mSystem = this;
    }

    mEmitterIx = 0;
    mPtclIx = 0;
    mStripeIx = 0;
    mNumCalcEmitter = 0;
    mNumCalcEmitterGPU = 0;
    mNumCalcPtcl = 0;
    mNumCalcPtclGPU = 0;
    mCreateID = 0;
    mEmitterSetIx = 0;
    mNumEmitterSetAlive = 0;
    field_0x8B4 = 0;
    mNumCalcStripe = 0;
    mNumCalcStripeVtx = 0;
    field_0x82C = 0;
    mNumFreeEmitter = mNumEmitter;
    for (s32 i = 0; i < mNumEmitter; i++) {
        mEmitters[i].mCalc = nullptr;
    }
    for (s32 i = 0; i < mNumPtcl; i++) {
        mPtcls[i].mRes = nullptr;
    }
    for (s32 i = 0; i < mNumStripe; i++) {
        mStripes[i].mRes = nullptr;
        mStripes[i].mHistoryStart = 0;
        mStripes[i].mNumHistory = 0;
    }

    mEditor = nullptr;
    if (config.editorHeap != nullptr) {
        mEditor = new (std::nothrow) PtclEditor(config.editorHeap, this, config.numResourceMax);
    }

    mViewMtx = Matrix34<f32>::ident;
    EmitterCalc::setupFluctuation_(mHeap);
    field_0x8C0 = 1.2f / tanf(0.7853982f);
}

// 0x0054F158 (name is ours)
sead::ptcl::PtclStripe::PtclStripe() : mColor()
{
}

// 0x0054F1B4 (name is ours)
u32* sead::ptcl::PtclSystem::renderStripeGroup(u32* cmd, u8 groupId)
{
    for (PtclStripe* stripe = mStripeGroupHead[groupId]; stripe != nullptr; stripe = stripe->mNext) {
        SimpleEmitterData* res = stripe->mRes;
        const StripeData* stripeData =
            reinterpret_cast<const StripeData*>(reinterpret_cast<const u8*>(res) + res->stripeDataOffset);
        stripe->mCnt++;
        stripe->mTexScrollU += stripeData->texScrollU;
        if (stripe->mTexScrollU < -2.0f) {
            stripe->mTexScrollU += 2.0f;
        }
        stripe->mTexScrollV += stripeData->texScrollV;
        if (stripe->mTexScrollU > 2.0f) {
            stripe->mTexScrollU -= 2.0f;
        }
        if (stripe->mTexScrollV < -2.0f) {
            stripe->mTexScrollV += 2.0f;
        }
        if (stripe->mTexScrollV > 2.0f) {
            stripe->mTexScrollV -= 2.0f;
        }
        mNumCalcStripe++;

        s32 num = stripeData->numDraw;
        if (num > stripe->mNumHistory) {
            num = stripe->mNumHistory;
        }
        s32 numVertex = num * 2 - 2;
        if (num < 3) {
            continue;
        }
        PtclRenderer* renderer = mRenderer;
        if (renderer->mNumStripeVtxUsed + numVertex >= static_cast<u32>(renderer->mNumStripeVtxMax)) {
            continue;
        }
        Vector3<f32>* pos = &renderer->mStripePos[renderer->mNumStripeVtxUsed];
        Vector3<f32>* tex = &renderer->mStripeTex[renderer->mNumStripeVtxUsed];
        nn::math::VEC4* dir = &renderer->mStripeDirection[renderer->mNumStripeVtxUsed];
        f32 alphaRange = stripeData->alphaEnd - stripeData->alphaStart;
        f32 step = 1.0f / (num - 2);
        for (s32 i = 0; i < num - 1; i++) {
            f32 t = i * step;
            f32 index = t * (stripe->mNumHistory - 2);
            s32 entry = static_cast<s32>(index + 0.5f);
            f32 frac = index - entry;
            if (frac < 0.0f) {
                frac = 0.0f;
            }
            s32 index0 = stripe->mHistoryPos - entry - 1;
            if (index0 < 0) {
                index0 += stripeData->numHistoryMax;
            }
            s32 index1 = index0 - 1;
            if (index1 < 0) {
                index1 += stripeData->numHistoryMax;
            }
            StripeHistory& h0 = stripe->mHistory[index0];
            StripeHistory& h1 = stripe->mHistory[index1];
            Vector3<f32> diff;
            Vector3CalcCtr<f32>::sub(toVEC3(diff), toVEC3(h1.pos), toVEC3(h0.pos));
            Vector3<f32> scaled = diff;
            Vector3CalcCtr<f32>::multScalar(toVEC3(scaled), toVEC3(scaled), frac);
            Vector3<f32> offset = scaled;
            Vector3<f32> p;
            Vector3CalcCtr<f32>::add(toVEC3(p), toVEC3(h0.pos), toVEC3(offset));
            if (stripeData->type != 0) {
                Vector3<f32> v;
                Vector3CalcCtr<f32>::add(toVEC3(v), toVEC3(p), toVEC3(h0.dir));
                pos[0] = v;
                Vector3CalcCtr<f32>::sub(toVEC3(v), toVEC3(p), toVEC3(h0.dir));
                pos[1] = v;
                if (res->stripeFlag & 1) {
                    Vector3CalcCtr<f32>::mul(toVEC3(pos[0]), stripe->mEmitterMtx, toVEC3(pos[0]));
                    Vector3CalcCtr<f32>::mul(toVEC3(pos[1]), stripe->mEmitterMtx, toVEC3(pos[1]));
                }
            } else {
                pos[0] = p;
                pos[1] = p;
                dir[0].x = h0.dir.x;
                dir[0].y = h0.dir.y;
                dir[0].z = h0.dir.z;
                dir[0].w = h0.scale;
                dir[1].x = h0.dir.x;
                dir[1].y = h0.dir.y;
                dir[1].z = h0.dir.z;
                dir[1].w = -h0.scale;
                if (res->stripeFlag & 1) {
                    Vector3CalcCtr<f32>::rotate(*reinterpret_cast<nn::math::VEC3*>(&dir[0]), stripe->mEmitterMtx,
                                                *reinterpret_cast<nn::math::VEC3*>(&dir[0]));
                    Vector3CalcCtr<f32>::rotate(*reinterpret_cast<nn::math::VEC3*>(&dir[1]), stripe->mEmitterMtx,
                                                *reinterpret_cast<nn::math::VEC3*>(&dir[1]));
                    Vector3CalcCtr<f32>::mul(toVEC3(pos[0]), stripe->mEmitterMtx, toVEC3(pos[0]));
                    Vector3CalcCtr<f32>::mul(toVEC3(pos[1]), stripe->mEmitterMtx, toVEC3(pos[1]));
                }
            }
            f32 alpha = (stripeData->alphaStart + alphaRange * t) * stripe->mColor.a;
            tex[0].x = 0.0f;
            tex[1].x = 1.0f;
            tex[0].y = t;
            tex[1].y = t;
            tex[0].z = alpha;
            tex[1].z = alpha;
            pos += 2;
            tex += 2;
            dir += 2;
        }
        cmd = mRenderer->renderStripe(cmd, stripe, numVertex);
        mRenderer->mNumStripeVtxUsed += numVertex;
        mRenderer->mNumStripeIndexUsed += numVertex / 2 * 6;
        mNumCalcStripeVtx += numVertex;
    }
    return cmd;
}

// 0x0054F690 | mk7dlp:bytes-fuzzy [tier B]
u32* sead::ptcl::PtclSystem::beginRender(u32* cmd, const Matrix44<f32>& proj, const Matrix34<f32>& view,
                                         const Matrix34<f32>& camera)
{
    Matrix34<f32> billboard;
    Matrix34CalcCtr<f32>::transposeTo(billboard, camera);
    billboard.m[2][3] = 0.0f;
    billboard.m[1][3] = 0.0f;
    billboard.m[0][3] = 0.0f;
    return mRenderer->beginRender(cmd, proj, view, billboard);
}

// 0x0054F6EC | mk7dlp:bytes-fuzzy [tier B]
void sead::ptcl::PtclSystem::killEmitter(EmitterInstance* emitter)
{
    for (PtclInstance* ptcl = emitter->mPtclHead; ptcl != nullptr; ptcl = ptcl->mNext) {
        ptcl->mRes = nullptr;
        if (ptcl->mStripe != nullptr) {
            removeStripe(ptcl->mStripe);
        }
    }
    for (PtclInstance* ptcl = emitter->mChildHead; ptcl != nullptr; ptcl = ptcl->mNext) {
        ptcl->mRes = nullptr;
    }
    emitter->mCalc = nullptr;
    emitter->mEmitterSet->mNumCreated--;
    emitter->mCreateID = -1;
    if (emitter->mEmitterSet->mNumCreated == 0) {
        mNumEmitterSetAlive--;
    }
    mNumFreeEmitter++;

    if (mEmitterGroupHead[emitter->mGroupID] == emitter) {
        mEmitterGroupHead[emitter->mGroupID] = emitter->mNext;
        if (emitter->mNext != nullptr) {
            emitter->mNext->mPrev = nullptr;
        }
    } else {
        if (emitter->mNext != nullptr) {
            emitter->mNext->mPrev = emitter->mPrev;
        }
        emitter->mPrev->mNext = emitter->mNext;
    }
}

// 0x0054F84C (name is ours)
void sead::ptcl::PtclSystem::removePtcl(EmitterInstance* emitter, PtclInstance* ptcl)
{
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
    if (ptcl->mStripe != nullptr) {
        removeStripe(ptcl->mStripe);
    }
}

// 0x0054F8A8 | mk7dlp:bytes-fuzzy [tier B]
void sead::ptcl::PtclSystem::removeStripe(PtclStripe* stripe)
{
    stripe->mRes = nullptr;
    if (mStripeGroupHead[stripe->mGroupID] == stripe) {
        mStripeGroupHead[stripe->mGroupID] = stripe->mNext;
        if (stripe->mNext != nullptr) {
            stripe->mNext->mPrev = nullptr;
        }
    } else {
        if (stripe->mNext != nullptr) {
            stripe->mNext->mPrev = stripe->mPrev;
        }
        stripe->mPrev->mNext = stripe->mNext;
    }
}

// 0x0054F92C (name is ours)
void sead::ptcl::PtclSystem::deleteResource(s32 resId)
{
    if (mEditor != nullptr) {
        mEditor->clearResource(mResources[resId]);
    }
    delete mResources[resId];
    mResources[resId] = nullptr;
}

// 0x0054F97C (name is ours)
void sead::ptcl::PtclSystem::entryResource(Heap* heap, void* data, s32 resId, BufferLocation location)
{
    Graphics::s_pInstance->lockDrawContext();
    if (mResources[resId] != nullptr) {
        if (mEditor != nullptr) {
            mEditor->clearResource(mResources[resId]);
        }
        delete mResources[resId];
        mResources[resId] = nullptr;
    }
    mResources[resId] = new (heap, 4) PtclResource(heap, data, resId, this, location);
    Graphics::s_pInstance->unlockDrawContext();
    if (mEditor != nullptr) {
        mEditor->bindResource(mResources[resId]);
    }
    mResources[resId]->mName.format("Icon = LAYOUT , Order = %d", 1000 - resId);
}

// 0x0054FA8C (name is ours)
void sead::ptcl::PtclSystem::killAll()
{
    memset(mEmitterGroupHead, 0, sizeof(mEmitterGroupHead));
    memset(mStripeGroupHead, 0, sizeof(mStripeGroupHead));
    for (s32 i = 0; i < mNumEmitter; i++) {
        mEmitters[i].mCalc = nullptr;
    }
    for (s32 i = 0; i < mNumPtcl; i++) {
        mPtcls[i].mRes = nullptr;
    }
    for (s32 i = 0; i < mNumStripe; i++) {
        mStripes[i].mRes = nullptr;
    }
    for (s32 i = 0; i < mNumEmitterSet; i++) {
        mEmitterSets[i].mNumCreated = 0;
    }
    mNumEmitterSetAlive = 0;
    mNumFreeEmitter = mNumEmitter;
    mEmitterIx = 0;
}

// 0x0054FCC0 | mk7dlp
void sead::ptcl::PtclSystem::reCreateEmitter(EmitterSet* set, s32 resId, s32 emitterSetId, u8 groupId)
{
    Random* random = GlobalRandom::s_pInstance;
    for (s32 i = set->mNumEmitter; i < 8; i++) {
        set->mRecord[i] = set->mRecord[0];
    }
    EmitterSet saved = *set;
    set->kill();
    *set = saved;
    set->mNumCreated = 0;
    set->mIsInfinite = false;

    s32 numEmitter = mResources[resId]->mEmitterSetTbl[emitterSetId].numEmitter;
    if (numEmitter <= mNumFreeEmitter) {
        for (s32 i = numEmitter - 1; i >= 0; i--) {
            EmitterInstance* emitter = allocEmitter_(this);
            if (emitter == nullptr) {
                warn_("エミッタが枯渇しました\n");
                break;
            }
            set->mInstance[set->mNumCreated++] = emitter;
            emitter->mRecord = &set->mRecord[i];
            emitter->mEmitterSet = set;
            emitter->mCreateID = set->mCreateID;
            emitter->mFadeAlpha = 1.0f;
            linkEmitter_(this, emitter, groupId);
            mNumFreeEmitter--;
            SimpleEmitterData* res = mResources[resId]->mEmitterSetTbl[emitterSetId].emitterTbl[i].data;
            initEmitter_(this, emitter, res, set, groupId, random);
        }
        set->mNumEmitter = set->mNumCreated;
        mNumEmitterSetAlive++;
    }
}

// 0x005502EC | mk7dlp
void sead::ptcl::PtclSystem::reCreateEmitter(void** resData, s32 num, s32 resId, s32 emitterSetId, bool killOnly)
{
    for (s32 i = 0; i < mNumEmitter; i++) {
    }
    for (s32 i = 0; i < mNumEmitterSet; i++) {
        EmitterSet* set = &mEmitterSets[i];
        bool found = false;
        u8 groupId = 0;
        for (s32 j = 0; j < set->mNumEmitter; j++) {
            EmitterInstance* emitter = set->mInstance[j];
            if (emitter->mCalc != nullptr && emitter->mCreateID == set->mCreateID) {
                for (s32 k = 0; k < num; k++) {
                    if (emitter->mRes == resData[k]) {
                        found = true;
                        groupId = emitter->mGroupID;
                        break;
                    }
                }
            }
        }
        if (found) {
            if (killOnly) {
                set->kill();
            } else {
                reCreateEmitter(set, resId, emitterSetId, groupId);
            }
        }
    }
}

// 0x0055040C (name is ours)
void sead::ptcl::PtclSystem::setStripeBuffer(void* vertexBuffer, u16* indexBuffer, s32 num)
{
    PtclRenderer* renderer = mRenderer;
    renderer->mNumStripeVtxUsed = 0;
    renderer->mNumStripeIndexUsed = 0;
    renderer->mStripePos = static_cast<Vector3<f32>*>(vertexBuffer);
    renderer->mStripeTex = renderer->mStripePos + num;
    renderer->mStripeDirection = reinterpret_cast<nn::math::VEC4*>(renderer->mStripeTex + num);
    renderer->mNumStripeVtxMax = num;
    renderer->mStripeIndex = indexBuffer;
    renderer->mStripeIndexStream.m_IsUnsignedByte = false;
}

// 0x0055047C (name is ours)
bool sead::ptcl::PtclSystem::hasEmitterGroup(u8 groupId) const
{
    return mEmitterGroupHead[groupId] != nullptr;
}

// 0x00550490 | mk7dlp
void sead::ptcl::PtclSystem::killEmitterGroup(u8 groupId)
{
    for (EmitterInstance* emitter = mEmitterGroupHead[groupId]; emitter != nullptr; emitter = emitter->mNext) {
        killEmitter(emitter);
    }
    for (PtclStripe* stripe = mStripeGroupHead[groupId]; stripe != nullptr; stripe = stripe->mNext) {
        removeStripe(stripe);
    }
}

// 0x00550698 | mk7dlp
bool sead::ptcl::PtclSystem::createEmitterSetID(Handle* handle, const Matrix34<f32>& mtx, s32 emitterSetId, s32 resId,
                                                u8 groupId, u32 emitterMask)
{
    Random* random = GlobalRandom::s_pInstance;
    s32 numEmitter = mResources[resId]->mEmitterSetTbl[emitterSetId].numEmitter;
    if (mNumFreeEmitter < numEmitter) {
        warn_("エミッタが枯渇しました\n");
        return false;
    }

    // (one counter for both searches, like the original)
    s32 i = 0;
    EmitterSet* set = nullptr;
    for (; i < mNumEmitterSet; i++) {
        mEmitterSetIx = (mEmitterSetIx + 1) & mEmitterSetMask;
        if (mEmitterSets[mEmitterSetIx].mNumCreated == 0) {
            set = &mEmitterSets[mEmitterSetIx];
            break;
        }
    }
    handle->mEmitterSet = set;
    if (set == nullptr) {
        warn_("エミッタセットが枯渇しました\n");
        return false;
    }

    set->mSRT = mtx;
    set->mRT = mtx;
    set->mScale = Vector3<f32>::ones;
    set->mEmitterVolumeScale = Vector3<f32>::ones;
    set->mPtclEmitScale = Vector2<f32>::ones;
    set->mPtclScaleXY.x = 1.0f;
    set->mPtclScaleXY.y = 1.0f;
    set->mStartFrame = 0;
    set->mPtclScale.y = 1.0f;
    set->mPtclScale.x = 1.0f;
    set->mColor.a = 1.0f;
    set->mColor.b = 1.0f;
    set->mColor.g = 1.0f;
    set->mColor.r = 1.0f;
    set->mRandomVelocityScale = 1.0f;
    set->mDirectionalVelocityScale = 1.0f;
    set->mFigureVelocityScale = 1.0f;
    set->mAddVelocity.z = 0.0f;
    set->mAddVelocity.y = 0.0f;
    set->mAddVelocity.x = 0.0f;
    set->mIsSetDirectional = false;
    set->mIsFadeRequest = false;
    set->mIsStopCalc = false;
    set->mIsInfinite = false;
    set->mNumEmitPos = 0;
    set->mCreateID = mCreateID;
    set->mResourceID = resId;
    set->mEmitterSetID = emitterSetId;
    handle->mCreateID = mCreateID;

    for (s32 index = numEmitter - 1; index >= 0; index--) {
        if ((emitterMask & (1 << index)) == 0) {
            continue;
        }
        EmitterInstance* emitter = nullptr;
        for (; i < mNumEmitter; i++) {
            mEmitterIx = (mEmitterIx + 1) & mEmitterMask;
            if (mEmitters[mEmitterIx].mCalc == nullptr) {
                emitter = &mEmitters[mEmitterIx];
                break;
            }
        }
        if (emitter == nullptr) {
            warn_("エミッタが枯渇しました\n");
            break;
        }
        set->mInstance[set->mNumCreated++] = emitter;
        EmitterSetRecord* record = &set->mRecord[index];
        emitter->mRecord = record;
        emitter->mEmitterSet = set;
        record->emitRate = 256;
        record->emitInterval = 256;
        record->life = 256;
        record->emitterCallback = nullptr;
        record->callback = nullptr;
        record->childCallback = nullptr;
        emitter->mCreateID = mCreateID;
        emitter->mFadeAlpha = 1.0f;
        linkEmitter_(this, emitter, groupId);
        mNumFreeEmitter--;
        SimpleEmitterData* res = mResources[resId]->mEmitterSetTbl[emitterSetId].emitterTbl[index].data;
        initEmitter_(this, emitter, res, set, groupId, random);
    }

    set->mNumEmitter = set->mNumCreated;
    mNumEmitterSetAlive++;
    mCreateID++;
    return true;
}

// 0x00550B88 | mk7dlp
sead::ptcl::PtclStripe* sead::ptcl::PtclSystem::allocAndConnectStripe(EmitterInstance* emitter, PtclInstance* ptcl)
{
    PtclStripe* stripe = nullptr;
    for (s32 i = 0; i < mNumStripe; i++) {
        mStripeIx = (mStripeIx + 1) & mStripeMask;
        if (mStripes[mStripeIx].mRes == nullptr) {
            stripe = &mStripes[mStripeIx];
            break;
        }
    }
    if (stripe == nullptr) {
        warn_("ストライプが枯渇しました\n");
        return nullptr;
    }

    PtclStripe*& head = mStripeGroupHead[emitter->mGroupID];
    if (head != nullptr) {
        head->mPrev = stripe;
        stripe->mNext = head;
        head = stripe;
    } else {
        head = stripe;
        stripe->mNext = nullptr;
    }
    stripe->mPrev = nullptr;
    stripe->mNumHistory = 0;
    stripe->mPtcl = ptcl;
    stripe->mHistoryStart = 0;
    stripe->mHistoryPos = 0;
    stripe->mRes = emitter->mRes;
    stripe->mGroupID = emitter->mGroupID;
    stripe->mCnt = 0;
    stripe->mTexScrollV = 0.0f;
    stripe->mTexScrollU = 0.0f;
    stripe->mHistory[0].dir = Vector3<f32>::zero;
    return stripe;
}

// 0x00550CEC (name is ours)
u32* sead::ptcl::PtclSystem::beginRenderBillboard(u32* cmd, const Matrix34<f32>& camera)
{
    Matrix34<f32> billboard;
    Matrix34CalcCtr<f32>::transposeTo(billboard, camera);
    billboard.m[2][3] = 0.0f;
    billboard.m[1][3] = 0.0f;
    billboard.m[0][3] = 0.0f;
    return mRenderer->beginRenderBillboard(cmd, billboard);
}

// 0x00550D34 (name is ours)
u32* sead::ptcl::PtclSystem::calcEmitterGroup(u32* cmd, u8 groupId)
{
    for (EmitterInstance* emitter = mEmitterGroupHead[groupId]; emitter != nullptr; emitter = emitter->mNext) {
        if (!emitter->mEmitterSet->mIsStopCalc) {
            s32 numPtcl;
            cmd = emitter->mCalc->calcCTR(cmd, emitter, &numPtcl);
            if (emitter->mRes->type == 2) {
                mNumCalcPtclGPU += numPtcl;
                mNumCalcEmitterGPU++;
            } else {
                mNumCalcPtcl += numPtcl;
            }
        }
        mNumCalcEmitter++;
    }
    return cmd;
}

// 0x00550DF4 | mk7dlp
void sead::ptcl::PtclSystem::warn_(const char* message)
{
    if (s_WarningEvent.mSlotList.mCount > 0) {
        s_WarningEvent.fire(message);
        return;
    }
    TickTime now;
    if (static_cast<s64>(now.mTick - mLastWarnTick.mTick) / TickSpan::sFrequency > 2) {
        mLastWarnTick = now;
    }
}

// 0x00550EA4 (name is ours)
u32* sead::ptcl::PtclSystem::endCalc(u32* cmd)
{
    return mRenderer->endCalc(cmd);
}

// (the original falls into PtclRenderer::beginStripeCalc with a nop)
// 0x0055687C (name is ours)
u32* sead::ptcl::PtclSystem::beginStripeCalc(u32* cmd)
{
    return mRenderer->beginStripeCalc(cmd);
}

// (the original falls into PtclRenderer::endCalc with a nop)
// 0x00556B38 (name is ours)
u32* sead::ptcl::PtclSystem::endStripeCalc(u32* cmd)
{
    return mRenderer->endCalc(cmd);
}

// (the original falls into PtclRenderer::endRender with a nop)
// 0x00556C10 (name is ours)
u32* sead::ptcl::PtclSystem::endRender(u32* cmd)
{
    return mRenderer->endRender(cmd);
}

// 0x00550EAC | mk7dlp:bytes [tier B]
void sead::ptcl::PtclSystem::initIBO(void* buffer, s32 numQuads)
{
    u16* index = static_cast<u16*>(buffer);
    for (s32 i = 0; i < numQuads; i++) {
        index[0] = i * 2;
        index[1] = i * 2 + 2;
        index[2] = i * 2 + 1;
        index[5] = i * 2 + 3;
        index[3] = i * 2 + 1;
        index[4] = i * 2 + 2;
        index += 6;
    }
    nngxUpdateBuffer(buffer, numQuads * 6 * sizeof(u16));
}

// 0x00550F20 | mk7dlp
sead::ptcl::PtclInstance* sead::ptcl::PtclSystem::allocPtcl()
{
    for (s32 i = 0; i < mNumPtcl; i++) {
        mPtclIx = (mPtclIx + 1) & mPtclMask;
        if (mPtcls[mPtclIx].mRes == nullptr) {
            mNumAllocPtcl++;
            return &mPtcls[mPtclIx];
        }
    }
    warn_("パーティクルが枯渇しました\n");
    return nullptr;
}

// 0x00550FD0 (name is ours)
u32* sead::ptcl::PtclSystem::beginCalc(u32* cmd)
{
    mRenderer->mCalcState = PtclRenderer::CALC_STATE_NONE;
    cmd = mRenderer->beginCalc(cmd);
    mNumCalcEmitter = 0;
    mNumCalcEmitterGPU = 0;
    mNumCalcPtcl = 0;
    mNumCalcPtclGPU = 0;
    mNumAllocPtcl = 0;
    mNumCalcStripe = 0;
    mNumCalcStripeVtx = 0;
    EmitterCalc::s_CurrentState.combinerType = 28;
    EmitterCalc::s_CurrentState.blendType = 4;
    EmitterCalc::s_CurrentState.depthAlphaType = 0;
    EmitterCalc::s_CurrentTexture = nullptr;
    return cmd;
}
} // namespace ptcl
} // namespace sead
