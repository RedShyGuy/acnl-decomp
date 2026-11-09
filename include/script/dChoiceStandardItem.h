#pragma once

#include "decomp.h"
#include "Item/dItemSelectWindow.h"
#include "script/dChoiceStandard.h"

namespace script {
class RenderGarden;

// The choices of the talk as a list with a title (the item select window of the game, a renderer per choice and one
// for the title; member names are ours).
// RTTI N6script18ChoiceStandardItemE @ 0x008D39B0
// vtable 0x0090A3E0 (vptr 0x0090A3E8), offset_to_top 0, 15 entries
class ChoiceStandardItem : public ::script::ChoiceStandard
{
public:
    static const s32 ITEM_NUM = 6;

    ChoiceStandardItem(); // 0x005E9404 (name after the RTTI)
    virtual ~ChoiceStandardItem(); // 0x005E954C slot 0x00
    // 0x005E953C slot 0x04 (deleting dtor)
    virtual void Update(); // 0x0071DD68 slot 0x10
    virtual s32 GetCursor() const; // 0x0075C200 slot 0x18
    virtual bool IsCancelPressed() const; // 0x0075C210 slot 0x20
    virtual s32 GetCancelValue() const; // 0x0075C208 slot 0x24
    virtual void OnSetup(); // 0x005E933C slot 0x30
    virtual void OnDecide(); // 0x002BB0AC slot 0x34
    virtual bool IsReady() const; // 0x0075C1EC slot 0x38

    // the title of the list
    void SetTitle(const IWord& title); // 0x005E8F68
    // the texts of the choices into the renderers and the text boxes of the window
    void UpdateTexts(); // 0x005E8F80
    // the renderers get the text boxes of the window once it is set up; true when they have them
    bool SetupTextBoxes(u32 arg); // 0x005E90F8
    // the state machine and the window
    void Run(); // 0x005E9250

    u8 m_IsSetup;                          // 0x0D8
    ItemSelectWindow m_Window;             // 0x0DC
    RenderGarden* m_pRenders[ITEM_NUM];    // 0xBEC
    WordFix<33> m_Title;                   // 0xC04
    RenderGarden* m_pTitle;                // 0xC60
};
ASSERT_SIZE(ChoiceStandardItem, 0xC64);
} // namespace script
