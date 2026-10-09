#include "nn/boss/boss_NsaDownloadAction.h"
#include "nn/boss/detail/boss_Property.h"
#include "nn/boss/detail/detail_Api.h"

extern "C" size_t strlcpy(char* pDestination, const char* pSource, size_t size); // 0x00151080

namespace nn {
namespace boss {
namespace {
// the codes of ChangeBossRetCodeToResult (names are ours)
const nn::boss::ResultCode CODE_NULL_VALUE = static_cast<nn::boss::ResultCode>(9);
const nn::boss::ResultCode CODE_INVALID_URL = static_cast<nn::boss::ResultCode>(29);

const u8 ACTION_CODE_NSA_DOWNLOAD = 2;
// the root certificates of the NSA servers (the ids of the system certificates)
const u32 ROOT_CA_ID_1 = 7;
const u32 ROOT_CA_ID_2 = 3;
const u32 ROOT_CA_ID_3 = 6;
} // namespace

// the base properties of the action (it only adds SetProperty)
// 0x0046B210 (name is ours)
nn::Result nn::boss::NsaDownloadAction::GetProperty(nn::boss::PropertyType type, void* pValue, unsigned size)
{
    return GetCommonProperty(type, pValue, size);
}

// 0x0046B9A0 | nintendogs:bytes [tier A]
nn::Result nn::boss::NsaDownloadAction::Initialize(const char* pUrl)
{
    if (pUrl == 0 || *pUrl == 0 || detail::strnlen(pUrl, sizeof(m_Config.url)) >= sizeof(m_Config.url)) {
        return detail::ChangeBossRetCodeToResult(CODE_INVALID_URL);
    }
    memset(&m_Config, 0, sizeof(m_Config));
    m_Config.actionCode = ACTION_CODE_NSA_DOWNLOAD;
    strlcpy(m_Config.url, pUrl, sizeof(m_Config.url));
    nn::Result result = SetRootCa(ROOT_CA_ID_1);
    if (result.IsFailure()) {
        return result;
    }
    result = SetRootCa(ROOT_CA_ID_2);
    if (result.IsFailure()) {
        return result;
    }
    result = SetRootCa(ROOT_CA_ID_3);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result();
}

// the name of the slot is ours
// 0x0046BA4C (name is ours)
nn::Result nn::boss::NsaDownloadAction::SetProperty(nn::boss::PropertyType type, const void* pValue, unsigned size)
{
    if (pValue == 0) {
        return detail::ChangeBossRetCodeToResult(CODE_NULL_VALUE);
    }
    if (type == PROPERTY_16) {
        detail::SetPropertyValue(m_Config.property16, pValue, size);
        return nn::Result();
    }
    return SetCommonProperty(type, pValue, size);
}

// 0x0046BAB4 | tier C
nn::boss::NsaDownloadAction::NsaDownloadAction()
{
}

// 0x0046A8B0
// 0x0046BACC (deleting dtor)
nn::boss::NsaDownloadAction::~NsaDownloadAction()
{
}

} // namespace boss
} // namespace nn
