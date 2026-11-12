#pragma once

#include "decomp.h"

// Globals of the save code that several files read (game code; names are ours; their files are not decompiled yet).

// the names of the save files
extern const char* g_SvAmiiboFileName;          // "amiibo.dat"
extern const char* g_SvGardenFileName;          // "/garden.dat" (the save file of the game before Welcome amiibo)
extern const char* g_SvGardenPlusFileName;      // "/garden_plus.dat"
extern const char* g_SvMovePlayerFileName;      // "/mvplayer.dat"
// "/mvplayer_pls.dat" for each player (the "Move In" character; statics of a BsPartsSaveMgr whose type is not known)
extern const char* g_SvMovePlayerPlsFileNames[4];
// the data of amiibo.dat (0x128 bytes)
extern void* g_pSvAmiiboData;
// the size of garden_plus.dat (read from memory by the save code)
extern const u32 g_SvGardenPlusSize;
// (0) the loaded saveInitialized must be above it and at most 1 (read from memory)
extern const s32 g_SvSaveInitializedMin;
// the state of mvplayer_pls.dat (like the statics of BsPartsSaveMgr<T>): the save number of the file, whether the data
// is there, the data
struct SvMovePlayerPls;
extern u32 g_SvMovePlayerPlsSaveNo;
extern bool g_SvHasMovePlayerPls;
extern SvMovePlayerPls* g_pSvMovePlayerPls;
// the error of the last save (-1: none; 11 failed, 12 broken, 33 secure value, 37 not initialized; shown by other code)
extern s32 g_SvErrorCode;
// (set when ModuleIncSave finds something to import for a missing garden_plus.dat)
extern u8 g_Unknown9516D1;
