#include "nw/snd/snd_RomSoundArchive.h"

namespace nw {
namespace snd {
// 0x004C057C | fefates:bytes [tier B]
nw::snd::RomSoundArchive::RomSoundArchive() : FsSoundArchiveBase(false)
{
}

// 0x004C05A8 slot 0x00
// 0x004C0598 (deleting dtor)
nw::snd::RomSoundArchive::~RomSoundArchive()
{
    // empty (in the original too)
}

// 0x004C85D4 | mk7dlp:callseq [tier A]
bool nw::snd::internal::FsSoundArchiveBase::LoadHeader(void* pBuffer, u32 size)
{
    const u32 offset = m_ArchiveFileReader.GetInfoBlockOffset();
    const u32 blockSize = m_ArchiveFileReader.GetInfoBlockSize();
    if (blockSize > size) {
        return false;
    }
    if (!m_FileReader.Seek(offset, io::FileStream::SEEK_ORIGIN_BEGIN)) {
        m_LastFsResult = m_FileReader.m_Result;
        return false;
    }
    int readSize = 0;
    if (!m_FileReader.Read(pBuffer, blockSize, &readSize)) {
        m_LastFsResult = m_FileReader.m_Result;
        return false;
    }
    if (static_cast<u32>(readSize) != blockSize) {
        return false;
    }
    m_ArchiveFileReader.SetInfoBlock(pBuffer);
    return true;
}

// 0x004C86E4 | mk7dlp:callseq [tier A]
bool nw::snd::internal::FsSoundArchiveBase::LoadLabelStringData(void* pBuffer, u32 size)
{
    const u32 offset = m_ArchiveFileReader.GetStringBlockOffset();
    const u32 blockSize = m_ArchiveFileReader.GetStringBlockSize();
    if (offset == 0xFFFFFFFF || blockSize > size) {
        return false;
    }
    if (!m_FileReader.Seek(offset, io::FileStream::SEEK_ORIGIN_BEGIN)) {
        m_LastFsResult = m_FileReader.m_Result;
        return false;
    }
    int readSize = 0;
    if (!m_FileReader.Read(pBuffer, blockSize, &readSize)) {
        m_LastFsResult = m_FileReader.m_Result;
        return false;
    }
    if (static_cast<u32>(readSize) != blockSize) {
        return false;
    }
    m_ArchiveFileReader.SetStringBlock(pBuffer);
    return true;
}

} // namespace snd
} // namespace nw
