#pragma once

#include "decomp.h"

// (game code, not decompiled yet; names after the addresses) what BsScene calls
// the fade: done / ready for the next step (Func566AD0 is in Bs/dBsSvMgr.h)
bool Func566B2C(); // 0x00566B2C
// starts a fade of the two types (the last argument is always 0)
void Func6A67E4(u8 typeA, u8 typeB, u32 value); // 0x006A67E4
void Func6A66D8(u32 param); // 0x006A66D8 (the fade out of a change)
void Func6A6660(u32 param); // 0x006A6660 (the fade in of a change)
// the states of BlackWait
bool Func6CF2C0(); // 0x006CF2C0
bool Func6CFA78(); // 0x006CFA78
bool Func6CFA2C(); // 0x006CFA2C

// an object of the game (pointer at 0x0095269C) with a state byte at +0xE24 that CanChange asks (meaning unknown)
struct Unknown95269C
{
    u8 _unknown0[0xE24];
    u8 state; // 0xE24
};
extern Unknown95269C* g_pUnknown95269C;
void Func6A4D84(Unknown95269C* object); // 0x006A4D84
void Func6A5A70(Unknown95269C* object); // 0x006A5A70 (before ScBoot)
void Func6A5008(Unknown95269C* object); // 0x006A5008 (before a stage scene)
bool Func6A4CE0(Unknown95269C* object); // 0x006A4CE0
bool Func6A5BCC(Unknown95269C* object); // 0x006A5BCC
// an object of the game (pointer at 0x00950C4C); no scene is made while it exists (type unknown)
extern void* g_pUnknown950C4C;

// the object that BsRoot updates every frame (type unknown; placeholder)
struct Unknown9CA054
{
    u32 _unknown;
};
extern Unknown9CA054 g_Unknown9CA054;
void Func4E2CC4(Unknown9CA054* object); // 0x004E2CC4
