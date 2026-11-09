#pragma once

#include "decomp.h"
#include "script/dProject.h"
#include "script/dProject_ISearchContentCondition.h"

// the names in the folders of a kind: the kind of a folder (33..41), the index of a folder (0..31) or all (32)
// RTTI N6script7Project19SearchContentFolderE @ 0x008D3AF0
// vtable 0x0090A960 (vptr 0x0090A968), offset_to_top 0, 3 entries
class script::Project::SearchContentFolder : public ::script::Project::ISearchContentCondition
{
public:
    static const u32 FOLDER_NUM = 32;
    static const u8 KIND_ALL = 32;

    // a folder of the message files
    struct Folder
    {
        u8 kind;
        const char* path;
        s32 length;
    };

    // (inline)
    explicit SearchContentFolder(u8 kind) : m_Kind(kind), m_pFound(NULL) {}
    virtual bool Match(const char* name); // 0x005F7E24 slot 0x00
    virtual ~SearchContentFolder(); // 0x005F7E20 slot 0x04
    // 0x005F7E1C slot 0x08 (deleting dtor)

    u8 m_Kind;            // 0x4
    const char* m_pFound; // 0x8 the name that matched

    static const Folder s_Folders[FOLDER_NUM];
};
