#pragma once

#include "decomp.h"
#include "nn/nex/nex_Operation.h"

namespace nn {
namespace nex {
class BackEndServices;

// An operation of BackEndServices (the login and logout). The member name is ours.
// RTTI N2nn3nex19RendezVousOperationE @ 0x008CE808
class RendezVousOperation : public ::nn::nex::Operation
{
public:
    // (inline)
    explicit RendezVousOperation(BackEndServices* pBackEndServices) : Operation(0), m_pBackEndServices(pBackEndServices) {}

    BackEndServices* m_pBackEndServices; // 0x10
};
ASSERT_SIZE(RendezVousOperation, 0x14);
} // namespace nex
} // namespace nn
