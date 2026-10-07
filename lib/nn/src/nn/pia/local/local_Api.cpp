#include "nn/pia/local/local_Api.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_Result.h"
#include "pead/peadExpHeap.h"
#include "pead/peadSafeStringBase.h"

namespace nn {
namespace pia {
namespace local {
namespace {
// 0x00975A6C
bool s_IsInitialized;
// 0x00975A6D
bool s_IsDuringSetup;
} // namespace

// 0x00414598 | fefates:bytes [tier B]
nn::Result BeginSetup()
{
    if (!s_IsInitialized) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (s_IsDuringSetup) {
        return common::RESULT_INVALID_STATE;
    }
    s_IsDuringSetup = true;
    common::HeapManager::SetCurrentHeap(MODULE_TYPE_LOCAL);
    return nn::Result();
}

// 0x004145E4 | fefates:bytes [tier B]
nn::Result Initialize()
{
    // (a debug log call was removed by the linker here)
    if (!common::IsInitialized()) {
        return common::RESULT_INVALID_STATE;
    }
    if (s_IsInitialized) {
        return common::RESULT_ALREADY_INITIALIZED;
    }
    common::HeapManager::Setup(MODULE_TYPE_LOCAL, 0, pead::SafeStringBase<char>("pia local heap"));
    s_IsInitialized = true;
    return nn::Result();
}

// 0x00416624
bool IsDuringSetup()
{
    return s_IsDuringSetup;
}

// 0x00416634
bool IsInitialized()
{
    return s_IsInitialized;
}

// 0x00425DD8 (name is ours, as in common/inet)
nn::Result EndSetup()
{
    if (!s_IsInitialized) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!s_IsDuringSetup) {
        return common::RESULT_INVALID_STATE;
    }
    common::HeapManager::GetHeap()->adjust();
    common::HeapManager::ClearCurrentHeap();
    s_IsDuringSetup = false;
    return nn::Result();
}

// 0x00425E30 | fefates:bytes [tier B]
void Finalize()
{
    if (!s_IsInitialized) {
        return;
    }
    if (s_IsDuringSetup) {
        common::HeapManager::GetHeap()->adjust();
        common::HeapManager::ClearCurrentHeap();
        s_IsDuringSetup = false;
    }
    common::HeapManager::Cleanup(MODULE_TYPE_LOCAL);
    s_IsInitialized = false;
}

} // namespace local
} // namespace pia
} // namespace nn
