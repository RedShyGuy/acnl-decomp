#include "nn/boss/boss_TaskActionBase.h"
#include "nn/boss/detail/boss_Property.h"
#include "nn/boss/detail/detail_Api.h"

namespace nn {
namespace boss {
namespace {
// the codes of ChangeBossRetCodeToResult (names are ours)
const nn::boss::ResultCode CODE_NULL_VALUE = static_cast<nn::boss::ResultCode>(9);
const nn::boss::ResultCode CODE_INVALID_PROPERTY = static_cast<nn::boss::ResultCode>(28);
const nn::boss::ResultCode CODE_TOO_MANY_ROOT_CAS = static_cast<nn::boss::ResultCode>(37);

const u32 ROOT_CA_COUNT_MAX = 3;
} // namespace

// 0x0046B214 (name is ours)
nn::Result nn::boss::TaskActionBase::GetCommonProperty(nn::boss::PropertyType type, void* pValue, unsigned size)
{
    if (pValue == 0) {
        return detail::ChangeBossRetCodeToResult(CODE_NULL_VALUE);
    }
    switch (type) {
    case PROPERTY_URL:
        detail::GetPropertyValue(pValue, m_Config.url, size);
        break;
    case PROPERTY_HEADER_FIELDS:
        detail::GetPropertyValue(pValue, m_Config.headerFields, size);
        break;
    case PROPERTY_CLIENT_CERTS:
        detail::GetPropertyValue(pValue, m_Config.clientCerts, size);
        break;
    case PROPERTY_ROOT_CA_COUNT:
        // (here the client certificate count: the counts are swapped against SendUserTaskAction)
        detail::GetPropertyValue(pValue, m_Config.clientCertCount, size);
        break;
    case PROPERTY_ROOT_CAS:
        detail::GetPropertyValue(pValue, m_Config.rootCas, size);
        break;
    case PROPERTY_CLIENT_CERT_COUNT:
        detail::GetPropertyValue(pValue, m_Config.rootCaCount, size);
        break;
    case PROPERTY_AP_INFO_TYPE:
        detail::GetPropertyValue(pValue, m_Config.apInfoType, size);
        break;
    case PROPERTY_CFG_INFO_TYPE:
        detail::GetPropertyValue(pValue, m_Config.cfgInfoType, size);
        break;
    case PROPERTY_FS_ROOT_CA:
        detail::GetPropertyValue(pValue, m_Config.fsRootCa, size);
        break;
    case PROPERTY_FS_CLIENT_CERT:
        detail::GetPropertyValue(pValue, m_Config.fsClientCert, size);
        break;
    case PROPERTY_15:
        detail::GetPropertyValue(pValue, m_Config.property15, size);
        break;
    default:
        return detail::ChangeBossRetCodeToResult(CODE_INVALID_PROPERTY);
    }
    return nn::Result();
}

// 0x0046B43C (name is ours)
nn::Result nn::boss::TaskActionBase::SetCommonProperty(nn::boss::PropertyType type, const void* pValue, unsigned size)
{
    if (pValue == 0) {
        return detail::ChangeBossRetCodeToResult(CODE_NULL_VALUE);
    }
    switch (type) {
    case PROPERTY_URL:
        detail::SetPropertyValue(m_Config.url, pValue, size);
        break;
    case PROPERTY_HEADER_FIELDS:
        detail::SetPropertyValue(m_Config.headerFields, pValue, size);
        break;
    case PROPERTY_CLIENT_CERTS:
        detail::SetPropertyValue(m_Config.clientCerts, pValue, size);
        break;
    case PROPERTY_ROOT_CA_COUNT:
        detail::SetPropertyValue(m_Config.clientCertCount, pValue, size);
        break;
    case PROPERTY_ROOT_CAS:
        detail::SetPropertyValue(m_Config.rootCas, pValue, size);
        break;
    case PROPERTY_CLIENT_CERT_COUNT:
        detail::SetPropertyValue(m_Config.rootCaCount, pValue, size);
        break;
    case PROPERTY_AP_INFO_TYPE:
        detail::SetPropertyValue(m_Config.apInfoType, pValue, size);
        break;
    case PROPERTY_CFG_INFO_TYPE:
        detail::SetPropertyValue(m_Config.cfgInfoType, pValue, size);
        break;
    case PROPERTY_FS_ROOT_CA:
        detail::SetPropertyValue(m_Config.fsRootCa, pValue, size);
        break;
    case PROPERTY_FS_CLIENT_CERT:
        detail::SetPropertyValue(m_Config.fsClientCert, pValue, size);
        break;
    case PROPERTY_15:
        detail::SetPropertyValue(m_Config.property15, pValue, size);
        break;
    default:
        return detail::ChangeBossRetCodeToResult(CODE_INVALID_PROPERTY);
    }
    return nn::Result();
}

// 0x0046B6E8 (name is ours)
nn::Result nn::boss::TaskActionBase::SetApInfoType(u8 type)
{
    m_Config.apInfoType = type;
    return nn::Result();
}

// 0x0046B6F4 | nintendogs:bytes [tier A]
nn::Result nn::boss::TaskActionBase::SetRootCa(unsigned certificateId)
{
    for (u32 i = 0; i < m_Config.rootCaCount; i++) {
        if (m_Config.rootCas[i] == certificateId) {
            return nn::Result();
        }
    }
    if (m_Config.rootCaCount >= ROOT_CA_COUNT_MAX) {
        return detail::ChangeBossRetCodeToResult(CODE_TOO_MANY_ROOT_CAS);
    }
    m_Config.rootCas[m_Config.rootCaCount] = certificateId;
    m_Config.rootCaCount++;
    return nn::Result();
}

// 0x0046B760
// 0x0046B75C (deleting dtor)
nn::boss::TaskActionBase::~TaskActionBase()
{
}

} // namespace boss
} // namespace nn
