#include "Catalog/dLetterList.h"

#include "Item/item_Api.h"

// 0x007F4BB4
template CatalogBase* CatalogBase::Create<LetterList>();

// 0x001AD488 slot 0x00
// 0x001AD448 (deleting dtor)
LetterList::~LetterList()
{
    // (only the members and the base are destroyed)
}

// 0x001AD3EC slot 0x34
bool LetterList::SetupItems()
{
    m_ItemNum = CollectIds(m_Ids, item::KIND_PAPER, false);
    SortIds(m_Ids, m_ItemNum);
    SetNames(m_Names, m_Ids, m_ItemNum);
    return true;
}
