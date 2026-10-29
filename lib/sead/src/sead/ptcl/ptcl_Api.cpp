#include "sead/ptcl/ptcl_Api.h"
#include "sead/ptcl/seadEmitterData.h"
#include "sead/ptcl/seadPtclInstance.h"

namespace sead {
namespace ptcl {
// 0x00557F54 | mk7dlp:bytes [tier B]
void _calcTexPatAnim(const SimpleEmitterData* res, PtclInstance* ptcl)
{
    s32 frame;
    if (res->texPatFreq != 0) {
        frame = ptcl->mCnt / res->texPatFreq;
    } else {
        frame = ptcl->mCnt * res->texPatTblUse / ptcl->mLife;
    }
    u8 pattern = res->texPatTbl[frame % res->texPatTblUse];
    s32 row = pattern / res->texPatDivX;
    s32 column = pattern % res->texPatDivX;
    ptcl->mTexOffset.x = res->texPatScale.x * column;
    ptcl->mTexOffset.y = res->texPatScale.y * row;
}

// 0x0055B308 | mk7dlp:bytes [tier B]
void _interpolate3Colors(const SimpleEmitterData* res, PtclInstance* ptcl)
{
    f32 r;
    f32 g;
    f32 b;
    if (res->flag & 1) {
        const Color4f& color = res->colors[ptcl->mRandom % 3];
        r = color.r;
        g = color.g;
        b = color.b;
    } else {
        s32 period = ptcl->mLife / res->colorNumRepeat;
        if (period == 0) {
            period = ptcl->mLife;
        }
        s32 time = (ptcl->mCnt - 1) % period;
        s32 section1 = res->colorSection1 * period / 100;
        if (time < section1) {
            r = res->colors[0].r;
            g = res->colors[0].g;
            b = res->colors[0].b;
        } else {
            s32 section2 = res->colorSection2 * period / 100;
            if (time < section2) {
                f32 t = static_cast<f32>(time - section1) / static_cast<f32>(section2 - section1);
                r = res->colors[0].r + (res->colors[1].r - res->colors[0].r) * t;
                g = res->colors[0].g + (res->colors[1].g - res->colors[0].g) * t;
                b = res->colors[0].b + (res->colors[1].b - res->colors[0].b) * t;
            } else {
                s32 section3 = res->colorSection3 * period / 100;
                if (time >= section3) {
                    r = res->colors[2].r;
                    g = res->colors[2].g;
                    b = res->colors[2].b;
                } else {
                    f32 t = static_cast<f32>(time - section2) / static_cast<f32>(section3 - section2);
                    r = res->colors[1].r + (res->colors[2].r - res->colors[1].r) * t;
                    g = res->colors[1].g + (res->colors[2].g - res->colors[1].g) * t;
                    b = res->colors[1].b + (res->colors[2].b - res->colors[1].b) * t;
                }
            }
        }
    }
    ptcl->mColor.r = r;
    ptcl->mColor.g = g;
    ptcl->mColor.b = b;
}
} // namespace ptcl
} // namespace sead
