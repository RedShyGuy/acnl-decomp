#include "Catalog/dMusicList.h"

#include "Item/item_Api.h"

// 0x007F4E10
template CatalogBase* CatalogBase::Create<MusicList>();

// 0x006E7890 slot 0x00
// 0x006E7850 (deleting dtor)
MusicList::~MusicList()
{
    // (only the members and the base are destroyed)
}

// 0x006E77FC slot 0x34
bool MusicList::SetupItems()
{
    m_ItemNum = CollectIds(m_Ids, item::KIND_MUSIC, false);
    SortIds(m_Ids, m_ItemNum);
    SetNames(m_Names, m_Ids, m_ItemNum);
    return true;
}
