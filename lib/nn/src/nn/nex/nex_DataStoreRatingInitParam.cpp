#include "nn/nex/nex_DataStoreRatingInitParam.h"
#include "nn/nex/nex_qMap.h"

namespace nn {
namespace nex {
// 0x003B24BC slot 0x00
// 0x003B24B8 (deleting dtor)
nn::nex::DataStoreRatingInitParam::~DataStoreRatingInitParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn

namespace __rw {
// 0x00804BBC | fefates:bytes [tier B]
template __rb_tree<s8, std::pair<const s8, nn::nex::DataStoreRatingInitParam>, __select1st<std::pair<const s8, nn::nex::DataStoreRatingInitParam>, s8>, std::less<s8>, nn::nex::MemAllocator<std::pair<const s8, nn::nex::DataStoreRatingInitParam>> >::iterator __rb_tree<s8, std::pair<const s8, nn::nex::DataStoreRatingInitParam>, __select1st<std::pair<const s8, nn::nex::DataStoreRatingInitParam>, s8>, std::less<s8>, nn::nex::MemAllocator<std::pair<const s8, nn::nex::DataStoreRatingInitParam>> >::_C_insert(__rb_tree<s8, std::pair<const s8, nn::nex::DataStoreRatingInitParam>, __select1st<std::pair<const s8, nn::nex::DataStoreRatingInitParam>, s8>, std::less<s8>, nn::nex::MemAllocator<std::pair<const s8, nn::nex::DataStoreRatingInitParam>> >::Node* pNode, __rb_tree<s8, std::pair<const s8, nn::nex::DataStoreRatingInitParam>, __select1st<std::pair<const s8, nn::nex::DataStoreRatingInitParam>, s8>, std::less<s8>, nn::nex::MemAllocator<std::pair<const s8, nn::nex::DataStoreRatingInitParam>> >::Node* pParent, const std::pair<const s8, nn::nex::DataStoreRatingInitParam>& value);
} // namespace __rw

namespace nn {
namespace nex {
// 0x003CE37C | fefates:bytes [tier B] (only the game calls it)
template qMap<s8, DataStoreRatingInitParam>::qMap();
} // namespace nex
} // namespace nn
