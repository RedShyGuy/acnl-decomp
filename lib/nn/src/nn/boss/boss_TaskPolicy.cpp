#include "nn/boss/boss_TaskPolicy.h"
#include "nn/boss/detail/boss_Property.h"
#include "nn/boss/detail/detail_Api.h"

namespace nn {
namespace boss {
namespace {
// the codes of ChangeBossRetCodeToResult (names are ours)
const nn::boss::ResultCode CODE_NULL_VALUE = static_cast<nn::boss::ResultCode>(9);
const nn::boss::ResultCode CODE_INVALID_PROPERTY = static_cast<nn::boss::ResultCode>(28);

// the policy of InitializeWithSecInterval
const u8 DEFAULT_PRIORITY = 125;
const u8 DEFAULT_SCHEDULING_POLICY = 1;
const u8 DEFAULT_PERMISSION = 2;
} // namespace

// 0x0046A8B8 | nintendogs:bytes-fuzzy [tier A]
nn::Result nn::boss::TaskPolicy::SetProperty(nn::boss::PropertyType type, const void* pValue, unsigned size)
{
    if (pValue == 0) {
        return detail::ChangeBossRetCodeToResult(CODE_NULL_VALUE);
    }
    switch (type) {
    case PROPERTY_PRIORITY:
        detail::SetPropertyValue(m_Config.priority, pValue, size);
        break;
    case PROPERTY_SCHEDULING_POLICY:
        detail::SetPropertyValue(m_Config.schedulingPolicy, pValue, size);
        break;
    case PROPERTY_TARGET_DURATION:
        detail::SetPropertyValue(m_Config.targetDuration, pValue, size);
        break;
    case PROPERTY_INTERVAL:
        detail::SetPropertyValue(m_Config.interval, pValue, size);
        break;
    case PROPERTY_COUNT:
        detail::SetPropertyValue(m_Config.count, pValue, size);
        break;
    case PROPERTY_PERMISSION:
        detail::SetPropertyValue(m_Config.permission, pValue, size);
        break;
    default:
        return detail::ChangeBossRetCodeToResult(CODE_INVALID_PROPERTY);
    }
    return nn::Result();
}

// 0x0046AA14 | nintendogs:bytes [tier A]
nn::Result nn::boss::TaskPolicy::InitializeWithSecInterval(unsigned interval, unsigned count)
{
    m_Config.priority = DEFAULT_PRIORITY;
    m_Config.schedulingPolicy = DEFAULT_SCHEDULING_POLICY;
    m_Config.targetDuration = 0;
    m_Config.permission = DEFAULT_PERMISSION;
    m_Config.interval = interval;
    m_Config.count = count;
    return nn::Result();
}

// 0x0046AA4C | tier C
nn::boss::TaskPolicy::TaskPolicy()
{
    memset(&m_Config, 0, sizeof(m_Config));
}

// 0x0046AA74
// 0x0046AA70 (deleting dtor)
nn::boss::TaskPolicy::~TaskPolicy()
{
}

} // namespace boss
} // namespace nn
