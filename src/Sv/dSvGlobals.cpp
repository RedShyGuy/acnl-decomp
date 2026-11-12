#include "Sv/dSvGlobals.h"

#include "Sound/sound_Extern.h"
#include "Sv/dSvHolder.h"
#include "Sv/dSvProcExtern.h"
#include "Sv/dSvSaveIo.h"

// Globals of save files that are not decompiled yet (names are ours). The ones from 0x0095CF84 on lie in front of
// g_SvSaveMemory (dSvApi.cpp).

// 0x0095CF84
const char* g_SvAmiiboFileName = "amiibo.dat";
// 0x0095CF88
void* g_pSvAmiiboData = NULL;
// 0x0095CF8C
const char* g_SvGardenFileName = "/garden.dat";
// 0x0095CF90
const char* g_SvGardenPlusFileName = "/garden_plus.dat";
// 0x009510F4
const char* g_SvMovePlayerFileName = "/mvplayer.dat";
// 0x00983014
const char* g_SvMovePlayerPlsFileNames[4] = {"/mvplayer_pls.dat", "/mvplayer_pls.dat", "/mvplayer_pls.dat", "/mvplayer_pls.dat"};
// 0x0088F5C4
extern const u32 g_SvGardenPlusSize = 0x89B00;
// 0x0088F5C8
extern const s32 g_SvSaveInitializedMin = 0;
// 0x00983000
u32 g_SvMovePlayerPlsSaveNo = 4;
// 0x0098300C
bool g_SvHasMovePlayerPls = false;
// 0x00983010
SvMovePlayerPls* g_pSvMovePlayerPls = NULL;
// 0x009516D1
u8 g_Unknown9516D1 = 0;
// 0x009516DC
s32 g_SvErrorCode = -1;
// 0x009510F0
u8 g_Unknown9510F0 = 0;
// 0x009510F1
u8 g_Unknown9510F1 = 0;
// (an object of other game code; BsSvMgr sets the byte at +0xA10)
// 0x0094F4C8
u8* g_pUnknown94F4C8;
// (an object of the exhibition code; BsSvMgr sets it up)
// 0x00953630
void* g_pUnknown953630;
// (globals of another file of the game; the step "preparation of the save" of BsSvMgr reads/sets them)
// 0x00951718
Unknown951718 g_Unknown951718 = {0, {5, 0, 0, 0}, 0, {0, 0}, 0, {0, 0, 1, 0, 0, 0, 0}, NULL};
// (a pointer of another file; BsSvMgr::Finalize reads it)
// 0x0094FD5C
void* g_pUnknown94FD5C;
// (another file of the game; the apply functions of SvProc set the three bytes to 4, 0, 0)
// 0x0094EDD4
u8 g_Unknown94EDD4[3];
