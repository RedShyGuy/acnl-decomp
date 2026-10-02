#pragma once

#include "decomp.h"

namespace nn {
namespace gr {
namespace CTR {
class Vertex
{
public:
    class LoadArray;
    struct IndexStream { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void DisableAttr_(unsigned); // 0x0034AC30 | nintendogs:bytes [tier A]
    void EnableAttrAsConst(const nn::gr::CTR::BindSymbolVSInput&, unsigned char, const float*); // 0x0034AEA0 | nintendogs:bytes [tier A]
    void MakeDrawCommand(unsigned*, const nn::gr::CTR::Vertex::IndexStream&) const; // 0x00727B4C | nintendogs:bytes [tier A]
    void MakeEnableAttrCommand_(unsigned*) const; // 0x00727BEC | mk7dlp:callgraph [tier A]
};
} // namespace CTR
} // namespace gr
} // namespace nn
