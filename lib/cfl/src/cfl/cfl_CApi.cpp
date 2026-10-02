#include "cfl/cfl_CApi.h"

extern "C" {
// 0x0013A8E0 | nintendogs:bytes [tier A]
void CFL_WaitAsync()
{
}

// 0x0013AA00 | nintendogs:bytes-fuzzy [tier A]
void CFLi_PutCRC16(int, void*, unsigned)
{
}

// 0x0013AA80 | nintendogs:callseq-callee [tier A]
void CFL_GetAsyncStatus()
{
}

// 0x0013DEBC | nintendogs:bytes [tier B]
void CFLi_FreeMemory(void*)
{
}

// 0x0013DEDC | nintendogs:callgraph [tier A]
void CFLi_GetManager()
{
}

// 0x00166374 | nintendogs:bytes [tier B]
void CFLi_GetCRC16(int, const void*, unsigned)
{
}

// 0x0016E244 | nintendogs:bytes [tier B]
void CFL_GetLastReason()
{
}

// 0x0016E260 | nintendogs:bytes-fuzzy [tier B]
void CFLi_CopyString16(unsigned short*, const unsigned short*, int)
{
}

// 0x00172664 | nintendogs:bytes [tier B]
void CFLi_GetNowDateTime(int*, int*, int*, int*, int*, int*)
{
}

// 0x00175AD8 | nintendogs:bytes-fuzzy [tier A]
void CFLi_GetStringLength8(const char*)
{
}

// 0x001762E8 | nintendogs:bytes [tier B]
void CFLi_IsNumberCharCode(unsigned short)
{
}

// 0x00176344 | nintendogs:bytes-fuzzy [tier B]
void CFLi_SearchCharacter8(const char*, char)
{
}

// 0x001DF8B4 | libgarden [tier A]
void SvGetBestFriendList()
{
}

// 0x002FBA40 | libgarden [tier A]
void SvGetCurrentPlayer()
{
}

// 0x002FBA60 | libgarden [tier A]
void SvGetPlayer(PlayerSaveNumber)
{
}

// 0x002FEB2C | libgarden [tier A]
void SvGetNetPlayer(netgame::PlayerNo)
{
}

// 0x005C13AC | libgarden [tier A]
void PlayerGetPosition(long&, long&, netgame::PlayerNo, bool)
{
}

// 0x005C3238 | libgarden [tier A]
void PlayerGetPositionForCamera()
{
}

// 0x005C3DDC | libgarden [tier A]
void PlayerGetState(netgame::PlayerNo)
{
}

// 0x005C3EA0 | libgarden [tier A]
void PlayerGetActor(netgame::PlayerNo, bool)
{
}

// 0x00748EA4 | libgarden [tier A]
void operator!=(FieldRect::iterator const&, FieldRect::iterator const&)
{
}

} // extern "C"
