#include "script/dITalkRecept.h"
#include "script/dScriptExtern.h"
#include "sead/seadStringFormatter.h"

namespace script {
// 0x005DE16C
ITalkRecept::ITalkRecept() : m_pOwner(NULL), m_Number(-1), m_NameMode(3)
{
}

// 0x00316D7C slot 0x00
// 0x005DE1B4 (deleting dtor)
ITalkRecept::~ITalkRecept()
{
    // (only the members are destroyed)
}

// 0x0075BB84 slot 0x8C
s32 ITalkRecept::GetSex()
{
    const WordInfo* info = static_cast<const IWord&>(m_Name).GetInfo();
    return info != NULL ? info->color : 4;
}

// 0x0075BBA8 slot 0x90
s32 ITalkRecept::GetSpeakerSex()
{
    WordFix<9> name;
    Func56DE5C(&name, 4);
    const WordInfo* info = static_cast<const IWord&>(name).GetInfo();
    return info != NULL ? info->color : 4;
}

// 0x005DE040
void ITalkRecept::SetName(const IWord& name, u8 value)
{
    m_Name.Copy(name);
    m_NameMode = value;
}

// 0x005DE088
void ITalkRecept::SetNumberText(s32 number)
{
    sead::StringFormatter formatter(&m_Label);
    formatter.append("%03d").appendValue(number).flush();
}

// 0x005DE158
// 0x0075BBE8 (copy)
TalkSystem* ITalkRecept::GetOwner() const
{
    return m_pOwner;
}

// 0x005DE160 slot 0x94
void ITalkRecept::Slot94()
{
    // empty (in the original too)
}

// 0x005DE13C slot 0x98
void ITalkRecept::OnMessage(TalkLabel*)
{
    // empty (in the original too)
}

// 0x005DE080 slot 0x9C
void ITalkRecept::OnMessageStart(TalkLabel*)
{
    // empty (in the original too)
}

// 0x005DE028 slot 0xA0
void ITalkRecept::OnBegin(u8)
{
    // empty (in the original too)
}

// 0x005DE064 slot 0xA4
void ITalkRecept::OnPageEnd(u8)
{
    // empty (in the original too)
}

// 0x005DE100 slot 0xA8
void ITalkRecept::OnChoice(const ChoiceItem*, s32)
{
    // empty (in the original too)
}

// 0x005DE104 slot 0xAC
void ITalkRecept::OnEnd()
{
    // empty (in the original too)
}

// 0x005DE118 slot 0xB8
void ITalkRecept::GetWord198(IWord*)
{
    // empty (in the original too)
}

// 0x005DE140 slot 0xBC
void ITalkRecept::OnTag7(u32)
{
    // empty (in the original too)
}

// 0x005DE14C slot 0xC0
void ITalkRecept::OnTag7_18(u8, u8, u16, u16)
{
    // empty (in the original too)
}

// 0x005DE02C slot 0xE0
void ITalkRecept::OnTag16(const Tag*)
{
    // empty (in the original too)
}

// 0x005DE084 slot 0xE4
void ITalkRecept::OnTag13_0(const Tag*)
{
    // empty (in the original too)
}

// 0x005DE150 slot 0xEC
void ITalkRecept::OnTag13_4(const Tag*)
{
    // empty (in the original too)
}

// 0x005DE154 slot 0xF0
void ITalkRecept::OnTag13_5(const Tag*)
{
    // empty (in the original too)
}

// 0x0075BB80 slot 0xF8
void ITalkRecept::GetVoices(sead::CheckedArray<u8, 4>*)
{
    // empty (in the original too)
}

// 0x005DE12C slot 0xC4
s32 ITalkRecept::GetBranch10_0()
{
    return 0;
}

// 0x005DE030 slot 0xC8
s32 ITalkRecept::GetBranch10_11()
{
    return 0;
}

// 0x005DE11C slot 0xCC
s32 ITalkRecept::GetBranch10_23()
{
    return 0;
}

// 0x005DE070 slot 0xD0
s32 ITalkRecept::GetBranch10_28()
{
    return 0;
}

// 0x005DE108 slot 0xD4
s32 ITalkRecept::GetBranch10_30()
{
    return 0;
}

// 0x0075BB78 slot 0xF4
s32 ITalkRecept::GetMood()
{
    return 0;
}

// 0x005DE068 slot 0xB0
bool ITalkRecept::ProcessTag2(const Tag*)
{
    return true;
}

// 0x005DE144 slot 0xE8
bool ITalkRecept::ProcessTag13_3(const Tag*)
{
    return true;
}

// 0x005DE020 slot 0xB4
bool ITalkRecept::ProcessTag3(const Tag*)
{
    return true;
}

// 0x005DE164 slot 0xD8
void ITalkRecept::OnTag11(const Tag*)
{
    // empty (in the original too)
}

// 0x005DE168 slot 0xDC
void ITalkRecept::OnTag12(const Tag*)
{
    // empty (in the original too)
}
} // namespace script
