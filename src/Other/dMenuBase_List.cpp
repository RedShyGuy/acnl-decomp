#include "Other/dMenuBase.h"

// 0x006AB408 slot 0x00 (name after libgarden)
// 0x006AB404 (deleting dtor)
MenuBase::List::~List()
{
    // (only the base is destroyed)
}
