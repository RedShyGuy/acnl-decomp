#include "Catalog/dSearchList.h"

#include "Other/dHeapInit.h"
#include "Other/dProcMgr.h"
#include "oml/framework/dProcessManager.h"

// (inline)
inline SearchList::SearchList() : m_pPages(NULL), m_pCatalog(NULL)
{
    m_TextLength = 33;
    m_ScrollBar.m_Unknown0x701 = false;
    m_pCatalog = reinterpret_cast<BsMenuCatalog*>(oml::framework::ProcessManager::Get()->FindProcess(
        static_cast<oml::framework::ProcessIndex>(PROC_BS_MENU_CATALOG), NULL));
}

// 0x007F4C18
template CatalogBase* CatalogBase::Create<SearchList>();

// 0x001AF8F0 slot 0x00
// 0x001AF8E0 (deleting dtor)
SearchList::~SearchList()
{
    // (only the base is destroyed)
}

// 0x001B0304 slot 0x0C
void SearchList::Finalize()
{
    m_pCatalog = NULL;
    delete[] m_pPages;
    SelectBase::Finalize();
}

// 0x001AF814 slot 0x34
bool SearchList::SetupItems()
{
    if (m_pCatalog != NULL) {
        void* results = *reinterpret_cast<void**>(reinterpret_cast<u8*>(m_pCatalog) + CATALOG_RESULTS_OFFSET);
        if (results != NULL) {
            const s32 num = Func14FD98(results);
            m_pPages = new (HeapInit::s_pUnknown953CC4, 4) SearchResultPage*[num];
            SearchResultPage* page = Func14FDA0(results);
            for (s32 i = 0; i < num; i++) {
                m_pPages[i] = page;
                page = page->pNext;
            }
            m_ItemNum = Func14FDA8(results);
        }
    }
    return true;
}
