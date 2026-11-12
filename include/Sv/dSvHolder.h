#pragma once

#include "decomp.h"
#include "Sv/dSvChecksum.h"
#include "Sv/dSvHouse.h"
#include "Sv/dSvMail.h"
#include "Sv/dSvMyDesign.h"
#include "Sv/dSvPlayer.h"
#include "Sv/dSvSaveMemory.h"
#include "Sv/dSvStepInner.h"

// The save files besides garden_plus.dat as the save code sees them: each starts with the CRC-32C of the rest. Only the
// class names svholder::Mail/MyDesign are from the binary (RTTI of BsPartsSaveMgr<T>); the other names are ours (after
// the file names); only the parts the save code needs are laid out. The checksum functions are copies in the shared area.

namespace svholder {
// the letters kept at the post office (mail<n>.dat)
class Mail
{
public:
    // (inline in BsPartsSaveMgr<Mail>::Initialize)
    Mail() : checksum(0) { unk_0x1C204 = 0; }
    // a new file (BsPartsSaveMgr: no file could be loaded; game code, not decompiled yet)
    void SetUpNew(); // 0x006CD78C
    DECOMP_EMIT_COPY DECOMP_NOINLINE u32 CalcChecksum() const // 0x0076FE44
    {
        return SvCalcChecksum(&checksum + 1, sizeof(*this) - sizeof(checksum));
    }
    // (isLogged: passed, not used)
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsChecksumValid(bool isLogged) const // 0x0076FE5C
    {
        (void)isLogged;
        return checksum == SvCalcChecksum(&checksum + 1, sizeof(*this) - sizeof(checksum));
    }

    u32 checksum;           // 0x00000
    SvMail letters[180];    // 0x00004
    u8 unk_0x1C204;         // 0x1C204
    u8 pad_0x1C205[3];      // 0x1C205
};
ASSERT_SIZE(Mail, 0x1C208);

// the designs saved at the Able Sisters (mydsgn<n>.dat)
class MyDesign
{
public:
    // (inline in BsPartsSaveMgr<MyDesign>::Initialize)
    MyDesign() : checksum(0) { unk_0x25F8D = 0; }
    // a new file (BsPartsSaveMgr: no file could be loaded; game code, not decompiled yet)
    void SetUpNew(); // 0x006CDA60
    DECOMP_EMIT_COPY DECOMP_NOINLINE u32 CalcChecksum() const // 0x0076FF64
    {
        return SvCalcChecksum(&checksum + 1, sizeof(*this) - sizeof(checksum));
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsChecksumValid(bool isLogged) const // 0x0076FFB0
    {
        (void)isLogged;
        return checksum == SvCalcChecksum(&checksum + 1, sizeof(*this) - sizeof(checksum));
    }

    u32 checksum;           // 0x00000
    SvMyDesign designs[72]; // 0x00004
    u8 unk_0x25F84[9];      // 0x25F84
    u8 unk_0x25F8D;         // 0x25F8D
    u8 pad_0x25F8E[2];      // 0x25F8E
};
ASSERT_SIZE(MyDesign, 0x25F90);
} // namespace svholder

// the data of exhibition.dat (the Happy Home Showcase: BsSvMgr loads and saves it around the stage Exhibition_Entrance;
// game code 0x002BEEBC.., not decompiled yet: only the size and the checksum are laid out)
struct SvExhibition
{
    SvExhibition(); // 0x002BF438
    ~SvExhibition(); // 0x002BF4F8
    // a new file (no file could be loaded)
    void SetUpNew(); // 0x002BF324
    DECOMP_EMIT_COPY DECOMP_NOINLINE u32 CalcChecksum() const // 0x0071E46C
    {
        return SvCalcChecksum(&checksum + 1, sizeof(*this) - sizeof(checksum));
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsChecksumValid(bool isLogged) const // 0x0071E4BC
    {
        (void)isLogged;
        return checksum == SvCalcChecksum(&checksum + 1, sizeof(*this) - sizeof(checksum));
    }

    u32 checksum;           // 0x000000
    u8 unk_0x4[0x17BE0C];   // 0x000004
};
ASSERT_SIZE(SvExhibition, 0x17BE10);

// The object that holds exhibition.dat (game code 0x002CDA88..0x002CDDA8; class and all names are ours): the data on
// the heap, a request of the save thread for its own loads and saves, and whether the data counts as loaded. It is
// the counterpart of BsPartsSaveMgr<T> for this file (begin/write/end of the save kind 7 in SvSave).
class SvExhibitionHolder
{
public:
    // (inline in the static initializer 0x0079C740)
    SvExhibitionHolder() : m_pData(NULL), m_Request(SvStepInner::FLAG_KEEP_ERROR)
    {
        m_IsLoaded = false;
        m_Detail = -1;
    }
    ~SvExhibitionHolder(); // 0x002CDD98

