#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/transport/transport_ThreadStreamManager.h"

namespace nn {
namespace pia {
namespace transport {
// ctor candidate(s) 0x00457F58, 0x004580A4 (unverified)
nn::pia::transport::ThreadStreamManager::ThreadStreamManager()
{
}

// 0x007360A4 slot 0x00 | virtual slot, introduced by nn::pia::transport::ThreadStreamManager
void nn::pia::transport::ThreadStreamManager::vf_0x00()
{
}

// 0x00457F58 | fefates:bytes [tier B]
void nn::pia::transport::ThreadStreamManager::CreateInstance(nn::pia::transport::NetworkFactory*, unsigned int, unsigned int, unsigned int, unsigned int, bool)
{
}

// 0x004580A4 | fefates:bytes [tier B]
void nn::pia::transport::ThreadStreamManager::DestroyInstance()
{
}

// 0x00458164 | fefates:bytes [tier B]
void nn::pia::transport::ThreadStreamManager::SetMonitoringData()
{
}

// 0x004581E0 | fefates:bytes [tier B]
void nn::pia::transport::ThreadStreamManager::Cleanup()
{
}

// 0x0045821C | fefates:bytes [tier B]
void nn::pia::transport::ThreadStreamManager::Startup()
{
}

} // namespace transport
} // namespace pia
} // namespace nn
