#pragma once

#include "decomp.h"

// Instantiations found in the binary:
//   UtlBase<AcStrc>  typeinfo 0x008CD3F8
//   UtlBase<Base>  typeinfo 0x008CD3EC
//   UtlBase<BsScene>  typeinfo 0x008CD404
//   UtlBase<DemoActor>  typeinfo 0x008CD410  vtable 0x008F9200
// a base class of the game over T (base after libgarden)
template <typename T0>
class UtlBase : public T0
{
public:
    // TODO: members unknown
};
