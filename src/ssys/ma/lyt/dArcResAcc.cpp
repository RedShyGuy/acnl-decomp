#include "ssys/ma/lyt/dArcResAcc.h"
#include "sead/seadFixedSafeString.h"

namespace ssys {
namespace ma {
namespace lyt {
// 0x0056A3C4 (name is ours)
bool ssys::ma::lyt::ArcResAcc::LoadTextures()
{
    nw::lyt::DarcArchive archive;
    nw::lyt::DarcArchive::Init(m_pData, &archive);
    nw::lyt::DarcDir dir;
    if (!archive.OpenTypeDir(L"/", 'timg', &dir)) {
        return false;
    }
    sead::FixedSafeString<64> name;
    nw::lyt::DarcDirEntry entry;
    while (dir.ReadWithExtension(L".bclim", 6, &entry)) {
        if (entry.isDir) {
            continue;
        }
        name.copyFromWideChar(sead::SafeStringBase<wchar_t>(entry.name));
        m_ArcResourceAccessor.nw::lyt::ArcResourceAccessor::GetTexture(name.getCString());
    }
    return true;
}

// 0x0056A5A0
ssys::ma::lyt::ArcResAcc::ArcResAcc() : ResAccInterface(&m_ArcResourceAccessor)
{
    // (only the base and the members are constructed)
}

// 0x0056A610 slot 0x00
// 0x0056A5F0 (deleting dtor)
ssys::ma::lyt::ArcResAcc::~ArcResAcc()
{
    // (only the members are destroyed)
}

} // namespace lyt
} // namespace ma
} // namespace ssys
