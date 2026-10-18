#include "nn/nex/nex_PseudoGlobalVariable.h"
#include "nn/nex/nex_BerkeleySocketDriver.h"
#include "nn/nex/nex_STLExtDDLDeclarations.h"
#include "nn/nex/nex_UtilityDDLDeclarations.h"
#include "nn/nex/nex_DataStoreDDLDeclarations.h"
#include "nn/nex/nex_OnlineCoreDDLDeclarations.h"
#include "nn/nex/nex_NATTraversalDDLDeclarations.h"
#include "nn/nex/nex_HealthServiceDDLDeclarations.h"
#include "nn/nex/nex_MessagingServiceDDLDeclarations.h"
#include "nn/nex/nex_MatchMakingServiceDDLDeclarations.h"
#include "nn/nex/nex_MatchmakeExtensionDDLDeclarations.h"
#include "nn/nex/nex_ProtocolFoundationDDLDeclarations.h"
#include "nn/nex/nex_NintendoAuthenticationDDLDeclarations.h"
#include "nn/nex/nex_SecureConnectionServiceDDLDeclarations.h"
#include "nn/nex/nex_AccountManagementServiceDDLDeclarations.h"
#include "nn/nex/nex_AuthenticationFoundationDDLDeclarations.h"
#include "nn/nex/nex_NintendoProtocolFoundationDDLDeclarations.h"

namespace nn {
namespace nex {
class DDLDeclarations;
class DynamicRunTimeInterface;

// the copies of the inline functions per type (in the vtables)
// PseudoGlobalVariable<SendMultiParameter>
// 0x007D76C0 slot 0x00
// 0x007D75E0 (deleting dtor)
template nn::nex::PseudoGlobalVariable<SendMultiParameter>::~PseudoGlobalVariable();
// 0x007D7488 slot 0x08
template void nn::nex::PseudoGlobalVariable<SendMultiParameter>::AllocateValues();
// 0x007D73F4 slot 0x0C
template void nn::nex::PseudoGlobalVariable<SendMultiParameter>::FreeValues();
// 0x007D73B8 slot 0x10
template void nn::nex::PseudoGlobalVariable<SendMultiParameter>::ResetValue(unsigned int context);
// 0x007D75D0 slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<SendMultiParameter>::GetNext();
// 0x007D75D8 slot 0x18
template void nn::nex::PseudoGlobalVariable<SendMultiParameter>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);

// PseudoGlobalVariable<nn::nex::DDLDeclarations*>
// 0x007DA938 slot 0x00
// 0x007DA8E8 (deleting dtor)
template nn::nex::PseudoGlobalVariable<nn::nex::DDLDeclarations*>::~PseudoGlobalVariable();
// 0x007DA874 slot 0x08
template void nn::nex::PseudoGlobalVariable<nn::nex::DDLDeclarations*>::AllocateValues();
// 0x007DA850 slot 0x0C
template void nn::nex::PseudoGlobalVariable<nn::nex::DDLDeclarations*>::FreeValues();
// 0x007DA840 slot 0x10
template void nn::nex::PseudoGlobalVariable<nn::nex::DDLDeclarations*>::ResetValue(unsigned int context);
// 0x007DA8D8 slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<nn::nex::DDLDeclarations*>::GetNext();
// 0x007DA8E0 slot 0x18
template void nn::nex::PseudoGlobalVariable<nn::nex::DDLDeclarations*>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);

// PseudoGlobalVariable<nn::nex::DynamicRunTimeInterface*>
// 0x007DAA78 slot 0x00
// 0x007DAA28 (deleting dtor)
template nn::nex::PseudoGlobalVariable<nn::nex::DynamicRunTimeInterface*>::~PseudoGlobalVariable();
// 0x007DA9B4 slot 0x08
template void nn::nex::PseudoGlobalVariable<nn::nex::DynamicRunTimeInterface*>::AllocateValues();
// 0x007DA990 slot 0x0C
template void nn::nex::PseudoGlobalVariable<nn::nex::DynamicRunTimeInterface*>::FreeValues();
// 0x007DA980 slot 0x10
template void nn::nex::PseudoGlobalVariable<nn::nex::DynamicRunTimeInterface*>::ResetValue(unsigned int context);
// 0x007DAA18 slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<nn::nex::DynamicRunTimeInterface*>::GetNext();
// 0x007DAA20 slot 0x18
template void nn::nex::PseudoGlobalVariable<nn::nex::DynamicRunTimeInterface*>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);

