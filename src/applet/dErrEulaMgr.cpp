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
} // namespace applet
