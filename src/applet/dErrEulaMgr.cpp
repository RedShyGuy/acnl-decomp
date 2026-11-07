#include "applet/dErrEulaMgr.h"

namespace applet {
} // namespace applet

namespace applet {
// 0x0094F3D8
ErrEulaMgr* ErrEulaMgr::s_pInstance;

// 0x005CEE38 (name is ours)
bool ErrEulaMgr::IsUnknownFADSet()
{
    return s_pInstance->m_UnknownFAD;
}

// 0x005CF1AC (name is ours)
bool ErrEulaMgr::IsInputBlocked()
{
    if (s_pInstance->m_Unknown0x10 != 0) {
        return false;
    }
    return s_pInstance->m_UnknownFAC != 0;
}
} // namespace applet
