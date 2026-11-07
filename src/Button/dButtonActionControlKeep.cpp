#include "Button/dButtonActionControlKeep.h"

// 0x00334FFC slot 0x2C (name after libgarden)
void ButtonActionControlKeep::SelectCurrent()
{
    m_pCurrent->BeginSelectOk();
    m_pFunc = &ButtonActionControl::State_Selecting;
}

// 0x00335038 slot 0x00 | slot vf_0x00 of ssys::st::List
// 0x00335028 (deleting dtor)
ButtonActionControlKeep::~ButtonActionControlKeep()
{
    // empty (in the original too)
}
