#pragma once

#include "decomp.h"
#include "List/dInstSelect.h"
#include "script/dWordFix.h"

class BsMenuCatalog;

// (game code in a CRO module, names are ours) a page of the results of a search of the catalog menu
struct SearchResultPage
{
    static const s32 ITEM_NUM = 32;

    script::WordFix<33> names[ITEM_NUM];  // 0x000
    u16 ids[ITEM_NUM];                    // 0xB80
    SearchResultPage* pNext;              // 0xBC0
};
ASSERT_OFFSET(SearchResultPage, ids, 0xB80);
ASSERT_OFFSET(SearchResultPage, pNext, 0xBC0);
// (import thunks into the module) the number of pages, the first page and the number of results of the results
// object of the catalog menu
s32 Func14FD98(void* results); // 0x0014FD98
SearchResultPage* Func14FDA0(void* results); // 0x0014FDA0
s32 Func14FDA8(void* results); // 0x0014FDA8

// The list of the results of a search in the catalog menu (BsMenuCatalog): the pages of the results are listed in
// an array, item i is entry i % 32 of page i / 32. Only the class name is from the RTTI; all other names are ours.
// RTTI 10SearchList @ 0x008CB1C4
// vtable 0x008EC3B0 (vptr 0x008EC3B8), offset_to_top 0, 26 entries
class SearchList : public ::InstSelect<8>
{
public:
    // the results object in the catalog menu
    static const u32 CATALOG_RESULTS_OFFSET = 0x3BAC;

    SearchList(); // (inline, in CatalogBase::Create<SearchList>)
    virtual ~SearchList(); // 0x001AF8F0 slot 0x00
    // 0x001AF8E0 slot 0x04 (deleting dtor)
    void Finalize() override; // 0x001B0304 slot 0x0C
    s32 GetSelectedId() const override // 0x0070BD48 slot 0x20
    {
        if (m_Selected == -1) {
            return -1;
        }
        return m_pPages[m_Selected >> 5]->ids[m_Selected & 31];
    }
    bool SetupItems() override; // 0x001AF814 slot 0x34
    script::IWord* GetItemWord(s32 index) override // 0x0070BD7C slot 0x64
    {
        return &m_pPages[index >> 5]->names[index & 31];
    }

    SearchResultPage** m_pPages;  // 0x12CC
    BsMenuCatalog* m_pCatalog;    // 0x12D0
};
ASSERT_OFFSET(SearchList, m_pPages, 0x12CC);
ASSERT_SIZE(SearchList, 0x12D4);
