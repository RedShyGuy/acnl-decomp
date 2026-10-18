#include "nn/nex/nex_RootCaReader.h"
#include "nn/fs/fs_Api.h"
#include "nn/fs/fs_FileInputStream.h"
#include "nn/nex/nex_GlobalVariables.h"
#include "nn/nex/nex_MemoryManager.h"
#include "nn/nex/nex_String.h"

#include <cstdio>

namespace nn {
namespace nex {
// 0x0095F5D8 (name is ours)
HppSettings g_HppSettings = {"LCSDITJXU", "conf:", "n.app.nintendowifi.net", "/hpp/"};

namespace {
// the length of the paths of the certificate files
const u32 PATH_LENGTH = 64;
// the directories of the mounted content
const u32 MAX_DIRECTORIES = 1;
} // namespace

// 0x0035FAA4 | fefates:bytes-fuzzy [tier B]
bool nn::nex::RootCaReader::Read(nn::nex::qVector<unsigned char>* pCertificates, const char** ppNames, unsigned int count)
{
    if (m_UsesCache) {
        qMap<String, qVector<u8> >& cache = GlobalVariables::s_pInstance->m_RootCaCache;
        u32 i;
        for (i = 0; i < count; i++) {
            if (cache.find(String(ppNames[i])) == cache.end()) {
                break;
            }
        }
        if (i == count) {
            // all of them are cached
            for (i = 0; i < count; i++) {
                pCertificates[i] = cache[String(ppNames[i])];
            }
            return true;
        }
    } else {
        GlobalVariables::s_pInstance->m_RootCaCache.clear();
    }
    size_t size = nn::fs::GetContentRequiredMemorySize(nn::fs::MEDIA_TYPE_NAND, CERTIFICATES_TITLE_ID, 0, count, MAX_DIRECTORIES);
    void* pBuffer = MemoryManager::Allocate(size);
    nn::Result result = nn::fs::MountContent(g_HppSettings.m_ArchiveName, nn::fs::MEDIA_TYPE_NAND, CERTIFICATES_TITLE_ID, 0, count, MAX_DIRECTORIES, pBuffer, size, false);
    if (result.IsFailure()) {
        MemoryManager::Free(pBuffer);
        // (a message was logged: "Could not mount shared content.")
        return false;
    }
    bool isRead = ReadImpl(pCertificates, ppNames, count);
    result = nn::fs::Unmount(g_HppSettings.m_ArchiveName);
    if (result.IsFailure()) {
        MemoryManager::Free(pBuffer);
        // (a message was logged: "Could not unmount shared content.")
        return false;
    }
    MemoryManager::Free(pBuffer);
    return isRead;
}

// 0x0035FD78 | fefates:bytes-fuzzy [tier B]
bool nn::nex::RootCaReader::ReadImpl(nn::nex::qVector<unsigned char>* pCertificates, const char** ppNames, unsigned int count)
{
    for (u32 i = 0; i < count; i++) {
        if (m_UsesCache) {
            qMap<String, qVector<u8> >& cache = GlobalVariables::s_pInstance->m_RootCaCache;
            qMap<String, qVector<u8> >::iterator it = cache.find(String(ppNames[i]));
            if (it != cache.end()) {
                pCertificates[i] = it->second;
                continue;
            }
        }
        nn::fs::FileInputStream file;
        char path[PATH_LENGTH];
        snprintf(path, sizeof(path), "%s/%s", g_HppSettings.m_ArchiveName, ppNames[i]);
        if (file.nn::fs::detail::FileBase::TryInitialize(path, nn::fs::OPEN_MODE_READ).IsFailure()) {
            return false;
        }
        s64 size = 0;
        nn::Result result = file.TryGetSize(&size);
        if (result.IsFailure()) {
            // (a message was logged: "Could not get file size of shared content.")
            return false;
        }
        qVector<u8>& certificate = pCertificates[i];
        certificate.ResizeInline(static_cast<u32>(size), 0);
        s32 readSize;
        result = file.nn::fs::detail::FileBase::TryRead(&readSize, certificate.begin(), certificate.size());
        file.Finalize();
        if (result.IsFailure()) {
            // (a message was logged: "Could not read shared content.")
            return false;
        }
        certificate.ResizeInline(static_cast<u32>(readSize), 0);
        if (m_UsesCache) {
            GlobalVariables::s_pInstance->m_RootCaCache.insert(std::pair<const String, qVector<u8> >(String(ppNames[i]), certificate));
        }
    }
    return true;
}

// 0x00360378 slot 0x00
// 0x00360374 (deleting dtor)
nn::nex::RootCaReader::~RootCaReader()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
