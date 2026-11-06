#include "asr/asr_Frontend.h"
#include "asr/asr_Search.h"
#include <cstring>

// One frame: gain, pre-emphasis, filter tree, voice activity detection, coefficients, adaption.
namespace asr {
namespace {
const s32 PRE_EMPHASIS = 0xFAE1;
const s32 BAND_FLOOR_FRAMES = 5;
// decodeFrame while nothing is left to decode
const s32 DECODE_FRAME_NONE = 30;
// the frame the search looks at: FRAME_RING - DECODE_DELAY frames back
const s32 DECODE_DELAY = 19;

DECOMP_ALWAYS_INLINE s32 WrapFrame(s32 frame)
{
    return frame % FRAME_RING;
}

DECOMP_ALWAYS_INLINE s32 Saturate(s32 value)
{
    if (value >= 32768) {
        return 32767;
    }
    if (value <= -32768) {
        return -32767;
    }
    return value;
}

// from the last sample to the first; the first one uses the last sample of the previous frame
DECOMP_ALWAYS_INLINE void PreEmphasize(Recognizer* rec, s32 num)
{
    const s16* samples = rec->samples;
    s16 last = samples[num - 1];
    for (s32 i = num - 1; i > 0; i--) {
        rec->emphasized[i] = (samples[i] << 8) - ((samples[i - 1] * PRE_EMPHASIS) >> 8);
    }
    rec->emphasized[0] = (samples[0] << 8) - ((rec->previousSample * PRE_EMPHASIS) >> 8);
    rec->previousSample = last;
}
} // namespace

// 0x00144E48
void CalcCoefficients(Recognizer* rec, const FrameInfo* info, s16* out)
{
    Dct& dct = rec->dct;
    for (s32 i = 0; i < dct.inputNum; i++) {
        if (info->band[i] != 0) {
            dct.logBand[i] = (Log2(info->band[i]) - 8192) * 2;
        }
    }
    for (s32 i = 0; i < dct.outputNum; i++) {
        s32 sum = 0;
        const s16* row = &s_DctTable[i * 13];
        for (s32 j = 0; j < dct.inputNum; j++) {
            sum += dct.logBand[j] * row[j];
        }
        out[i] = ((sum >> 7) * 88) >> s_DctShifts[i];
    }
    out[0] += 192;
}

// 0x00145BB8
void CalcDeltas(Recognizer* rec, s32 frame, s32 offset)
{
    // the frame two back gets the slope over the five frames around it
    const s16* f0 = &rec->ring[frame][offset];
    const s16* f1 = &rec->ring[WrapFrame(frame + 28)][offset];
    const s16* f3 = &rec->ring[WrapFrame(frame + 26)][offset];
    const s16* f4 = &rec->ring[WrapFrame(frame + 25)][offset];
    s16* out = &rec->ring[WrapFrame(frame + 27)][offset + COEFFICIENT_NUM];
    for (s32 i = 0; i < COEFFICIENT_NUM; i++) {
        s32 value = f1[i] - (f3[i] + f4[i] * 2) + f0[i] * 2;
        out[i] = (value >> 3) + (value >> 4) + (value >> 6);
    }
}

// 0x00145CE4
void AdaptMeans(Recognizer* rec)
{
    const s16* weights;
    s16* means = rec->model.means;
    s32 codewordNum = rec->model.codewordNum;
    switch (rec->adaptStep) {
    case 1: {
        s32 difference[COEFFICIENT_NUM];
        const s32* mean = rec->channelMean;
        if (rec->floorSum > 32768) {
            for (s32 i = 0; i < COEFFICIENT_NUM; i++) {
                difference[i] = rec->silenceSum[i] - mean[i] - ((rec->stats[i] * 6) >> 3);
            }
        } else {
            for (s32 i = 0; i < COEFFICIENT_NUM; i++) {
                difference[i] = rec->silenceSum[i] - mean[i];
            }
        }
        for (s32 stream = 0; stream < 3; stream++) {
            const s16* matrix = rec->adaption[stream].matrix;
            for (s32 row = 0; row < 5; row++) {
                for (s32 j = 0; j < COEFFICIENT_NUM; j++) {
                    const s16* line = &matrix[row * 144 + j * 12];
                    s32 sum = 0;
                    for (s32 k = 0; k < COEFFICIENT_NUM; k++) {
                        sum += line[k] * difference[k];
                    }
                    s32 shift = GetVectorShift(j) + 3;
                    rec->adaptOffsets[stream][row][j] = shift > 28 ? sum << (shift - 28) : sum >> (28 - shift);
                }
            }
        }
        weights = rec->adaption[0].weights;
        for (s32 i = 0; i < codewordNum; i++) {
            s16 offset[COEFFICIENT_NUM] = {0};
            for (s32 row = 0; row < 5; row++) {
                s32 weight = *weights++;
                for (s32 j = 0; j < COEFFICIENT_NUM; j++) {
                    offset[j] += (rec->adaptOffsets[0][row][j] * weight) >> 11;
                }
            }
            for (s32 j = 0; j < COEFFICIENT_NUM; j++) {
                means[j] += offset[j];
            }
            means += VECTOR_NUM;
        }
        rec->adaptStep = 2;
        break;
    }
    case 2:
        weights = rec->adaption[1].weights;
        for (s32 i = 0; i < codewordNum; i++) {
            s16 offset[COEFFICIENT_NUM] = {0};
            for (s32 row = 0; row < 5; row++) {
                s32 weight = *weights++;
                for (s32 j = 0; j < COEFFICIENT_NUM; j++) {
                    offset[j] += (rec->adaptOffsets[1][row][j] * weight) >> 11;
                }
            }
            for (s32 j = 0; j < COEFFICIENT_NUM; j++) {
                means[COEFFICIENT_NUM + j] += offset[j];
            }
            means += VECTOR_NUM;
        }
        rec->adaptStep = 3;
        break;
    case 3:
        weights = rec->adaption[2].weights;
        for (s32 i = 0; i < codewordNum; i++) {
            s16 offset[COEFFICIENT_NUM] = {0};
            for (s32 row = 0; row < 5; row++) {
                s32 weight = *weights++;
                for (s32 j = 0; j < COEFFICIENT_NUM; j++) {
                    offset[j] += (rec->adaptOffsets[2][row][j] * weight) >> 11;
                }
            }
            for (s32 j = 0; j < COEFFICIENT_NUM; j++) {
                means[COEFFICIENT_NUM * 2 + j] += offset[j];
            }
            means += VECTOR_NUM;
        }
        rec->adaptStep = -2;
        break;
    }
}

// 0x00143CD0
s32 ProcessFrame(Recognizer* rec, VadResult** result)
{
    s32 isStarted = 0;
    if (rec->isAgc == 1) {
        for (s32 i = 1; i < FRAME_SAMPLES + 1; i++) {
            s16& sample = rec->samples[i - 1];
            s32 value = sample;
            s32 magnitude = value >= 0 ? value : -value;
            if (rec->peakSample <= magnitude) {
                rec->peakSample = magnitude;
            }
            sample = Saturate((sample * rec->gain) >> 8);
        }
    }
    PreEmphasize(rec, FRAME_SAMPLES);
    FilterFrame(rec->emphasized, rec->frame.band);
    VadUpdate(&rec->frame, &rec->result, &rec->vad);
    *result = &rec->result;
    if (!rec->isAdaption) {
        rec->result.state = VAD_SPEECH;
    }

    if (rec->bandFloorNum < BAND_FLOOR_FRAMES) {
        for (s32 i = rec->frame.bandNum - 1; i >= 0; i--) {
            rec->bandFloor[i] += rec->frame.band[i];
        }
        if (++rec->bandFloorNum == BAND_FLOOR_FRAMES) {
            rec->floorSum = 0;
            for (s32 i = rec->frame.bandNum - 1; i >= 0; i--) {
                rec->bandFloor[i] /= BAND_FLOOR_FRAMES;
                rec->floorSum += rec->bandFloor[i];
                rec->bandFloor[i] = s_BandFloor[i] > rec->bandFloor[i] ? s_BandFloor[i] - rec->bandFloor[i] : 0;
            }
        }
    } else {
        for (s32 i = rec->frame.bandNum - 1; i >= 0; i--) {
            rec->frame.band[i] += rec->bandFloor[i];
        }
    }

    switch (rec->result.state) {
    case VAD_SPEECH:
        if (rec->adaptStep != 0) {
            break;
        }
        rec->adaptStep = 1;
        if (!rec->isSilenceDone) {
            for (s32 i = 0; i < COEFFICIENT_NUM; i++) {
                rec->silenceSum[i] = (rec->silenceSum[i] << (8 - GetVectorShift(i))) / rec->silenceNum;
            }
            AdaptMeans(rec);
            rec->isSilenceDone = 1;
        }
        if (rec->isAdaption && rec->frameCount > 0) {
            s32 back = rec->frameCount >= 15 ? 15 : rec->frameCount;
            rec->decodeFrame = WrapFrame(rec->frameIndex - back + DECODE_DELAY);
        }
        isStarted = 1;
        break;
    case VAD_TOO_LOUD:
        return -4;
    }

    CalcCoefficients(rec, &rec->frame, rec->ring[rec->frameIndex]);
    if (rec->result.state == VAD_SILENCE && rec->adaptStep == 0) {
        const s16* frame = rec->ring[WrapFrame(rec->frameIndex + DECODE_DELAY)];
        for (s32 i = 0; i < COEFFICIENT_NUM; i++) {
            rec->silenceSum[i] += frame[i];
        }
        rec->silenceNum++;
    }
    CalcDeltas(rec, rec->frameIndex, 0);
    CalcDeltas(rec, WrapFrame(rec->frameIndex + 27), COEFFICIENT_NUM);

    s32 count = rec->frameCount++;
    if (count >= 0 && rec->adaptStep != 0) {
        if (rec->decodeFrame == DECODE_FRAME_NONE) {
            const s16* frame = rec->ring[WrapFrame(rec->frameIndex + DECODE_DELAY)];
            while (DecodeFrame(frame, rec) != -2) {
            }
        } else if (rec->adaptStep == -2) {
            s32 last = WrapFrame(rec->frameIndex + DECODE_DELAY);
            for (s32 i = rec->search.frameNum > -1 ? 2 : 1; i >= 0; i--) {
                if (DecodeFrame(rec->ring[rec->decodeFrame], rec) == -2) {
                    if (rec->decodeFrame == last) {
                        rec->decodeFrame = DECODE_FRAME_NONE;
                        break;
                    }
                    rec->decodeFrame = WrapFrame(rec->decodeFrame + 1);
                }
            }
        } else if (!isStarted) {
            AdaptMeans(rec);
            if (rec->adaptStep == -2) {
                s32 num = rec->model.codewordNum;
                s16* means = rec->model.means;
                s16 offset[COEFFICIENT_NUM];
                for (s32 i = COEFFICIENT_NUM - 1; i >= 0; i--) {
                    offset[i] = (rec->stats[i] * 10) >> (11 - GetVectorShift(i));
                }
                for (s32 i = 0; i < num; i++) {
                    for (s32 j = 0; j < COEFFICIENT_NUM; j++) {
                        means[j] += offset[j];
                    }
                    means += VECTOR_NUM;
                }
            }
        }
    }
    rec->frameIndex = WrapFrame(rec->frameIndex + 1);
    return 0;
}

// 0x00144800
s32 ProcessFirstFrame(Recognizer* rec)
{
    rec->previousSample = 0;
    rec->adaptStep = 0;
    rec->_691C = 0;
    rec->decodeFrame = DECODE_FRAME_NONE;
    if (rec->params->threshold == 0) {
        rec->isAdaption = 0;
    }
    rec->frameIndex = 0;
    for (s32 i = 0; i < FRAME_RING; i++) {
        memset(rec->ring[i], 0, sizeof(rec->ring[i]));
    }
    rec->isSilenceDone = 0;
    memcpy(rec->model.means, rec->originalMeans, rec->model.codewordNum * sizeof(s16) * VECTOR_NUM);
    for (s32 i = 0; i < COEFFICIENT_NUM; i++) {
        rec->silenceSum[i] = 0;
    }
    rec->silenceNum = 0;
    VadReset(&rec->vad);
    VadSetCalibration(16, &rec->vad);
    memset(rec->bandFloor, 0, rec->frame.bandNum * sizeof(s32));
    rec->bandFloorNum = 0;
    if (rec->isAgc == 1) {
        if (rec->peakSample > 1024) {
            rec->gain = (9000 / (rec->peakSample >> 8) + rec->gain * 7) >> 3;
        }
        rec->peakSample = 0;
        for (s32 i = 1; i < FIRST_FRAME_SAMPLES + 1; i++) {
            s16& sample = rec->samples[i - 1];
            s32 value = sample;
            s32 magnitude = value >= 0 ? value : -value;
            if (rec->peakSample <= magnitude) {
                rec->peakSample = magnitude;
            }
            sample = Saturate((sample * rec->gain) >> 8);
        }
    }
    rec->previousSample = rec->samples[0];
    PreEmphasize(rec, FIRST_FRAME_SAMPLES);
    FilterFirstFrame(rec->emphasized, rec->frame.band);
    rec->frame.sampleNum = FIRST_FRAME_SAMPLES;
    VadUpdate(&rec->frame, &rec->result, &rec->vad);
    rec->frame.sampleNum = FRAME_SAMPLES;
    CalcCoefficients(rec, &rec->frame, rec->ring[rec->frameIndex]);
    rec->frameIndex = 1;
    rec->frameCount = -21;
    rec->startScore = 0;
    SearchReset(&rec->search);
    rec->decodeStep = 0;
    return 0;
}

// 0x00143BC4
s32 SaveChannelStats(Recognizer* rec, void* out)
{
    if (rec->speechNum == 0) {
        return 0;
    }
    for (s32 i = 0; i < COEFFICIENT_NUM; i++) {
        rec->speechSum[i] = rec->speechSum[i] / rec->speechNum * 700;
        rec->savedStats[i] = rec->savedStats[i] / rec->speechNum * 700;
    }
    memcpy(out, rec->speechSum, sizeof(rec->speechSum) + sizeof(rec->defaultStats));
    return 96;
}

// 0x00143C9C
void FinishRecognition(Recognizer* rec)
{
    rec->isReady = 1;
    if (rec->originalMeans != NULL) {
        rec->model.means = rec->originalMeans;
    }
}

// 0x0014664C
void ResetRecognition(Recognizer* rec)
{
    rec->adaptStep = 0;
    VadResetLevels(&rec->vad);
    memset(rec->bandFloor, 0, rec->frame.bandNum * sizeof(s32));
    rec->bandFloorNum = 0;
    SearchReset(&rec->search);
    rec->decodeStep = 0;
    rec->startScore = 0;
}

// 0x00147F74
void UpdateChannelStats(Recognizer* rec)
{
    if (rec->speechNum == 0) {
        return;
    }
    for (s32 i = 0; i < COEFFICIENT_NUM; i++) {
        rec->stats[i] = (rec->speechSum[i] - rec->savedStats[i]) / rec->speechNum;
    }
    if (rec->speechNum > 1400) {
        for (s32 i = 0; i < COEFFICIENT_NUM; i++) {
            rec->speechSum[i] = rec->speechSum[i] / rec->speechNum * 700;
            rec->savedStats[i] = rec->savedStats[i] / rec->speechNum * 700;
        }
        rec->speechNum = 700;
    }
}

// 0x00145760
s32 DecodeFrame(const s16* frame, Recognizer* rec)
{
    Model* model = &rec->model;
    if (rec->decodeStep == 0) {
        ModelStartDistances(frame, model);
        rec->decodeStep = 1;
        return 0;
    }
    if (rec->decodeStep != 1) {
        return -1;
    }
    ModelFinishDistances(frame, model);
    SearchStep(frame, &rec->search);
    rec->startScore += ModelBestStateScore(0, 2, model);
    s32 bestScore = SCORE_NONE;
    u32 best = 0;
    s32 senoneNum = model->senoneNum;
    for (s32 i = senoneNum - 1; i >= 0; i--) {
        s32 score = model->senoneScores[i];
        if (score != 0 && bestScore <= score) {
            bestScore = score;
            best = i;
        }
    }
    if (bestScore != SCORE_NONE) {
        s32 streamNum = model->streamNum;
        s32 index = 0;
        for (s32 stream = 0; stream < streamNum && index < COEFFICIENT_NUM; stream++) {
            u32 codeword;
            if (model->codewordNum == 256) {
                codeword = static_cast<const u8*>(model->senoneCodewords)[best * streamNum + stream];
            } else {
                codeword = static_cast<const u16*>(model->senoneCodewords)[best * streamNum + stream];
            }
            const s16* mean = &model->means[codeword * VECTOR_NUM + index];
            for (s32 i = VECTOR_NUM / streamNum; index < COEFFICIENT_NUM && i != 0; i--) {
                rec->savedStats[index] += *mean++ << (8 - GetVectorShift(index));
                index++;
            }
        }
        for (s32 i = COEFFICIENT_NUM - 1; i >= 0; i--) {
            rec->speechSum[i] += frame[i] << (8 - GetVectorShift(i));
        }
        rec->speechNum++;
    }
    rec->decodeStep = 0;
    return -2;
}
} // namespace asr
