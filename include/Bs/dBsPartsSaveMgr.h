#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Sv/dSvStepInner.h"

// the parts of the save data held by a BsPartsSaveMgr (class names from the RTTI)
namespace svholder {
class Mail;
class MyDesign;
} // namespace svholder
struct SvBestFriendList;

// The process that holds a save file of the player besides garden_plus.dat (friend<n>.dat, mail<n>.dat,
// mydsgn<n>.dat): while at least one instance runs, the file is loaded (a new one is set up if it cannot be read); when
// the last one ends, the file is saved without a commit and freed. Class name from the RTTI; all other names are ours.
// Instantiations found in the binary:
//   BsPartsSaveMgr<SvBestFriendList>  typeinfo 0x008CBC38  vtable 0x008F0088
//   BsPartsSaveMgr<svholder::Mail>  typeinfo 0x008CBC44  vtable 0x008F00D0
//   BsPartsSaveMgr<svholder::MyDesign>  typeinfo 0x008CBC50  vtable 0x008F0118
template <typename T0>
class BsPartsSaveMgr : public ::Base
{
public:
    BsPartsSaveMgr() : m_InstanceNum(0) {}
    virtual ~BsPartsSaveMgr() {}
    // the first instance loads the file (RETRY until the request is done)
    oml::framework::Result Initialize() override;
    // the last instance saves the file and frees it (RETRY until the request is done)
    oml::framework::Result Finalize() override;
    oml::framework::Result Calc() override;
    oml::framework::Result Draw() override;

    // the save kind of T0 in SvSave: the flag "saving" is set / the file is written (8 without data) / the flag is
    // cleared
    static void BeginSave();
    static s32 Write();
    static void EndSave();

    // the save number whose file is used (4 = none)
    static u32 s_SaveNo;
    // the detail of the last request
    static s32 s_Detail;
    // the data is there (loaded or set up)
    static bool s_HasData;
    static T0* s_pData;
    // the file of each player ("/friend1.dat"...)
    static const char* s_FileNames[4];
    // set while the file is saved (SvGetBestFriendList etc. return NULL then)
    static bool s_IsSaving;
    // the request of the load and the save
    // 0x00AF8850 (guard 0x00983030) [<SvBestFriendList>]
    // 0x00AF8888 (guard 0x00983058) [<svholder::Mail>]
    // 0x00AF88C0 (guard 0x00983080) [<svholder::MyDesign>]
    static SvStepInner s_Request;
    // the alignment of the data and the flags of the requests (read from memory)
    static const s32 ALIGNMENT;
    static const u32 LOAD_FLAGS;
    static const u32 SAVE_FLAGS;

    s32 m_InstanceNum;  // 0x14 (only the first and the last instance count)
};

// (the requests are built where they are used, each guarded; the statics of each instantiation lie together: save number,
// detail, guard of the request, data flag, data, file names, saving flag)
template <typename T0>
SvStepInner BsPartsSaveMgr<T0>::s_Request(SvStepInner::FLAG_KEEP_ERROR);

// the same for mvplayer_pls.dat (save kind 11; its holder has no class in the RTTI, the statics lie in front of the ones
// of BsPartsSaveMgr<SvBestFriendList>; names are ours)
void SvBeginSaveMovePlayerPls(); // 0x007CBBB8
s32 SvWriteMovePlayerPls(); // 0x007CBB5C
void SvEndSaveMovePlayerPls(); // 0x007CBA68
extern s32 g_SvMovePlayerPlsDetail;
extern bool g_SvIsSavingMovePlayerPls;
// the request of the load (guarded like BsPartsSaveMgr<T>::s_Request)
extern SvStepInner g_SvMovePlayerPlsRequest;
// the alignment of the data (read from memory)
extern const s32 g_SvMovePlayerPlsAlignment;
// the load runs (true when it is done; *result: the result of the request, a missing file counts as a new one;
// result may be NULL) / the data is freed
bool SvUpdateLoadMovePlayerPls(s32* result); // 0x007CBA7C
void SvFreeMovePlayerPls(); // 0x007CB800
// (another file of the game, 0x002F3964..) g_Unknown9510F0 = a Move In file exists (mvplayer_pls.dat of the first
// player, else the old mvplayer.dat, then g_Unknown9510F1 too) / both files are deleted (after a save of the whole town) /
// the load is prepared: the request, the data on the heap (NULL: the stage heap) and the save number
void SvCheckMovePlayerFiles(); // 0x002F3964
void SvDeleteMovePlayerFiles(); // 0x002F39B4
namespace sead {
class Heap;
} // namespace sead
void SvStartLoadMovePlayerPls(sead::Heap* heap); // 0x002F39E8
