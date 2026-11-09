#include "script/dTalkSystem.h"
#include "sead/seadStringFormatter.h"

namespace script {
// (both are built by a static initializer that sets up globals of several files; not annotated)
// 0x00AD6B7C (name is ours)
sead::FixedSafeString<8> TalkLabel::s_EndLabel(sead::SafeStringBase<char>("000"));
// 0x00AD6874 (name is ours)
sead::FixedSafeString<256> TalkLabel::s_LabelBuffer;

// 0x005E2444 (name is ours)
void TalkLabel::SetFile(const char* file)
{
    m_File.copy(sead::SafeStringBase<char>(file), -1);
}

// 0x005E2478 (name is ours)
void TalkLabel::SetIndex(s32 index)
{
    m_Index = index;
    m_Flags = FLAG_INDEX;
}

// 0x005E2488 (name is ours)
void TalkLabel::SetName(const IWord& name, u8 mode)
{
    static_cast<IWord&>(m_Name).Copy(name);
    m_NameMode = mode;
}

// 0x005E24AC (name is ours)
void TalkLabel::ClearName()
{
    static_cast<IWord&>(m_Name).Clear();
    m_NameMode = 3;
}

// 0x005E24D0 (name is ours)
void TalkLabel::SetNumber(s32 number)
{
    m_Number = number;
    m_Flags = FLAG_NUMBER;
}

// 0x005E24E0 (name is ours)
bool TalkLabel::Resolve(libms::Message* message)
{
    bool result = false;
    if ((m_Flags & FLAG_INDEX) != 0 && m_Index != -1) {
        if (message->IsInitialized() && message->GetLabel(m_Index, const_cast<char*>(s_LabelBuffer.getCString()))) {
            m_Label.copy(s_LabelBuffer, -1);
            m_Flags |= FLAG_LABEL;
            s32 number = ParseNumber(m_Label);
            m_Number = number;
            if (number != -1) {
                m_Flags |= FLAG_NUMBER;
            }
        }
        result = true;
    } else if ((m_Flags & FLAG_NUMBER) != 0 && m_Number != -1) {
        FormatLabelNumber(&m_Label, m_Number);
        if (m_Label.isEmpty()) {
            return false;
        }
        m_Flags |= FLAG_LABEL;
        if (message->IsInitialized()) {
            s32 index = message->GetTextIndex(m_Label.getCString());
            if (index >= 0) {
                m_Index = index;
                m_Flags |= FLAG_INDEX;
                result = true;
            }
        }
    } else if ((m_Flags & FLAG_LABEL) != 0) {
        if (m_Label.isEmpty()) {
            return false;
        }
        if (message->IsInitialized()) {
            s32 index = message->GetTextIndex(m_Label.getCString());
            if (index >= 0) {
                m_Index = index;
                m_Flags |= FLAG_INDEX;
                result = true;
            }
        }
        s32 number = ParseNumber(m_Label);
        m_Number = number;
        if (number != -1) {
            m_Flags |= FLAG_NUMBER;
        }
    }
    return result;
}

// 0x005EAC28 (name is ours)
void FormatLabelNumber(sead::BufferedSafeStringBase<char>* label, s32 number)
{
    sead::StringFormatter formatter(label);
    formatter.append("%03d").appendValue(number).flush();
}
} // namespace script
