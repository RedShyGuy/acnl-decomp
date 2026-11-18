#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// The base of the scene processes (ScBoot, ScStage, the mini game scenes): the change of the scene with its fades, the
// children of the scene paused and hidden until the black wait and the stage transitions of the net game are done.
// Only the class and slot names from the RTTI/libgarden; all other names are ours.
// RTTI 7BsScene @ 0x008CD348
// vtable 0x008F8E6C (vptr 0x008F8E74), offset_to_top 0, 17 entries
class BsScene : public ::Base
{
public:
    BsScene(); // 0x00607938
    virtual ~BsScene(); // 0x00313AF4 slot 0x00
    // 0x00607974 slot 0x04 (deleting dtor)
    virtual bool CanInitialize() const; // 0x006078B0 slot 0x08
    virtual void HandleInitializationResult(oml::framework::Result result); // 0x00607194 slot 0x10
    virtual bool CanFinalize() const; // 0x00607924 slot 0x14
    virtual void HandleFinalizationResult(oml::framework::Result result); // 0x00607198 slot 0x1C
    virtual bool CanCalc() const; // 0x006071DC slot 0x20
    virtual void HandleCalcResult(oml::framework::Result result); // 0x006074F8 slot 0x28
    virtual bool CanDraw() const; // 0x00607898 slot 0x2C
    virtual void ProcessDrawResult(oml::framework::Result result); // 0x006078AC slot 0x34
    virtual u32 GetTypeMask(); // 0x0075E164 slot 0x3C (name is ours)
    // (pure: __cxa_pure_virtual in the vtable; SUCCESS keeps the current stage at the start, see CanInitialize)
    virtual oml::framework::Result vf_0x40() = 0; // slot 0x40

    // the request of a scene change: the scene process of the next stage and the fades
    struct ChangeRequest
    {
        bool isChanging;    // 0x00 the new scene is created and the old one not finalized yet
        bool isFirstCalc;   // 0x01 (set by the constructor, cleared by the first calc)
        bool isShown;       // 0x02 the black wait has ended (the children may be shown)
        u8 typeA;           // 0x03 the fade out
        u8 faderA;          // 0x04 the fade in
        u8 typeB;           // 0x05 the fade out (second value)
        u8 faderB;          // 0x06 the fade in (second value)
        bool isFading;      // 0x07
        u16 name;           // 0x08 ProcName of the next scene (0xFFFF: none)
        u16 current;        // 0x0A
        u16 previous;       // 0x0C
        u16 _unknownE;
        oml::framework::Process* pScene;  // 0x10 the current scene process
        u32 arg;            // 0x14 the argument of the next scene
        u32 fadeOutParam;   // 0x18
        u32 fadeInParam;    // 0x1C
    };
    // a fade type of 6: the one of the last change
    static const u8 FADE_TYPE_KEEP = 6;
    static const u16 NO_SCENE = 0xFFFF;
    static void RequestChange(u16 name, u8 typeA, u8 faderA, u8 typeB, u8 faderB, u32 arg); // 0x006074FC
    // a change to ScBoot (without fade)
    static void RequestBoot(); // 0x0060753C
    // makes the requested scene (BsRoot first if there is none; NULL: nothing made)
    static oml::framework::Process* ProcessChange(); // 0x0060758C
    // whether the scene name can be made now (the fields of the previous scene are finished)
    static bool CanChange(u16 name); // 0x006076A8
    // the children of the scene are resumed / paused
    void Resume(); // 0x006077F8
    void Pause(); // 0x00607848

    static ChangeRequest s_ChangeRequest;
    // the last requested scene
    static u16 s_LastRequested;

    // (in the tail padding of Process; UtlBase<BsScene> keeps its flags behind them at 0x15)
    bool m_IsPaused;  // 0x13
    bool m_IsStarted; // 0x14 the children are resumed and shown
};
ASSERT_SIZE(BsScene::ChangeRequest, 0x20);
