#include "nw/snd/internal/snd_FileReader.h"

namespace nw {
namespace snd {
namespace internal {
// 0x004C5B98 | fefates:bytes [tier B]
bool nw::snd::internal::FileReader::Initialize(const char* pPath, bool isTry)
{
    m_IsTry = isTry;
    if (m_IsTry) {
        m_Result = m_File.TryInitialize(pPath);
        return m_Result.IsSuccess();
    }
    nn::Result result = m_File.TryInitialize(pPath);
    nn::err::CTR::ThrowFatalErrAllIfFailure(result);
    return true;
}

// 0x004C5CF8 | fefates:bytes [tier B]
bool nw::snd::internal::FileReader::Read(void* pBuffer, int size, int* pReadSize)
{
    if (m_IsTry) {
        m_Result = m_File.nn::fs::detail::FileBase::TryRead(pReadSize, pBuffer, size);
        return m_Result.IsSuccess();
    }
    s32 readSize;
    nn::Result result = m_File.nn::fs::detail::FileBase::TryRead(&readSize, pBuffer, size);
    nn::err::CTR::ThrowFatalErrAllIfFailure(result);
    *pReadSize = readSize;
    return true;
}

// 0x004C5D5C (name is ours; symbols.json says FileBase::TrySeek)
bool nw::snd::internal::FileReader::Seek(int offset, int origin)
{
    // (the original passes an undefined base for another origin)
    nn::fs::PositionBase base;
    switch (origin) {
    case 0:
        base = nn::fs::POSITION_BASE_BEGIN;
        break;
    case 1:
        base = nn::fs::POSITION_BASE_CURRENT;
        break;
    case 2:
        base = nn::fs::POSITION_BASE_END;
        break;
    }
    if (m_IsTry) {
        m_Result = m_File.nn::fs::detail::FileBase::TrySeek(offset, base);
        return m_Result.IsSuccess();
    }
    nn::Result result = m_File.nn::fs::detail::FileBase::TrySeek(offset, base);
    nn::err::CTR::ThrowFatalErrAllIfFailure(result);
    return true;
}

// 0x004C5DD8 | fefates:bytes [tier B]
void nw::snd::internal::FileReader::Finalize()
{
    m_File.nn::fs::detail::FileBase::Finalize();
}

} // namespace internal
} // namespace snd
} // namespace nw
