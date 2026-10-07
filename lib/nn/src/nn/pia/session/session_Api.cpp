#include "nn/pia/session/session_Api.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/session/session_Mesh.h"
#include "pead/peadExpHeap.h"
#include "pead/peadSafeStringBase.h"

namespace nn {
namespace pia {
namespace session {
namespace {
// 0x00975A7C
bool s_IsInitialized;
// 0x00975A7D
bool s_IsInSetupMode;
} // namespace

// 0x0042A0B4 | fefates:bytes [tier B]
nn::Result BeginSetup()
{
    if (!s_IsInitialized) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (s_IsInSetupMode) {
        return common::RESULT_INVALID_STATE;
    }
    common::HeapManager::SetCurrentHeap(MODULE_TYPE_SESSION);
    s_IsInSetupMode = true;
    return nn::Result();
}

// 0x0042A100 | fefates:bytes [tier B]
nn::Result Initialize()
{
    if (!common::IsInitialized()) {
        return common::RESULT_INVALID_STATE;
    }
    if (s_IsInitialized) {
        return common::RESULT_ALREADY_INITIALIZED;
    }
    common::HeapManager::Setup(MODULE_TYPE_SESSION, 0, pead::SafeStringBase<char>("pia session heap"));
    s_IsInitialized = true;
    return nn::Result();
}

// 0x004310D0 (name is ours)
bool IsInSetupMode()
{
    return s_IsInSetupMode;
}

// 0x004310E0 (name is ours)
bool IsInitialized()
{
    return s_IsInitialized;
}

// 0x0044D1F8 (name is ours)
nn::Result EndSetup()
{
    if (!s_IsInitialized) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!s_IsInSetupMode) {
        return common::RESULT_INVALID_STATE;
    }
    common::HeapManager::GetHeap()->adjust();
    common::HeapManager::ClearCurrentHeap();
    s_IsInSetupMode = false;
    return nn::Result();
}

// 0x0044D250 | fefates:bytes [tier B]
void Finalize()
{
    if (!s_IsInitialized) {
        return;
    }
    if (s_IsInSetupMode) {
        common::HeapManager::GetHeap()->adjust();
        common::HeapManager::ClearCurrentHeap();
        s_IsInSetupMode = false;
    }
    if (common::IsValidPointer(Mesh::s_pInstance)) {
        Mesh::s_pInstance->Cleanup();
        Mesh::DestroyInstance();
    }
    common::HeapManager::Cleanup(MODULE_TYPE_SESSION);
    s_IsInitialized = false;
}

} // namespace session
} // namespace pia
} // namespace nn
