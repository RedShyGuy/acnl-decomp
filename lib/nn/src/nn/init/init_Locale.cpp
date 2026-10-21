#include "nn/init/init_Api.h"
#include "clib/clib_Locale.h"

// The locale of the C library at the start of the program (see clib_Locale.h).

extern "C" {
// (the class of EOF is in front of the characters)
// 0x0011CCB8 | fefates:bytes [tier B]
void nninitLocale()
{
    clib::CLibraryLocale* pLocale = GetCLibraryLocale();
    pLocale->m_pCtypeClasses = GetCtypeLocale(0, nullptr) + 1;
    pLocale->m_pNumeric = GetNumericLocale(0, nullptr);
}
} // extern "C"
