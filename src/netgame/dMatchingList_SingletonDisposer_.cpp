#include "netgame/dMatchingList_SingletonDisposer_.h"

// 0x006206B4 slot 0x00 (name after the RTTI)
// 0x0062064C (deleting dtor)
netgame::MatchingList::SingletonDisposer_::~SingletonDisposer_()
{
    if (this == s_pSingletonDisposer) {
        s_pSingletonDisposer = nullptr;
        s_pInstance->~MatchingList();
        s_pInstance = nullptr;
    }
}
