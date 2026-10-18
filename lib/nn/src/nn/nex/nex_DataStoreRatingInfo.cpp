#include "nn/nex/nex_DataStoreRatingInfo.h"

namespace nn {
namespace nex {
// 0x00392F60
nn::nex::DataStoreRatingInfo::DataStoreRatingInfo()
{
    // nothing to do
}

// 0x00392F7C slot 0x00
// 0x00392F78 (deleting dtor)
nn::nex::DataStoreRatingInfo::~DataStoreRatingInfo()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn

namespace std {
// 0x00830F8C
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStoreRatingInfo, nn::nex::MemAllocator<nn::nex::DataStoreRatingInfo> >::_C_insert_aux(nn::nex::DataStoreRatingInfo* pos, const nn::nex::DataStoreRatingInfo& value)
{
    InsertAuxInline(pos, value);
}
} // namespace std

namespace __rw {
// 0x00803660 (name is ours)
template __rb_tree<s8, std::pair<const s8, nn::nex::DataStoreRatingInfo>, __select1st<std::pair<const s8, nn::nex::DataStoreRatingInfo>, s8>, std::less<s8>, nn::nex::MemAllocator<std::pair<const s8, nn::nex::DataStoreRatingInfo>> >::iterator __rb_tree<s8, std::pair<const s8, nn::nex::DataStoreRatingInfo>, __select1st<std::pair<const s8, nn::nex::DataStoreRatingInfo>, s8>, std::less<s8>, nn::nex::MemAllocator<std::pair<const s8, nn::nex::DataStoreRatingInfo>> >::erase(iterator it);
} // namespace __rw

namespace __rw {
// 0x00804344 (name is ours)
template __rb_tree<s8, std::pair<const s8, nn::nex::DataStoreRatingInfo>, __select1st<std::pair<const s8, nn::nex::DataStoreRatingInfo>, s8>, std::less<s8>, nn::nex::MemAllocator<std::pair<const s8, nn::nex::DataStoreRatingInfo>> >& __rb_tree<s8, std::pair<const s8, nn::nex::DataStoreRatingInfo>, __select1st<std::pair<const s8, nn::nex::DataStoreRatingInfo>, s8>, std::less<s8>, nn::nex::MemAllocator<std::pair<const s8, nn::nex::DataStoreRatingInfo>> >::operator=(const __rb_tree<s8, std::pair<const s8, nn::nex::DataStoreRatingInfo>, __select1st<std::pair<const s8, nn::nex::DataStoreRatingInfo>, s8>, std::less<s8>, nn::nex::MemAllocator<std::pair<const s8, nn::nex::DataStoreRatingInfo>> >& other);
} // namespace __rw
