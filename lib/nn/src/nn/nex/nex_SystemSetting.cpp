#include "nn/nex/nex_SystemSetting.h"

namespace nn {
namespace nex {
// 0x0036EA74 | mk7dlp:callgraph [tier A]
nn::nex::SystemSetting::List& nn::nex::SystemSetting::GetList()
{
    // 0x00ADDC10 (name is ours)
    static List s_List;
    return s_List;
}

// 0x0036EAE8 | mk7dlp:bytes [tier A]
nn::nex::SystemSetting::SystemSetting(const wchar_t* pName, unsigned int value)
    : m_pChainNext(nullptr), m_pChainPrev(nullptr), m_pName(pName), m_Value(value), m_Unknown0x14(false)
{
    GetList().push_back(this);
}

// 0x0036EB88 | mk7dlp:bytes [tier A]
// 0x0036EB58 (deleting dtor)
nn::nex::SystemSetting::~SystemSetting()
{
    List::iterator it = GetList().begin();
    while (it != GetList().end() && *it != this) {
        it = List::iterator((*it)->m_pChainNext);
    }
    GetList().erase(it);
}

// 0x007E42C4
// 0x007E4238 (deleting dtor)
template class qChain<SystemSetting*, DefaultChainPolicy<SystemSetting*> >;

} // namespace nex
} // namespace nn
