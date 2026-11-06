#pragma once

#include "asr/asr_Internal.h"

namespace asr {
void AdaptMeans(Recognizer* rec); // 0x00145CE4
s32 SaveChannelStats(Recognizer* rec, void* out); // 0x00143BC4
void FinishRecognition(Recognizer* rec); // 0x00143C9C
void ResetRecognition(Recognizer* rec); // 0x0014664C
void UpdateChannelStats(Recognizer* rec); // 0x00147F74
// one frame in two steps; -2 when the frame is finished
s32 DecodeFrame(const s16* frame, Recognizer* rec); // 0x00145760

// the distances of a frame to the codebook: 70 percent in the first step, the rest in the second
void ModelCalcDistances(Model* model, const s16* frame, s32 from, s32 to); // 0x0014875C
void ModelStartDistances(const s16* frame, Model* model); // 0x001474BC
void ModelFinishDistances(const s16* frame, Model* model); // 0x00147490
s32 ModelBestStateScore(s32, s32, const Model* model); // 0x00147450

void SearchReset(Search* search); // 0x00147BE4
void SearchStep(const s16* frame, Search* search); // 0x00147544
void SearchGetBest(s16* words, s32* scores, s32* num, Search* search); // 0x001479FC
void GetResults(Recognizer* rec, s16* words, s32* scores, s32* num, s32* outScores); // 0x001466B8
} // namespace asr
