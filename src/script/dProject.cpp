#include "script/dProject.h"
#include "script/dProject_SearchContentFolder.h"

namespace script {
// 0x0095EED4
Project* Project::s_pInstance;
// 0x0095EED8 (name is ours)
Project::SingletonDisposer_* Project::s_pSingletonDisposer;

// 0x0075D7F8 (name is ours)
bool Project::GetFileName(sead::BufferedSafeStringBase<char>* fileName, const char* name)
{
    // 0x00AD6764 (guard 0x0095EF24) (name is ours)
    static sead::FixedSafeString<2> s_FolderSeparator(sead::SafeStringBase<char>("/"));
    // 0x00AD6774 (guard 0x0095EF20) (name is ours)
    static sead::FixedSafeString<2> s_ExtensionSeparator(sead::SafeStringBase<char>("."));

    bool result = false;
    m_Path.copy(sead::SafeStringBase<char>(name), -1);

    // after the last folder separator
    s32 length = m_Path.getLength();
    s32 separatorLength = s_FolderSeparator.getLength();
    s32 index;
    for (index = length - separatorLength; index >= 0; index--) {
        if (sead::SafeStringBase<char>(m_Path.getCString() + index).compare(s_FolderSeparator, separatorLength) == 0) {
            break;
        }
    }
    if (index < 0) {
        index = m_Path.getLength() + 1;
    }
    s32 end = m_Path.getLength() + 1;
    if (index == end || index + 1 == end) {
        return result;
    }
    m_Name.copy(sead::SafeStringBase<char>(&m_Path.at(index + 1)), -1);

    // up to the first extension separator
    s32 nameLength = m_Name.getLength();
    s32 extensionLength = s_ExtensionSeparator.getLength();
    for (s32 i = 0; i <= nameLength - extensionLength; i++) {
        if (sead::SafeStringBase<char>(m_Name.getCString() + i).compare(s_ExtensionSeparator, extensionLength) == 0) {
            if (i > 0 && fileName->copy(m_Name, i) == i) {
                result = true;
            }
            return result;
        }
    }
    return result;
}

// 0x0075D658 (name is ours)
bool Project::HasContent(u8 kind, const sead::SafeStringBase<char>& fileName)
{
    SearchContentFolder condition(kind);
    sead::FixedSafeString<32> contentName;
    for (s32 i = 0; i < m_Project.GetNameNum(); i++) {
        const char* name = m_Project.GetName(i);
        if (name == NULL || !condition.Match(name)) {
            continue;
        }
        if (GetFileName(&contentName, condition.m_pFound) && fileName.isEqual(contentName)) {
            return true;
        }
    }
    return false;
}
} // namespace script
