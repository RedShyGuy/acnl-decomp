#pragma once

#include "sead/ptcl/seadEmitterCalc.h"
#include "sead/ptcl/seadEmitterData.h"
#include "sead/ptcl/seadEmitterInstance.h"
#include "sead/ptcl/seadEmitterSet.h"
#include "sead/ptcl/seadPtclInstance.h"

namespace sead {
namespace ptcl {
DECOMP_ALWAYS_INLINE bool EmitterCalc::calcEmission_(EmitterInstance* emitter, bool isComplex)
{
    EmitterSet* set = emitter->mEmitterSet;
    SimpleEmitterData* res = emitter->mRes;
    PtclSystem* system = s_pSystem;
    bool canEmit;
    if (set->mIsFadeRequest) {
        emitter->mFadeAlpha = emitter->mFadeAlpha - res->fadeSpeed;
        canEmit = !res->isStopEmitInFade;
        if (emitter->mFadeAlpha <= 0.0f) {
            system->killEmitter(emitter);
            return false;
        }
    } else {
        f32 alpha = emitter->mFadeAlpha + res->fadeSpeed;
        canEmit = true;
        emitter->mFadeAlpha = alpha;
        if (alpha > 1.0f) {
            alpha = 1.0f;
        }
        emitter->mFadeAlpha = alpha;
    }

    s32 frame = emitter->mCnt - set->mStartFrame;
    if (res->emitEndFrame > frame && canEmit) {
        if (res->emitStartFrame > frame) {
            return true;
        }
        if (((emitter->mEmitInterval * emitter->mRecord->emitInterval) >> 8) > emitter->mEmitCnt) {
            emitter->mEmitCnt++;
            return true;
        }
        emitter->mEmitCnt = 0;
        if (set->mNumEmitPos > 0) {
            for (s32 i = 0; i < set->mNumEmitPos; i++) {
                PtclInstance* oldHead = emitter->mPtclHead;
                s_EmitFunctions[res->volumeType](emitter);
                for (PtclInstance* ptcl = emitter->mPtclHead; ptcl != oldHead; ptcl = ptcl->mNext) {
                    ptcl->mPos.x += set->mEmitPos[i].x;
                    ptcl->mPos.y += set->mEmitPos[i].y;
                    ptcl->mPos.z += set->mEmitPos[i].z;
                }
            }
        } else {
            s_EmitFunctions[res->volumeType](emitter);
        }
        emitter->mEmitInterval = emitter->mRandom.getS32Range(res->emitIntervalRandom) + res->emitInterval;
        return true;
    }
    if (emitter->mPtclHead == nullptr && (!isComplex || emitter->mChildHead == nullptr)) {
        system->killEmitter(emitter);
        return false;
    }
    return true;
}
} // namespace ptcl
} // namespace sead