// PseudoGlobalVariable<wchar_t*>
// 0x007DABF4 slot 0x00
// 0x007DAB80 (deleting dtor)
template nn::nex::PseudoGlobalVariable<wchar_t*>::~PseudoGlobalVariable();
// 0x007DAB0C slot 0x08
template void nn::nex::PseudoGlobalVariable<wchar_t*>::AllocateValues();
// 0x007DAAD0 slot 0x0C
template void nn::nex::PseudoGlobalVariable<wchar_t*>::FreeValues();
// 0x007DAAC0 slot 0x10
template void nn::nex::PseudoGlobalVariable<wchar_t*>::ResetValue(unsigned int context);
// 0x007DAB70 slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<wchar_t*>::GetNext();
// 0x007DAB78 slot 0x18
template void nn::nex::PseudoGlobalVariable<wchar_t*>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);

// PseudoGlobalVariable<bool>
// 0x007DAD7C slot 0x00
// 0x007DAD08 (deleting dtor)
template nn::nex::PseudoGlobalVariable<bool>::~PseudoGlobalVariable();
// 0x007DAC9C slot 0x08
template void nn::nex::PseudoGlobalVariable<bool>::AllocateValues();
// 0x007DAC60 slot 0x0C
template void nn::nex::PseudoGlobalVariable<bool>::FreeValues();
// 0x007DAC50 slot 0x10
template void nn::nex::PseudoGlobalVariable<bool>::ResetValue(unsigned int context);
// 0x007DACF8 slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<bool>::GetNext();
// 0x007DAD00 slot 0x18
template void nn::nex::PseudoGlobalVariable<bool>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);

// PseudoGlobalVariable<unsigned char>
// 0x007DAF04 slot 0x00
// 0x007DAE90 (deleting dtor)
template nn::nex::PseudoGlobalVariable<unsigned char>::~PseudoGlobalVariable();
// 0x007DAE24 slot 0x08
template void nn::nex::PseudoGlobalVariable<unsigned char>::AllocateValues();
// 0x007DADE8 slot 0x0C
template void nn::nex::PseudoGlobalVariable<unsigned char>::FreeValues();
// 0x007DADD8 slot 0x10
template void nn::nex::PseudoGlobalVariable<unsigned char>::ResetValue(unsigned int context);
// 0x007DAE80 slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<unsigned char>::GetNext();
// 0x007DAE88 slot 0x18
template void nn::nex::PseudoGlobalVariable<unsigned char>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);

// PseudoGlobalVariable<unsigned int>
// 0x007DB058 slot 0x00
// 0x007DB008 (deleting dtor)
template nn::nex::PseudoGlobalVariable<unsigned int>::~PseudoGlobalVariable();
// 0x007DAF94 slot 0x08
template void nn::nex::PseudoGlobalVariable<unsigned int>::AllocateValues();
// 0x007DAF70 slot 0x0C
template void nn::nex::PseudoGlobalVariable<unsigned int>::FreeValues();
// 0x007DAF60 slot 0x10
template void nn::nex::PseudoGlobalVariable<unsigned int>::ResetValue(unsigned int context);
// 0x007DAFF8 slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<unsigned int>::GetNext();
// 0x007DB000 slot 0x18
template void nn::nex::PseudoGlobalVariable<unsigned int>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);

// PseudoGlobalVariable<unsigned short>
// 0x007DB1D8 slot 0x00
// 0x007DB164 (deleting dtor)
template nn::nex::PseudoGlobalVariable<unsigned short>::~PseudoGlobalVariable();
// 0x007DB0F0 slot 0x08
template void nn::nex::PseudoGlobalVariable<unsigned short>::AllocateValues();
// 0x007DB0B4 slot 0x0C
template void nn::nex::PseudoGlobalVariable<unsigned short>::FreeValues();
// 0x007DB0A0 slot 0x10
template void nn::nex::PseudoGlobalVariable<unsigned short>::ResetValue(unsigned int context);
// 0x007DB154 slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<unsigned short>::GetNext();
// 0x007DB15C slot 0x18
template void nn::nex::PseudoGlobalVariable<unsigned short>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);

