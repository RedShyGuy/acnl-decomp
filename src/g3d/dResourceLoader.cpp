#include "g3d/dResourceSet.h"
#include "g3d/dResourceLoader.h"

namespace g3d {
// 0x00318014 slot 0x00 | virtual slot, introduced by g3d::ResourceSet
void g3d::ResourceLoader::vf_0x00()
{
}

// 0x004EEC5C slot 0x04 | virtual slot, introduced by g3d::ResourceSet
void g3d::ResourceLoader::vf_0x04()
{
}

// 0x00317C38 | libgarden [tier A]
void g3d::ResourceLoader::LoadAsync(sead::SafeStringBase<char> const&, sead::Heap*, unsigned int)
{
}

// 0x00317C70 | libgarden [tier A]
void g3d::ResourceLoader::InitializeCgfx(nw::os::IAllocator*, g3d::ResourceLoader*, bool)
{
}

// 0x004EB42C | libgarden [tier A]
void g3d::ResourceLoader::Read(unsigned long, unsigned long, unsigned long)
{
}

// 0x004EEBE4 | libgarden [tier A]
void g3d::ResourceLoader::Load(sead::SafeStringBase<char> const&, sead::Heap*, unsigned int)
{
}

// 0x004EEC1C | libgarden [tier A]
g3d::ResourceLoader::ResourceLoader()
{
}

} // namespace g3d
