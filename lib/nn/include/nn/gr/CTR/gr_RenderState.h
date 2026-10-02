#pragma once

#include "decomp.h"

namespace nn {
namespace gr {
namespace CTR {
class RenderState
{
public:
    class Blend;
    class ShadowMap;
    class FBAccess;
    class StencilTest;
    class Culling;
    class AlphaTest;
    class DepthTest;
    RenderState(); // 0x00123C08 | fefates:bytes [tier B]
    void MakeCommand(unsigned*, bool) const; // 0x001269F0 | nintendogs:callgraph [tier A]
};
} // namespace CTR
} // namespace gr
} // namespace nn
