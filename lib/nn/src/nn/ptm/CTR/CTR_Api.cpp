#include "nn/ptm/CTR/CTR_Api.h"
#include "nn/ptm/CTR/detail/detail_Api.h"
#include "nn/srv/srv_Api.h"
#include "nn/svc/svc_Api.h"

#include <string.h>

namespace nn {
namespace ptm {
namespace CTR {

namespace detail {
// 0x0097F024
nn::Handle s_Session;
} // namespace detail

namespace {
// constants of this file (names are ours)
// 0x008A3758
const nn::Handle INVALID_HANDLE;
// the PTM services (3dbrew "PTM Services"); this library connects to the first
// 0x008A375C
const char* const SERVICE_NAMES[] = {"ptm:u", "ptm:s", "ptm:sysm", "ptm:play", "ptm:gets"};
} // namespace

// 0x0011E2F8 | nintendogs:bytes [tier B]
nn::Result Initialize()
{
    const char* name = SERVICE_NAMES[0];
    if (detail::s_Session.IsValid()) {
        return nn::Result();
    }
    nn::Result result = nn::srv::GetServiceHandle(&detail::s_Session, name, strlen(name), 0);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result();
}

// 0x0013112C | nintendogs:bytes [tier B]
nn::Result Finalize()
{
    if (detail::s_Session.IsValid()) {
        nn::Result result = nn::svc::CloseHandle(detail::s_Session);
        if (result.IsFailure()) {
            return result;
        }
        detail::s_Session = INVALID_HANDLE;
    }
    return nn::Result();
}

} // namespace CTR
} // namespace ptm
} // namespace nn
