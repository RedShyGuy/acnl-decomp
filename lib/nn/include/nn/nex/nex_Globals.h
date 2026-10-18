#pragma once

#include "decomp.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_StreamBundling.h"
#include "nn/nex/nex_TransportBufferThread.h"

// The globals of nex that pia reads (the monitoring data of inet::NatTraverser, the state check of
// inet::NexFacade::Startup): Core::s_ThreadMode, Core::s_IsTransportThreaded and the settings of
// the transport buffer threads.
