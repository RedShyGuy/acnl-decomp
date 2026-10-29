#include "sead/ptcl/seadEmitterSimpleCalc.h"
#include "sead/ptcl/seadEmitterData.h"
#include "sead/ptcl/seadEmitterInstance.h"
#include "sead/ptcl/seadEmitterSet.h"
#include "sead/ptcl/seadPtclInstance.h"
#include "sead/ptcl/seadPtclRenderer.h"
#include "sead/ptcl/seadPtclSystem.h"
#include "sead/seadMathCalcCommon.h"

namespace sead {
namespace ptcl {
namespace {
// the draw state by blend type and by depth/alpha test type (each source file of the original has its own
// copy of these tables; names are ours)
// 0x0097F768
u32 s_SimpleBlendFuncTbl[4] = {0x76760000, 0x16160000, 0x16160202, 0x01010200};
// 0x0097F778
u32 s_SimpleDepthFuncTbl[3] = {0x00000F41, 0x00000F10, 0x00001F41};
// 0x0097F784
u32 s_SimpleAlphaTestTbl[3] = {0x00000061, 0x00000061, 0x00008061};
// 0x0097F790
u32 s_SimpleDepthReadTbl[3] = {2, 0, 2};
// 0x0097F79C
u32 s_SimpleDepthWriteTbl[3] = {0, 0, 2};
// 0x0097F7A8
u32 s_SimpleColorReadTbl[3] = {0xF, 0xF, 0};
} // namespace

// 0x0055805C | mk7dlp
u32* sead::ptcl::EmitterSimpleCalc::emitterDrawSetting_(u32* cmd, EmitterInstance* emitter)
{
    SimpleEmitterData* res = emitter->mRes;
    EmitterSet* set = emitter->mEmitterSet;
    Matrix34CalcCtr<f32>::multiply(emitter->mRT, set->mRT, res->emitterRT);
    Matrix34CalcCtr<f32>::multiply(emitter->mSRT, set->mSRT, res->emitterSRT);
    f32 colorR = res->combinerColor.x * set->mColor.r;
    f32 colorG = res->combinerColor.y * set->mColor.g;
    f32 colorB = res->combinerColor.z * set->mColor.b;
    f32 billboardType = static_cast<f32>(res->billboardType);

    // the billboard of the emitter (uniforms from c4)
    f32* out = reinterpret_cast<f32*>(cmd);
    if (res->isRotateEmitter) {
        cmd[0] = 0x80000004;
        cmd[1] = 0xF02C0;
        cmd[2] = 0;
        cmd[3] = 0xFF02C1;
        out[4] = billboardType;
        out[5] = res->billboardParam.y;
        out[6] = res->billboardParam.x;
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
        out[5] = res->billboardParam.y;
        out[6] = res->billboardParam.x;
        if (res->billboardType == 3) {
            out[7] = 3.0f;
            cmd[3] = 0x7F02C1;
            cmd += 12;
        } else if (res->billboardType == 4) {
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
        *cmd++ = s_SimpleBlendFuncTbl[res->blendType];
        *cmd++ = 0xF0101;
        s_CurrentState.blendType = res->blendType;
    }
    u32 color = r | (g << 8) | (b << 16);
    if (res->depthAlphaType != s_CurrentState.depthAlphaType) {
        cmd[0] = s_SimpleDepthFuncTbl[res->depthAlphaType];
        cmd[1] = 0xF0107;
        cmd[2] = s_SimpleAlphaTestTbl[res->depthAlphaType];
        cmd[3] = 0x30104;
        cmd[4] = s_SimpleColorReadTbl[res->depthAlphaType];
        cmd[5] = 0x10112;
        cmd[6] = s_SimpleDepthReadTbl[res->depthAlphaType];
        cmd[7] = 0x10114;
        cmd[8] = s_SimpleDepthWriteTbl[res->depthAlphaType];
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
        cmd[4] = color;
        cmd[3] = combiner[3];
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

// 0x00558464 slot 0x00 (name after EmitterSimpleGPUCalc::calcCTR)
u32* sead::ptcl::EmitterSimpleCalc::calcCTR(u32* cmd, EmitterInstance* emitter, s32* numPtcl)
{
    EmitterSet* set = emitter->mEmitterSet;
    SimpleEmitterData* res = emitter->mRes;
    if (!calcEmission_(emitter, false)) {
        *numPtcl = 0;
        return cmd;
    }
    emitter->mCnt++;
    if (emitter->mPtclHead == nullptr) {
        *numPtcl = 0;
        return cmd;
    }

    cmd = s_pRenderer->beginCalc(cmd);
    cmd = emitterDrawSetting_(cmd, emitter);
    s32 count = 0;
    s32 quad = 0;
    PtclInstance* ptcl = emitter->mPtclHead;
    while (ptcl != nullptr) {
        if (ptcl->mCnt >= ptcl->mLife) {
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

        // the block of the particle: its position, scale, alpha and texture, the rotation and color, and
        // the extra rows of the movement (uniforms from c8 + 9 * quad)
        cmd[0] = 0x80000000 | (quad * 9 + 8);
        cmd[1] = 0xF02C0;
        s32 rows = s_PreCalcFunctions[res->preCalcType](cmd, emitter, ptcl);
        f32 alpha = ptcl->mAlpha * set->mColor.a;
        f32 scaleX = ptcl->mScale.x * set->mPtclScaleXY.x;
        f32 scaleY = ptcl->mScale.y * set->mPtclScaleXY.y;
        alpha = alpha * emitter->mFadeAlpha;
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
            // the drawing of eight quads
            cmd[0] = 48;
            cmd[1] = 0xF0228;
            cmd[2] = 0;
            cmd[3] = 0xF022F;
            cmd[4] = 1;
            cmd[5] = 0xF0231;
            cmd[6] = 0;
            cmd[7] = 0x8025E;
            cmd[8] = 0;
            cmd[9] = 0x8025E;
            cmd += 10;
            quad = 0;
        }
        count++;
        ptcl = ptcl->mNext;
    }
    if (quad != 0) {
        cmd[0] = quad * 6;
        cmd[1] = 0xF0228;
        cmd[2] = 0;
        cmd[3] = 0xF022F;
        cmd[4] = 1;
        cmd[5] = 0xF0231;
        cmd[6] = 0;
        cmd[7] = 0x8025E;
        cmd[8] = 0;
        cmd[9] = 0x8025E;
        cmd += 10;
    }
    *numPtcl = count;
    return cmd;
}
} // namespace ptcl
} // namespace sead
