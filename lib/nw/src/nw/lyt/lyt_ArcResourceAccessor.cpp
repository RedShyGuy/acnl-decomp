#include "nw/lyt/lyt_ArcResourceAccessor.h"

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <cwchar>

#include "nw/font/font_ResFont.h"
#include "nw/lyt/lyt_Api.h"
#include "nw/lyt/lyt_Layout.h"

// the case-insensitive compare of wide strings of the C library (Thumb code at 0x00101974; name
// is ours)
extern "C" int wcsicmp(const wchar_t* str1, const wchar_t* str2);

namespace nw {
namespace lyt {
namespace {
// the header of a DARC archive (see 3dbrew "DARC"; names are ours)
struct DarcHeader
{
    u32 magic;              // 0x00 'darc'
    u16 byteOrder;          // 0x04
    u16 headerLength;       // 0x06
    u32 version;            // 0x08
    u32 fileSize;           // 0x0C
    u32 fileTableOffset;    // 0x10
    u32 fileTableLength;    // 0x14
    u32 fileDataOffset;     // 0x18
};

// the paths and names longer than this are cut (in characters with the zero)
const u32 PATH_MAX_LEN = 128;

// (inline) the characters of a path as wide characters (at most maxLen - 1)
inline void CopyPath(wchar_t* dst, const char* src, u32 maxLen)
{
    u32 i = 0;
    while (*src != '\0') {
        *dst++ = static_cast<signed char>(*src++);
        if (++i >= maxLen - 1) {
            break;
        }
    }
    *dst = L'\0';
}

// (inline) the resource type as a directory name (four characters)
inline void MakeResTypeName(wchar_t* typeName, u32 resType)
{
    typeName[0] = (resType >> 24) & 0xFF;
    typeName[1] = (resType >> 16) & 0xFF;
    typeName[2] = (resType >> 8) & 0xFF;
    typeName[3] = resType & 0xFF;
    typeName[4] = L'\0';
}

// 0x004B8488 (name is ours)
// the file of the name in the current directory and below; ENTRY_NUM_NONE if there is none
s32 FindNameResource(DarcArchive* pArc, const wchar_t* resName)
{
    s32 entryNum = DarcArchive::ENTRY_NUM_NONE;
    DarcDir dir;
    DarcDirEntry dirEntry;
    pArc->OpenDir(L".", &dir);
    while (dir.Read(&dirEntry)) {
        if (dirEntry.isDir) {
            pArc->ChangeDir(dirEntry.entryNum);
            entryNum = FindNameResource(pArc, resName);
            pArc->ChangeDir(dir.entryNum);
            if (entryNum != DarcArchive::ENTRY_NUM_NONE) {
                break;
            }
        } else if (std::wcscmp(resName, dirEntry.name) == 0) {
            entryNum = dirEntry.entryNum;
            break;
        }
    }
    return entryNum;
}

// 0x004B83C0 (name is ours)
// the same without case
s32 FindNameResourceIgnoreCase(DarcArchive* pArc, const wchar_t* resName)
{
    s32 entryNum = DarcArchive::ENTRY_NUM_NONE;
    DarcDir dir;
    DarcDirEntry dirEntry;
    pArc->OpenDir(L".", &dir);
    while (dir.Read(&dirEntry)) {
        if (dirEntry.isDir) {
            pArc->ChangeDir(dirEntry.name);
            entryNum = FindNameResourceIgnoreCase(pArc, resName);
            pArc->ChangeDir(L"..");
            if (entryNum != DarcArchive::ENTRY_NUM_NONE) {
                break;
            }
        } else if (wcsicmp(resName, dirEntry.name) == 0) {
            entryNum = dirEntry.entryNum;
            break;
        }
    }
    return entryNum;
}

// 0x004B80B4 (name is ours)
// The data of the file resRootDir/<type>/resName (type 0: anywhere below resRootDir, without
// case if there is no exact one) and its size; NULL if there is none.
void* FindResource(DarcArchive* pArc, const wchar_t* resRootDir, u32 resType, const wchar_t* resName, u32* pSize)
{
    s32 entryNum = DarcArchive::ENTRY_NUM_NONE;
    const s32 dirEntryNum = pArc->ConvertPathToEntryNum(resRootDir);
    if (dirEntryNum == DarcArchive::ENTRY_NUM_NONE) {
        return nullptr;
    }
    pArc->ConvertPathToEntryNum(L".");
    if (!pArc->ChangeDir(dirEntryNum)) {
        return nullptr;
    }

    if (resType != 0) {
        wchar_t resTypeName[5];
        MakeResTypeName(resTypeName, resType);
        const s32 typeDirEntryNum = pArc->ConvertPathToEntryNum(resTypeName);
        if (typeDirEntryNum != DarcArchive::ENTRY_NUM_NONE && pArc->ChangeDir(typeDirEntryNum)) {
            entryNum = pArc->ConvertPathToEntryNum(resName);
            pArc->ChangeDir(dirEntryNum);
        }
    } else {
        entryNum = FindNameResource(pArc, resName);
        if (entryNum == DarcArchive::ENTRY_NUM_NONE) {
            entryNum = FindNameResourceIgnoreCase(pArc, resName);
            if (entryNum != DarcArchive::ENTRY_NUM_NONE) {
                // the name differs in case (the warning is not in the program)
                char pathA[PATH_MAX_LEN];
                wchar_t pathW[PATH_MAX_LEN];
                std::wcstombs(pathA, resName, PATH_MAX_LEN - 1);
                pathA[PATH_MAX_LEN - 1] = '\0';
                if (pArc->GetPath(entryNum, pathW, PATH_MAX_LEN - 1)) {
                    std::wcstombs(pathA, pathW, PATH_MAX_LEN - 1);
                    pathA[PATH_MAX_LEN - 1] = '\0';
                }
            }
        }
    }

    pArc->ChangeDir(L"..");
    if (entryNum == DarcArchive::ENTRY_NUM_NONE) {
        return nullptr;
    }

    DarcFileInfo fileInfo;
    pArc->OpenFile(entryNum, &fileInfo);
    void* pData = const_cast<void*>(fileInfo.GetData());
    if (pSize != nullptr) {
        *pSize = fileInfo.length;
    }
    return pData;
}
} // namespace

// 0x004B51CC (name is ours)
bool nw::lyt::DarcArchive::Init(const void* pArchive, DarcArchive* pHandle)
{
    const DarcHeader* pHeader = static_cast<const DarcHeader*>(pArchive);
    pHandle->m_pArchive = pArchive;
    const DarcEntry* entries = reinterpret_cast<const DarcEntry*>(reinterpret_cast<uptr>(pArchive) + pHeader->fileTableOffset);
    pHandle->m_pEntries = entries;
    pHandle->m_pFileData = reinterpret_cast<const void*>(reinterpret_cast<uptr>(pArchive) + pHeader->fileDataOffset);
    const u32 entryNum = entries[0].size;
    pHandle->m_pNameTable = reinterpret_cast<const wchar_t*>(&entries[entryNum]);
    pHandle->m_EntryNum = entryNum;
    pHandle->m_FileTableLength = pHeader->fileTableLength;
    pHandle->m_CurrentDir = 0;
    return true;
}

// 0x004B6DB4 (name is ours)
s32 nw::lyt::DarcArchive::ConvertPathToEntryNum(const wchar_t* pPath) const
{
    const wchar_t* const pOrigPath = pPath;
    const DarcEntry* entries = m_pEntries;
    s32 entryNum = m_CurrentDir;

    for (;;) {
        const wchar_t c = pPath[0];
        if (c == L'\0') {
            return entryNum;
        }
        if (c == L'/') {
            entryNum = 0;
            ++pPath;
            continue;
        }
        if (c == L'.') {
            if (pPath[1] == L'.') {
                if (pPath[2] == L'/') {
                    entryNum = entries[entryNum].offset;
                    pPath += 3;
                    continue;
                }
                if (pPath[2] == L'\0') {
                    return entries[entryNum].offset;
                }
            } else if (pPath[1] == L'/') {
                pPath += 2;
                continue;
            } else if (pPath[1] == L'\0') {
                return entryNum;
            }
        }

        // the next name of the path
        const wchar_t* pNameEnd = pPath;
        while (*pNameEnd != L'\0' && *pNameEnd != L'/') {
            ++pNameEnd;
        }
        const bool isDir = (*pNameEnd == L'/');
        const s32 nameLen = pNameEnd - pPath;
        const DarcEntry& dirEntry = entries[entryNum];
        const u32 dirNext = dirEntry.size;
        bool isFound = false;
        u32 i;

        // the name as it is
        for (i = entryNum + 1; i < dirNext;) {
            const bool isEntryDir = entries[i].IsDir();
            if (!isDir || isEntryDir) {
                const wchar_t* pName = GetName(i);
                if (pName[0] == L'.' && pName[1] == L'\0') {
                    ++i;
                    continue;
                }
                if (pName[0] == c) {
                    const wchar_t* p1 = pPath + 1;
                    const wchar_t* p2 = pName + 1;
                    bool isSame = true;
                    while (*p2 != L'\0') {
                        if (*p1++ != *p2++) {
                            isSame = false;
                            break;
                        }
                    }
                    if (isSame && (*p1 == L'/' || *p1 == L'\0')) {
                        isFound = true;
                        goto found;
                    }
                }
            }
            if (isEntryDir) {
                i = entries[i].size;
            } else {
                ++i;
            }
        }

        // the name without case
        for (i = entryNum + 1; i < dirEntry.size;) {
            if (!entries[i].IsDir() && isDir) {
                ++i;
                continue;
            }
            const wchar_t* pName = GetName(i);
            if (pName[0] == L'.' && pName[1] == L'\0') {
                ++i;
                continue;
            }
            const wchar_t* p1 = pPath;
            const wchar_t* p2 = pName;
            bool isSame = true;
            if (*p2 != L'\0') {
                do {
                    if ((tolower)(*p1++) != (tolower)(*p2++)) {
                        isSame = false;
                        break;
                    }
                } while (*p2 != L'\0');
            }
            if (isSame && (*p1 == L'/' || *p1 == L'\0')) {
                goto found;
            }
            if (entries[i].IsDir()) {
                i = entries[i].size;
            } else {
                ++i;
            }
        }
        return ENTRY_NUM_NONE;

    found:
        if (isDir) {
            entryNum = i;
            pPath += nameLen + 1;
            continue;
        }
        if (!isFound) {
            // the name differs in case (the warning is not in the program)
            char pathA[PATH_MAX_LEN];
            wchar_t pathW[PATH_MAX_LEN];
            std::wcstombs(pathA, pOrigPath, PATH_MAX_LEN - 1);
            pathA[PATH_MAX_LEN - 1] = '\0';
            if (GetPath(i, pathW, PATH_MAX_LEN - 1)) {
                std::wcstombs(pathA, pathW, PATH_MAX_LEN - 1);
                pathA[PATH_MAX_LEN - 1] = '\0';
            }
        }
        return i;
    }
}

// 0x004B50D0 (name is ours)
bool nw::lyt::DarcArchive::ChangeDir(s32 entryNum)
{
    if (entryNum < 0 || static_cast<s32>(m_EntryNum) <= entryNum || !m_pEntries[entryNum].IsDir()) {
        return false;
    }
    m_CurrentDir = entryNum;
    return true;
}

// 0x004B5098 (name is ours)
bool nw::lyt::DarcArchive::ChangeDir(const wchar_t* pPath)
{
    const s32 entryNum = ConvertPathToEntryNum(pPath);
    if (entryNum < 0 || !m_pEntries[entryNum].IsDir()) {
        return false;
    }
    m_CurrentDir = entryNum;
    return true;
}

// 0x004B4A04 (name is ours)
bool nw::lyt::DarcArchive::OpenDir(const wchar_t* pPath, DarcDir* pDir) const
{
    const s32 entryNum = ConvertPathToEntryNum(pPath);
    if (entryNum < 0 || !m_pEntries[entryNum].IsDir()) {
        return false;
    }
    pDir->pArchive = this;
    pDir->entryNum = entryNum;
    pDir->location = entryNum + 1;
    pDir->next = m_pEntries[entryNum].size;
    return true;
}

// 0x004BBA74 (name is ours)
bool nw::lyt::DarcArchive::OpenTypeDir(const wchar_t* pRootPath, u32 resType, DarcDir* pDir)
{
    if (!ChangeDir(pRootPath)) {
        return false;
    }
    wchar_t resTypeName[5];
    MakeResTypeName(resTypeName, resType);
    return OpenDir(resTypeName, pDir);
}

// 0x004B4B2C (name is ours)
bool nw::lyt::DarcArchive::OpenFile(s32 entryNum, DarcFileInfo* pFileInfo) const
{
    if (entryNum < 0 || static_cast<s32>(m_EntryNum) <= entryNum || m_pEntries[entryNum].IsDir()) {
        return false;
    }
    pFileInfo->pArchive = this;
    pFileInfo->offset = m_pEntries[entryNum].offset;
    pFileInfo->length = m_pEntries[entryNum].size;
    return true;
}

// 0x004B4F80 (name is ours)
DECOMP_NOINLINE u32 nw::lyt::DarcArchive::GetPathRecursive(s32 entryNum, wchar_t* pPath, u32 maxLen) const
{
    if (entryNum == 0) {
        return 0;
    }

    const DarcEntry* entries = m_pEntries;
    const wchar_t* pName = GetName(entryNum);
    u32 parent;
    if (entries[entryNum].IsDir()) {
        parent = entries[entryNum].offset;
    } else {
        // the directory of the file: the last one before it that holds it
        for (parent = entryNum; parent != 0; --parent) {
            if (entries[parent].IsDir() && entries[parent].size > static_cast<u32>(entryNum)) {
                break;
            }
        }
    }

    const u32 len = GetPathRecursive(parent, pPath, maxLen);
    if (len == maxLen) {
        return len;
    }
    pPath[len] = L'/';
    wchar_t* dst = pPath + len + 1;
    const u32 rest = maxLen - (len + 1);
    u32 n = rest;
    while (n != 0 && *pName != L'\0') {
        *dst++ = *pName++;
        --n;
    }
    return rest - n + len + 1;
}

// 0x004B6D3C (name is ours)
bool nw::lyt::DarcArchive::GetPath(s32 entryNum, wchar_t* pPath, u32 maxLen) const
{
    const DarcEntry* entries = m_pEntries;
    u32 len = GetPathRecursive(entryNum, pPath, maxLen);
    if (len == maxLen) {
        pPath[maxLen - 1] = L'\0';
        return false;
    }
    if (entries[entryNum].IsDir()) {
        if (len == maxLen - 1) {
            pPath[len] = L'\0';
            return false;
        }
        pPath[len++] = L'/';
    }
    pPath[len] = L'\0';
    return true;
}

// 0x004B6D2C (name is ours)
const void* nw::lyt::DarcFileInfo::GetData() const
{
    return reinterpret_cast<const void*>(reinterpret_cast<uptr>(pArchive->m_pArchive) + offset);
}

// 0x004B4A5C (name is ours)
bool nw::lyt::DarcDir::Read(DarcDirEntry* pEntry)
{
    const DarcArchive* pArc = pArchive;
    const DarcEntry* entries = pArc->m_pEntries;
    u32 loc = location;
    for (;;) {
        if (!(static_cast<u32>(entryNum) < loc && loc < next)) {
            return false;
        }
        pEntry->entryNum = loc;
        pEntry->pArchive = pArc;
        pEntry->isDir = entries[loc].IsDir();
        const wchar_t* pName = pArc->GetName(loc);
        pEntry->name = pName;
        if (pName[0] == L'.' && pName[1] == L'\0') {
            ++loc;
            continue;
        }
        location = entries[loc].IsDir() ? entries[loc].size : loc + 1;
        return true;
    }
}

// 0x004BBADC (name is ours)
bool nw::lyt::DarcDir::ReadWithExtension(const wchar_t* ext, u32 extLen, DarcDirEntry* pEntry)
{
    while (Read(pEntry)) {
        const u32 len = std::wcslen(pEntry->name);
        if (len > extLen && wcsicmp(pEntry->name + len - extLen, ext) == 0) {
            return true;
        }
    }
    return false;
}

// (inline)
nw::lyt::FontRefLink::~FontRefLink()
{
    if (m_IsOwned) {
        if (m_pFont != nullptr) {
            font::ResFont* pResFont = static_cast<font::ResFont*>(m_pFont);
            void* pDrawBuffer = pResFont->m_pTextureObjects;
            if (pDrawBuffer != nullptr) {
                pResFont->SetDrawBuffer(nullptr);
                Layout::FreeMemory(pDrawBuffer);
            }
        }
        Layout::DeleteObj(m_pFont);
        m_pFont = nullptr;
    }
}

// 0x004B5468 (name is ours)
nw::lyt::FontRefLinkList::~FontRefLinkList()
{
    while (!empty()) {
        FontRefLink* pLink = GetPointerFromNode(begin().GetNode());
        Erase(pLink);
        Layout::DeleteObj(pLink);
    }
}

// 0x004B5300 (name is ours)
FontRefLink* nw::lyt::FontRefLinkList::Regist(const char* name, const font::Font* pFont, bool isOwned)
{
    FontRefLink* pLink = Layout::NewObj<FontRefLink>();
    if (pLink == nullptr) {
        return nullptr;
    }
    internal::CopyString(pLink->m_FontName, sizeof(pLink->m_FontName), name);
    pLink->m_pFont = const_cast<font::Font*>(pFont);
    pLink->m_IsOwned = isOwned;
    PushBack(pLink);
    return pLink;
}

// 0x004B5398 (name is ours)
void nw::lyt::FontRefLinkList::Unregist(FontRefLink* pLink)
{
    Erase(pLink);
    Layout::DeleteObj(pLink);
}

// 0x004B5418 (name is ours)
font::Font* nw::lyt::FontRefLinkList::Find(const char* name)
{
    for (iterator it = begin(); it != end(); ++it) {
        FontRefLink* pLink = GetPointerFromNode(it.GetNode());
        if (std::strcmp(name, pLink->m_FontName) == 0) {
            return pLink->m_pFont;
        }
    }
    return nullptr;
}

// (inline)
nw::lyt::TextureRefLink::~TextureRefLink()
{
    DeleteTexture(m_TexInfo);
}

// 0x004B6014 (name is ours)
nw::lyt::TextureRefLinkList::~TextureRefLinkList()
{
    Clear();
}

// 0x004B5E88 (name is ours)
TextureRefLink* nw::lyt::TextureRefLinkList::Regist(const char* name, const TextureInfo& texInfo)
{
    TextureRefLink* pLink = Layout::NewObj<TextureRefLink>();
    if (pLink == nullptr) {
        return nullptr;
    }
    internal::CopyString(pLink->m_TexName, sizeof(pLink->m_TexName), name);
    pLink->m_TexInfo = texInfo;
    PushBack(pLink);
    return pLink;
}

// 0x004B5F30 (name is ours)
TextureInfo nw::lyt::TextureRefLinkList::Find(const char* name)
{
    for (iterator it = begin(); it != end(); ++it) {
        TextureRefLink* pLink = GetPointerFromNode(it.GetNode());
        if (std::strcmp(name, pLink->m_TexName) == 0) {
            return pLink->m_TexInfo;
        }
    }
    return TextureInfo();
}

// 0x004B5FC0 (name is ours)
void nw::lyt::TextureRefLinkList::Clear()
{
    while (!empty()) {
        TextureRefLink* pLink = GetPointerFromNode(begin().GetNode());
        Erase(pLink);
        Layout::DeleteObj(pLink);
    }
}

// 0x0012BF18
nw::lyt::ArcResourceAccessor::ArcResourceAccessor() : m_pArchive(nullptr)
{
}

// 0x004B6D04 slot 0x00 (name is ours)
// 0x004B6CD8 (deleting dtor)
nw::lyt::ArcResourceAccessor::~ArcResourceAccessor()
{
    // nothing to do: the members are destroyed by the compiler
}

// 0x004B6C20 (name is ours)
bool nw::lyt::ArcResourceAccessor::Attach(void* pArchive, const char* rootDirectory)
{
    if (!DarcArchive::Init(pArchive, &m_ArcHandle)) {
        return false;
    }
    m_pArchive = pArchive;
    CopyPath(m_ResRootDir, rootDirectory, ROOTPATH_MAX);
    return true;
}

// 0x00137B04 (name is ours)
void* nw::lyt::ArcResourceAccessor::Detach()
{
    void* pArchive = m_pArchive;
    m_pArchive = nullptr;
    return pArchive;
}

// 0x004B6BCC slot 0x08 (name is ours)
void* nw::lyt::ArcResourceAccessor::GetResource(u32 resType, const char* name, u32* pSize)
{
    CopyPath(m_ResName, name, NAME_MAX);
    return FindResource(&m_ArcHandle, m_ResRootDir, resType, m_ResName, pSize);
}

// 0x004B6C84 slot 0x0C (name is ours)
font::Font* nw::lyt::ArcResourceAccessor::GetFont(const char* name)
{
    font::Font* pFont = m_FontList.Find(name);
    if (pFont == nullptr) {
        pFont = LoadFont(name);
        if (pFont != nullptr) {
            m_FontList.Regist(name, pFont, true);
        }
    }
    return pFont;
}

// 0x004B6B64 slot 0x10 | libgarden
TextureInfo nw::lyt::ArcResourceAccessor::GetTexture(const char* name)
{
    TextureInfo texInfo = m_TextureList.Find(name);
    if (texInfo.physicalAddress == 0) {
        texInfo = LoadTexture(name);
        if (texInfo.physicalAddress != 0) {
            m_TextureList.Regist(name, texInfo);
        }
    }
    return texInfo;
}

// 0x004B52F4 | libgarden [tier A]
FontRefLink* nw::lyt::ArcResourceAccessor::RegistFont(const char* name, const font::Font* pFont)
{
    return m_FontList.Regist(name, pFont, false);
}

// 0x004B5390 (name is ours)
void nw::lyt::ArcResourceAccessor::UnregistFont(FontRefLink* pLink)
{
    m_FontList.Unregist(pLink);
}

} // namespace lyt
} // namespace nw
