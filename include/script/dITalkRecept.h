#pragma once

#include "decomp.h"
#include "script/dIRecept.h"
#include "script/dWordFix.h"
#include "sead/seadFixedSafeString.h"

namespace sead {
template <typename T, s32 N>
class CheckedArray;
} // namespace sead

namespace script {
struct Tag;
struct TalkLabel;
struct ChoiceItem;
class TalkSystem;

// The recept of the conversations: the object the game gives a talk (an NPC, an object, a menu) that answers the tags
// of TalkSeq (branches, events) on top of the words of IRecept, with the name of the speaker for the sex tags. The
// slots after IRecept have default bodies here; their names are ours, after the tags of TalkSeq that call them (the
// others after their slot).
// RTTI N6script11ITalkReceptE @ 0x008D37DC
// vtable 0x00909EF8 (vptr 0x00909F00), offset_to_top 0, 63 entries
class ITalkRecept : public ::script::IRecept
{
public:
    ITalkRecept(); // 0x005DE16C
    virtual ~ITalkRecept(); // 0x00316D7C slot 0x00
    // 0x005DE1B4 slot 0x04 (deleting dtor)

    // the sex of the speaker after the gender of its name (4: unknown)
    virtual s32 GetSex(); // 0x0075BB84 slot 0x8C
    // the sex of the speaker for the sex tags (1: male, 2: female); by default the gender of the name Func56DE5C
    // gives for 4 (4: unknown)
    virtual s32 GetSpeakerSex(); // 0x0075BBA8 slot 0x90
    virtual void Slot94(); // 0x005DE160 slot 0x94
    // a new message starts (the message is copied from the jump)
    virtual void OnMessage(TalkLabel* message); // 0x005DE13C slot 0x98
    // the file of the message is loaded
    virtual void OnMessageStart(TalkLabel* message); // 0x005DE080 slot 0x9C
    // the text starts (byte 3 of the attribute of the message, < 18)
    virtual void OnBegin(u8 value); // 0x005DE028 slot 0xA0
    // a page is done (byte 4 of the attribute of the message, < 18)
    virtual void OnPageEnd(u8 value); // 0x005DE064 slot 0xA4
    // a choice was made
    virtual void OnChoice(const ChoiceItem* item, s32 cursor); // 0x005DE100 slot 0xA8
    // the message ends
    virtual void OnEnd(); // 0x005DE104 slot 0xAC
    virtual bool ProcessTag2(const Tag* tag); // 0x005DE068 slot 0xB0
    // the tag 3 (index < 47): false holds the text at the tag until the recept is ready
    virtual bool ProcessTag3(const Tag* tag); // 0x005DE020 slot 0xB4
    // the word of the insert slot 0x198 (TalkInsert::UpdateWord198)
    virtual void GetWord198(IWord* word); // 0x005DE118 slot 0xB8
    virtual void OnTag7(u32 index); // 0x005DE140 slot 0xBC
    virtual void OnTag7_18(u8 a, u8 b, u16 c, u16 d); // 0x005DE14C slot 0xC0
    virtual s32 GetBranch10_0(); // 0x005DE12C slot 0xC4
    // the label of the branch tag 10/11 (negative: none)
    virtual s32 GetBranch10_11(); // 0x005DE030 slot 0xC8
    virtual s32 GetBranch10_23(); // 0x005DE11C slot 0xCC
    // the label of the branch tag 10/28 (negative: none)
    virtual s32 GetBranch10_28(); // 0x005DE070 slot 0xD0
    virtual s32 GetBranch10_30(); // 0x005DE108 slot 0xD4
    // the tags 11 (index < 6) and 12 (index < 10)
    virtual void OnTag11(const Tag* tag); // 0x005DE164 slot 0xD8
    virtual void OnTag12(const Tag* tag); // 0x005DE168 slot 0xDC
    virtual void OnTag16(const Tag* tag); // 0x005DE02C slot 0xE0
    virtual void OnTag13_0(const Tag* tag); // 0x005DE084 slot 0xE4
    virtual bool ProcessTag13_3(const Tag* tag); // 0x005DE144 slot 0xE8
    virtual void OnTag13_4(const Tag* tag); // 0x005DE150 slot 0xEC
    virtual void OnTag13_5(const Tag* tag); // 0x005DE154 slot 0xF0
    // the mood of the speech sounds (< 6)
    virtual s32 GetMood(); // 0x0075BB78 slot 0xF4
    // the voices of the speech sounds
    virtual void GetVoices(sead::CheckedArray<u8, 4>* voices); // 0x0075BB80 slot 0xF8

    // the name of the speaker (and the byte +0x78 with it)
    void SetName(const IWord& name, u8 value); // 0x005DE040
    // the number as a text of three digits
    void SetNumberText(s32 number); // 0x005DE088
    // the talk the recept belongs to (copy 0x0075BBE8)
    TalkSystem* GetOwner() const; // 0x005DE158

    // the next message (the talk takes it when a message ends without a jump; see TalkLabel)
    TalkSystem* m_pOwner;                  // 0x04 (set by TalkSystem::SetRecept)
    sead::FixedSafeString<32> m_File;      // 0x08 the file of the next message
    sead::FixedSafeString<8> m_Label;      // 0x34 its label (empty: none)
    s32 m_Number;                          // 0x48 its label as a number (-1: none)
    WordFix<9> m_Name;                     // 0x4C the name of the speaker
    u8 m_NameMode;                         // 0x78 (3)
};
ASSERT_SIZE(ITalkRecept, 0x7C);
} // namespace script
