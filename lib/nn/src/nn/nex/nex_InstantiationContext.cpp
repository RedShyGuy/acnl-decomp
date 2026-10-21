#include "nn/nex/nex_InstantiationContext.h"

namespace nn {
namespace nex {
namespace {
const wchar_t* const FILE_NAME = L"D:/home/jenkins/workspace/Manual-Pack/5129d543/OnlineCore/src/Core/InstantiationContext.cpp";
} // namespace

// 0x003967D8 (name is ours)
bool nn::nex::InstantiationContext::Register(InstanceControl* pInstance, u32 kind)
{
    if (kind >= INSTANCE_COUNT) {
        SystemError::SignalError(const_cast<wchar_t*>(FILE_NAME), 62, SystemError::ERROR_OUT_OF_RANGE, 0);
        return false;
    }
    if (m_Instances[kind] == nullptr) {
        m_Instances[kind] = pInstance;
        return true;
    }
    SystemError::SignalError(const_cast<wchar_t*>(FILE_NAME), 68, SystemError::ERROR_ALREADY_EXISTS, 0);
    return false;
}

// 0x00396898
// 0x00396838 (deleting dtor)
nn::nex::InstantiationContext::~InstantiationContext()
{
    for (u32 i = 0; i < INSTANCE_COUNT; i++) {
        m_Instances[i] = nullptr;
    }
}

} // namespace nex
} // namespace nn
