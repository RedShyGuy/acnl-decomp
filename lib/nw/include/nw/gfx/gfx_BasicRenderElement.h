#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResMesh.h"

namespace nw {
namespace gfx {
class Model;

// An entry of a render queue: a mesh of a model (or a command) and the key the queue is sorted by
// (the class and quantizer names are from the reference symbols, the rest is ours).
template <typename TKey>
class BasicRenderElement
{
public:
    // the depth (0 near, 1 far) as 24 bits, the far depths first
    struct ReverseDepth24bitQuantizer
    {
        TKey operator()(f32 depth) const { return static_cast<TKey>((1.0f - depth) * 16777215.0f) & 0xFFFFFF; }
    };
    // the depth as 24 bits, the near depths first
    struct Depth24bitQuantizer
    {
        TKey operator()(f32 depth) const { return static_cast<TKey>(depth * 16777215.0f) & 0xFFFFFF; }
    };

    // (inline)
    BasicRenderElement(res::ResMesh mesh, Model* pModel) : m_Key(0), m_Mesh(mesh), m_pModel(pModel) {}

    TKey m_Key;       // 0x0
    res::ResMesh m_Mesh; // 0x8 (NULL: a command)
    Model* m_pModel;  // 0xC
};
} // namespace gfx
} // namespace nw
