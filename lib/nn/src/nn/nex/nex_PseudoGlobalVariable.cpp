#include "nn/nex/nex_PseudoGlobalVariable.h"

namespace nn {
namespace nex {
class DDLDeclarations;
class DynamicRunTimeInterface;

// the copies of the inline functions per type (in the vtables)
// PseudoGlobalVariable<DDLDeclarations*>
// 0x007DA938 slot 0x00
// 0x007DA8E8 (deleting dtor)
template PseudoGlobalVariable<DDLDeclarations*>::~PseudoGlobalVariable();
// 0x007DA874 slot 0x08
template void PseudoGlobalVariable<DDLDeclarations*>::AllocateValues();
// 0x007DA850 slot 0x0C
template void PseudoGlobalVariable<DDLDeclarations*>::FreeValues();
// 0x007DA840 slot 0x10
template void PseudoGlobalVariable<DDLDeclarations*>::ResetValue(u32 context);
// 0x007DA8D8 slot 0x14
template PseudoGlobalVariableRoot* PseudoGlobalVariable<DDLDeclarations*>::GetNext();
// 0x007DA8E0 slot 0x18
template void PseudoGlobalVariable<DDLDeclarations*>::SetNext(PseudoGlobalVariableRoot* pNext);

// PseudoGlobalVariable<DynamicRunTimeInterface*>
// 0x007DAA78 slot 0x00
// 0x007DAA28 (deleting dtor)
template PseudoGlobalVariable<DynamicRunTimeInterface*>::~PseudoGlobalVariable();
// 0x007DA9B4 slot 0x08
template void PseudoGlobalVariable<DynamicRunTimeInterface*>::AllocateValues();
// 0x007DA990 slot 0x0C
template void PseudoGlobalVariable<DynamicRunTimeInterface*>::FreeValues();
// 0x007DA980 slot 0x10
template void PseudoGlobalVariable<DynamicRunTimeInterface*>::ResetValue(u32 context);
// 0x007DAA18 slot 0x14
template PseudoGlobalVariableRoot* PseudoGlobalVariable<DynamicRunTimeInterface*>::GetNext();
// 0x007DAA20 slot 0x18
template void PseudoGlobalVariable<DynamicRunTimeInterface*>::SetNext(PseudoGlobalVariableRoot* pNext);

// PseudoGlobalVariable<wchar_t*>
// 0x007DABF4 slot 0x00
// 0x007DAB80 (deleting dtor)
template PseudoGlobalVariable<wchar_t*>::~PseudoGlobalVariable();
// 0x007DAB0C slot 0x08
template void PseudoGlobalVariable<wchar_t*>::AllocateValues();
// 0x007DAAD0 slot 0x0C
template void PseudoGlobalVariable<wchar_t*>::FreeValues();
// 0x007DAAC0 slot 0x10
template void PseudoGlobalVariable<wchar_t*>::ResetValue(u32 context);
// 0x007DAB70 slot 0x14
template PseudoGlobalVariableRoot* PseudoGlobalVariable<wchar_t*>::GetNext();
// 0x007DAB78 slot 0x18
template void PseudoGlobalVariable<wchar_t*>::SetNext(PseudoGlobalVariableRoot* pNext);

// PseudoGlobalVariable<bool>
// 0x007DAD7C slot 0x00
// 0x007DAD08 (deleting dtor)
template PseudoGlobalVariable<bool>::~PseudoGlobalVariable();
// 0x007DAC9C slot 0x08
template void PseudoGlobalVariable<bool>::AllocateValues();
// 0x007DAC60 slot 0x0C
template void PseudoGlobalVariable<bool>::FreeValues();
// 0x007DAC50 slot 0x10
template void PseudoGlobalVariable<bool>::ResetValue(u32 context);
// 0x007DACF8 slot 0x14
template PseudoGlobalVariableRoot* PseudoGlobalVariable<bool>::GetNext();
// 0x007DAD00 slot 0x18
template void PseudoGlobalVariable<bool>::SetNext(PseudoGlobalVariableRoot* pNext);

// PseudoGlobalVariable<u8>
// 0x007DAF04 slot 0x00
// 0x007DAE90 (deleting dtor)
template PseudoGlobalVariable<u8>::~PseudoGlobalVariable();
// 0x007DAE24 slot 0x08
template void PseudoGlobalVariable<u8>::AllocateValues();
// 0x007DADE8 slot 0x0C
template void PseudoGlobalVariable<u8>::FreeValues();
// 0x007DADD8 slot 0x10
template void PseudoGlobalVariable<u8>::ResetValue(u32 context);
// 0x007DAE80 slot 0x14
template PseudoGlobalVariableRoot* PseudoGlobalVariable<u8>::GetNext();
// 0x007DAE88 slot 0x18
template void PseudoGlobalVariable<u8>::SetNext(PseudoGlobalVariableRoot* pNext);

// PseudoGlobalVariable<u32>
// 0x007DB058 slot 0x00
// 0x007DB008 (deleting dtor)
template PseudoGlobalVariable<u32>::~PseudoGlobalVariable();
// 0x007DAF94 slot 0x08
template void PseudoGlobalVariable<u32>::AllocateValues();
// 0x007DAF70 slot 0x0C
template void PseudoGlobalVariable<u32>::FreeValues();
// 0x007DAF60 slot 0x10
template void PseudoGlobalVariable<u32>::ResetValue(u32 context);
// 0x007DAFF8 slot 0x14
template PseudoGlobalVariableRoot* PseudoGlobalVariable<u32>::GetNext();
// 0x007DB000 slot 0x18
template void PseudoGlobalVariable<u32>::SetNext(PseudoGlobalVariableRoot* pNext);

// PseudoGlobalVariable<u16>
// 0x007DB1D8 slot 0x00
// 0x007DB164 (deleting dtor)
template PseudoGlobalVariable<u16>::~PseudoGlobalVariable();
// 0x007DB0F0 slot 0x08
template void PseudoGlobalVariable<u16>::AllocateValues();
// 0x007DB0B4 slot 0x0C
template void PseudoGlobalVariable<u16>::FreeValues();
// 0x007DB0A0 slot 0x10
template void PseudoGlobalVariable<u16>::ResetValue(u32 context);
// 0x007DB154 slot 0x14
template PseudoGlobalVariableRoot* PseudoGlobalVariable<u16>::GetNext();
// 0x007DB15C slot 0x18
template void PseudoGlobalVariable<u16>::SetNext(PseudoGlobalVariableRoot* pNext);
} // namespace nex
} // namespace nn