// nn::nex::PseudoGlobalVariable<nn::nex::STLExtDDLDeclarations>
// 0x007D8128 slot 0x00
// 0x007D8094 (deleting dtor)
template nn::nex::PseudoGlobalVariable<nn::nex::STLExtDDLDeclarations>::~PseudoGlobalVariable();
// 0x007D7FB4 slot 0x08
template void nn::nex::PseudoGlobalVariable<nn::nex::STLExtDDLDeclarations>::AllocateValues();
// 0x007D7F4C slot 0x0C
template void nn::nex::PseudoGlobalVariable<nn::nex::STLExtDDLDeclarations>::FreeValues();
// 0x007D7F1C slot 0x10
template void nn::nex::PseudoGlobalVariable<nn::nex::STLExtDDLDeclarations>::ResetValue(unsigned int context);
// 0x007D8084 slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<nn::nex::STLExtDDLDeclarations>::GetNext();
// 0x007D808C slot 0x18
template void nn::nex::PseudoGlobalVariable<nn::nex::STLExtDDLDeclarations>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);

// nn::nex::PseudoGlobalVariable<nn::nex::UtilityDDLDeclarations>
// 0x007D83D8 slot 0x00
// 0x007D832C (deleting dtor)
template nn::nex::PseudoGlobalVariable<nn::nex::UtilityDDLDeclarations>::~PseudoGlobalVariable();
// 0x007D824C slot 0x08
template void nn::nex::PseudoGlobalVariable<nn::nex::UtilityDDLDeclarations>::AllocateValues();
// 0x007D81E4 slot 0x0C
template void nn::nex::PseudoGlobalVariable<nn::nex::UtilityDDLDeclarations>::FreeValues();
// 0x007D81B4 slot 0x10
template void nn::nex::PseudoGlobalVariable<nn::nex::UtilityDDLDeclarations>::ResetValue(unsigned int context);
// 0x007D831C slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<nn::nex::UtilityDDLDeclarations>::GetNext();
// 0x007D8324 slot 0x18
template void nn::nex::PseudoGlobalVariable<nn::nex::UtilityDDLDeclarations>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);

// nn::nex::PseudoGlobalVariable<nn::nex::DataStoreDDLDeclarations>
// 0x007D869C slot 0x00
// 0x007D85F0 (deleting dtor)
template nn::nex::PseudoGlobalVariable<nn::nex::DataStoreDDLDeclarations>::~PseudoGlobalVariable();
// 0x007D8510 slot 0x08
template void nn::nex::PseudoGlobalVariable<nn::nex::DataStoreDDLDeclarations>::AllocateValues();
// 0x007D84A8 slot 0x0C
template void nn::nex::PseudoGlobalVariable<nn::nex::DataStoreDDLDeclarations>::FreeValues();
// 0x007D8478 slot 0x10
template void nn::nex::PseudoGlobalVariable<nn::nex::DataStoreDDLDeclarations>::ResetValue(unsigned int context);
// 0x007D85E0 slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<nn::nex::DataStoreDDLDeclarations>::GetNext();
// 0x007D85E8 slot 0x18
template void nn::nex::PseudoGlobalVariable<nn::nex::DataStoreDDLDeclarations>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);

// nn::nex::PseudoGlobalVariable<nn::nex::OnlineCoreDDLDeclarations>
// 0x007D8948 slot 0x00
// 0x007D88B4 (deleting dtor)
template nn::nex::PseudoGlobalVariable<nn::nex::OnlineCoreDDLDeclarations>::~PseudoGlobalVariable();
// 0x007D87D4 slot 0x08
template void nn::nex::PseudoGlobalVariable<nn::nex::OnlineCoreDDLDeclarations>::AllocateValues();
// 0x007D876C slot 0x0C
template void nn::nex::PseudoGlobalVariable<nn::nex::OnlineCoreDDLDeclarations>::FreeValues();
// 0x007D873C slot 0x10
template void nn::nex::PseudoGlobalVariable<nn::nex::OnlineCoreDDLDeclarations>::ResetValue(unsigned int context);
// 0x007D88A4 slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<nn::nex::OnlineCoreDDLDeclarations>::GetNext();
// 0x007D88AC slot 0x18
template void nn::nex::PseudoGlobalVariable<nn::nex::OnlineCoreDDLDeclarations>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);

