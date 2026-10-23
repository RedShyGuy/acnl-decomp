#include "nw/gfx/gfx_BasicRenderQueue.h"

#include "nw/gfx/gfx_ISceneUpdater.h"
#include "nw/gfx/gfx_PriorDepthRenderKeyFactory.h"
#include "nw/gfx/gfx_PriorMaterialRenderKeyFactory.h"

namespace nw {
namespace gfx {
template <typename TElement, typename TContainer, typename TKeyFactory>
const TypeInfo BasicRenderQueue<TElement, TContainer, TKeyFactory>::s_TypeInfo(nullptr);

// (the type info is set by a static initializer of the game)
template const TypeInfo RenderQueue::s_TypeInfo;

// 0x007E67D8 (name is ours)
// 0x007E6754 (deleting dtor)
template nw::gfx::BasicRenderQueue<nw::gfx::BasicRenderElement<unsigned long long>, nw::ut::MoveArray<nw::gfx::BasicRenderElement<unsigned long long>>, nw::gfx::BasicRenderKeyFactory<unsigned long long>>::~BasicRenderQueue();
// 0x007E64C8 (name is ours)
template nw::gfx::BasicRenderElement<unsigned long long>* nw::gfx::BasicRenderQueue<nw::gfx::BasicRenderElement<unsigned long long>, nw::ut::MoveArray<nw::gfx::BasicRenderElement<unsigned long long>>, nw::gfx::BasicRenderKeyFactory<unsigned long long>>::Enqueue(
    const nw::gfx::BasicRenderElement<unsigned long long>& element, f32 depth, u8 translucencyKind, u8 layer);
// 0x00828054 | nintendogs (C)
template f32 nw::gfx::BasicRenderQueue<nw::gfx::BasicRenderElement<unsigned long long>, nw::ut::MoveArray<nw::gfx::BasicRenderElement<unsigned long long>>, nw::gfx::BasicRenderKeyFactory<unsigned long long>>::CalculateDepthOfSkeletalModelFunctor::operator()<
    nw::gfx::BasicRenderQueue<nw::gfx::BasicRenderElement<unsigned long long>, nw::ut::MoveArray<nw::gfx::BasicRenderElement<unsigned long long>>, nw::gfx::BasicRenderKeyFactory<unsigned long long>>::AlwaysCalculatingFunctor>(res::ResMesh mesh, const Camera& camera) const;


// 0x004A4398 (name is ours)
// 0x004A4394 (deleting dtor)
template nw::gfx::PriorDepthRenderKeyFactory<unsigned long long, nw::gfx::BasicRenderElement<unsigned long long>::ReverseDepth24bitQuantizer>::~PriorDepthRenderKeyFactory();
// 0x007E6DCC slot 0x08 (name is ours)
template u64 nw::gfx::PriorDepthRenderKeyFactory<unsigned long long, nw::gfx::BasicRenderElement<unsigned long long>::ReverseDepth24bitQuantizer>::CreateRenderKey(const nw::gfx::BasicRenderElement<unsigned long long>& element, f32 depth, u8 layer);
// 0x007E6E68 slot 0x0C (name is ours)
template u64 nw::gfx::PriorDepthRenderKeyFactory<unsigned long long, nw::gfx::BasicRenderElement<unsigned long long>::ReverseDepth24bitQuantizer>::CreateCommandRenderKey(u32 command, u8 translucencyKind, u8 priority, u8 layer);

// 0x004A477C (name is ours)
// 0x004A4778 (deleting dtor)
template nw::gfx::PriorMaterialRenderKeyFactory<unsigned long long, nw::gfx::BasicRenderElement<unsigned long long>::Depth24bitQuantizer>::~PriorMaterialRenderKeyFactory();
// 0x007E6E94 slot 0x08 (name is ours)
template u64 nw::gfx::PriorMaterialRenderKeyFactory<unsigned long long, nw::gfx::BasicRenderElement<unsigned long long>::Depth24bitQuantizer>::CreateRenderKey(const nw::gfx::BasicRenderElement<unsigned long long>& element, f32 depth, u8 layer);
// 0x007E6F44 slot 0x0C (name is ours)
template u64 nw::gfx::PriorMaterialRenderKeyFactory<unsigned long long, nw::gfx::BasicRenderElement<unsigned long long>::Depth24bitQuantizer>::CreateCommandRenderKey(u32 command, u8 translucencyKind, u8 priority, u8 layer);
} // namespace gfx
} // namespace nw
