#include "script/dProject_SearchContentFolder.h"
#include "sead/seadSafeStringBase.h"

// the folders of the message files with their kinds and the lengths of their paths
// 0x00891C8C
const script::Project::SearchContentFolder::Folder script::Project::SearchContentFolder::s_Folders[FOLDER_NUM] = {
    {33, "FLOW/", 5},
    {34, "GAME/GAME_ISLAND/", 17},
    {34, "GAME/GAME_PANEPON/", 18},
    {34, "GAME/", 5},
    {35, "NPC_AUTOCAMP/AUTOCAMP_A/", 24},
    {35, "NPC_AUTOCAMP/AUTOCAMP_B/", 24},
    {35, "NPC_AUTOCAMP/AUTOCAMP_C/", 24},
    {35, "NPC_B1_BO/", 10},
    {35, "NPC_B2_HA/", 10},
    {35, "NPC_B3_KO/", 10},
    {35, "NPC_B4_ZK/", 10},
    {35, "NPC_G1_FU/", 10},
    {35, "NPC_G2_GE/", 10},
    {35, "NPC_G3_OT/", 10},
    {35, "NPC_G4_AN/", 10},
    {35, "NPC_OBJ/", 8},
    {35, "NPC_SP/", 7},
    {36, "SYS_2D/", 7},
    {37, "SYS_BBS/", 8},
    {38, "SYS_MAIL_NNPC/FROM_NNPC", 23},
    {39, "SYS_MAIL_NNPC/MSG_NNPC", 22},
    {40, "SYS_MAIL_NNPC/TO_NNPC", 21},
    {41, "SYS_MAIL_RND/FROM_RND", 21},
    {41, "SYS_MAIL_RND/MSG_RND_A", 22},
    {41, "SYS_MAIL_RND/MSG_RND_B", 22},
    {41, "SYS_MAIL_RND/MSG_RND_C", 22},
    {41, "SYS_MAIL_RND/TO_RND", 19},
    {38, "SYS_MAIL_SP/FROM_SP", 19},
    {39, "SYS_MAIL_SP/MSG_SP", 18},
    {40, "SYS_MAIL_SP/TO_SP", 17},
    {35, "SYS_MESS/", 9},
    {36, "SYS_STRING/", 11},
};

// 0x005F7E24 slot 0x00
bool script::Project::SearchContentFolder::Match(const char* name)
{
    sead::SafeStringBase<char> nameString(name);
    bool isAll = m_Kind == KIND_ALL;
    for (u32 i = 0; i < FOLDER_NUM; i++) {
        const Folder* folder = &s_Folders[i];
        if (m_Kind == i || folder->kind == m_Kind || isAll) {
            if (nameString.compare(sead::SafeStringBase<char>(folder->path), folder->length) == 0) {
                m_pFound = name;
                return true;
            }
        }
    }
    m_pFound = NULL;
    return false;
}

// 0x005F7E20 slot 0x04
// 0x005F7E1C (deleting dtor)
script::Project::SearchContentFolder::~SearchContentFolder()
{
    // empty (in the original too)
}