// nn::nex::PseudoGlobalVariable<nn::nex::NATTraversalDDLDeclarations>
// 0x007D8BF8 slot 0x00
// 0x007D8B4C (deleting dtor)
template nn::nex::PseudoGlobalVariable<nn::nex::NATTraversalDDLDeclarations>::~PseudoGlobalVariable();
// 0x007D8A6C slot 0x08
template void nn::nex::PseudoGlobalVariable<nn::nex::NATTraversalDDLDeclarations>::AllocateValues();
// 0x007D8A04 slot 0x0C
template void nn::nex::PseudoGlobalVariable<nn::nex::NATTraversalDDLDeclarations>::FreeValues();
// 0x007D89D4 slot 0x10
template void nn::nex::PseudoGlobalVariable<nn::nex::NATTraversalDDLDeclarations>::ResetValue(unsigned int context);
// 0x007D8B3C slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<nn::nex::NATTraversalDDLDeclarations>::GetNext();
// 0x007D8B44 slot 0x18
template void nn::nex::PseudoGlobalVariable<nn::nex::NATTraversalDDLDeclarations>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);

// nn::nex::PseudoGlobalVariable<nn::nex::HealthServiceDDLDeclarations>
// 0x007D8EBC slot 0x00
// 0x007D8E10 (deleting dtor)
template nn::nex::PseudoGlobalVariable<nn::nex::HealthServiceDDLDeclarations>::~PseudoGlobalVariable();
// 0x007D8D30 slot 0x08
template void nn::nex::PseudoGlobalVariable<nn::nex::HealthServiceDDLDeclarations>::AllocateValues();
// 0x007D8CC8 slot 0x0C
template void nn::nex::PseudoGlobalVariable<nn::nex::HealthServiceDDLDeclarations>::FreeValues();
// 0x007D8C98 slot 0x10
template void nn::nex::PseudoGlobalVariable<nn::nex::HealthServiceDDLDeclarations>::ResetValue(unsigned int context);
// 0x007D8E00 slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<nn::nex::HealthServiceDDLDeclarations>::GetNext();
// 0x007D8E08 slot 0x18
template void nn::nex::PseudoGlobalVariable<nn::nex::HealthServiceDDLDeclarations>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);

// nn::nex::PseudoGlobalVariable<nn::nex::MessagingServiceDDLDeclarations>
// 0x007D9180 slot 0x00
// 0x007D90D4 (deleting dtor)
template nn::nex::PseudoGlobalVariable<nn::nex::MessagingServiceDDLDeclarations>::~PseudoGlobalVariable();
// 0x007D8FF4 slot 0x08
template void nn::nex::PseudoGlobalVariable<nn::nex::MessagingServiceDDLDeclarations>::AllocateValues();
// 0x007D8F8C slot 0x0C
template void nn::nex::PseudoGlobalVariable<nn::nex::MessagingServiceDDLDeclarations>::FreeValues();
// 0x007D8F5C slot 0x10
template void nn::nex::PseudoGlobalVariable<nn::nex::MessagingServiceDDLDeclarations>::ResetValue(unsigned int context);
// 0x007D90C4 slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<nn::nex::MessagingServiceDDLDeclarations>::GetNext();
// 0x007D90CC slot 0x18
template void nn::nex::PseudoGlobalVariable<nn::nex::MessagingServiceDDLDeclarations>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);

// nn::nex::PseudoGlobalVariable<nn::nex::MatchMakingServiceDDLDeclarations>
// 0x007D9444 slot 0x00
// 0x007D9398 (deleting dtor)
template nn::nex::PseudoGlobalVariable<nn::nex::MatchMakingServiceDDLDeclarations>::~PseudoGlobalVariable();
// 0x007D92B8 slot 0x08
template void nn::nex::PseudoGlobalVariable<nn::nex::MatchMakingServiceDDLDeclarations>::AllocateValues();
// 0x007D9250 slot 0x0C
template void nn::nex::PseudoGlobalVariable<nn::nex::MatchMakingServiceDDLDeclarations>::FreeValues();
// 0x007D9220 slot 0x10
template void nn::nex::PseudoGlobalVariable<nn::nex::MatchMakingServiceDDLDeclarations>::ResetValue(unsigned int context);
// 0x007D9388 slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<nn::nex::MatchMakingServiceDDLDeclarations>::GetNext();
// 0x007D9390 slot 0x18
template void nn::nex::PseudoGlobalVariable<nn::nex::MatchMakingServiceDDLDeclarations>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);

