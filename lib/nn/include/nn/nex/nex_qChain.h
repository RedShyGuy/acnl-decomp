#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
// Instantiations found in the binary:
//   nn::nex::qChain<nn::nex::BandwidthCounter*, nn::nex::DefaultChainPolicy<nn::nex::BandwidthCounter*> >  typeinfo 0x008CF60C  vtable 0x008FFAB8
//   nn::nex::qChain<nn::nex::Job*, nn::nex::DefaultChainPolicy<nn::nex::Job*> >  typeinfo 0x008CF624  vtable 0x008FFAD8
//   nn::nex::qChain<nn::nex::Packet*, nn::nex::ChainPolicyHistoryPacket<nn::nex::Packet*> >  typeinfo 0x008CF648  vtable 0x008FFB08
//   nn::nex::qChain<nn::nex::Packet*, nn::nex::ChainPolicyTemp<nn::nex::Packet*> >  typeinfo 0x008CF630  vtable 0x008FFAE8
//   nn::nex::qChain<nn::nex::Packet*, nn::nex::DefaultChainPolicy<nn::nex::Packet*> >  typeinfo 0x008CF63C  vtable 0x008FFAF8
//   nn::nex::qChain<nn::nex::ProfilingUnit*, nn::nex::DefaultChainPolicy<nn::nex::ProfilingUnit*> >  typeinfo 0x008CF5F4  vtable 0x008FFA98
//   nn::nex::qChain<nn::nex::SystemSetting*, nn::nex::DefaultChainPolicy<nn::nex::SystemSetting*> >  typeinfo 0x008CF600  vtable 0x008FFAA8
//   nn::nex::qChain<nn::nex::ThreadVariableRoot*, nn::nex::DefaultChainPolicy<nn::nex::ThreadVariableRoot*> >  typeinfo 0x008CF618  vtable 0x008FFAC8
template <typename T0, typename T1>
class qChain
{
public:
    // TODO: members unknown
};
} // namespace nex
} // namespace nn
