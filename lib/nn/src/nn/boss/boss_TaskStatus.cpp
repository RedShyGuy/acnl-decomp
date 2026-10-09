#include "nn/boss/boss_TaskStatus.h"
#include "nn/boss/detail/boss_Property.h"
#include "nn/boss/detail/detail_Api.h"

namespace nn {
namespace boss {
namespace {
// the codes of ChangeBossRetCodeToResult (names are ours)
const nn::boss::ResultCode CODE_NULL_VALUE = static_cast<nn::boss::ResultCode>(9);
const nn::boss::ResultCode CODE_INVALID_PROPERTY = static_cast<nn::boss::ResultCode>(28);
} // namespace

// 0x0046AA78 | fefates:bytes-fuzzy [tier B]
nn::Result nn::boss::TaskStatus::GetProperty(nn::boss::PropertyType type, void* pValue, unsigned int size)
{
    if (pValue == 0) {
        return detail::ChangeBossRetCodeToResult(CODE_NULL_VALUE);
    }
    switch (type) {
    case PROPERTY_TASK_STATE_CODE:
        detail::GetPropertyValue(pValue, m_Info.stateCode, size);
        break;
    case PROPERTY_TASK_RESULT_CODE:
        detail::GetPropertyValue(pValue, m_Info.resultCode, size);
        break;
    case PROPERTY_TASK_SERVICE_STATUS:
        detail::GetPropertyValue(pValue, m_Info.serviceStatus, size);
        break;
    case PROPERTY_22:
        detail::GetPropertyValue(pValue, m_Info.property22, size);
        break;
    case PROPERTY_COMM_ERROR_CODE:
        detail::GetPropertyValue(pValue, m_Info.commErrorCode, size);
        break;
    case PROPERTY_24:
        detail::GetPropertyValue(pValue, m_Info.property24, size);
        break;
    case PROPERTY_25:
        detail::GetPropertyValue(pValue, m_Info.property25, size);
        break;
    case PROPERTY_26:
        detail::GetPropertyValue(pValue, m_Info.property26, size);
        break;
    case PROPERTY_27:
        detail::GetPropertyValue(pValue, m_Info.property27, size);
        break;
    case PROPERTY_LAST_SUCCESSFUL_TIMESTAMP:
        detail::GetPropertyValue(pValue, m_Info.lastSuccessfulTimestamp, size);
        break;
    case PROPERTY_29:
        detail::GetPropertyValue(pValue, m_Info.property29, size);
        break;
    case PROPERTY_2A:
        detail::GetPropertyValue(pValue, m_Info.property2A, size);
        break;
    case PROPERTY_2B:
        detail::GetPropertyValue(pValue, m_Info.property2B, size);
        break;
    case PROPERTY_2C:
        detail::GetPropertyValue(pValue, m_Info.property2C, size);
        break;
    case PROPERTY_LAST_MODIFIED_HEADER:
        detail::GetPropertyValue(pValue, m_Info.lastModified, size);
        break;
    default:
        return detail::ChangeBossRetCodeToResult(CODE_INVALID_PROPERTY);
    }
    return nn::Result();
}

// 0x0046AD08 | fefates:bytes [tier B]
nn::boss::TaskStatus::TaskStatus()
{
    memset(&m_Info, 0, sizeof(m_Info));
}

// 0x0046AD34
// 0x0046AD30 (deleting dtor)
nn::boss::TaskStatus::~TaskStatus()
{
}

} // namespace boss
} // namespace nn
