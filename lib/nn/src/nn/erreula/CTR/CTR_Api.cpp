#include "nn/erreula/CTR/CTR_Api.h"
#include "nn/applet/CTR/CTR_Api.h"
#include "nn/applet/CTR/detail/detail_Api.h"
#include "nn/dbg/dbg_Api.h"

namespace nn {
namespace erreula {
namespace CTR {
namespace {
// the error / EULA applet (3dbrew "NS and APT Services", AppID)
const u32 APP_ID_ERREULA = 0x406;
const size_t PARAMETER_SIZE = 0xF80;
// PrepareToStartLibraryApplet: already prepared (module 51 applet, status, invalid state, 1020)
const bit32 RESULT_ALREADY_PREPARED = 0xC8A0CFFC;
} // namespace

// 0x0048A874 | nintendogs:bytes [tier A]
void StartErrEulaApplet(nn::applet::CTR::WakeupState* pWakeupState, nn::erreula::CTR::Parameter* pParameter)
{
    unsigned senderAppId = 0;
    int size = 0;
    nn::Result result = nn::applet::CTR::detail::PrepareToStartLibraryApplet(APP_ID_ERREULA);
    if (result.IsFailure() && result != RESULT_ALREADY_PREPARED) {
        nndbgPanic();
    }
    nn::applet::CTR::detail::StartLibraryApplet(APP_ID_ERREULA, reinterpret_cast<const unsigned char*>(pParameter),
                                                PARAMETER_SIZE, nn::applet::CTR::INVALID_HANDLE);
    *pWakeupState = nn::applet::CTR::detail::WaitForStarting(&senderAppId, reinterpret_cast<unsigned char*>(pParameter),
                                                             PARAMETER_SIZE, &size, 0, nn::applet::CTR::WAIT_INFINITE);
}

} // namespace CTR
} // namespace erreula
} // namespace nn
