#include "nn/nex/nex_ThreadVariable.h"
#include "nn/nex/nex_ObjectThreadRoot.h"

// The instances of ThreadVariable (armlink placed them with the other template instances).

namespace nn {
namespace nex {
// 0x007D6AF8 | fefates:callseq [tier B]
// 0x007D6AC8 (deleting dtor)
template nn::nex::ThreadVariable<unsigned int>::~ThreadVariable();
// 0x007D6904 (name is ours)
template void nn::nex::ThreadVariable<unsigned int>::ClearAll();
// 0x007D6848 (name is ours)
template void nn::nex::ThreadVariable<unsigned int>::ClearCurrentThread();
// 0x007D67A8 (name is ours)
template void nn::nex::ThreadVariable<unsigned int>::ClearThread(unsigned int);
// 0x007D6970 | fefates:callseq [tier B]
template unsigned int& nn::nex::ThreadVariable<unsigned int>::GetRef();
// 0x007D6C64 (symbols.json names it nn::nex::PseudoSingleton::GetCurrentContext)
template const unsigned int& nn::nex::ThreadVariable<unsigned int>::GetRef() const;

// 0x007D6578
// 0x007D6548 (deleting dtor)
template nn::nex::ThreadVariable<nn::nex::ObjectThreadRoot*>::~ThreadVariable();
// 0x007D64DC (name is ours)
template void nn::nex::ThreadVariable<nn::nex::ObjectThreadRoot*>::ClearAll();
// 0x007D6420 (name is ours)
template void nn::nex::ThreadVariable<nn::nex::ObjectThreadRoot*>::ClearCurrentThread();
// 0x007D6380 (name is ours)
template void nn::nex::ThreadVariable<nn::nex::ObjectThreadRoot*>::ClearThread(unsigned int);

} // namespace nex
} // namespace nn

namespace std {
// 0x0082E998 | fefates:callseq-callee [tier C]
template <>
DECOMP_NOINLINE nn::nex::ObjectThreadRoot*& map<u32, nn::nex::ObjectThreadRoot*>::operator[](const u32& key)
{
    return IndexInline(key);
}
} // namespace std

namespace __rw {
// 0x0080716C (name is ours)
template __rb_tree<u32, std::pair<const u32, nn::nex::ObjectThreadRoot*>, __select1st<std::pair<const u32, nn::nex::ObjectThreadRoot*>, u32>, std::less<u32>, nn::nex::MemAllocator<std::pair<const u32, nn::nex::ObjectThreadRoot*> > >::iterator __rb_tree<u32, std::pair<const u32, nn::nex::ObjectThreadRoot*>, __select1st<std::pair<const u32, nn::nex::ObjectThreadRoot*>, u32>, std::less<u32>, nn::nex::MemAllocator<std::pair<const u32, nn::nex::ObjectThreadRoot*> > >::_C_insert(__rb_tree<u32, std::pair<const u32, nn::nex::ObjectThreadRoot*>, __select1st<std::pair<const u32, nn::nex::ObjectThreadRoot*>, u32>, std::less<u32>, nn::nex::MemAllocator<std::pair<const u32, nn::nex::ObjectThreadRoot*> > >::Node* pNode, __rb_tree<u32, std::pair<const u32, nn::nex::ObjectThreadRoot*>, __select1st<std::pair<const u32, nn::nex::ObjectThreadRoot*>, u32>, std::less<u32>, nn::nex::MemAllocator<std::pair<const u32, nn::nex::ObjectThreadRoot*> > >::Node* pParent, const std::pair<const u32, nn::nex::ObjectThreadRoot*>& value);
} // namespace __rw
