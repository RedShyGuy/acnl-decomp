#pragma once

#include "asr/asr_Internal.h"

// The interface for the application: errors are negative. Process gives the number of results at the end of an
// utterance, -7 nothing was said, -8 too long, -9 too loud; otherwise 128 once speech has started.
namespace asr {
u32 GetWorkSize(); // 0x00151F00
s32 Initialize(u8 language, File* files, s32 fileNum, void* workArea); // 0x00151F10
// out: the channel statistics for the next start, with a checksum in front (the size or an error)
DECOMP_NOINLINE s32 Finalize(void* out); // 0x00142BC4
s32 FinalizeEntry(void* out); // 0x00142BC0
s32 GetConfig(Config* config); // 0x00151E64
s32 SetConfig(Config* config); // 0x00152608
// lines "id:word" ended by "END"
s32 SetWords(const char* data, s16 size); // 0x00152474
s32 Start(); // 0x001523C4
s32 Process(const s16* samples, u32 count); // 0x00142CFC
s16 GetResult(s16 index); // 0x00151E0C
} // namespace asr
