#include "sead/ptcl/seadEmitterSimpleGPUCalc.h"
#include "sead/ptcl/seadEmitterData.h"
#include "sead/ptcl/seadEmitterInstance.h"
#include "sead/ptcl/seadEmitterSet.h"
#include "sead/ptcl/seadGpuPtcle.h"
#include "sead/ptcl/seadPtclRenderer.h"
#include "sead/ptcl/seadPtclSystem.h"
#include "sead/seadMathCalcCommon.h"

namespace sead {
namespace ptcl {
namespace {
// the factor of the gravity for the shaders (name is ours)
// 0x0097F7B4
f32 s_GpuGravityScale = 0.5f;

// the draw state by blend type and by depth/alpha test type (each source file of the original has its own
// copy of these tables; names are ours)
// 0x0097F7B8
u32 s_GpuBlendFuncTbl[4] = {0x76760000, 0x16160000, 0x16160202, 0x01010200};
// 0x0097F7C8
u32 s_GpuDepthFuncTbl[3] = {0x00000F41, 0x00000F10, 0x00001F41};
// 0x0097F7D4
u32 s_GpuAlphaTestTbl[3] = {0x00000061, 0x00000061, 0x00008061};
// 0x0097F7E0
u32 s_GpuDepthReadTbl[3] = {2, 0, 2};
// 0x0097F7EC
u32 s_GpuDepthWriteTbl[3] = {0, 0, 2};
// 0x0097F7F8
u32 s_GpuColorReadTbl[3] = {0xF, 0xF, 0};

// an angle index in radians (2 pi / 2^32)
const f32 IDX_TO_RAD = 1.4629180792671596e-9f;
// the frame of the end of SimpleEmitterData::emitEndFrame for an emitter that emits forever
const s32 EMIT_END_INFINITE = 0x7FFFFFFF;
} // namespace

// 0x0055B480 | mk7dlp
u32* sead::ptcl::EmitterSimpleGPUCalc::emitterDrawSetting_(u32* cmd, EmitterInstance* emitter)
{
    SimpleEmitterData* res = emitter->mRes;
    EmitterSet* set = emitter->mEmitterSet;
    Matrix34CalcCtr<f32>::multiply(emitter->mRT, set->mRT, res->emitterRT);
    Matrix34CalcCtr<f32>::multiply(emitter->mSRT, set->mSRT, res->emitterSRT);

    // the flush of the frame buffer cache
    cmd[0] = 1;
    cmd[1] = 0xF0111;
    cmd[2] = 1;
    cmd[3] = 0xF0110;
    cmd += 4;

    f32 colorR = res->combinerColor.x * set->mColor.r;
    f32 colorG = res->combinerColor.y * set->mColor.g;
    f32 colorB = res->combinerColor.z * set->mColor.b;
    TextureHandle* texture = res->texture;
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
        s_CurrentTexture = res->texture;
    }

    u32 r = static_cast<u32>(colorR) & 0xFF;
    u32 g = static_cast<u32>(colorG) & 0xFF;
    u32 b = static_cast<u32>(colorB) & 0xFF;
    if (res->blendType != s_CurrentState.blendType) {
        *cmd++ = s_GpuBlendFuncTbl[res->blendType];
        *cmd++ = 0xF0101;
        s_CurrentState.blendType = res->blendType;
    }
    u32 color = r | (g << 8) | (b << 16);
    if (res->depthAlphaType != s_CurrentState.depthAlphaType) {
        cmd[0] = s_GpuDepthFuncTbl[res->depthAlphaType];
        cmd[1] = 0xF0107;
        cmd[2] = s_GpuAlphaTestTbl[res->depthAlphaType];
        cmd[3] = 0x30104;
        cmd[4] = s_GpuColorReadTbl[res->depthAlphaType];
        cmd[5] = 0x10112;
        cmd[6] = s_GpuDepthReadTbl[res->depthAlphaType];
        cmd[7] = 0x10114;
        cmd[8] = s_GpuDepthWriteTbl[res->depthAlphaType];
        cmd[9] = 0x10115;
        cmd += 10;
        s_CurrentState.depthAlphaType = res->depthAlphaType;
    }

