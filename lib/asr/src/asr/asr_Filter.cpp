#include "asr/asr_Frontend.h"
#include <cstring>

// The half band filter tree: each level splits its input into a low and a high half at half the rate. The original
// has one function per position of the ring (FILTER_TAPS samples in a ring of 16; the functions are the same with
// rotated indices).
namespace asr {
namespace {
// 0x00982E6C
FilterTreeControl s_FilterControl;
// 0x00AF69F0
FilterTreeRings s_FilterRings;

} // namespace

template <s32 HEAD>
void FilterAt(const s32* ring, s32* low, s32* high)
{
#define ASR_TAP(age) ring[(HEAD - (age)) & 15]
    s32 value = (ASR_TAP(6) + ASR_TAP(8)) * 80 + ASR_TAP(7) * 128 - (ASR_TAP(4) + ASR_TAP(10)) * 24 +
                (ASR_TAP(2) + ASR_TAP(12)) * 11 - (ASR_TAP(0) + ASR_TAP(14)) * 5;
    value >>= 8;
    *low = value;
    *high = ASR_TAP(7) - value;
#undef ASR_TAP
}

// 0x007A31FC
template void FilterAt<0>(const s32* ring, s32* low, s32* high);
// 0x007A327C
template void FilterAt<1>(const s32* ring, s32* low, s32* high);
// 0x007A32FC
template void FilterAt<10>(const s32* ring, s32* low, s32* high);
// 0x007A337C
template void FilterAt<11>(const s32* ring, s32* low, s32* high);
// 0x007A33FC
template void FilterAt<12>(const s32* ring, s32* low, s32* high);
// 0x007A347C
template void FilterAt<13>(const s32* ring, s32* low, s32* high);
// 0x007A34FC
template void FilterAt<14>(const s32* ring, s32* low, s32* high);
// 0x007A357C
template void FilterAt<15>(const s32* ring, s32* low, s32* high);
// 0x007A35FC
template void FilterAt<2>(const s32* ring, s32* low, s32* high);
// 0x007A367C
template void FilterAt<3>(const s32* ring, s32* low, s32* high);
// 0x007A36FC
template void FilterAt<4>(const s32* ring, s32* low, s32* high);
// 0x007A377C
template void FilterAt<5>(const s32* ring, s32* low, s32* high);
// 0x007A37FC
template void FilterAt<6>(const s32* ring, s32* low, s32* high);
// 0x007A387C
template void FilterAt<7>(const s32* ring, s32* low, s32* high);
// 0x007A38FC
template void FilterAt<8>(const s32* ring, s32* low, s32* high);
// 0x007A397C
template void FilterAt<9>(const s32* ring, s32* low, s32* high);

typedef void (*FilterFunc)(const s32* ring, s32* low, s32* high);

// 0x008C3300
extern const FilterFunc s_FilterFuncs[16] = {
    FilterAt<0>,  FilterAt<1>,  FilterAt<2>,  FilterAt<3>,  FilterAt<4>,  FilterAt<5>,  FilterAt<6>,  FilterAt<7>,
    FilterAt<8>,  FilterAt<9>,  FilterAt<10>, FilterAt<11>, FilterAt<12>, FilterAt<13>, FilterAt<14>, FilterAt<15>,
};

namespace {
DECOMP_ALWAYS_INLINE void AdvanceHead(s16& head, s32 mask)
{
    head = (head + 1) & mask;
}

// one level of the tree: in the first frame a level stays silent until the ring it reads holds 14 samples (its head
// reaches 13); afterwards every other sample of its input goes through the filter
enum LevelStep
{
    LEVEL_STOP,   // nothing more for this sample
    LEVEL_SKIP,   // the level has just become ready: on to the next level without filtering
    LEVEL_FILTER, // filter this level, then the next one
};

template <bool IS_FIRST>
DECOMP_ALWAYS_INLINE s32 EnterLevel(FilterTreeControl& c, s32 level)
{
    if (IS_FIRST && !c.isFilled[level]) {
        if (static_cast<u16>(c.head[level]) != 13) {
            return LEVEL_STOP;
        }
        c.isFilled[level] = 1;
        return LEVEL_SKIP;
    }
    if (!c.isOdd[level]) {
        c.isOdd[level] = 1;
        return LEVEL_STOP;
    }
    return LEVEL_FILTER;
}

template <bool IS_FIRST>
DECOMP_ALWAYS_INLINE void FilterTreePush(s32 sample)
{
    FilterTreeControl& c = s_FilterControl;
    FilterTreeRings& r = s_FilterRings;
    AdvanceHead(c.head[0], 15);
    r.input[c.head[0]] = sample;
    s32 step = EnterLevel<IS_FIRST>(c, 0);
    if (step == LEVEL_STOP) {
        return;
    }
    if (step == LEVEL_FILTER) {
        AdvanceHead(c.head[1], 15);
        c.filter = s_FilterFuncs[c.head[0]];
        c.filter(r.input, &r.low1[c.head[1]], &r.high1[c.head[1]]);
        c.isOdd[0] = 0;
    }
    step = EnterLevel<IS_FIRST>(c, 1);
    if (step == LEVEL_STOP) {
        return;
    }
    if (step == LEVEL_FILTER) {
        AdvanceHead(c.head[2], 15);
        c.filter = s_FilterFuncs[c.head[1]];
        c.filter(r.low1, &r.low2[c.head[2]], &r.high2[c.head[2]]);
        c.filter(r.high1, &r.highHigh[c.head[2]], &r.lowHigh[c.head[2]]);
        c.isOdd[1] = 0;
    }
    step = EnterLevel<IS_FIRST>(c, 2);
    if (step == LEVEL_STOP) {
        return;
    }
    if (step == LEVEL_FILTER) {
        AdvanceHead(c.head[5], 63);
        AdvanceHead(c.head[3], 15);
        c.filter = s_FilterFuncs[c.head[2]];
        c.filter(r.low2, &r.low3[c.head[3]], &r.high3[c.head[3]]);
        c.filter(r.high2, &r.lowOfHigh2[c.head[3]], &r.highOfHigh2[c.head[3]]);
        c.filter(r.lowHigh, &r.band9[c.head[5]], &r.band10[c.head[5]]);
        c.filter(r.highHigh, &r.band12[c.head[5]], &r.band11[c.head[5]]);
        c.isOdd[2] = 0;
    }
    step = EnterLevel<IS_FIRST>(c, 3);
    if (step == LEVEL_STOP) {
        return;
    }
    if (step == LEVEL_FILTER) {
        AdvanceHead(c.head[6], 31);
        AdvanceHead(c.head[4], 15);
        c.filter = s_FilterFuncs[c.head[3]];
        c.filter(r.low3, &r.low4[c.head[4]], &r.high4[c.head[4]]);
        c.filter(r.high3, &r.band4[c.head[6]], &r.band3[c.head[6]]);
        c.filter(r.highOfHigh2, &r.band5[c.head[6]], &r.band6[c.head[6]]);
        c.filter(r.lowOfHigh2, &r.band8[c.head[6]], &r.band7[c.head[6]]);
        c.isOdd[3] = 0;
    }
    step = EnterLevel<IS_FIRST>(c, 4);
    if (step == LEVEL_STOP) {
        return;
    }
    if (step == LEVEL_FILTER) {
        AdvanceHead(c.head[7], 15);
        c.filter = s_FilterFuncs[c.head[4]];
        c.filter(r.low4, &r.low5[c.head[7]], &r.band0[c.head[7]]);
        c.filter(r.high4, &r.band2[c.head[7]], &r.band1[c.head[7]]);
        c.isOdd[4] = 0;
    }
}

// the sum of the magnitudes of the last num values of a ring
DECOMP_ALWAYS_INLINE s32 SumRing(const s32* ring, s32 head, s32 mask, s32 num)
{
    s32 pos = head & mask;
    s32 count = pos < num - 1 ? pos + 1 : num;
    s32 sum = 0;
    const s32* p = ring + pos;
    for (s32 i = count; i != 0; i--) {
        s32 value = *p--;
        if (value < 0) {
            sum -= value;
        } else {
            sum += value;
        }
    }
    p = ring + mask;
    for (s32 i = num - count; i > 0; i--) {
        s32 value = *p--;
        if (value < 0) {
            sum -= value;
        } else {
            sum += value;
        }
    }
    return sum;
}

// 0x001480B8
DECOMP_NOINLINE void CalcBands(s32* bands)
{
    FilterTreeControl& c = s_FilterControl;
    FilterTreeRings& r = s_FilterRings;
    s32 head = c.head[7] + 16;
    bands[0] = (SumRing(r.band0, head, 15, 6) * 21) >> 7;
    bands[1] = (SumRing(r.band1, head, 15, 6) * 21) >> 7;
    bands[2] = (SumRing(r.band2, head, 15, 6) * 21) >> 7;
    head = c.head[6] + 25;
    bands[3] = (SumRing(r.band3, head, 31, 12) * 10) >> 7;
    bands[4] = (SumRing(r.band4, head, 31, 12) * 10) >> 7;
    bands[5] = (SumRing(r.band5, head, 31, 12) * 10) >> 7;
    bands[6] = (SumRing(r.band6, head, 31, 12) * 10) >> 7;
    bands[7] = (SumRing(r.band7, head, 31, 12) * 10) >> 7;
    bands[8] = (SumRing(r.band8, head, 31, 12) * 10) >> 7;
    head = c.head[5] + 43;
    bands[9] = (SumRing(r.band9, head, 63, 24) * 5) >> 7;
    bands[10] = (SumRing(r.band10, head, 63, 24) * 5) >> 7;
    bands[11] = (SumRing(r.band11, head, 63, 24) * 5) >> 7;
    bands[12] = (SumRing(r.band12, head, 63, 24) * 5) >> 7;
}
} // namespace

// 0x00146844
void FilterFrame(const s32* samples, s32* bands)
{
    for (s32 i = 0; i < FRAME_SAMPLES; i++) {
        FilterTreePush<false>(samples[i]);
    }
    CalcBands(bands);
}

// 0x00146B74
void FilterFirstFrame(const s32* samples, s32* bands)
{
    FilterTreeControl& c = s_FilterControl;
    for (s32 i = 0; i < 5; i++) {
        c.head[i] = -1;
        c.isFilled[i] = 0;
        c.isOdd[i] = 1;
    }
    c.head[5] = -1;
    c.head[6] = -1;
    c.head[7] = -1;
    memset(&s_FilterRings, 0, sizeof(s_FilterRings));
    for (s32 i = 0; i < FIRST_FRAME_SAMPLES; i++) {
        FilterTreePush<true>(samples[i]);
    }
    CalcBands(bands);
}
} // namespace asr
