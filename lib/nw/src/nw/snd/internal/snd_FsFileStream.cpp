#include "nw/snd/internal/snd_FsFileStream.h"

#include "nw/ut/ut_Api.h"

namespace nw {
namespace snd {
namespace internal {
namespace {
// the alignment of the reads at the end of the part
const u32 READ_ALIGNMENT = 32;
} // namespace

// 0x004C6BF4 | fefates:bytes [tier B]
bool nw::snd::internal::FsFileStream::SetPriority(FileReader::Priority priority)
{
    if (!m_pFileReader->SetPriority(priority)) {
        CallErrorCallback();
        return false;
    }
    return true;
}

// 0x004C6C74 slot 0x24 | fefates:bytes
s32 nw::snd::internal::FsFileStream::Read(void* pBuffer, u32 size)
{
    // at the end of the part: up to the end, rounded up
    const u32 position = m_FilePosition.m_Position;
    if (m_Offset + m_Size < position + size) {
        size = ut::RoundUp(m_Offset + m_Size - position, READ_ALIGNMENT);
    }
    if (!m_pFileReader->Seek(position, SEEK_ORIGIN_BEGIN)) {
        CallErrorCallback();
        return -1;
    }
    int readSize = 0;
    if (!m_pFileReader->Read(pBuffer, size, &readSize)) {
        CallErrorCallback();
        return -1;
    }
    if (readSize > 0) {
        m_FilePosition.Skip(readSize);
    }
    return readSize;
}

// 0x004C6DB0 slot 0x40 | fefates:bytes
void nw::snd::internal::FsFileStream::Seek(s32 offset, u32 origin)
{
    s32 position;
    switch (origin) {
    case SEEK_ORIGIN_BEGIN:
        position = m_Offset + offset;
        break;
    case SEEK_ORIGIN_CURRENT:
        position = m_FilePosition.m_Position + offset;
        break;
    case SEEK_ORIGIN_END:
        position = m_Offset + m_Size - offset;
        break;
    default:
        return;
    }
    // within the part
    if (static_cast<s32>(m_Offset) > position) {
        position = m_Offset;
    } else if (static_cast<s32>(m_Offset + m_Size) < position) {
        position = m_Offset + m_Size;
    }
    m_FilePosition.Seek(position, SEEK_ORIGIN_BEGIN);
}

// 0x004C6E14 slot 0x38 | fefates:bytes
void nw::snd::internal::FsFileStream::Close()
{
    if (m_IsOwnFileReader && m_IsOpened) {
        m_pFileReader->Finalize();
        m_IsOpened = false;
    }
}

// 0x004C6E64 | fefates:bytes [tier B]
nw::snd::internal::FsFileStream::FsFileStream(const char* pPath, u32 offset, u32 size, bool isTry, ErrorCallback pCallback, void* pCallbackArg)
    : m_FilePosition(), m_Offset(offset), m_Size(size), m_pCallback(pCallback), m_pCallbackArg(pCallbackArg), m_IsOpened(false),
      m_IsOwnFileReader(true)
{
    if (!m_FileReader.Initialize(pPath, isTry)) {
        if (m_pCallback != nullptr) {
            m_pCallback(m_FileReader.m_Result, m_pCallbackArg);
        }
        return;
    }
    m_pFileReader = &m_FileReader;
    s32 fileSize = 0;
    if (!m_pFileReader->GetSize(&fileSize)) {
        CallErrorCallback();
        return;
    }
    if (m_Size == 0) {
        m_Size = fileSize;
    }
    m_FilePosition.m_FileSize = fileSize;
    m_FilePosition.Seek(0, SEEK_ORIGIN_BEGIN);
    m_IsCloseOnDestroy = true;
    m_IsOpened = true;
}

// 0x004C6FEC | fefates:bytes [tier B]
nw::snd::internal::FsFileStream::FsFileStream(FileReader* pFileReader, u32 offset, u32 size, ErrorCallback pCallback, void* pCallbackArg)
    : m_pFileReader(pFileReader), m_FilePosition(), m_Offset(offset), m_Size(size), m_pCallback(pCallback), m_pCallbackArg(pCallbackArg),
      m_IsOpened(false), m_IsOwnFileReader(false)
{
    // (the original tests m_IsCloseOnDestroy before it is set)
    if (m_IsCloseOnDestroy) {
        Close();
    }
    s32 fileSize = 0;
    if (!m_pFileReader->GetSize(&fileSize)) {
        CallErrorCallback();
        return;
    }
    if (m_Size == 0) {
        m_Size = fileSize;
    }
    m_FilePosition.m_FileSize = fileSize;
    m_FilePosition.Seek(0, SEEK_ORIGIN_BEGIN);
    m_IsCloseOnDestroy = false;
    m_IsOpened = true;
}

// 0x004C721C
// 0x004C7188 (deleting dtor)
nw::snd::internal::FsFileStream::~FsFileStream()
{
    if (m_IsCloseOnDestroy) {
        Close();
    }
}

} // namespace internal
} // namespace snd
} // namespace nw
