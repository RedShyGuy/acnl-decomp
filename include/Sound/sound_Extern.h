#pragma once

#include "decomp.h"
#include "nn/fnd/fnd_DateTime.h"
#include "nn/math/math_Vector3.h"

// game functions that the sound code calls (not decompiled yet; names after the address until their meaning is known)

// the rain level of the weather (0 none; the rain sounds are full at 32, the heavy rain at 128)
s32 Func22A20C(); // 0x0022A20C
// the kind of weather of the current date (3: rain, 4: heavy rain for the sounds)
s32 Func62FF00(); // 0x0062FF00
// sets up the biquad filters of the game (SoundBiquadFilterWithFade, BiquadNwAdaptor)
void Func321FD4(); // 0x00321FD4
// a state of the date for Club LOL (0 or 1; read with 0x56CF44)
s32 Func1668A0(); // 0x001668A0
// m_GroundCover of EnterStage (a value of 0x1E6444 is 5..7 or 9)
bool Func6305DC(); // 0x006305DC
bool Func63061C(); // 0x0063061C
// a curve from (x0, y0) to (x1, y1)
f32 Func4DC39C(s32 kind, f32 x, f32 x0, f32 x1, f32 y0, f32 y1, f32 curve); // 0x004DC39C
// the lights of the club follow the music (only in the stage of the club)
void Func4E7AAC(s32 mode, s32 arg); // 0x004E7AAC
// the voice of the player: true BOY (pitch 1.1), false GAL (pitch 1.05) (a byte of the player data at 0xAAFF18)
bool Func5C116C(); // 0x005C116C
// a world position relative to the camera (camera::Mgr view matrix +0x29C; zero without a camera)
void Func1E5E50(nn::math::VEC3* out, const nn::math::VEC3* world); // 0x001E5E50
// a music object of the game at 0x950320 (the requests of kind 13 of the BGM scheduler start and stop it)
struct Unknown950320;
extern Unknown950320* g_p950320;
void Func694AC8(Unknown950320* obj, u8 id); // 0x00694AC8
void Func694770(Unknown950320* obj); // 0x00694770
// the weather of the hour (0..6; the music follows it)
u8 Func62FB14(); // 0x0062FB14
// the 16 notes of a tune packed into 8 bytes
struct SoundMelody;
void UnpackMelody(const u8* packed, SoundMelody* melody); // 0x0076D700
// the night owl ordinance is on (a byte of SvTownFlags; called with &townData.unk_0xEE2C, which it does not read)
bool Func759E24(const void* obj); // 0x00759E24
// the event of the game that is going on (event::EventId)
u8 Func3189EC(); // 0x003189EC
// the event of the game is on at the date
bool Func309C6C(u8 id, const nn::fnd::DateTimeParameters* date); // 0x00309C6C
// another event of the game that is going on (event::EventId)
u8 Func30BA74(); // 0x0030BA74
// the cleaning runs: 0x951718 mode 3, its counter +0x10 above 0, flag5 clear, bit 3 and not bit 6 of the byte at town +0x1CA90
// (the music of EvOpusCleaning; no date in here)
bool Func2932E4(); // 0x002932E4
// the player can be seen (flags of the player actor and of 0xAAEAD4)
u8 Func5C68C8(); // 0x005C68C8
// the weather of the next hour (0..6)
u8 Func62FEC8(); // 0x0062FEC8
// a state object of the game at 0x951718 (meaning unknown)
struct Unknown951718
{
    u8 mode;   // 0x00
    u8 _1[4];  // 0x01
    u8 flag5;  // 0x05
    u8 _6[10]; // 0x06
    void* p10; // 0x10
};
extern Unknown951718 g_Unknown951718;
// the position of the player actor (NULL without one; PlayerGetActor + 0x14)
const nn::math::VEC3* Func5C37B0(u8 player, bool flag); // 0x005C37B0
// two kinds of the field at the position (StageOpusVillageDowntown compares them with 1/5 and 4)
void Func307340(s32* kindX, s32* kindZ, const nn::math::VEC3* pos); // 0x00307340
// a state of the game for StageOpusDream (bool)
bool Func6A6640(); // 0x006A6640
// a state of the game for StageOpusShowroom (bool)
bool Func4E212C(); // 0x004E212C
// a state of the game for StageOpusPrologue2 (1: the first arrival?)
s32 Func5C58C0(); // 0x005C58C0
// a table of the game at 0xA8449C: +4 points to 399 entries of 88 bytes with an item at +4 (StageOpusPavilion)
struct UnknownA8449CEntry
{
    u32 _0;
    u8 item[4]; // an SvFgName
    u8 _8[0x50];
};
struct UnknownA8449C
{
    u32 _0;
    UnknownA8449CEntry* entries;
};
extern UnknownA8449C g_UnknownA8449C;
// a game object at 0x9516C0 (+0 u8 mode, +4 pointer); StageOpusVillageDowntown and SubState call Func6DDC18/Func6DDE90
struct Unknown9516C0
{
    u8 mode;
    u8 _1[3];
    void* p4;
};
extern Unknown9516C0 g_Unknown9516C0;
void Func6DDC18(void* obj); // 0x006DDC18
void Func6DDE90(void* obj); // 0x006DDE90
// a value of the stage (StageOpusMgr::SelectOpus passes it to Func300928)
s32 Func31336C(u8 stage); // 0x0031336C
// a check on the villagers of the town with that value (Thumb)
bool Func300928(void* npcs, s32 value); // 0x00300928
// a state of the game (Thumb)
bool Func11A3A8(); // 0x0011A3A8
// a flag of the game at 0x951157 (StageOpusMgr::SelectOpus: the opus 46)
extern u8 g_Unknown951157;
// the speech recognition library (no symbols; SoundMicAsr calls it)
s32 Func142CFC(const s16* samples, u32 count); // 0x00142CFC
void Func142BC0(s32 arg); // 0x00142BC0
u32 Func151F00(); // 0x00151F00
s16 Func151E0C(s16 index); // 0x00151E0C
s32 Func151E64(s16* config); // 0x00151E64
s32 Func152608(s16* config); // 0x00152608
s32 Func152474(const void* data, s16 size); // 0x00152474
s32 Func1523C4(); // 0x001523C4
s32 Func151F10(u8 language, void* files, s32 fileNum, void* work); // 0x00151F10
