#pragma once

// The town tune of the save data: 16 notes of 4 bits in 8 bytes (SvTownData::townTune). All names are ours.

#include "decomp.h"

struct SoundMelody;

// the town tune of the loaded save data is copied (8 bytes); without valid save data a random default melody of SoundMgr
// is packed (it goes through the buffer g_SvDefaultTownTune)
void InitTownTune(u8* tune); // 0x006BCB9C
// 16 notes (low 4 bits each) are packed into 8 bytes, two notes per byte (the first in the low half); the counterpart of
// UnpackMelody (0x0076D700)
void PackMelody(u8* packed, const SoundMelody* melody); // 0x006BCC54

extern SoundMelody g_SvDefaultTownTune;
