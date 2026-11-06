#include "Sound/dSoundHaniwaMgr.h"

// (in a file of their own: GCC would otherwise resolve the call in Update and fold the table)
// 0x0083B92C
void (SoundHaniwaMgr::*const SoundHaniwaMgr::s_UpdateFuncs[2])() = {
    &SoundHaniwaMgr::UpdateLevel,
    &SoundHaniwaMgr::UpdateNothing,
};
// the sound players that count for the level
// 0x0083B93C
const u8 SoundHaniwaMgr::s_IsPlayerCounted[18] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0};
// 0x0083B950
const s32 SoundHaniwaMgr::s_StopFrames = 5;

#include "Sound/dSoundObjHaniwa.h"

// the note of the dance per group and level
// 0x0083ED08
const s16 SoundObjHaniwa::s_LevelNotes[GROUP_NUM][LEVEL_NUM] = {{7, 3, 1, 0}, {14, 6, 2, 0}, {28, 12, 4, 0}, {24, 16, 8, 0}};