    // the data is allocated (false: it is there already)
    bool Allocate(); // 0x002CDB40
    // the data counts as loaded (isNew: it is set up new first)
    void SetLoaded(bool isNew); // 0x002CDA88
    // the data is freed
    void Free(); // 0x002CDBB0
    // exhibition.dat is read / written (a result of SvSaveIo)
    s32 Read(); // 0x002CDAEC
    DECOMP_NOINLINE s32 Write(); // 0x002CDB14
    // the checksum of the data is right (falls through into SvExhibition::IsChecksumValid)
    bool IsValid();
    // the request is prepared for the save kind 7 with the callbacks after the load, before the commit and at the end
    void PrepareSave(SvStepInner::StepCallback afterLoad, SvStepInner::StepCallback beforeCommit,
                     SvStepInner::Callback onEnd); // 0x002CDC0C
    // the save goes without a commit (the flags are replaced)
    void SetNoCommit(); // 0x002CDAD4
    // the load runs: the data is allocated and the request is started; true when it is done (*result: the result of
    // the request; a missing file counts as a new one)
    bool UpdateLoad(s32* result); // 0x002CDC44
    // the save runs: the prepared request is started; true when it is done (*result: the result of the request)
    bool UpdateSave(s32* result); // 0x002CDD48

    // the save kind 7 of SvSave: begin (the data is blocked), write exhibition.dat, end
    static void BeginSave(); // 0x002CDD84
    static s32 WriteFile(); // 0x002CDB08
    static void EndSave(); // 0x002CDBF8
    // the data (NULL while the file is saved)
    static SvExhibition* GetData(); // 0x002CDAB8

    SvExhibition* m_pData;  // 0x00
    u32 unk_0x4;            // 0x04
    SvStepInner m_Request;  // 0x08
    bool m_IsLoaded;        // 0x40
    s32 m_Detail;           // 0x44 the detail of the last request
};
ASSERT_SIZE(SvExhibitionHolder, 0x48);
extern SvExhibitionHolder g_SvExhibitionHolder;
// the state of the data (globals of the holder file): blocked while the file is saved, the data
extern u8 g_SvIsSavingExhibition;
extern SvExhibition* g_pSvExhibition;
// the data (NULL while the file is saved; the call in front of the holder was removed by armlink) / always
SvExhibition* SvGetExhibitionEntry(); // 0x005CB1C8
DECOMP_NOINLINE SvExhibition* SvGetExhibition(); // 0x005CB1CC
SvExhibition* SvGetExhibitionAlways(); // 0x005CB788
// (game code: the object g_pUnknown953630 when its byte +0xA1 is set, else NULL; Free clears the byte)
u8* Func537B10(); // 0x00537B10

// the "Move In" character from another town (mvplayer_pls.dat; the checksum leaves out the second word)
struct SvMovePlayerPls
{
    SvMovePlayerPls(); // 0x001B2C84
    // a new file (no file could be loaded; game code, not decompiled yet)
    void SetUpNew(); // 0x001B2C4C
    // (the destructor is implicit here: ARMCC destroys the member arrays inline in SvFreeMovePlayerPls, all of them with
    // empty destructors)
    DECOMP_EMIT_COPY DECOMP_NOINLINE void UpdateChecksum() // 0x0070C2F8
    {
        SvUpdateCrc(this, &player, &checksum, sizeof(*this));
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsChecksumValid(bool isLogged) // 0x0070C30C
    {
        return SvIsCrcValid(this, &player, &checksum, sizeof(*this), isLogged);
    }

    // (SvApplyMovePlayerPls copies each part into player 0 of the town)
    u32 checksum;           // 0x0000
    u32 unk_0x4;            // 0x0004
    SvPlayer player;        // 0x0008
    SvPlayerHouse house;    // 0xA488
    u8 unk_0xB6B0[0x5D4];   // 0xB6B0 (copied with 0x006CDAB8 into a part of the shared data)
    u8 unk_0xBC84[0xA3C];   // 0xBC84 (copied with 0x002BF890 into minigameData)
    u8 unk_0xC6C0[0x1F0];   // 0xC6C0 (copied with 0x002D49E8 into unk_0x52BB0)
    u8 unk_0xC8B0;          // 0xC8B0 (set: SvApplyMovePlayerPls sets bits in the town and the player)
    u8 pad_0xC8B1[7];       // 0xC8B1
};
ASSERT_SIZE(SvMovePlayerPls, 0xC8B8);
