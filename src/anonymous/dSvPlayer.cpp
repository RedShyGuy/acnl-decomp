// Classes from the anonymous namespace of the original dSvPlayer.cpp

#include "decomp.h"
#include "sead/seadDelegate1.h"

namespace script {
class Phrase;
} // namespace script

namespace {

// vtable 0x0088FEFC (vptr 0x0088FF04), offset_to_top 0, 2 entries
class TakumiImportPhrase
{
public:
};

} // namespace

// the instances of the delegate of TakumiImportPhrase
// 0x008146C4 slot 0x00
template void sead::Delegate1<TakumiImportPhrase, script::Phrase*>::invoke(script::Phrase*);
// 0x0082A1D0 slot 0x04
template sead::IDelegate1<script::Phrase*>* sead::Delegate1<TakumiImportPhrase, script::Phrase*>::clone(sead::Heap*) const;
