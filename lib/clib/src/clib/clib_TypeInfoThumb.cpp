// The type_info classes of the C++ run-time (Thumb code in the original): the RTTI of the program only uses their
// vtables, which hold nothing but the destructors (the program has neither exceptions nor dynamic_cast). The deleting
// destructors free nothing. Class names from the RTTI of the binary. Not for use together with GCC's C++ headers.

#include "decomp.h"
#include "clib/clib_Thumb.h"

namespace std {
class type_info
{
public:
    virtual ~type_info() {}
    static void operator delete(void*) {}

protected:
    const char* __name;
};
} // namespace std

namespace __cxxabiv1 {
class __class_type_info : public std::type_info
{
public:
    virtual ~__class_type_info();
};

class __si_class_type_info : public __class_type_info
{
public:
    virtual ~__si_class_type_info();

private:
    const __class_type_info* __base_type;
};

class __vmi_class_type_info : public __class_type_info
{
public:
    virtual ~__vmi_class_type_info();

private:
    unsigned int __flags;
    unsigned int __base_count;
};

// 0x00300748 (name after the RTTI)
// 0x00106BE8 (deleting dtor)
__class_type_info::~__class_type_info()
{
    // (only the base is destroyed)
}

// 0x00106BF8 (name after the RTTI)
// 0x00106BF0 (deleting dtor)
__si_class_type_info::~__si_class_type_info()
{
    // (only the base is destroyed)
}

// 0x00106C08 (name after the RTTI)
// 0x00106C00 (deleting dtor)
__vmi_class_type_info::~__vmi_class_type_info()
{
    // (only the base is destroyed)
}
} // namespace __cxxabiv1
