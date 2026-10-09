#pragma once

#include "decomp.h"
#include "nn/applet/CTR/applet_Types.h"

namespace nn {
namespace erreula {
namespace CTR {
// the parameter of the error / EULA applet: sent to it and filled by it (0xF80 bytes; the layout
// is not known yet)
struct Parameter;

// starts the applet with the parameter, waits until it returns and writes its result back into
// the parameter
void StartErrEulaApplet(nn::applet::CTR::WakeupState* pWakeupState, nn::erreula::CTR::Parameter* pParameter); // 0x0048A874 | nintendogs:bytes [tier A]
} // namespace CTR
} // namespace erreula
} // namespace nn