// nn::nex::PseudoGlobalVariable<nn::nex::MatchmakeExtensionDDLDeclarations>
// 0x007D9708 slot 0x00
// 0x007D965C (deleting dtor)
template nn::nex::PseudoGlobalVariable<nn::nex::MatchmakeExtensionDDLDeclarations>::~PseudoGlobalVariable();
// 0x007D957C slot 0x08
template void nn::nex::PseudoGlobalVariable<nn::nex::MatchmakeExtensionDDLDeclarations>::AllocateValues();
// 0x007D9514 slot 0x0C
template void nn::nex::PseudoGlobalVariable<nn::nex::MatchmakeExtensionDDLDeclarations>::FreeValues();
// 0x007D94E4 slot 0x10
template void nn::nex::PseudoGlobalVariable<nn::nex::MatchmakeExtensionDDLDeclarations>::ResetValue(unsigned int context);
// 0x007D964C slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<nn::nex::MatchmakeExtensionDDLDeclarations>::GetNext();
// 0x007D9654 slot 0x18
template void nn::nex::PseudoGlobalVariable<nn::nex::MatchmakeExtensionDDLDeclarations>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);

// nn::nex::PseudoGlobalVariable<nn::nex::ProtocolFoundationDDLDeclarations>
// 0x007D99CC slot 0x00
// 0x007D9920 (deleting dtor)
template nn::nex::PseudoGlobalVariable<nn::nex::ProtocolFoundationDDLDeclarations>::~PseudoGlobalVariable();
// 0x007D9840 slot 0x08
template void nn::nex::PseudoGlobalVariable<nn::nex::ProtocolFoundationDDLDeclarations>::AllocateValues();
// 0x007D97D8 slot 0x0C
template void nn::nex::PseudoGlobalVariable<nn::nex::ProtocolFoundationDDLDeclarations>::FreeValues();
// 0x007D97A8 slot 0x10
template void nn::nex::PseudoGlobalVariable<nn::nex::ProtocolFoundationDDLDeclarations>::ResetValue(unsigned int context);
// 0x007D9910 slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<nn::nex::ProtocolFoundationDDLDeclarations>::GetNext();
// 0x007D9918 slot 0x18
template void nn::nex::PseudoGlobalVariable<nn::nex::ProtocolFoundationDDLDeclarations>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);

// nn::nex::PseudoGlobalVariable<nn::nex::NintendoAuthenticationDDLDeclarations>
// 0x007D9C90 slot 0x00
// 0x007D9BE4 (deleting dtor)
template nn::nex::PseudoGlobalVariable<nn::nex::NintendoAuthenticationDDLDeclarations>::~PseudoGlobalVariable();
// 0x007D9B04 slot 0x08
template void nn::nex::PseudoGlobalVariable<nn::nex::NintendoAuthenticationDDLDeclarations>::AllocateValues();
// 0x007D9A9C slot 0x0C
template void nn::nex::PseudoGlobalVariable<nn::nex::NintendoAuthenticationDDLDeclarations>::FreeValues();
// 0x007D9A6C slot 0x10
template void nn::nex::PseudoGlobalVariable<nn::nex::NintendoAuthenticationDDLDeclarations>::ResetValue(unsigned int context);
// 0x007D9BD4 slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<nn::nex::NintendoAuthenticationDDLDeclarations>::GetNext();
// 0x007D9BDC slot 0x18
template void nn::nex::PseudoGlobalVariable<nn::nex::NintendoAuthenticationDDLDeclarations>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);

// nn::nex::PseudoGlobalVariable<nn::nex::SecureConnectionServiceDDLDeclarations>
// 0x007D9F54 slot 0x00
// 0x007D9EA8 (deleting dtor)
template nn::nex::PseudoGlobalVariable<nn::nex::SecureConnectionServiceDDLDeclarations>::~PseudoGlobalVariable();
// 0x007D9DC8 slot 0x08
template void nn::nex::PseudoGlobalVariable<nn::nex::SecureConnectionServiceDDLDeclarations>::AllocateValues();
// 0x007D9D60 slot 0x0C
template void nn::nex::PseudoGlobalVariable<nn::nex::SecureConnectionServiceDDLDeclarations>::FreeValues();
// 0x007D9D30 slot 0x10
template void nn::nex::PseudoGlobalVariable<nn::nex::SecureConnectionServiceDDLDeclarations>::ResetValue(unsigned int context);
// 0x007D9E98 slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<nn::nex::SecureConnectionServiceDDLDeclarations>::GetNext();
// 0x007D9EA0 slot 0x18
template void nn::nex::PseudoGlobalVariable<nn::nex::SecureConnectionServiceDDLDeclarations>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);

