#pragma once

#include "decomp.h"

namespace nn {
namespace gxlow {
namespace CTR {
class Gpu;
class InterruptReceiver;

namespace detail {
// A request in the command queue of the GPU module (3dbrew "GSP Shared Memory": command
// header and seven parameter words). The type name is from the symbols, the members are ours.
struct CmdReq
{
    u8 id;             // 0x00, CMD_REQ_*
    u8 unknown01;      // 0x01 (1 in the template)
    u8 unknown02;      // 0x02
    u8 flags;          // 0x03 (the interrupt flag of CTR_Api.cpp; 0 for cache flushes)
    bit32 params[7];   // 0x04
};
ASSERT_SIZE(CmdReq, 0x20);

// the ids of the requests (3dbrew)
const u8 CMD_REQ_REQUEST_DMA = 0;
const u8 CMD_REQ_SET_COMMAND_LIST = 1;
const u8 CMD_REQ_MEMORY_FILL = 2;
const u8 CMD_REQ_DISPLAY_TRANSFER = 3;
const u8 CMD_REQ_TEXTURE_COPY = 4;
const u8 CMD_REQ_FLUSH_CACHE_REGIONS = 5;

DECOMP_NOINLINE bool IsAppletMode(); // 0x0012ACB4 | nintendogs:callgraph [tier A]
DECOMP_NOINLINE bool IsInitialized(); // 0x00131194 | nintendogs:callgraph [tier A]
DECOMP_NOINLINE bool IsFatalErrMode(); // 0x001372BC | nintendogs:callgraph [tier C]
DECOMP_NOINLINE nn::gxlow::CTR::InterruptReceiver* GetInterruptReceiver(); // 0x001372CC | nintendogs:callgraph [tier A]
DECOMP_NOINLINE nn::gxlow::CTR::Gpu* GetGpuIpc(); // 0x001372DC | nintendogs:callgraph [tier A]
} // namespace detail
} // namespace CTR
} // namespace gxlow
} // namespace nn
