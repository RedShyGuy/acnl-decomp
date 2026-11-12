#pragma once

#include "decomp.h"

class SaveProcess;
namespace svholder {
class Mail;
class MyDesign;
} // namespace svholder

// The save kinds of the save thread (SvStepInner::FLAG_SAVE_KIND_MASK 1..19; all names are ours). Each kind has four
// functions in tables (index kind - 1): begin, prepare (the checksums), write (a result of SvSaveIo; the writes go out
// of a copy of the save data), end.
//
//  1 garden_plus.dat           8 mydsgn<n>.dat          15 (ModuleIncSave? default import target)
//  2 nothing                   9 mail<n>.dat            16 the part of the shared data at +0x24
//  3 census + player          10 friend<n>.dat          17 census
//  4 NPC data                 11 mvplayer_pls.dat       18 amiibo.dat
//  5 garden data              12 /identity              19 shared data
//  6 shared data (first part) 13 minigame data
//  7 exhibition.dat           14 garden data +0x52BB0
namespace sv {
typedef void (*SaveFunction)();
typedef s32 (*WriteFunction)();
const s32 SAVE_KIND_NUM = 19;
} // namespace sv

// the save kind of flags is saved (and with FLAG_SAVE_FRIENDS also kind 10); false: failed (g_SvErrorCode is set)
bool SvSave(SaveProcess* process, u64 flags); // 0x0070F2BC
// the steps before the save: FLAG_EXTRA; kinds 1 and 5: Func5CBAA0 (unless FLAG_SAVE_FIRST); kind 1: updated.dat is made,
// garden.dat and /identity are deleted (always true)
bool SvSaveExtra(SaveProcess* process, u64 flags); // 0x0070F3B4

// the prepare functions in the file of SaveProcess
void SvPrepareSaveMail(); // 0x001D3BCC
void SvPrepareSaveAmiibo(); // 0x001D3BE4
void SvPrepareSavePlayer(); // 0x001D3C0C
void SvPrepareSaveGardenPlus(); // 0x001D3C34
void SvPrepareSaveMyDesign(); // 0x001D3C70
void SvPrepareSaveNothing(); // 0x001D3C88
void SvPrepareSaveBestFriendList(); // 0x001D3C8C
void SvPrepareSaveExhibition(); // 0x001D3CA4
void SvPrepareSaveMovePlayerPls(); // 0x001D3CBC
// the write of kind 12
s32 SvWriteIdentity(); // 0x001D3C44

// the data of the part files (NULL while there is none or it is saved), and the same also while it is saved (like
// SvGetBestFriendList / SvGetBestFriendListAlways; dSvApi.cpp)
svholder::Mail* SvGetMail(); // 0x001DF8E0
svholder::MyDesign* SvGetMyDesign(); // 0x001DF924
svholder::Mail* SvGetMailAlways(); // 0x001DF970
svholder::MyDesign* SvGetMyDesignAlways(); // 0x001DF9C8
