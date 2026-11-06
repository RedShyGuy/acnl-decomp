#pragma once

#include "asr/asr_Internal.h"

namespace asr {
struct Recognizer;

extern const u16 s_SqrtMantissa[64];
extern const u16 s_SqrtExponent[26];
extern const u16 s_InverseTable[64];
extern const s32 s_BandFloor[14];
extern const s16 s_DctTable[252];
extern const s32 s_DctShifts[12];
extern const u16 s_LogTable[128];

// the filter tree: samples -> 13 band magnitudes
void FilterFrame(const s32* samples, s32* bands); // 0x00146844
void FilterFirstFrame(const s32* samples, s32* bands); // 0x00146B74

// the fixed point logarithm of value (0 for 0)
DECOMP_NOINLINE s16 Log2(u32 value); // 0x00147F2C

void VadInitialize(Vad* vad, s32 bandNum, const VadParams* params); // 0x001589F8
void VadSetCalibration(s32 num, Vad* vad); // 0x00144FA0
void VadReset(Vad* vad); // 0x001456B0
void VadUpdate(FrameInfo* info, VadResult* result, Vad* vad); // 0x00144FAC
void VadResetLevels(Vad* vad); // 0x0014751C
DECOMP_NOINLINE void VadCalcNoise(s32 num, s32 flag, Vad* vad); // 0x00147CEC

// one frame of 36 values from the bands
void CalcCoefficients(Recognizer* rec, const FrameInfo* info, s16* out); // 0x00144E48
void CalcDeltas(Recognizer* rec, s32 frame, s32 offset); // 0x00145BB8

s32 ProcessFirstFrame(Recognizer* rec); // 0x00144800
s32 ProcessFrame(Recognizer* rec, VadResult** result); // 0x00143CD0
} // namespace asr
