#include "nn/pia/local/local_UdsNetworkFactory.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/local/local_UdsMatchmakeSession.h"
#include "nn/pia/local/local_UdsSessionInfo.h"
#include "nn/pia/session/session_SessionInfoList.h"

namespace nn {
namespace pia {
namespace local {
// 0x00416CCC
nn::pia::session::ISessionInfoList* nn::pia::local::UdsNetworkFactory::CreateSessionInfoList(u32 capacity)
{
    return new session::SessionInfoList<UdsSessionInfo>(capacity);
}

// 0x00416D7C
nn::pia::session::CommonMatchmakeSession* nn::pia::local::UdsNetworkFactory::CreateMatchmakeSession()
{
    return new UdsMatchmakeSession();
}

// 0x00416DA0
nn::pia::local::UdsNetworkFactory::UdsNetworkFactory()
{
    // only the base and the vptr (in the original too)
}

// 0x004183AC
// 0x00416DB8 (deleting dtor)
nn::pia::local::UdsNetworkFactory::~UdsNetworkFactory()
{
    // empty (in the original too)
}

// 0x007302AC
u32 nn::pia::local::UdsNetworkFactory::GetSessionInfoNumMax()
{
    return 16;
}

// 0x007302B4
void nn::pia::local::UdsNetworkFactory::vf_0xAC()
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
