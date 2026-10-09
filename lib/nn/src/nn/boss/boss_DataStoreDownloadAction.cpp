#include "nn/boss/boss_DataStoreDownloadAction.h"
#include "nn/boss/detail/boss_Property.h"
#include "nn/boss/detail/detail_Api.h"

namespace nn {
namespace boss {
namespace {
// results (module 62; the values of ChangeBossRetCodeToResult, written out here)
const bit32 RESULT_NULL_VALUE = 0xD8E0F809;        // 9
const bit32 RESULT_INVALID_PROPERTY = 0xD8E0F81C;  // 28
const bit32 RESULT_INVALID_KEY = 0xD8E0FBEC;       // 1004
const bit32 RESULT_NULL_KEY = 0xD8E0FBF6;          // 1014
const bit32 RESULT_NOT_DATA_STORE = 0xC960F84D;    // 77

const u8 ACTION_CODE_DATA_STORE_DOWNLOAD = 10;
const u8 ACTION_DATA_TYPE_DATA_STORE = 5;
const size_t KEY_LENGTH_MAX = 9;

inline nn::boss::DataStoreDownloadData* GetData(nn::boss::TaskActionConfig& config)
{
    return config.actionCode == ACTION_CODE_DATA_STORE_DOWNLOAD
               ? reinterpret_cast<nn::boss::DataStoreDownloadData*>(config.actionData)
               : 0;
}
} // namespace

// 0x0046BEE4 | fefates:bytes [tier B]
nn::Result nn::boss::DataStoreDownloadAction::Initialize(unsigned int gameId, const wchar_t* pKey)
{
    if (pKey == 0) {
        return nn::Result(RESULT_NULL_KEY);
    }
    size_t length = detail::wcsnlen(pKey, KEY_LENGTH_MAX);
    if (length == 0 || length >= KEY_LENGTH_MAX) {
        return nn::Result(RESULT_INVALID_KEY);
    }
    memset(&m_Config, 0, sizeof(m_Config));
    m_Config.actionCode = ACTION_CODE_DATA_STORE_DOWNLOAD;
    m_Config.actionDataType = ACTION_DATA_TYPE_DATA_STORE;
    nn::boss::DataStoreDownloadData* pData = reinterpret_cast<nn::boss::DataStoreDownloadData*>(m_Config.actionData);
    pData->gameId = gameId;
    memcpy(pData->key, pKey, length * sizeof(wchar_t));
    return nn::Result();
}

// 0x0046BF68 (name is ours)
nn::Result nn::boss::DataStoreDownloadAction::GetProperty(nn::boss::PropertyType type, void* pValue, unsigned size)
{
    if (pValue == 0) {
        return nn::Result(RESULT_NULL_VALUE);
    }
    nn::boss::DataStoreDownloadData* pData = GetData(m_Config);
    switch (type) {
    case PROPERTY_ACTION_CODE:
        detail::GetPropertyValue(pValue, m_Config.actionCode, size);
        return nn::Result();
    case PROPERTY_ACTION_DATA_TYPE:
        detail::GetPropertyValue(pValue, m_Config.actionDataType, size);
        return nn::Result();
    }
    if (pData == 0) {
        return nn::Result(RESULT_INVALID_PROPERTY);
    }
    switch (type) {
    case PROPERTY_DATA_STORE_40:
        detail::GetPropertyValue(pValue, pData->gameId, size);
        break;
    case PROPERTY_DATA_STORE_41:
        detail::GetPropertyValue(pValue, pData->key, size);
        break;
    case PROPERTY_DATA_STORE_42:
        detail::GetPropertyValue(pValue, pData->data42, size);
        break;
    case PROPERTY_DATA_STORE_43:
        detail::GetPropertyValue(pValue, pData->data43, size);
        break;
    case PROPERTY_DATA_STORE_44:
        detail::GetPropertyValue(pValue, pData->data44, size);
        break;
    case PROPERTY_DATA_STORE_45:
        detail::GetPropertyValue(pValue, pData->data45, size);
        break;
    case PROPERTY_DATA_STORE_46:
        detail::GetPropertyValue(pValue, pData->data46, size);
        break;
    default:
        return nn::Result(RESULT_INVALID_PROPERTY);
    }
    return nn::Result();
}

// clears the results of the download (everything but the game id and the key)
// 0x0046C124 (name is ours)
nn::Result nn::boss::DataStoreDownloadAction::ClearData()
{
    if (m_Config.actionCode != ACTION_CODE_DATA_STORE_DOWNLOAD) {
        return nn::Result(RESULT_NOT_DATA_STORE);
    }
    nn::boss::DataStoreDownloadData* pData = reinterpret_cast<nn::boss::DataStoreDownloadData*>(m_Config.actionData);
    if (pData == 0) {
        return nn::Result(RESULT_NOT_DATA_STORE);
    }
    pData->data45 = 0;
    pData->data46 = 0;
    memset(pData->data42, 0, sizeof(pData->data42));
    memset(pData->data43, 0, sizeof(pData->data43));
    memset(pData->data44, 0, sizeof(pData->data44));
    return nn::Result();
}

// 0x0046C180 | tier C
nn::boss::DataStoreDownloadAction::DataStoreDownloadAction()
{
}

// 0x0046C1A8
// 0x0046C198 (deleting dtor)
nn::boss::DataStoreDownloadAction::~DataStoreDownloadAction()
{
}

} // namespace boss
} // namespace nn
