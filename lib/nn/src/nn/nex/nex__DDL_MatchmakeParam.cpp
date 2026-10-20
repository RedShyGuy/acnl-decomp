#include "nn/nex/nex__DDL_MatchmakeParam.h"

namespace __rw {
// 0x007FCA80 | fefates:bytes
template <>
DECOMP_NOINLINE __rb_tree<nn::nex::String, std::pair<const nn::nex::String, nn::nex::Variant>, __select1st<std::pair<const nn::nex::String, nn::nex::Variant>, nn::nex::String>, std::less<nn::nex::String>, nn::nex::MemAllocator<std::pair<const nn::nex::String, nn::nex::Variant> > >::iterator __rb_tree<nn::nex::String, std::pair<const nn::nex::String, nn::nex::Variant>, __select1st<std::pair<const nn::nex::String, nn::nex::Variant>, nn::nex::String>, std::less<nn::nex::String>, nn::nex::MemAllocator<std::pair<const nn::nex::String, nn::nex::Variant> > >::erase(iterator first, iterator last)
{
    return EraseRangeInline(first, last);
}

// 0x007FC9D0
template <>
DECOMP_NOINLINE __rb_tree<nn::nex::String, std::pair<const nn::nex::String, nn::nex::Variant>, __select1st<std::pair<const nn::nex::String, nn::nex::Variant>, nn::nex::String>, std::less<nn::nex::String>, nn::nex::MemAllocator<std::pair<const nn::nex::String, nn::nex::Variant> > >::Node* __rb_tree<nn::nex::String, std::pair<const nn::nex::String, nn::nex::Variant>, __select1st<std::pair<const nn::nex::String, nn::nex::Variant>, nn::nex::String>, std::less<nn::nex::String>, nn::nex::MemAllocator<std::pair<const nn::nex::String, nn::nex::Variant> > >::_C_get_link()
{
    return GetLinkInline();
}

// the functions of the tree of the parameters
// 0x007FD3B0 | fefates:bytes
template __rb_tree<nn::nex::String, std::pair<const nn::nex::String, nn::nex::Variant>, __select1st<std::pair<const nn::nex::String, nn::nex::Variant>, nn::nex::String>, std::less<nn::nex::String>, nn::nex::MemAllocator<std::pair<const nn::nex::String, nn::nex::Variant> > >::iterator __rb_tree<nn::nex::String, std::pair<const nn::nex::String, nn::nex::Variant>, __select1st<std::pair<const nn::nex::String, nn::nex::Variant>, nn::nex::String>, std::less<nn::nex::String>, nn::nex::MemAllocator<std::pair<const nn::nex::String, nn::nex::Variant> > >::_C_insert(Node* pNode, Node* pParent, const std::pair<const nn::nex::String, nn::nex::Variant>& value);
// 0x007FD230 | fefates:bytes
template __rb_tree<nn::nex::String, std::pair<const nn::nex::String, nn::nex::Variant>, __select1st<std::pair<const nn::nex::String, nn::nex::Variant>, nn::nex::String>, std::less<nn::nex::String>, nn::nex::MemAllocator<std::pair<const nn::nex::String, nn::nex::Variant> > >::Node* __rb_tree<nn::nex::String, std::pair<const nn::nex::String, nn::nex::Variant>, __select1st<std::pair<const nn::nex::String, nn::nex::Variant>, nn::nex::String>, std::less<nn::nex::String>, nn::nex::MemAllocator<std::pair<const nn::nex::String, nn::nex::Variant> > >::_C_copy(Node* pNode, Node* pParent);
} // namespace __rw

namespace nn {
namespace nex {
// 0x003951A0 slot 0x00 | fefates:bytes
// 0x00395190 (deleting dtor)
nn::nex::_DDL_MatchmakeParam::~_DDL_MatchmakeParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x00394D18 | fefates:bytes [tier B]
void nn::nex::_DDL_MatchmakeParam::Add(nn::nex::Message* pMessage, const nn::nex::_DDL_MatchmakeParam& data)
{
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 start = 0;
    if (!isWithoutHeader) {
        start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
    }
    _Type_map<_Type_string, _Type_variant>::Add(pMessage, data.m_Params);
    if (!isWithoutHeader) {
        DDLStructure::EndAdd(pMessage, start);
    }
}

// 0x00394E64 | fefates:bytes [tier B]
void nn::nex::_DDL_MatchmakeParam::Extract(nn::nex::Message* pMessage, nn::nex::_DDL_MatchmakeParam* pData)
{
    pData->m_StructureVersion = 0;
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 size = 0;
    if (!isWithoutHeader && !DDLStructure::BeginExtract(pMessage, &pData->m_StructureVersion, &size)) {
        return;
    }
    if (pMessage->m_IsError) {
        return;
    }
    u32 start = pMessage->m_Position;
    _Type_map<_Type_string, _Type_variant>::Extract(pMessage, &pData->m_Params);
    if (pMessage->m_IsError) {
        return;
    }
    if (!isWithoutHeader) {
        DDLStructure::EndExtract(pMessage, start, size);
    }
}

// 0x00395248 | fefates:bytes [tier B]
nn::nex::_DDL_MatchmakeParam& nn::nex::_DDL_MatchmakeParam::operator=(const nn::nex::_DDL_MatchmakeParam& other)
{
    m_StructureVersion = other.m_StructureVersion;
    m_Params = other.m_Params;
    return *this;
}

} // namespace nex
} // namespace nn
