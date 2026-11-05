// the state functions of the font loaded on request (in their own file: GCC would fold the table into Calc)
#include "font/dMgr.h"

namespace font {
// 0x00866384
const Mgr::ReloadableFontData::StateFunction Mgr::ReloadableFontData::s_StateFunctions[LOAD_STATE_NUM] = {
    NULL,
    &ReloadableFontData::StateFreePhoto,
    &ReloadableFontData::StateLoading,
    NULL,
    &ReloadableFontData::StateUnloading,
    &ReloadableFontData::StateRestorePhoto,
};
} // namespace font
