#pragma once

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "nn/fnd/fnd_DateTime.h"
#include "Sv/dSvFgName.h"

// Game functions that the Bug-Off (dSvInsectConvention.cpp) calls, not decompiled yet (names after the addresses while
// the purpose is unknown).
struct SvBulletinBoardMessage;
struct SvNpc;
struct SvPersonalId;
namespace script {
class Phrase;
} // namespace script
namespace sead {
template <typename T0>
class IDelegate1;
} // namespace sead

// the first date is later than the second / a date goes on (a copy of time::Func30A5AC)
bool Func2FB6DC(nn::fnd::DateTimeParameters date, nn::fnd::DateTimeParameters other); // 0x002FB6DC
void Func312848(nn::fnd::DateTimeParameters* date, s32 days, s32 hours, s32 minutes, s32 seconds); // 0x00312848
// checks of a villager (by its index / its data)
s32 Func309D64(s32 index); // 0x00309D64
bool Func312888(const SvNpc* npc); // 0x00312888
bool Func11A1E0(const SvNpc* npc); // 0x0011A1E0 (Thumb)
bool Func118E3C(SvNpc* npcs, const SvNpc* npc); // 0x00118E3C (Thumb)
void Func10F514(SvNpc* npcs); // 0x0010F514 (Thumb)
// the save number of a player of the town (Func71AC00 finds it, Func30A1FC converts it)
u32 Func71AC00(void* players, const SvPersonalId* player); // 0x0071AC00
u32 Func30A1FC(void* players, u32 index); // 0x0030A1FC
// a message of the bulletin board is written from a label with a callback for the words (true: written)
bool Func3020CC(SvBulletinBoardMessage* message, const char* file, s32 label, const nn::fnd::DateTimeParameters* date,
                const void* a, const void* b, sead::IDelegate1<script::Phrase*>* callback); // 0x003020CC (Thumb)
// a check of an object of an actor (KameyamaForeachFunction)
bool Func753588(const void* object); // 0x00753588
// the insect that is caught, its size and a factor (the kind 0..5 decides the insects) on a date
void Func701518(SvFgName* item, u8* size, f32* factor, u32 kind, const nn::fnd::DateTimeParameters* date); // 0x00701518
// a paper item of a group (the prize letters of the Bug-Off use 41 and 16)
void Func6BC8A4(SvFgName* item, u32 a, u32 b); // 0x006BC8A4

// the fish that a villager of the Fishing Tourney catches with a factor of its size (svfscnve; names after the
// addresses): a fish of a kind (the next two values: -3 and a direction 1 / -1 after the hour) / a fish like the fish of
// the tourney (factor > 1: not like it) / any fish of a kind / the fish of the tourney of a date (-3, 0)
SvFgName Func6175B0(f32* factor, u8 kind, s32 a, s32 b, const nn::fnd::DateTimeParameters* date); // 0x006175B0
SvFgName Func61749C(f32* factor, u8* kind, const SvFgName* fish, const nn::fnd::DateTimeParameters* date); // 0x0061749C
SvFgName Func617798(f32* factor, u8 kind, const nn::fnd::DateTimeParameters* date); // 0x00617798
SvFgName Func6173D4(f32* factor, s32 a, s32 b, const nn::fnd::DateTimeParameters* date); // 0x006173D4

// A set of up to 32 indices as bits (the villagers of the town; name is ours): the number of places, the bits, the
// number of set bits.
struct BitSet32 {
    // (game code, Thumb, not decompiled yet) a bit is set / cleared / all places are set or none / a copy (two copies)
    // / a random set index
    void Add(u32 index); // 0x00116E48 (Thumb)
    void Remove(u32 index); // 0x00116E66 (Thumb)
    void Init(u32 num, bool isFull); // 0x00116EBC (Thumb)
    BitSet32& operator=(const BitSet32& other); // 0x00116F32 (Thumb)
    // 0x00116EAA (copy)
    s32 PickRandom() const; // 0x002FE468 (Thumb)

    /* 0x0000 */ u32 num;
    /* 0x0004 */ u32 bits;
    /* 0x0008 */ s32 count;
};
ASSERT_SIZE(BitSet32, 0xC);
