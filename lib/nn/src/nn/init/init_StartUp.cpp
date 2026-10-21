#include "nn/applet/CTR/CTR_Api.h"
#include "nn/init/init_Api.h"
#include "nn/ndm/ndm_Api.h"
#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/os/os_Api.h"
#include "nn/srv/srv_Api.h"

// The C functions the entry code at 0x00100000 calls one after the other (nninitRegion,
// nninitLocale, InitializeSystem, the start-up of the program, the C++ runtime,
// nninitCallStaticInitializers, nninitSetup, then the main function).

// the bounds of the zero-initialized data and of the table of static initializers (the symbols
// of the GNU linker script; ARMCC's linker gives the addresses directly)
extern "C" u32 __bss_start__[];
extern "C" u32 __bss_end__[];
typedef void (*StaticInitializer)();
extern "C" StaticInitializer __init_array_start[];
extern "C" StaticInitializer __init_array_end[];

// 0x00100024 | nintendogs:bytes [tier B]
extern "C" void nninitRegion()
{
    for (u32* p = __bss_start__; p < __bss_end__;) {
        *p++ = 0;
    }
}

// 0x0011CC84 | nintendogs:bytes [tier A]
extern "C" void nninitCallStaticInitializers()
{
    for (StaticInitializer* p = __init_array_start; p < __init_array_end; p++) {
        (*p)();
    }
}

// 0x0011E2B4 | nintendogs:callgraph [tier A]
extern "C" void nninitSetupDaemons()
{
    nn::ndm::SetupDaemonsDefault();
}

// 0x0011CD0C | nintendogs:bytes [tier A]
extern "C" void nninitSetup()
{
    nn::srv::StartNotification();
    nn::os::CTR::detail::SetInternalErrorHandlingMode(false);
    nn::applet::CTR::Initialize(0);
    nninitSetupDaemons();
}

namespace nn {
namespace init {
// os, then the service manager (the call of srv::Initialize is a nop that falls into it)
// 0x0012A7F0 (name is ours)
void InitializeSystem()
{
    nn::os::Initialize();
    nn::srv::Initialize();
}
} // namespace init
} // namespace nn
