#include "nn/boss/boss_TaskAction.h"
#include "nn/boss/detail/boss_Property.h"
#include "nn/boss/detail/detail_Api.h"

namespace nn {
namespace boss {
namespace {
// the codes of ChangeBossRetCodeToResult (names are ours)
const nn::boss::ResultCode CODE_NULL_VALUE = static_cast<nn::boss::ResultCode>(9);
const nn::boss::ResultCode CODE_INVALID_PROPERTY = static_cast<nn::boss::ResultCode>(28);

// the action data types (what actionData holds; names are ours)
const u8 ACTION_DATA_TYPE_0A = 0;
const u8 ACTION_DATA_TYPE_0B = 1;
const u8 ACTION_DATA_TYPE_FILE_HANDLE = 2;
} // namespace

// 0x0046A7A0 (name is ours)
nn::Result nn::boss::TaskAction::GetProperty(nn::boss::PropertyType type, void* pValue, unsigned size)
{
    if (pValue == 0) {
        return detail::ChangeBossRetCodeToResult(CODE_NULL_VALUE);
    }
    nn::Result result;
    switch (type) {
    case PROPERTY_ACTION_CODE:
        detail::GetPropertyValue(pValue, m_Config.actionCode, size);
        break;
    case PROPERTY_16:
        detail::GetPropertyValue(pValue, m_Config.property16, size);
        break;
    case PROPERTY_RESERVED:
        result = detail::ChangeBossRetCodeToResult(CODE_INVALID_PROPERTY);
        break;
    default:
        result = GetCommonProperty(type, pValue, size);
        if (result.IsFailure()) {
            result = GetActionDataProperty(type, pValue, size);
        }
        break;
    }
    return result;
}

// 0x0046A86C | nintendogs:bytes [tier A]
nn::boss::TaskAction::TaskAction()
{
}

// 0x0046A8B4
// 0x0046A8AC (deleting dtor)
nn::boss::TaskAction::~TaskAction()
{
}

// 0x0046B130 (name is ours)
nn::Result nn::boss::TaskAction::GetActionDataProperty(nn::boss::PropertyType type, void* pValue, unsigned size)
{
    if (pValue == 0) {
        return detail::ChangeBossRetCodeToResult(CODE_NULL_VALUE);
    }
    switch (type) {
    case PROPERTY_08:
        detail::GetPropertyValue(pValue, m_Config.property08, size);
        break;
    case PROPERTY_ACTION_DATA_TYPE:
        detail::GetPropertyValue(pValue, m_Config.actionDataType, size);
        break;
    case PROPERTY_ACTION_DATA_0A:
        if (m_Config.actionDataType != ACTION_DATA_TYPE_0A) {
            return detail::ChangeBossRetCodeToResult(CODE_INVALID_PROPERTY);
        }
        memcpy(pValue, m_Config.actionData, size > 0x100 ? 0x100 : size);
        break;
    case PROPERTY_ACTION_DATA_0B:
        if (m_Config.actionDataType != ACTION_DATA_TYPE_0B) {
            return detail::ChangeBossRetCodeToResult(CODE_INVALID_PROPERTY);
        }
        memcpy(pValue, m_Config.actionData, size > 0x200 ? 0x200 : size);
        break;
    case PROPERTY_FILE_HANDLE:
        if (m_Config.actionDataType != ACTION_DATA_TYPE_FILE_HANDLE) {
            return detail::ChangeBossRetCodeToResult(CODE_INVALID_PROPERTY);
        }
        *static_cast<nn::Handle*>(pValue) = m_Config.fileHandle;
        break;
    default:
        return detail::ChangeBossRetCodeToResult(CODE_INVALID_PROPERTY);
    }
    return nn::Result();
}

} // namespace boss
} // namespace nn
