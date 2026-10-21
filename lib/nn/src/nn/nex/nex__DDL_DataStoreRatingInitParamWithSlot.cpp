#include "nn/nex/nex__DDL_DataStoreRatingInitParamWithSlot.h"
#include "nn/nex/nex_DataStoreRatingInitParamWithSlot.h"

namespace nn {
namespace nex {
// 0x003CC3E8 slot 0x00
// 0x003CC3D8 (deleting dtor)
nn::nex::_DDL_DataStoreRatingInitParamWithSlot::~_DDL_DataStoreRatingInitParamWithSlot()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003CC3F8
nn::nex::_DDL_DataStoreRatingInitParamWithSlot& nn::nex::_DDL_DataStoreRatingInitParamWithSlot::operator=(const nn::nex::_DDL_DataStoreRatingInitParamWithSlot& other)
{
    m_StructureVersion = other.m_StructureVersion;
    m_Slot = other.m_Slot;
    m_Param = other.m_Param;
    return *this;
}

} // namespace nex
} // namespace nn
