#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_BasicRenderElement.h"
#include "nw/gfx/gfx_GfxObject.h"

namespace nw {
namespace gfx {
// Instantiations found in the binary:
//   nw::gfx::BasicRenderKeyFactory<unsigned long long>  typeinfo 0x008D070C
// Makes the sort keys of the elements of a render queue (the class name is from the reference
// symbols, the rest is ours). The keys share this layout (bits of the 64-bit key):
//   56-63 the layer, 54-55 the translucency kind of the material, 53 a command, 45-52 the render
//   priority of the mesh; the rest is the depth and the material, ordered by the factory.
template <typename TKey>
class BasicRenderKeyFactory : public GfxObject
{
public:
    typedef BasicRenderElement<TKey> Element;

    static const int LAYER_SHIFT = 56;
    static const int TRANSLUCENCY_KIND_SHIFT = 54;
    static const int COMMAND_SHIFT = 53;
    static const int PRIORITY_SHIFT = 45;

    // (inline)
    explicit BasicRenderKeyFactory(os::IAllocator* pAllocator) : GfxObject(pAllocator) {}
    virtual ~BasicRenderKeyFactory() {}

    // the key of a mesh element at the depth (0 near, 1 far)
    virtual TKey CreateRenderKey(const Element& element, f32 depth, u8 layer) = 0; // slot 0x08 (name is ours)
    // the key of a command (its low 32 bits are the command)
    virtual TKey CreateCommandRenderKey(u32 command, u8 translucencyKind, u8 priority, u8 layer) = 0; // slot 0x0C (name is ours)
};
} // namespace gfx
} // namespace nw
