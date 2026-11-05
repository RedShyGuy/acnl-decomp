#pragma once

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "nn/fnd/fnd_DateTime.h"

// Functions and globals of the game that the events use (not decompiled yet; names are ours, FuncXXXXXX while the purpose
// is unknown).
namespace event {
bool Func750EFC(const nn::fnd::DateTimeParameters* date); // 0x00750EFC
u8 Func75EAF4(void* object, nn::fnd::DateTimeParameters date); // 0x0075EAF4
bool Func314104(nn::fnd::DateTimeParameters date, nn::fnd::DateTimeParameters other); // 0x00314104
nn::fnd::DateTimeParameters MakeDateTime(s32 year, s32 month, s32 day, s32 hour, s32 minute, s32 second,
                                         s32 milliSecond); // 0x002FD5E8
// a is not before b
bool IsNotBefore(nn::fnd::DateTimeParameters a, nn::fnd::DateTimeParameters b); // 0x002FD3E4
// < 0: the date is before (year, month, day)
s32 CompareSvDate(const SvDate* date, u16 year, u8 month, u8 day); // 0x00301234
void AddDays(nn::fnd::DateTimeParameters* date, s32 days); // 0x003012E8
bool IsLeapYear(s32 year); // 0x00301FEC
u32 Func302218(void* object); // 0x00302218
nn::fnd::DateTimeParameters Func30A478(const SvDate* date); // 0x0030A478
void Func30F6DC(SvDate* date, const nn::fnd::DateTimeParameters* from); // 0x0030F6DC
void SetSvDate(SvDate* date, u16 year, u8 month, u8 day); // 0x005CB9F8
bool Func71D24C(void* object); // 0x0071D24C
void Func6140F8(void* object); // 0x006140F8
void Func6137F4(void* object, const nn::fnd::DateTimeParameters* date, bool flag); // 0x006137F4
void Func613BAC(void* object, const nn::fnd::DateTimeParameters* date); // 0x00613BAC
void Func18FA70(void* object); // 0x0018FA70
void Func240B48(); // 0x00240B48
void Func240EF8(); // 0x00240EF8
s32 Func165B08(); // 0x00165B08
bool Func119504(void* npcData); // 0x00119504 (Thumb)
s32 Func118C8C(void* npcData); // 0x00118C8C (Thumb)
void* Func300928(void* npcData, s32 index); // 0x00300928
s32 Func112FE8(const void* npc); // 0x00112FE8 (Thumb)
s32 Func112788(const void* npc); // 0x00112788 (Thumb)

// a table of 399 entries of 34 bytes at +4 (the villagers by their id)
struct UnknownA84AEC
{
    u32 _0;
    u8* table;
};
extern UnknownA84AEC g_UnknownA84AEC;
extern void* g_pUnknown94FE58;
extern u8 g_Unknown94EDF8;
} // namespace event
