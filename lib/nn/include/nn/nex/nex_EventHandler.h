#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex12EventHandlerE @ 0x008CE0BC
// vtable 0x008FC4BC (vptr 0x008FC4C4), offset_to_top 0, 2 entries
class EventHandler : public ::nn::nex::RootObject
{
public:
    EventHandler(); // ctor candidate(s) 0x0035C88C (unverified)
    virtual void vf_0x00(); // 0x0035C998 slot 0x00 | fefates:callseq
    virtual ~EventHandler(); // 0x0035C968 slot 0x04 | slot vf_0x04 of nn::nex::EventHandler
};
} // namespace nex
} // namespace nn
