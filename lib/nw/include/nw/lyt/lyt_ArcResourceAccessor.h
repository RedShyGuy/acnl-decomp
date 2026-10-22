#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_ResourceAccessor.h"
#include "nw/lyt/lyt_Types.h"
#include "nw/ut/ut_LinkList.h"

namespace nw {
namespace font {
class Font;
} // namespace font

namespace lyt {
// A DARC archive in memory (see 3dbrew "DARC"): a table of entries (directories with the index
// behind their last entry, files with their offset and size) and the UTF-16 names. ArcResource-
// Accessor and the game (ssys::ma::lyt) use it. Class, function and member names are ours; most
// functions are inline in the original (copies out of line next to their users).
struct DarcEntry
{
    static const u32 FLAG_DIRECTORY = 0xFF000000; // in nameOffset
    bool IsDir() const { return (nameOffset & FLAG_DIRECTORY) != 0; }
    u32 GetNameOffset() const { return nameOffset & ~FLAG_DIRECTORY; }

    u32 nameOffset; // 0x0 (with the directory flag)
    u32 offset;     // 0x4 the data (directories: the parent)
    u32 size;       // 0x8 the size (directories: the index behind the last entry)
};
ASSERT_SIZE(DarcEntry, 0xC);

class DarcArchive;

// a file of the archive
struct DarcFileInfo
{
    // the data of the file
    const void* GetData() const; // 0x004B6D2C (name is ours)

    const DarcArchive* pArchive;    // 0x0
    u32 offset;                     // 0x4
    u32 length;                     // 0x8
};
ASSERT_SIZE(DarcFileInfo, 0xC);

// an entry of a directory
struct DarcDirEntry
{
    const DarcArchive* pArchive;    // 0x0
    s32 entryNum;                   // 0x4
    const wchar_t* name;            // 0x8
    bool isDir;                     // 0xC
};
ASSERT_SIZE(DarcDirEntry, 0x10);

// an open directory
struct DarcDir
{
    // the next entry (not "."); false at the end
    bool Read(DarcDirEntry* pEntry); // 0x004B4A5C (name is ours)
    // the next entry whose name ends with ext (extLen characters, case-insensitive)
    bool ReadWithExtension(const wchar_t* ext, u32 extLen, DarcDirEntry* pEntry); // 0x004BBADC (name is ours)

    const DarcArchive* pArchive;    // 0x0
    s32 entryNum;                   // 0x4
    u32 location;                   // 0x8 the next entry
    u32 next;                       // 0xC behind the last entry
};
ASSERT_SIZE(DarcDir, 0x10);

class DarcArchive
{
public:
    static const s32 ENTRY_NUM_NONE = -1;

    // the archive at pArchive into pHandle (always true)
    static bool Init(const void* pArchive, DarcArchive* pHandle); // 0x004B51CC (name is ours)

    // the entry of the path (relative to the current directory; "/", "." and ".." work; a name
    // that is not there is looked for without case); ENTRY_NUM_NONE if there is none
    s32 ConvertPathToEntryNum(const wchar_t* pPath) const; // 0x004B6DB4 (name is ours)
    // the current directory
    bool ChangeDir(s32 entryNum); // 0x004B50D0 (name is ours)
    bool ChangeDir(const wchar_t* pPath); // 0x004B5098 (name is ours)
    bool OpenDir(const wchar_t* pPath, DarcDir* pDir) const; // 0x004B4A04 (name is ours)
    // the directory of the resource type (four characters) under the root path
    bool OpenTypeDir(const wchar_t* pRootPath, u32 resType, DarcDir* pDir); // 0x004BBA74 (name is ours)
    bool OpenFile(s32 entryNum, DarcFileInfo* pFileInfo) const; // 0x004B4B2C (name is ours)
    // the path of the entry (directories end with '/'); false if it does not fit
    bool GetPath(s32 entryNum, wchar_t* pPath, u32 maxLen) const; // 0x004B6D3C (name is ours)

