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
