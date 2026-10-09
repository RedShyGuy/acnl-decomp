#include "nn/boss/boss_NsData.h"
#include "nn/boss/detail/boss_IpcManager.h"
#include "nn/boss/detail/detail_Api.h"
#include "nn/fnd/fnd_DateTime.h"

namespace nn {
namespace boss {
namespace {
// the codes of ChangeBossRetCodeToResult (names are ours)
const nn::boss::ResultCode CODE_NULL_HEADER_INFO = static_cast<nn::boss::ResultCode>(14);
const nn::boss::ResultCode CODE_NULL_FLAG = static_cast<nn::boss::ResultCode>(16);
const nn::boss::ResultCode CODE_NULL_DATE_TIME = static_cast<nn::boss::ResultCode>(17);

// the program id of a new object (meaning unknown)
const u64 DEFAULT_PROGRAM_ID = 0xA010;

// the errors of ReadData
const s32 READ_ERROR_SIZE = -1;
const s32 READ_ERROR_READ = -2;
const s32 READ_ERROR_SESSION = -3;
const s32 READ_ERROR_CHANGED = -4;
} // namespace

// 0x0046C854 | nintendogs:bytes [tier A]
nn::Result nn::boss::NsData::Initialize(unsigned serialId)
{
    m_SerialId = serialId;
    m_Size = 0;
    m_ReadOffset = 0;
    m_Version = 0;
    return nn::Result();
}

// 0x0046C874 | nintendogs:bytes [tier A]
nn::Result nn::boss::NsData::GetReadFlag(bool* pFlag)
{
    if (pFlag == 0) {
        return detail::ChangeBossRetCodeToResult(CODE_NULL_FLAG);
    }
    detail::Privileged* pPrivileged;
    nn::Result result = detail::GetPrivilegedIpcInstance(pPrivileged);
    if (m_IsPrivileged) {
        if (result.IsFailure()) {
            return result;
        }
        return pPrivileged->GetNsDataNewFlagPrivileged(m_ProgramId, m_SerialId, pFlag);
    }
    if (result.IsSuccess()) {
        return pPrivileged->GetNsDataNewFlag(m_SerialId, pFlag);
    }
    detail::User* pUser;
    result = detail::GetUserIpcInstance(pUser);
    if (result.IsFailure()) {
        return result;
    }
    return pUser->GetNsDataNewFlag(m_SerialId, pFlag);
}

// 0x0046C920 | nintendogs:bytes [tier A]
nn::Result nn::boss::NsData::SetReadFlag(bool flag)
{
    detail::Privileged* pPrivileged;
    nn::Result result = detail::GetPrivilegedIpcInstance(pPrivileged);
    if (m_IsPrivileged) {
        if (result.IsFailure()) {
            return result;
        }
        return pPrivileged->SetNsDataNewFlagPrivileged(m_ProgramId, m_SerialId, flag);
    }
    if (result.IsSuccess()) {
        return pPrivileged->SetNsDataNewFlag(m_SerialId, flag);
    }
    detail::User* pUser;
    result = detail::GetUserIpcInstance(pUser);
    if (result.IsFailure()) {
        return result;
    }
    return pUser->SetNsDataNewFlag(m_SerialId, flag);
}

// 0x0046C9B8 | nintendogs:bytes [tier A]
nn::Result nn::boss::NsData::GetHeaderInfo(nn::boss::HeaderInfoType type, void* pValue, unsigned size)
{
    if (pValue == 0) {
        return detail::ChangeBossRetCodeToResult(CODE_NULL_HEADER_INFO);
    }
    detail::Privileged* pPrivileged;
    nn::Result result = detail::GetPrivilegedIpcInstance(pPrivileged);
    if (m_IsPrivileged) {
        if (result.IsFailure()) {
            return result;
        }
        return pPrivileged->GetNsDataHeaderInfoPrivileged(m_ProgramId, m_SerialId, type, static_cast<unsigned char*>(pValue), size);
    }
    if (result.IsSuccess()) {
        return pPrivileged->GetNsDataHeaderInfo(m_SerialId, type, static_cast<unsigned char*>(pValue), size);
    }
    detail::User* pUser;
    result = detail::GetUserIpcInstance(pUser);
    if (result.IsFailure()) {
        return result;
    }
    return pUser->GetNsDataHeaderInfo(m_SerialId, type, static_cast<unsigned char*>(pValue), size);
}

// the time is packed: milliseconds (10 bits), seconds, minutes (6 bits each), hours; then the
// day (6 bits), the month (4 bits) and the year
// 0x0046CA84 | nintendogs:bytes [tier B]
nn::Result nn::boss::NsData::GetLastUpdated(nn::fnd::DateTime* pDateTime)
{
    if (pDateTime == 0) {
        return detail::ChangeBossRetCodeToResult(CODE_NULL_DATE_TIME);
    }
    long long time = 0;
    nn::Result result;
    detail::Privileged* pPrivileged;
    result = detail::GetPrivilegedIpcInstance(pPrivileged);
    if (m_IsPrivileged) {
        if (result.IsFailure()) {
            return result;
        }
        result = pPrivileged->GetNsDataLastUpdatedPrivileged(m_ProgramId, m_SerialId, &time);
    } else if (result.IsSuccess()) {
        result = pPrivileged->GetNsDataLastUpdated(m_SerialId, &time);
    } else {
        detail::User* pUser;
        result = detail::GetUserIpcInstance(pUser);
        if (result.IsFailure()) {
            return result;
        }
        result = pUser->GetNsDataLastUpdated(m_SerialId, &time);
    }
    if (result.IsFailure()) {
        return result;
    }
    u32 low = static_cast<u32>(time);
    u32 high = static_cast<u32>(static_cast<u64>(time) >> 32);
    *pDateTime = nn::fnd::DateTime(high >> 10, (high >> 6) & 0xF, high & 0x3F, low >> 22, (low >> 16) & 0x3F,
                                   (low >> 10) & 0x3F, low & 0x3FF);
    return result;
}

// 0x0046CBA8 (name is ours)
nn::Result nn::boss::NsData::Delete()
{
    detail::Privileged* pPrivileged;
    nn::Result result = detail::GetPrivilegedIpcInstance(pPrivileged);
    if (m_IsPrivileged) {
        if (result.IsFailure()) {
            return result;
        }
        return pPrivileged->DeleteNsDataPrivileged(m_ProgramId, m_SerialId);
    }
    if (result.IsSuccess()) {
        return pPrivileged->DeleteNsData(m_SerialId);
    }
    detail::User* pUser;
    result = detail::GetUserIpcInstance(pUser);
    if (result.IsFailure()) {
        return result;
    }
    return pUser->DeleteNsData(m_SerialId);
}

// 0x0046CC34 | nintendogs:bytes [tier A]
s32 nn::boss::NsData::ReadData(unsigned char* pBuffer, unsigned size)
{
    if (pBuffer == 0 || size == 0) {
        return 0;
    }
    if (m_Size == 0) {
        if (GetHeaderInfo(HEADER_INFO_TYPE_SIZE, &m_Size, sizeof(m_Size)).IsFailure()) {
            return READ_ERROR_SIZE;
        }
    }
    if (static_cast<s64>(m_Size) < m_ReadOffset + static_cast<s64>(size)) {
        size = m_Size - static_cast<u32>(m_ReadOffset);
    }

    unsigned version = 0;
    detail::Privileged* pPrivileged;
    nn::Result result = detail::GetPrivilegedIpcInstance(pPrivileged);
    unsigned readSize = 0;
    if (m_IsPrivileged) {
        if (result.IsFailure()) {
            return READ_ERROR_SESSION;
        }
        result = pPrivileged->ReadNsDataPrivileged(m_ProgramId, m_SerialId, m_ReadOffset, pBuffer, size, &readSize, &version);
    } else if (result.IsSuccess()) {
        result = pPrivileged->ReadNsData(m_SerialId, m_ReadOffset, pBuffer, size, &readSize, &version);
    } else {
        detail::User* pUser;
        if (detail::GetUserIpcInstance(pUser).IsFailure()) {
            return READ_ERROR_SESSION;
        }
        result = pUser->ReadNsData(m_SerialId, m_ReadOffset, pBuffer, size, &readSize, &version);
    }
    if (result.IsFailure()) {
        return READ_ERROR_READ;
    }
    if (m_Version == 0) {
        m_Version = version;
    }
    if (m_Version != version) {
        return READ_ERROR_CHANGED;
    }
    m_ReadOffset += readSize;
    return readSize;
}

// 0x0046CED8 | nintendogs:bytes [tier A]
nn::boss::NsData::NsData()
    : m_IsPrivileged(false), m_SerialId(0), m_Size(0), m_Version(0), m_ProgramId(DEFAULT_PROGRAM_ID), m_ReadOffset(0)
{
}

// 0x0046CF20
// 0x0046CF1C (deleting dtor)
nn::boss::NsData::~NsData()
{
}

} // namespace boss
} // namespace nn
