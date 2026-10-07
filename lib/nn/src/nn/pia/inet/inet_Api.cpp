#include "nn/pia/inet/inet_Api.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_PayloadSizeManager.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "pead/peadExpHeap.h"
#include "pead/peadSafeStringBase.h"

namespace nn {
namespace pia {
namespace inet {
namespace {
// 0x00975A60
bool s_IsInitialized;
// 0x00975A61
bool s_IsInSetupMode;
} // namespace

// 0x003E2598 | fefates:bytes [tier B]
nn::Result BeginSetup()
{
    if (!s_IsInitialized) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (s_IsInSetupMode) {
        return common::RESULT_INVALID_STATE;
    }
    common::HeapManager::SetCurrentHeap(MODULE_TYPE_INET);
    s_IsInSetupMode = true;
    return nn::Result();
}

// 0x003E25E4 | fefates:bytes [tier B]
nn::Result Initialize(const nn::pia::inet::Setting& setting)
{
    // (a debug log call was removed by the linker here)
    if (s_IsInitialized) {
        return common::RESULT_ALREADY_INITIALIZED;
    }
    if (setting.m_MtuSize > MTU_SIZE_MAX ||
        common::PayloadSizeManager::s_pInstance->SetMtuSize(setting.m_MtuSize - IP_UDP_HEADER_SIZE).IsFailure()) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    common::g_SessionBeginMonitoringContent.m_Unknown0x4C = setting.m_MtuSize;
    common::HeapManager::Setup(MODULE_TYPE_INET, 0, pead::SafeStringBase<char>("pia inet heap"));
    s_IsInitialized = true;
    return nn::Result();
}

// 0x003E6B84 (name is ours)
bool IsInSetupMode()
{
    return s_IsInSetupMode;
}

// 0x003E6B94 (name is ours)
bool IsInitialized()
{
    return s_IsInitialized;
}

// 0x00412C08 | fefates:bytes [tier B]
nn::Result EndSetup()
{
    if (!s_IsInitialized) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!s_IsInSetupMode) {
        return common::RESULT_INVALID_STATE;
    }
    common::HeapManager::GetHeap(MODULE_TYPE_INET)->adjust();
    s_IsInSetupMode = false;
    common::HeapManager::ClearCurrentHeap();
    return nn::Result();
}

// 0x00412C64 | fefates:bytes [tier B]
void Finalize()
{
    if (!s_IsInitialized) {
        return;
    }
    common::HeapManager::Cleanup(MODULE_TYPE_INET);
    s_IsInitialized = false;
}

} // namespace inet
} // namespace pia
} // namespace nn