    cmd[0] = 1;
    cmd[1] = 0xF0111;
    cmd[2] = 1;
    cmd[3] = 0xF0110;
    cmd += 4;

    if (res->combinerType != s_CurrentState.combinerType) {
        const u32* combiner = s_CombinerTbl[res->combinerType];
        cmd[0] = combiner[0];
        cmd[1] = combiner[1];
        cmd[2] = combiner[2];
        cmd[3] = combiner[3];
        cmd[4] = color;
        cmd[5] = combiner[5];
        cmd[6] = combiner[6];
        cmd[7] = combiner[7];
        cmd[8] = combiner[8];
        cmd[9] = combiner[9];
        cmd += 10;
        s_CurrentState.combinerType = res->combinerType;
    } else {
        *cmd++ = color;
        *cmd++ = 0xF00C3;
    }

    *cmd++ = (res->flag & 0x80) ? s_DrawStateValue : 0;
    *cmd++ = 0x500E0;
    return cmd;
}

// The particles of the emitter are not stored: the shader calculates particle i of the emissions from the
// frame (time), the emission parameters and the random vertices of mGpuSet. numPtcl gets the number drawn.
// 0x0055B76C slot 0x00 | mk7dlp
u32* sead::ptcl::EmitterSimpleGPUCalc::calcCTR(u32* cmd, EmitterInstance* emitter, s32* numPtcl)
{
    EmitterSet* set = emitter->mEmitterSet;
    SimpleEmitterData* res = emitter->mRes;
    if (emitter->mCnt == 0) {
        u32 seed = res->randomSeed;
        emitter->mGpuSet = seed;
        if (seed == 0) {
            seed = emitter->mRandom.getU32();
            emitter->mGpuSet = seed;
        }
        emitter->mGpuSet = seed % sGpuParticlePrimitive::NUM_SET;
    }

    // the fade (the particles of the shader can't die early: without a fade speed the emitter lives until
    // its last particle is dead)
    if (set->mIsFadeRequest) {
        if (emitter->mGpuStopFrame == -1 && res->isStopEmitInFade) {
            if (res->emitEndFrame == EMIT_END_INFINITE) {
                emitter->mGpuStopFrame = emitter->mGpuCnt;
            } else {
                emitter->mGpuStopFrame = emitter->mCnt;
            }
            emitter->mGpuFadeCnt = res->ptclLife;
        }
        emitter->mFadeAlpha = emitter->mFadeAlpha - res->fadeSpeed;
        emitter->mGpuFadeCnt--;
        if (emitter->mFadeAlpha <= 0.0f || (res->fadeSpeed == 0.0f && emitter->mGpuFadeCnt < 0)) {
            s_pSystem->killEmitter(emitter);
            *numPtcl = 0;
            return cmd;
        }
    } else {
        f32 alpha = emitter->mFadeAlpha + res->fadeSpeed;
        emitter->mFadeAlpha = alpha;
        if (alpha > 1.0f) {
            alpha = 1.0f;
        }
        emitter->mFadeAlpha = alpha;
    }
    emitter->mCnt++;
    emitter->mGpuCnt++;
    cmd = emitterDrawSetting_(cmd, emitter);
    s32 num = 0;

    // the frames: the last frame of an emission, the frame of the loop and the one where the frame wraps
    s32 endFrame;
    s32 loopFrame;
    s32 wrapFrame;
    if (res->emitEndFrame != EMIT_END_INFINITE) {
        endFrame = res->emitEndFrame;
        if (emitter->mGpuStopFrame != -1 && endFrame > emitter->mGpuStopFrame) {
            endFrame = emitter->mGpuStopFrame;
        }
        loopFrame = res->emitEndFrame;
        wrapFrame = res->emitStartFrame + res->emitEndFrame + res->ptclLife;
        if (emitter->mCnt > wrapFrame) {
            s_pSystem->killEmitter(emitter);
            *numPtcl = 0;
            return cmd;
        }
    } else {
        s32 period = (res->ptclLife * 2 / res->emitInterval + 2) * res->emitInterval;
        if (res->ptclLife > period) {
            period = res->ptclLife;
        }
        wrapFrame = period * 2 - 2;
        loopFrame = period - 1;
        endFrame = period - 1;
        if (emitter->mGpuLoopState == 1 || emitter->mGpuLoopState == 2) {
            if (emitter->mGpuStopFrame < endFrame) {
                endFrame = emitter->mGpuStopFrame;
            }
        }
    }
    s32 interval = res->emitInterval + 1;
    if (interval > endFrame) {
        interval = endFrame;
    }
    s32 time;
    s32 frame;
    if (res->emitEndFrame == EMIT_END_INFINITE) {
        time = emitter->mGpuCnt;
        frame = time;
        if (frame > endFrame) {
            frame = endFrame;
        }
    } else {
        s32 last = endFrame - res->emitStartFrame;
        time = emitter->mGpuCnt - res->emitStartFrame;
        frame = time;
        if (frame > last) {
            frame = last;
        }
    }
    // the particles emitted until now and the first one alive
    s32 numEmission = frame / interval;
    if (frame >= 0) {
        numEmission++;
    }
    s32 life = res->ptclLife;
    s32 emitRate = res->emitRate;
    s32 deadFrame = frame - life;
    if (deadFrame < 0) {
        deadFrame = 0;
    }
    s32 end = emitRate * numEmission;
    s32 first = emitRate * (deadFrame / interval);

    // the uniforms of the shader
    nn::math::VEC4 timeParam;
    timeParam.x = static_cast<f32>(time);
    timeParam.y = static_cast<f32>(life);
    timeParam.z = static_cast<f32>(interval);
    timeParam.w = 1.0f / static_cast<f32>(emitRate);

    f32 screenScale = s_pSystem->field_0x8C0 * 240.0f;
    nn::math::VEC4 scaleParam;
    scaleParam.x = res->scaleInit.x * set->mPtclScaleXY.x * (set->mPtclEmitScale.x * screenScale);
    scaleParam.y = res->scaleInit.y * set->mPtclScaleXY.y * (set->mPtclEmitScale.y * screenScale);
    scaleParam.z = res->scaleRandom;
    scaleParam.w = res->figureVelocity * emitter->mEmitterSet->mFigureVelocityScale;

    f32 scaleXY = set->mPtclScaleXY.x * screenScale;
    nn::math::VEC4 scaleAnm;
    scaleAnm.x = static_cast<f32>(res->scaleSection1) * 0.01f;
    scaleAnm.y = static_cast<f32>(res->scaleSection2) * 0.01f;
    scaleAnm.z = res->scaleDiff1.x * set->mPtclEmitScale.x * scaleXY;
    scaleAnm.w = res->scaleDiff2.x * set->mPtclEmitScale.x * scaleXY;

    // the velocity of the emitter with the added velocity of the set in the axes of the set
    f32 velScale = res->emitterVelocity * set->mDirectionalVelocityScale;
    nn::math::VEC4 emitterVel;
    if (set->mIsSetDirectional) {
        emitterVel.x = set->mDirectional.x * velScale;
        emitterVel.y = set->mDirectional.y * velScale;
        emitterVel.z = set->mDirectional.z * velScale;
    } else {
        emitterVel.x = res->emitterVelocityDir.x * velScale;
        emitterVel.y = res->emitterVelocityDir.y * velScale;
        emitterVel.z = res->emitterVelocityDir.z * velScale;
    }
    emitterVel.w = res->initVelocityRandom * set->mRandomVelocityScale;
    const Vector3<f32>& add = set->mAddVelocity;
    const Matrix34<f32>& setRT = set->mRT;
    emitterVel.x = emitterVel.x + add.x * setRT.m[0][0] + (add.y * setRT.m[1][0] + add.z * setRT.m[2][0]);
    emitterVel.y = emitterVel.y + add.x * setRT.m[0][1] + (add.y * setRT.m[1][1] + add.z * setRT.m[2][1]);
    emitterVel.z = emitterVel.z + add.x * setRT.m[0][2] + (add.y * setRT.m[1][2] + add.z * setRT.m[2][2]);

    nn::math::VEC4 spreadVec;
    spreadVec.x = res->spreadVec.x;
    spreadVec.y = res->spreadVec.y;
    spreadVec.z = res->spreadVec.z;
    spreadVec.w = res->airRegist;

    nn::math::VEC4 gravity;
    if (res->isWorldGravity) {
        const Vector3<f32>& g = res->gravity;
        const Matrix34<f32>& rt = emitter->mRT;
        gravity.x = (g.x * rt.m[0][0] + g.y * rt.m[1][0] + g.z * rt.m[2][0]) * s_GpuGravityScale;
        gravity.y = (g.x * rt.m[0][1] + g.y * rt.m[1][1] + g.z * rt.m[2][1]) * s_GpuGravityScale;
        gravity.z = (g.x * rt.m[0][2] + g.y * rt.m[1][2] + g.z * rt.m[2][2]) * s_GpuGravityScale;
    } else {
        gravity.x = res->gravity.x * s_GpuGravityScale;
        gravity.y = res->gravity.y * s_GpuGravityScale;
        gravity.z = res->gravity.z * s_GpuGravityScale;
    }
    gravity.w = static_cast<f32>(res->ptclLifeRandom) / static_cast<f32>(res->ptclLife);

    nn::math::VEC4 color;
    color.x = res->colors[0].r;
    color.y = res->colors[0].g;
    color.z = res->colors[0].b;
    color.w = res->alphaInit * emitter->mFadeAlpha;

    // the colors: the three colors one after the other (flag 0x100) or the first one only
    nn::math::VEC4 colorPk;
    nn::math::VEC4 colorEd;
    if (res->flag & 0x100) {
        colorPk.x = res->colors[1].r;
        colorPk.y = res->colors[1].g;
        colorPk.z = res->colors[1].b;
        colorPk.w = static_cast<f32>(res->colorSection2) * 0.01f;
        colorEd.x = res->colors[2].r;
        colorEd.y = res->colors[2].g;
        colorEd.z = res->colors[2].b;
    } else {
        colorPk.x = res->colors[0].r;
        colorPk.y = res->colors[0].g;
        colorPk.z = res->colors[0].b;
        colorPk.w = static_cast<f32>(res->colorSection2) * 0.01f;
        colorEd.x = res->colors[0].r;
        colorEd.y = res->colors[0].g;
        colorEd.z = res->colors[0].b;
    }
    colorEd.w = 1.0f / (static_cast<f32>(res->colorSection3 - res->colorSection2) * 0.01f);
    nn::math::VEC4 colorAnm;
    colorAnm.x = static_cast<f32>(res->colorSection1) * 0.01f;
    colorAnm.y = 1.0f / (static_cast<f32>(res->colorSection2 - res->colorSection1) * 0.01f);
    colorAnm.z = static_cast<f32>(res->colorNumRepeat);
    colorAnm.w = 0.0f;

    nn::math::VEC4 alphaAnm;
    alphaAnm.x = static_cast<f32>(res->alphaSection1) * 0.01f;
    alphaAnm.y = static_cast<f32>(res->alphaSection2) * 0.01f;
    alphaAnm.z = res->alphaDiff1 * emitter->mFadeAlpha;
    alphaAnm.w = res->alphaDiff2 * emitter->mFadeAlpha;
    if (alphaAnm.x < 0.02f) {
        // no first section: the particles start at its end
        color.w = color.w + alphaAnm.z;
        alphaAnm.x = 0.0f;
        alphaAnm.z = 0.0f;
    }

    // the rotation around z of the ptclCalcTypes with it (in radians; the random parts are scaled for the
    // random value of the shader)
    nn::math::VEC4 rotateZ;
    if (res->ptclCalcType == 3 || res->ptclCalcType == 8 || res->ptclCalcType == 13) {
        rotateZ.x = -(static_cast<f32>(res->rotInit[2]) * IDX_TO_RAD);
        rotateZ.y = static_cast<f32>(res->rotInitRandom[2]) * IDX_TO_RAD * -255.0f;
        rotateZ.z = -(static_cast<f32>(res->rotVel[2]) * IDX_TO_RAD);
        rotateZ.w = static_cast<f32>(res->rotVelRandom[2]) * IDX_TO_RAD * -255.0f;
    } else {
        rotateZ.x = 0.0f;
        rotateZ.y = 0.0f;
        rotateZ.z = 0.0f;
        rotateZ.w = 0.0f;
    }

    // an infinite emitter stopped in a fade: where the stop frame is in the loop
    if (res->emitEndFrame == EMIT_END_INFINITE && emitter->mGpuStopFrame != -1) {
        if (emitter->mGpuLoopState == 0) {
            emitter->mGpuLoopState = emitter->mGpuStopFrame < loopFrame ? 1 : 3;
        } else if (emitter->mGpuLoopState == 1) {
            if (emitter->mGpuCnt > loopFrame) {
                emitter->mGpuLoopState = 2;
            }
        } else if (emitter->mGpuLoopState == 3) {
            if (emitter->mGpuCnt < loopFrame) {
                emitter->mGpuLoopState = 4;
            }
        }
    }

    s32 count = end - first;
    if (count > 0) {
        num = count;
        cmd = s_pRenderer->PreDraw(cmd);
        if (emitter->mGpuLoopState != 4) {
            cmd = s_pRenderer->Draw(cmd, emitter->mSRT, timeParam, scaleParam, scaleAnm, emitterVel, spreadVec,
                                    gravity, color, colorPk, colorEd, colorAnm, alphaAnm, rotateZ, end, first,
                                    emitter->mGpuSet);
        }
        // the particles of the previous loop of an infinite emitter
        if (res->emitEndFrame == EMIT_END_INFINITE && emitter->mCnt > loopFrame && emitter->mGpuLoopState != 2) {
            f32 loop = static_cast<f32>(loopFrame);
            if (timeParam.x <= loop) {
                timeParam.x = timeParam.x + static_cast<f32>(loopFrame);
            } else {
                timeParam.x = timeParam.x - static_cast<f32>(loopFrame);
            }
            if (emitter->mGpuLoopState == 3 || emitter->mGpuLoopState == 4) {
                loopFrame = emitter->mGpuStopFrame - loopFrame;
            }
            s32 prevFrame = static_cast<s32>(timeParam.x);
            if (prevFrame > loopFrame) {
                prevFrame = loopFrame;
            }
            s32 prevEmission = prevFrame / interval;
            if (prevFrame >= 0) {
                prevEmission++;
            }
            s32 prevEnd = emitRate * prevEmission;
            num = emitRate * (loopFrame / interval) / 2;
            if (prevEnd >= 1) {
                cmd = s_pRenderer->Draw(cmd, emitter->mSRT, timeParam, scaleParam, scaleAnm, emitterVel, spreadVec,
                                        gravity, color, colorPk, colorEd, colorAnm, alphaAnm, rotateZ, prevEnd, 0,
                                        emitter->mGpuSet);
            }
            if (emitter->mGpuCnt > wrapFrame) {
                emitter->mGpuCnt -= wrapFrame;
            }
        }
        cmd = s_pRenderer->PostDraw(cmd);
    }
    *numPtcl = num;
    return cmd;
}
} // namespace ptcl
} // namespace sead
