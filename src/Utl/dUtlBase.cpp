#include "Utl/dUtlBase.h"

#include "Ac/dAcStrc.h"
#include "Bs/dBsScene.h"
#include "Other/dBase.h"
#include "Other/dDemoActor.h"

// the copies of Initialize and Finalize for each T (and the destructor of UtlBase<DemoActor>, the only instance with a
// vtable of its own)

// 0x0082067C
template oml::framework::Result UtlBase<Base>::Initialize();
// 0x00820718
template oml::framework::Result UtlBase<Base>::Finalize();
// 0x00820810
template oml::framework::Result UtlBase<AcStrc>::Initialize();
// 0x008208AC
template oml::framework::Result UtlBase<AcStrc>::Finalize();
// 0x008209A4
template oml::framework::Result UtlBase<BsScene>::Initialize();
// 0x00820A40
template oml::framework::Result UtlBase<BsScene>::Finalize();
// 0x00820B38
template oml::framework::Result UtlBase<DemoActor>::Initialize();
// 0x00820BD4
template oml::framework::Result UtlBase<DemoActor>::Finalize();
// 0x00614954
// 0x00614944 (deleting dtor)
template UtlBase<DemoActor>::~UtlBase();