    // (inline)
    const DarcEntry* GetEntries() const { return m_pEntries; }
    const wchar_t* GetName(s32 entryNum) const
    {
        return reinterpret_cast<const wchar_t*>(reinterpret_cast<uptr>(m_pNameTable) + m_pEntries[entryNum].GetNameOffset());
    }

private:
    // the path of the entry from the root without the terminating zero; its length
    u32 GetPathRecursive(s32 entryNum, wchar_t* pPath, u32 maxLen) const; // 0x004B4F80 (name is ours)

public:
    const void* m_pArchive;         // 0x00
    const DarcEntry* m_pEntries;    // 0x04
    const void* m_pFileData;        // 0x08
    u32 m_EntryNum;                 // 0x0C
    const wchar_t* m_pNameTable;    // 0x10
    u32 m_FileTableLength;          // 0x14
    s32 m_CurrentDir;               // 0x18
};
ASSERT_SIZE(DarcArchive, 0x1C);

// a font the accessor keeps
struct FontRefLink
{
    static const int FONT_NAME_MAX = 128;

    // (inline)
    FontRefLink() : m_pFont(nullptr), m_IsOwned(false) {}
    // (inline) an owned font is deleted with its draw buffer
    ~FontRefLink();

    ut::LinkListNode m_Link;            // 0x00
    char m_FontName[FONT_NAME_MAX];     // 0x08
    font::Font* m_pFont;                // 0x88
    bool m_IsOwned;                     // 0x8C
};
ASSERT_SIZE(FontRefLink, 0x90);

class FontRefLinkList : public ut::LinkList<FontRefLink, 0>
{
public:
    ~FontRefLinkList(); // 0x004B5468 (name is ours)

    FontRefLink* Regist(const char* name, const font::Font* pFont, bool isOwned); // 0x004B5300 (name is ours)
    void Unregist(FontRefLink* pLink); // 0x004B5398 (name is ours)
    font::Font* Find(const char* name); // 0x004B5418 (name is ours)
};

// a texture the accessor keeps
struct TextureRefLink
{
    static const int TEXTURE_NAME_MAX = 128;

    // (inline)
    TextureRefLink() : m_TexInfo() { m_TexName[0] = '\0'; }
    // (inline) the texture is deleted
    ~TextureRefLink();

    ut::LinkListNode m_Link;                // 0x00
    char m_TexName[TEXTURE_NAME_MAX];       // 0x08
    TextureInfo m_TexInfo;                  // 0x88
};
ASSERT_SIZE(TextureRefLink, 0x9C);

class TextureRefLinkList : public ut::LinkList<TextureRefLink, 0>
{
public:
    ~TextureRefLinkList(); // 0x004B6014 (name is ours)

    TextureRefLink* Regist(const char* name, const TextureInfo& texInfo); // 0x004B5E88 (name is ours)
    TextureInfo Find(const char* name); // 0x004B5F30 (name is ours)
    void Clear(); // 0x004B5FC0 (name is ours)
};

// RTTI N2nw3lyt19ArcResourceAccessorE @ 0x008D085C
// vtable 0x00902B74 (vptr 0x00902B7C), offset_to_top 0, 7 entries
//
// The files of a layout from a DARC archive: <root>/<type>/<name> (any type: anywhere below
// the root). Fonts and textures are kept in lists.
class ArcResourceAccessor : public ::nw::lyt::ResourceAccessor
{
public:
    static const int ROOTPATH_MAX = 64;
    static const int NAME_MAX = 64;

    ArcResourceAccessor(); // 0x0012BF18
    virtual ~ArcResourceAccessor(); // 0x004B6D04 slot 0x00 (name is ours)
    // 0x004B6CD8 (deleting dtor)
    virtual void* GetResource(u32 resType, const char* name, u32* pSize); // 0x004B6BCC slot 0x08 (name is ours)
    virtual font::Font* GetFont(const char* name); // 0x004B6C84 slot 0x0C (name is ours)
    virtual TextureInfo GetTexture(const char* name); // 0x004B6B64 slot 0x10 | libgarden

    // the archive (rootDirectory: the root of the resources in it); false if it is not valid
    bool Attach(void* pArchive, const char* rootDirectory); // 0x004B6C20 (name is ours)
    FontRefLink* RegistFont(const char* name, const font::Font* pFont); // 0x004B52F4 | libgarden [tier A]
    void UnregistFont(FontRefLink* pLink); // 0x004B5390 (name is ours)

    DarcArchive m_ArcHandle;                    // 0x004
    void* m_pArchive;                           // 0x020
    FontRefLinkList m_FontList;                 // 0x024
    TextureRefLinkList m_TextureList;           // 0x030
    wchar_t m_ResRootDir[ROOTPATH_MAX];         // 0x03C
    wchar_t m_ResName[NAME_MAX];                // 0x0BC
};
ASSERT_SIZE(ArcResourceAccessor, 0x13C);
} // namespace lyt
} // namespace nw
