#include "mutil/dBaseResourceMgr.h"
#include "g3d/dResourceLoader.h"
#include "field/dFieldBuilderUtil.h"

namespace mutil {

// 0x005B2504 (name is ours)
ResourceKey::ResourceKey() : m_Hash(0), m_Flag(false)
{
}

// 0x005B24F0 (name is ours)
ResourceKey::ResourceKey(const ResourceKey& other) : m_Hash(other.m_Hash), m_Flag(false)
{
}

// 0x005B24D0 (name is ours)
void ResourceKey::Set(u32 hash)
{
    m_Hash = hash;
    m_Flag = false;
}

// 0x005B24E0 (name is ours)
void ResourceKey::Clear()
{
    m_Hash = 0;
    m_Flag = false;
}

// 0x005B2514 (name is ours)
ResourceKey& ResourceKey::operator=(const ResourceKey& other)
{
    if (this != &other) {
        m_Hash = other.m_Hash;
        m_Flag = false;
    }
    return *this;
}

template <typename T, u32 N>
BaseResourceMgr<T, N>::ResourceNode::ResourceNode()
{
    // (only the members are constructed)
}

template <typename T, u32 N>
BaseResourceMgr<T, N>::ResourceNode::~ResourceNode()
{
    // (only the members are destroyed)
}

// the nodes of the instances (one copy for each instance)
// 0x005B2530
template mutil::BaseResourceMgr<FgDataLoader, 18u>::ResourceNode::ResourceNode();
// 0x005B2580 slot 0x00
// 0x005B2558 (deleting dtor)
template mutil::BaseResourceMgr<FgDataLoader, 18u>::ResourceNode::~ResourceNode();
// 0x005B25A4
template mutil::BaseResourceMgr<g3d::ResourceLoader, 13u>::ResourceNode::ResourceNode();
// 0x005B25F4 slot 0x00
// 0x005B25CC (deleting dtor)
template mutil::BaseResourceMgr<g3d::ResourceLoader, 13u>::ResourceNode::~ResourceNode();
// 0x005B2618
template mutil::BaseResourceMgr<g3d::ResourceLoader, 15u>::ResourceNode::ResourceNode();
// 0x005B2668 slot 0x00
// 0x005B2640 (deleting dtor)
template mutil::BaseResourceMgr<g3d::ResourceLoader, 15u>::ResourceNode::~ResourceNode();
// 0x005B268C
template mutil::BaseResourceMgr<g3d::ResourceLoader, 1u>::ResourceNode::ResourceNode();
// 0x005B26DC slot 0x00
// 0x005B26B4 (deleting dtor)
template mutil::BaseResourceMgr<g3d::ResourceLoader, 1u>::ResourceNode::~ResourceNode();
// 0x005B2700
template mutil::BaseResourceMgr<g3d::ResourceLoader, 2u>::ResourceNode::ResourceNode();
// 0x005B2750 slot 0x00
// 0x005B2728 (deleting dtor)
template mutil::BaseResourceMgr<g3d::ResourceLoader, 2u>::ResourceNode::~ResourceNode();
// 0x005B2774
template mutil::BaseResourceMgr<g3d::ResourceLoader, 3u>::ResourceNode::ResourceNode();
// 0x005B27C4 slot 0x00
// 0x005B279C (deleting dtor)
template mutil::BaseResourceMgr<g3d::ResourceLoader, 3u>::ResourceNode::~ResourceNode();
// 0x005B27E8
template mutil::BaseResourceMgr<g3d::ResourceLoader, 4u>::ResourceNode::ResourceNode();
// 0x005B2838 slot 0x00
// 0x005B2810 (deleting dtor)
template mutil::BaseResourceMgr<g3d::ResourceLoader, 4u>::ResourceNode::~ResourceNode();
// 0x005B285C
template mutil::BaseResourceMgr<g3d::ResourceLoader, 64u>::ResourceNode::ResourceNode();
// 0x005B28AC slot 0x00
// 0x005B2884 (deleting dtor)
template mutil::BaseResourceMgr<g3d::ResourceLoader, 64u>::ResourceNode::~ResourceNode();

} // namespace mutil
