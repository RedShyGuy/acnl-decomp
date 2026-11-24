#pragma once

#include "decomp.h"
#include "Other/dMenuBase.h"
#include "state/dMode.h"
#include "Sound/dSoundLabel.h"

// RTTI 14BsCommonDialog @ 0x008CBB54
// vtable 0x008EFB4C (vptr 0x008EFB54), offset_to_top 0, 25 entries
// vtable 0x008EFBB8 (vptr 0x008EFBC0), offset_to_top -40, 3 entries
class BsCommonDialog : public ::MenuBase, public ::state::Mode<BsCommonDialog>
{
public:
    BsCommonDialog(); // ctor candidate(s) 0x007EE8C4 (unverified)
    virtual ~BsCommonDialog(); // 0x00251E88 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00251E5C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Initialize(); // 0x00251B70 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual oml::framework::Result Finalize(); // 0x00251D90 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual oml::framework::Result Calc(); // 0x00251CD0 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x00251B5C slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void OnClose(); // 0x00251940 slot 0x44 | slot vf_0x44 of MenuBase

    // (game code that is not decompiled yet; names are ours) the next message of g_CommonDialogArg
    void ShowNext(); // 0x00251908
    // (copy for the game) the dialog waits (the message was answered)
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsWaiting() const { return ::state::Mode<BsCommonDialog>::GetState() == &BsCommonDialog::State_Wait; } // 0x00251DD4
    void State_Wait(); // 0x0025195C
    // the open dialog (NULL: none)
    static BsCommonDialog* GetInstance(); // 0x00251930

    static u8 s_Unknown94FE4C; // (set to 1 by Finalize)
    static BsCommonDialog* s_pInstance;
};

// the request of the common dialog (BsMenuMgr::Func6D32D0 resets it; names are ours)
struct CommonDialogArg
{
    u8 kind;               // 0x00 (0: notices, 1: a choice)
    u8 _unknown0x01;       // 0x01
    s32 labels[4];         // 0x04 the messages (indices in the file; -1: none)
    SeID se;               // 0x14
    s32 _unknown0x18;      // 0x18
    s32 _unknown0x1C;      // 0x1C
    u32 _unknown0x20;      // 0x20
    const char* pFile;     // 0x24 the message file
    u32 _unknown0x28[4];   // 0x28
    bool _unknown0x38;     // 0x38
    bool isLast;           // 0x39 the last message (else another follows)
    bool _unknown0x3A;     // 0x3A
    bool _unknown0x3B;     // 0x3B
};
ASSERT_SIZE(CommonDialogArg, 0x3C);

extern CommonDialogArg g_CommonDialogArg;
