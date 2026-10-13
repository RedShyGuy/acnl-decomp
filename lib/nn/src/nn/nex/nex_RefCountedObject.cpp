#include "nn/nex/nex_RefCountedObject.h"

namespace nn {
namespace nex {
// 0x003839C4 | fefates:bytes [tier B]
nn::nex::RefCountedObject::RefCountedObject() : m_RefCount(0)
{
    m_RefCount = 1;
    m_IsDeleted = false;
}

// 0x00383A10 | fefates:callgraph
// 0x003839E4 (deleting dtor)
nn::nex::RefCountedObject::~RefCountedObject()
{
    m_RefCount = 0;
}

} // namespace nex
} // namespace nn