// nn::nex::PseudoGlobalVariable<nn::nex::AccountManagementServiceDDLDeclarations>
// 0x007DA218 slot 0x00
// 0x007DA16C (deleting dtor)
template nn::nex::PseudoGlobalVariable<nn::nex::AccountManagementServiceDDLDeclarations>::~PseudoGlobalVariable();
// 0x007DA08C slot 0x08
template void nn::nex::PseudoGlobalVariable<nn::nex::AccountManagementServiceDDLDeclarations>::AllocateValues();
// 0x007DA024 slot 0x0C
template void nn::nex::PseudoGlobalVariable<nn::nex::AccountManagementServiceDDLDeclarations>::FreeValues();
// 0x007D9FF4 slot 0x10
template void nn::nex::PseudoGlobalVariable<nn::nex::AccountManagementServiceDDLDeclarations>::ResetValue(unsigned int context);
// 0x007DA15C slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<nn::nex::AccountManagementServiceDDLDeclarations>::GetNext();
// 0x007DA164 slot 0x18
template void nn::nex::PseudoGlobalVariable<nn::nex::AccountManagementServiceDDLDeclarations>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);

// nn::nex::PseudoGlobalVariable<nn::nex::AuthenticationFoundationDDLDeclarations>
// 0x007DA4DC slot 0x00
// 0x007DA430 (deleting dtor)
template nn::nex::PseudoGlobalVariable<nn::nex::AuthenticationFoundationDDLDeclarations>::~PseudoGlobalVariable();
// 0x007DA350 slot 0x08
template void nn::nex::PseudoGlobalVariable<nn::nex::AuthenticationFoundationDDLDeclarations>::AllocateValues();
// 0x007DA2E8 slot 0x0C
template void nn::nex::PseudoGlobalVariable<nn::nex::AuthenticationFoundationDDLDeclarations>::FreeValues();
// 0x007DA2B8 slot 0x10
template void nn::nex::PseudoGlobalVariable<nn::nex::AuthenticationFoundationDDLDeclarations>::ResetValue(unsigned int context);
// 0x007DA420 slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<nn::nex::AuthenticationFoundationDDLDeclarations>::GetNext();
// 0x007DA428 slot 0x18
template void nn::nex::PseudoGlobalVariable<nn::nex::AuthenticationFoundationDDLDeclarations>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);

// nn::nex::PseudoGlobalVariable<nn::nex::NintendoProtocolFoundationDDLDeclarations>
// 0x007DA7A0 slot 0x00
// 0x007DA6F4 (deleting dtor)
template nn::nex::PseudoGlobalVariable<nn::nex::NintendoProtocolFoundationDDLDeclarations>::~PseudoGlobalVariable();
// 0x007DA614 slot 0x08
template void nn::nex::PseudoGlobalVariable<nn::nex::NintendoProtocolFoundationDDLDeclarations>::AllocateValues();
// 0x007DA5AC slot 0x0C
template void nn::nex::PseudoGlobalVariable<nn::nex::NintendoProtocolFoundationDDLDeclarations>::FreeValues();
// 0x007DA57C slot 0x10
template void nn::nex::PseudoGlobalVariable<nn::nex::NintendoProtocolFoundationDDLDeclarations>::ResetValue(unsigned int context);
// 0x007DA6E4 slot 0x14
template nn::nex::PseudoGlobalVariableRoot* nn::nex::PseudoGlobalVariable<nn::nex::NintendoProtocolFoundationDDLDeclarations>::GetNext();
// 0x007DA6EC slot 0x18
template void nn::nex::PseudoGlobalVariable<nn::nex::NintendoProtocolFoundationDDLDeclarations>::SetNext(nn::nex::PseudoGlobalVariableRoot* pNext);
} // namespace nex
} // namespace nn
