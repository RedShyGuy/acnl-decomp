#include "nn/pia/common/common_SignatureManager.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_PayloadSizeManager.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SignatureSetting.h"
#include <string.h>

namespace nn {
namespace pia {
namespace common {
namespace {
// binarySearch: a Specified with the station address of the key
// 0x00426E34 (name is ours)
int CompareSpecifiedAndAddress(const void* a, const void* b)
{
    return StationAddress::Compare(static_cast<const SignatureManager::Specified*>(a)->m_StationAddress,
                                   *static_cast<const StationAddress*>(b));
}

// sort: by the station address
// 0x00427E60 (name is ours)
int CompareSpecified(const void* a, const void* b)
{
    return StationAddress::Compare(static_cast<const SignatureManager::Specified*>(a)->m_StationAddress,
                                   static_cast<const SignatureManager::Specified*>(b)->m_StationAddress);
}
} // namespace

// 0x0097E3D4
SignatureManager* SignatureManager::s_pInstance;

// 0x004275B0 (name is ours)
bool nn::pia::common::SignatureManager::SignatureContext::Check(const void* pData, unsigned int size)
{
    unsigned int signatureSize = GetSignatureSize();
    if (signatureSize == 0) {
        return true;
    }
    if (size <= signatureSize) {
        return false;
    }
    unsigned int payloadSize = size - signatureSize;
    const u8* pSignature = static_cast<const u8*>(pData) + payloadSize;
    u8 signature[Hmac::MAX_HASH_SIZE];
    m_ReceiveHmac.Calc(signature, pData, payloadSize);
    if (memcmp(signature, pSignature, signatureSize) != 0) {
        return false;
    }
    return true;
}

// 0x00427638 (name is ours)
unsigned int nn::pia::common::SignatureManager::SignatureContext::Append(void* pData, unsigned int, unsigned int dataSize)
{
    unsigned int signatureSize = GetSignatureSize();
    m_SendHmac.Calc(static_cast<u8*>(pData) + dataSize, pData, dataSize);
    return signatureSize;
}

// 0x00427680 | fefates:bytes [tier B]
void nn::pia::common::SignatureManager::SetNecessity(bool isNecessary)
{
    if (m_State == STATE_NONE) {
        m_IsNecessary = isNecessary;
        m_State = STATE_NECESSITY_SET;
    } else if (m_State == STATE_NECESSITY_SET) {
        m_IsNecessary = isNecessary;
    }
}

// 0x004276A8 | fefates:bytes [tier B]
bool nn::pia::common::SignatureManager::CheckSignature(const nn::pia::common::StationAddress& address, const void* pData, unsigned int size, unsigned int* pPayloadSize)
{
    if (m_State != STATE_SETUP) {
        return false;
    }
    SignatureContext* pContext;
    int index = m_SpecifiedArray.binarySearch(&address, CompareSpecifiedAndAddress);
    if (index >= 0) {
        pContext = &m_SpecifiedArray.at(index)->m_Context;
    } else {
        pContext = &m_DefaultContext;
    }
    unsigned int signatureSize = pContext->GetSignatureSize();
    if (signatureSize == 0) {
        *pPayloadSize = size;
        return true;
    }
    if (size <= signatureSize) {
        return false;
    }
    unsigned int payloadSize = size - signatureSize;
    const u8* pSignature = static_cast<const u8*>(pData) + payloadSize;
    u8 signature[Hmac::MAX_HASH_SIZE];
    pContext->m_ReceiveHmac.Calc(signature, pData, payloadSize);
    if (memcmp(signature, pSignature, signatureSize) != 0) {
        return false;
    }
    *pPayloadSize = payloadSize;
    return true;
}

// 0x004277FC | fefates:bytes [tier B]
nn::Result nn::pia::common::SignatureManager::CreateInstance()
{
    if (!IsInitialized()) {
        return RESULT_NOT_INITIALIZED;
    }
    if (!IsInSetupMode()) {
        return RESULT_INVALID_STATE;
    }
    if (s_pInstance != nullptr) {
        return RESULT_ALREADY_EXISTS;
    }
    s_pInstance = new SignatureManager();
    return nn::Result();
}

// 0x004278B8 | fefates:bytes [tier B]
void nn::pia::common::SignatureManager::ResetNecessity()
{
    switch (m_State) {
    case STATE_NONE:
        return;
    case STATE_NECESSITY_SET:
        break;
    case STATE_SETUP:
        m_DefaultContext.Reset();
        m_SpecifiedArray.clear();
        PayloadSizeManager::GetInstance()->SetSignatureSize(0);
        break;
    default:
        return;
    }
    m_State = STATE_NONE;
    m_IsNecessary = false;
}

// 0x00427930 | fefates:bytes [tier B]
unsigned int nn::pia::common::SignatureManager::AppendSignature(const nn::pia::common::StationAddress& address, void* pData, unsigned int, unsigned int dataSize)
{
    SignatureContext* pContext;
    int index = m_SpecifiedArray.binarySearch(&address, CompareSpecifiedAndAddress);
    if (index >= 0) {
        pContext = &m_SpecifiedArray.at(index)->m_Context;
    } else {
        pContext = &m_DefaultContext;
    }
    unsigned int signatureSize = pContext->GetSignatureSize();
    pContext->m_SendHmac.Calc(static_cast<u8*>(pData) + dataSize, pData, dataSize);
    return signatureSize;
}

// 0x004279F0 | fefates:bytes [tier B]
void nn::pia::common::SignatureManager::DestroyInstance()
{
    if (s_pInstance != nullptr) {
        delete s_pInstance;
        s_pInstance = nullptr;
    }
}

// 0x00427A38 | fefates:bytes [tier B]
unsigned int nn::pia::common::SignatureManager::UpdateSignature(const nn::pia::common::StationAddress& address, void* pData, unsigned int size)
{
    SignatureContext* pContext;
    int index = m_SpecifiedArray.binarySearch(&address, CompareSpecifiedAndAddress);
    if (index >= 0) {
        pContext = &m_SpecifiedArray.at(index)->m_Context;
    } else {
        pContext = &m_DefaultContext;
    }
    unsigned int signatureSize = pContext->GetSignatureSize();
    unsigned int payloadSize = size - signatureSize;
    pContext->m_SendHmac.Calc(static_cast<u8*>(pData) + payloadSize, pData, payloadSize);
    return signatureSize;
}

// 0x00427AC0 | fefates:bytes [tier B]
void nn::pia::common::SignatureManager::Cleanup()
{
    if (m_State == STATE_NONE || m_State == STATE_NECESSITY_SET) {
        return;
    }
    if (m_State != STATE_SETUP) {
        return;
    }
    m_DefaultContext.Reset();
    m_SpecifiedArray.clear();
    PayloadSizeManager::GetInstance()->SetSignatureSize(0);
    m_State = STATE_NECESSITY_SET;
}

// 0x00427B34 (name after C++)
nn::pia::common::SignatureManager::StationSetting::StationSetting() : m_pSetting(nullptr)
{
}

// 0x00427B48 (name after C++)
nn::pia::common::SignatureManager::StationSetting::~StationSetting()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x00427B64 (name is ours)
nn::Result nn::pia::common::SignatureManager::Setup(const Setting& setting)
{
    if (m_State != STATE_NECESSITY_SET) {
        return RESULT_INVALID_STATE;
    }
    // all settings must be there, and must sign if signing is necessary
    if (!IsValidPointer(setting.m_pDefaultSetting)) {
        return RESULT_INVALID_ARGUMENT;
    }
    if (m_IsNecessary && setting.m_pDefaultSetting->m_Mode == SignatureSetting::MODE_NONE) {
        return RESULT_INVALID_ARGUMENT;
    }
    for (u32 i = 0; i < SPECIFIED_NUM; i++) {
        const StationSetting& stationSetting = setting.m_StationSettings[i];
        if (stationSetting.m_StationAddress.IsValid()) {
            if (!IsValidPointer(stationSetting.m_pSetting)) {
                return RESULT_INVALID_ARGUMENT;
            }
            if (m_IsNecessary && stationSetting.m_pSetting->m_Mode == SignatureSetting::MODE_NONE) {
                return RESULT_INVALID_ARGUMENT;
            }
        }
    }

    const SignatureSetting* pDefaultSetting = setting.m_pDefaultSetting;
    switch (pDefaultSetting->m_Mode) {
    case SignatureSetting::MODE_NONE:
        m_DefaultContext.Reset();
        break;
    case SignatureSetting::MODE_HMAC_MD5:
        m_DefaultContext.SetKey(pDefaultSetting->m_pKey, pDefaultSetting->m_KeySize);
        break;
    default:
        break;
    }
    unsigned int maxSignatureSize = m_DefaultContext.GetSignatureSize();

    int specifiedNum = 0;
    for (u32 i = 0; i < SPECIFIED_NUM; i++) {
        const StationSetting& stationSetting = setting.m_StationSettings[i];
        if (!stationSetting.m_StationAddress.IsValid()) {
            continue;
        }
        Specified* pSpecified = &m_Specified[specifiedNum++];
        pSpecified->m_StationAddress = stationSetting.m_StationAddress;
        const SignatureSetting* pSetting = stationSetting.m_pSetting;
        switch (pSetting->m_Mode) {
        case SignatureSetting::MODE_NONE:
            pSpecified->m_Context.Reset();
            break;
        case SignatureSetting::MODE_HMAC_MD5:
            pSpecified->m_Context.SetKey(pSetting->m_pKey, pSetting->m_KeySize);
            break;
        default:
            pSpecified->m_Context.Reset();
            break;
        }
        m_SpecifiedArray.pushBack(pSpecified);
        unsigned int signatureSize = pSpecified->m_Context.GetSignatureSize();
        if (maxSignatureSize < signatureSize) {
            maxSignatureSize = signatureSize;
        }
    }
    m_SpecifiedArray.sort(CompareSpecified);
    PayloadSizeManager::GetInstance()->SetSignatureSize(maxSignatureSize);
    m_State = STATE_SETUP;
    return nn::Result();
}

// 0x00427E6C | fefates:bytes [tier B]
nn::pia::common::SignatureManager::Specified::Specified()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x00427E90 | fefates:bytes [tier B]
nn::pia::common::SignatureManager::Specified::~Specified()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace common
} // namespace pia
} // namespace nn
