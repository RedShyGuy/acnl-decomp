// std::numpunct<char> of the C++ library (Thumb code in the original): the punctuation of numbers and the names of
// the bool values, read once through the virtual functions. See rw/rw_Locale.h.

#include "decomp.h"
#include "rw/rw_Locale.h"
#include "clib/clib_Thumb.h"

// 0x0010C224 (name after the RTTI)
__rw::__rw_punct_data<char>::~__rw_punct_data()
{
    // (only the members are destroyed)
}

// 0x0010C230 (name after the RTTI)
__rw::__rw_numpunct_impl_data<char>::~__rw_numpunct_impl_data()
{
    // (only the members are destroyed)
}

// 0x0011C014 (name after the C++ standard)
// 0x0011C008 (deleting dtor)
DECOMP_NOINLINE DECOMP_NOIPA std::numpunct<char>::~numpunct()
{
    // (only the bases are destroyed)
}

// the values of the virtual functions, and the names of true (1) and false (0) for reading bools
// 0x0011C67C (name is ours)
void std::numpunct<char>::Initialize(const locale& loc)
{
    m_DecimalPoint = do_decimal_point();
    m_ThousandsSep = do_thousands_sep();
    m_Grouping = do_grouping();
    m_FalseName = do_falsename();
    m_TrueName = do_truename();
    m_BoolNames[0].m_pName = m_TrueName.c_str();
    m_BoolNames[0].m_Value = 1;
    m_BoolNames[1].m_pName = m_FalseName.c_str();
    m_BoolNames[1].m_Value = 0;
    m_BoolNameCount = 2;
    m_pPunct = static_cast<__rw::__rw_numpunct_impl<char>*>(this);
}

// 0x0011C46E (name after the C++ standard)
char std::numpunct<char>::do_decimal_point() const
{
    return m_DecimalPoint;
}

// 0x0011C472 (name after the C++ standard)
char std::numpunct<char>::do_thousands_sep() const
{
    return m_ThousandsSep;
}

// 0x0011C450 (name after the C++ standard)
std::string std::numpunct<char>::do_grouping() const
{
    return m_Grouping;
}

// 0x0011C45A (name after the C++ standard)
std::string std::numpunct<char>::do_truename() const
{
    return m_TrueName;
}

// 0x0011C464 (name after the C++ standard)
std::string std::numpunct<char>::do_falsename() const
{
    return m_FalseName;
}
