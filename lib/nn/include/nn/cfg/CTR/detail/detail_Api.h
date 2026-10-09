#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/cfg/CTR/cfg_Types.h"

namespace nn {
namespace os {
class CriticalSection;
} // namespace os
namespace cfg {
namespace CTR {
namespace detail {
// the port a function opened for itself (the type name is from the binary; the values are ours)
enum _IPCPortType : u8
{
    IPC_PORT_TYPE_USER = 0, // cfg:u
    IPC_PORT_TYPE_SYS = 1,  // cfg:s
    IPC_PORT_TYPE_INIT = 2, // cfg:i
};

// cfg:u (counted; errors other than "not found" count as done)
nn::Result Initialize(); // 0x0011FCF0 | nintendogs:callgraph [tier A]
void Finalize(); // 0x001367E4 | fefates:bytes [tier B]
// cfg:s and cfg:i (counted); IpcUser then uses their session
nn::Result InitializeSys(); // 0x001243D0 | nintendogs:callgraph [tier A]
void FinalizeSys(); // 0x001242E0 | fefates:bytes [tier B]
nn::Result InitializeInit(); // 0x00124478 | nintendogs:callgraph [tier A]
void FinalizeInit(); // 0x0012435C | fefates:bytes [tier B]
// the first of cfg:u, cfg:s, cfg:i that opens
nn::Result InitializeProperPort(nn::cfg::CTR::detail::_IPCPortType* type); // 0x0011FE84 | nintendogs:bytes [tier A]
void FinalizeProperPort(nn::cfg::CTR::detail::_IPCPortType type); // 0x0011FDCC | nintendogs:callgraph [tier A]
// a session of the service of the name / its end
nn::Result InitializeBase(nn::Handle* session, const char* name); // 0x00129BE8 | nintendogs:bytes [tier A]
nn::Result FinalizeBase(nn::Handle* session); // 0x00129BA4 | nintendogs:bytes [tier A]
// made on the first call (from any thread)
nn::os::CriticalSection* GetCriticalSectionForInitializeFinalize(); // 0x00129C44 | fefates:bytes [tier B]

nn::cfg::CTR::CfgRegionCode GetRegion(); // 0x0011FEF0 | nintendogs:bytes [tier A]
nn::Result GetConfig(void* buffer, size_t size, u32 blockId); // 0x00124524 | nintendogs:callgraph [tier A]
nn::Result GetTransferableId(u32 unknown, u64* id); // 0x00350F48 | nintendogs:callgraph [tier A]
// (names after 3dbrew)
nn::Result GetRegionCanadaUSA(bool* isCanadaOrUsa); // 0x00350F90
nn::Result GetCountryCodeString(char* string, u16 countryCodeId); // 0x00350FD0

// the session of the commands (cfg:u, or cfg:s / cfg:i while they are open; name is ours)
extern nn::Handle s_Session; // 0x0097E83C
} // namespace detail
} // namespace CTR
} // namespace cfg
} // namespace nn
