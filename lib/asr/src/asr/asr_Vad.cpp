#include "asr/asr_Frontend.h"
#include <cstring>

// The voice activity detection: the first frames measure the noise, later frames compare four groups of band energies
// against it.
namespace asr {
namespace {
const u32 INITIAL_MIN_ENERGY = 5000;
const s32 INITIAL_RELATIVE_LEVEL = 500;
const s32 INITIAL_MAX_LEVEL = -100;

// after 25 silent frames the noise model follows the frame 25 frames ago
const s32 NOISE_RING = 25;

DECOMP_ALWAYS_INLINE u32 SumEnergy(const u32* energy)
{
    return energy[2] + energy[3] + energy[1] + energy[0];
}
} // namespace

// 0x00147F2C
s16 Log2(u32 value)
{
    if (value == 0) {
        return 0;
    }
    s16 exponent = 0;
    while ((value & ~0x7Fu) != 0) {
        exponent++;
        value >>= 1;
    }
    return s_LogTable[static_cast<s16>(value)] + (exponent << 10);
}

// 0x001589F8
void VadInitialize(Vad* vad, s32 bandNum, const VadParams* params)
{
    memset(vad, 0, sizeof(Vad));
    vad->_7C = params;
    vad->minEnergy = INITIAL_MIN_ENERGY;
    vad->params = params;
    vad->bandNum = bandNum;
    VadReset(vad);
}

// 0x00144FA0
void VadSetCalibration(s32 num, Vad* vad)
{
    vad->calibrationNum = num;
    vad->calibrationLeft = num;
}

// 0x001456B0
void VadReset(Vad* vad)
{
    memset(vad->isVoiced, 0, sizeof(vad->isVoiced));
    memset(vad->history, 0, sizeof(vad->history));
    for (s32 i = 0; i < 4; i++) {
        vad->noise[i] = 0;
        vad->inverseVariance[i] = 0;
    }
    vad->averagePeak = 0;
    vad->calibrationNum = 0;
    vad->noiseLevel = 0;
    vad->isSpeech = 0;
    vad->dcOffset = 0;
    vad->sampleNum = 0;
    vad->ringNum = 0;
    vad->ringPos = 0;
    vad->maxLevel = INITIAL_MAX_LEVEL;
    vad->minEnergy = INITIAL_MIN_ENERGY;
    vad->relativeLevel = INITIAL_RELATIVE_LEVEL;
    vad->threshold = vad->params->threshold;
    memset(vad->sortedPeak, 0, sizeof(vad->sortedPeak));
    memset(vad->entries, 0, sizeof(vad->entries));
}

// 0x0014751C
void VadResetLevels(Vad* vad)
{
    vad->maxLevel = INITIAL_MAX_LEVEL;
    vad->minEnergy = INITIAL_MIN_ENERGY;
    vad->relativeLevel = INITIAL_RELATIVE_LEVEL;
    vad->threshold = vad->params->threshold;
}

// 0x00147CEC
void VadCalcNoise(s32, s32 flag, Vad* vad)
{
    if (flag != 1) {
        return;
    }
    u32 median = (vad->entries[7].sum >> 1) + (vad->entries[8].sum >> 1);
    s32 num = 16;
    while (vad->entries[num - 1].sum / 10 > median) {
        if (--num == 0) {
            return;
        }
    }
    if (num == 0) {
        return;
    }
    for (s32 i = num - 1; i >= 0; i--) {
        const VadEntry& entry = vad->entries[i];
        for (s32 j = 3; j >= 0; j--) {
            u32 value = entry.band[j];
            vad->noise[j] += value;
            s32 scaled = static_cast<s32>(value) >> 6;
            vad->inverseVariance[j] += scaled * scaled;
        }
    }
    u32 total = 0;
    for (s32 i = 0; i < 4; i++) {
        u32 mean = vad->noise[i] / num;
        u32 scaled = mean >> 6;
        vad->noise[i] = mean;
        u32 variance = vad->inverseVariance[i] / num;
        vad->inverseVariance[i] = variance;
        s32 difference = variance - scaled * scaled;
        if (difference < 0) {
            difference = 0;
        }
        total += static_cast<s32>(mean) >= 0 ? mean : -mean;
        vad->inverseVariance[i] = difference;
        if (static_cast<u32>(difference) > 0x100000) {
            vad->inverseVariance[i] = 0x100000;
        } else if (static_cast<u32>(difference) <= 10) {
            vad->inverseVariance[i] = 10;
        }
        if (mean <= 5) {
            mean = 5;
        }
        vad->noise[i] = mean;
        // the inverse of the square root of the variance as a mantissa and a shift
        s32 value = vad->inverseVariance[i];
        s32 exponent = 0;
        while ((value & ~0x3F) != 0) {
            exponent++;
            value >>= 1;
        }
        value = s_SqrtMantissa[value] * s_SqrtExponent[exponent];
        exponent = 0;
        while ((value & ~0x3F) != 0) {
            exponent++;
            value >>= 1;
        }
        vad->inverseShift[i] = exponent + 9;
        vad->inverseVariance[i] = s_InverseTable[value] + 1;
    }
    s32 level = Log2(SumEnergy(vad->noise));
    vad->noiseLevel = (level * 3) >> 10;
    if (total < 10) {
        vad->threshold = 1;
    }
    vad->dcOffset = vad->dcOffset / static_cast<s32>(vad->sampleNum);
    vad->averagePeak = (vad->sortedPeak[7] + vad->sortedPeak[8]) >> 1;
    vad->noiseEnergy = SumEnergy(vad->noise);
}

namespace {
DECOMP_ALWAYS_INLINE u32 CalcGroupScore(const Vad* vad, s32 group)
{
    s32 difference = vad->energy[group] - vad->noise[group];
    if (difference <= 0) {
        difference = 0;
    }
    u32 score = vad->inverseVariance[group] * difference;
    s32 shift = vad->inverseShift[group];
    return shift > 0 ? score >> shift : score << -shift;
}
} // namespace

// 0x00144FAC
void VadUpdate(FrameInfo* info, VadResult* result, Vad* vad)
{
    s32 groupBands = vad->bandNum >> 2;
    s16* samples = info->samples;
    s32 peak = 0;
    for (s32 i = 1; i < info->sampleNum + 1; i++) {
        if (vad->calibrationLeft != 0) {
            vad->dcOffset += *samples;
        } else {
            *samples -= vad->dcOffset;
        }
        s32 sample = *samples++;
        s32 magnitude = sample >= 0 ? sample : -sample;
        s32 peakMagnitude = peak >= 0 ? peak : -peak;
        if (magnitude > peakMagnitude) {
            peak = sample;
        }
    }
    if (vad->calibrationLeft != 0) {
        vad->sampleNum += info->sampleNum;
    }
    vad->peak = peak >= 0 ? peak : -peak;
    s32 band = 0;
    for (s32 group = 0; group < 4; group++) {
        u32 sum = 0;
        for (s32 i = 0; i < groupBands; i++) {
            s32 value = info->band[band + i] >> 7;
            sum += value * value;
        }
        vad->energy[group] = sum >> 1;
        band += groupBands;
    }

    if (vad->calibrationLeft != 0) {
        s32 index = vad->calibrationNum - vad->calibrationLeft;
        vad->sortedPeak[index] = vad->peak;
        for (s32 i = index; i > 0 && vad->sortedPeak[i - 1] > vad->sortedPeak[i]; i--) {
            s16 peak0 = vad->sortedPeak[i - 1];
            vad->sortedPeak[i - 1] = vad->sortedPeak[i];
            vad->sortedPeak[i] = peak0;
        }
        index = vad->calibrationNum - vad->calibrationLeft;
        vad->entries[index].sum = SumEnergy(vad->energy);
        for (s32 i = 0; i < 4; i++) {
            vad->entries[index].band[i] = vad->energy[i];
        }
        for (s32 i = index; i > 0 && vad->entries[i].sum < vad->entries[i - 1].sum; i--) {
            VadEntry entry = vad->entries[i];
            vad->entries[i] = vad->entries[i - 1];
            vad->entries[i - 1] = entry;
        }
        VadCalcNoise(vad->calibrationNum, 0, vad);
        if (--vad->calibrationLeft == 0) {
            VadCalcNoise(vad->calibrationNum, 1, vad);
            if (result == NULL) {
                return;
            }
            result->state = vad->noiseLevel > vad->params->maxLevel ? VAD_TOO_LOUD : 0;
        } else {
            if (result == NULL) {
                return;
            }
            result->state = 0;
        }
        result->relativeLevel = INITIAL_RELATIVE_LEVEL;
        result->maxLevel = 0;
        return;
    }

    u32 score = CalcGroupScore(vad, 0) + CalcGroupScore(vad, 1) + CalcGroupScore(vad, 2) + CalcGroupScore(vad, 3);
    u32 total = SumEnergy(vad->energy);
    switch (vad->params->mode) {
    case 2: {
        if (static_cast<s32>(total) < static_cast<s32>(vad->minEnergy)) {
            score = 0;
        }
        s32 limit = vad->averagePeak * 7;
        if (limit < 6000) {
            limit /= 4;
        } else {
            limit = 1500;
        }
        if (limit > vad->peak) {
            score = 0;
        }
        break;
    }
    case 3: {
        u32 averagePeak = vad->averagePeak;
        u32 noise = vad->noiseEnergy;
        u32 averagePeak3 = averagePeak * 3;
        bool isQuiet = averagePeak3 <= noise;
        s32 minTotal = noise >= 5000 ? 150000 : 5000;
        s32 limit;
        if (noise > 1500) {
            s32 half = static_cast<s32>(averagePeak3 * 4) >> 3;
            limit = 1500 < half ? half : 1500;
            score = score > 25 ? 1 : 0;
        } else {
            s32 base = isQuiet ? 5000 : 1500;
            limit = averagePeak3;
            if (base <= static_cast<s32>(averagePeak3)) {
                limit = isQuiet ? 5000 : 1500;
            }
            score = 1;
        }
        if (static_cast<s32>(total) < minTotal) {
            score = 0;
        }
        if (vad->peak < limit) {
            score = 0;
        }
        break;
    }
    }

    u8 isVoiced = vad->threshold <= score;
    u8 voiced0 = vad->isVoiced[0];
    u8 voiced1 = vad->isVoiced[1];
    u8 voiced2 = vad->isVoiced[2];
    u8 voiced3 = vad->isVoiced[3];
    vad->isVoiced[0] = isVoiced;
    vad->isVoiced[4] = voiced3;
    vad->isVoiced[3] = voiced2;
    vad->isVoiced[2] = voiced1;
    vad->isVoiced[1] = voiced0;
    u8 isAllVoiced = isVoiced && voiced0 && voiced1 && voiced2 && voiced3;
    for (s32 i = 14; i > 0; i--) {
        vad->history[i] = vad->history[i - 1];
    }
    vad->history[0] = isAllVoiced;
    bool isAny = false;
    for (s32 i = 0; i < 15; i++) {
        if (vad->history[i] != 0) {
            isAny = true;
        }
    }

    if (isAny) {
        s32 level = -1 + ((Log2(SumEnergy(vad->energy)) * 3) >> 10);
        if (vad->maxLevel < level) {
            vad->maxLevel = level;
        }
        vad->relativeLevel = vad->maxLevel - vad->noiseLevel;
        vad->ringNum = 0;
        vad->isSpeech = 1;
        vad->ringPos = 0;
        result->state = VAD_SPEECH;
    } else {
        vad->peakRing[vad->ringPos] = vad->peak;
        for (s32 i = 0; i < 4; i++) {
            vad->energyRing[vad->ringPos][i] = vad->energy[i];
        }
        vad->ringPos = (vad->ringPos + 1) % NOISE_RING;
        if (++vad->ringNum > NOISE_RING) {
            vad->averagePeak = ((vad->averagePeak * 15) >> 4) + (vad->peakRing[vad->ringPos] >> 4);
            for (s32 i = 3; i >= 0; i--) {
                vad->noise[i] = (vad->energyRing[vad->ringPos][i] >> 4) + ((vad->noise[i] * 15) >> 4);
            }
        }
        result->state = VAD_SILENCE;
    }
    result->relativeLevel = vad->relativeLevel;
    result->maxLevel = vad->maxLevel;
    if (vad->noiseLevel > vad->params->maxLevel) {
        result->state = VAD_TOO_LOUD;
    }
}
} // namespace asr
