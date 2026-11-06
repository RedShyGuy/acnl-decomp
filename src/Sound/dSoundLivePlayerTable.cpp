#include "Sound/dSoundLivePlayer.h"

// (in a file of their own: GCC would otherwise fold the tables into the loops)
// 0x0083C0EC
const SoundLivePlayer::SpeedEvent SoundLivePlayer::s_DefaultSpeed = {0, 1.0f, 0};
// the number of notes read from each table
// 0x0083C0F8
const u32 SoundLivePlayer::s_NoteTableSizes[NOTE_TABLE_NUM] = {180, 80, 140, 70, 60, 120, 50, 520, 50, 80, 150};
// the track of the sequence of each table
// 0x0083C124
const u8 SoundLivePlayer::s_NoteTracks[NOTE_TABLE_NUM] = {2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13};
// what the show does for a note of each table
// 0x0083C130
const s16 SoundLivePlayer::s_NoteKinds[NOTE_TABLE_NUM] = {4, 2, 5, 3, 1, 4, 3, 3, 0, 4, 5};
