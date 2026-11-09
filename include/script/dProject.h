#pragma once

#include "decomp.h"
#include "libms/libms.h"
#include "script/dLoader.h"
#include "sead/seadFixedSafeString.h"

namespace script {
// The message project of the game (Script/CTR_GardenPlus.msbp, MsgPrjBn: colors, the names of the message files with
// their folders). Singleton, created and loaded by the setup of the scripts; the names are ours.
class Project
{
public:
    class ISearchContentCondition;
    class SearchContentFolder;
    class SingletonDisposer_;

    // (inline in the setup)
    Project() : m_Loader(true) {}

    // the file name (without folder and extension) of a name of the project into fileName; true: there is one
    bool GetFileName(sead::BufferedSafeStringBase<char>* fileName, const char* name); // 0x0075D7F8
    // true: a name of the folders of kind (SearchContentFolder) has the file name fileName
    bool HasContent(u8 kind, const sead::SafeStringBase<char>& fileName); // 0x0075D658

    u32 m_SingletonDisposerBuffer[4];   // 0x000 the SingletonDisposer_
    Loader m_Loader;                    // 0x010 the project file
    libms::Project m_Project;           // 0x110
    sead::FixedSafeString<64> m_Path;   // 0x118 the name with its folders (GetFileName)
    sead::FixedSafeString<64> m_Name;   // 0x164 the name without folders (GetFileName)

    static Project* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;
};
ASSERT_SIZE(Project, 0x1B0);
} // namespace script
