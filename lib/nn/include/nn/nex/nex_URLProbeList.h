#pragma once

#include "decomp.h"
#include "nn/nex/nex_URLProbe.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
class StationURL;

// (out of line in the original; nex_URLProbeList.cpp)
template <>
DECOMP_NOINLINE void qList<URLProbe>::AddBuffer(bool isFirst);
template <>
DECOMP_NOINLINE void qList<URLProbe>::push_back(const URLProbe& value);

// The probes of the NAT traversal engine. The names of the functions without a reference name are
// ours.
// RTTI N2nn3nex12URLProbeListE
class URLProbeList : public ::nn::nex::qList<nn::nex::URLProbe>
{
public:
    // (inline)
    URLProbeList() {}
    virtual ~URLProbeList(); // 0x003617A0 slot 0x00 | fefates:bytes
    // 0x00361770 slot 0x04 (deleting dtor)
    // the probe of the address and RV connection id of the URL (null if there is none)
    nn::nex::URLProbe* FindProbe(const nn::nex::StationURL& url); // 0x00361678 | fefates:bytes [tier B]
};
ASSERT_SIZE(URLProbeList, 0x1C);
} // namespace nex
} // namespace nn
