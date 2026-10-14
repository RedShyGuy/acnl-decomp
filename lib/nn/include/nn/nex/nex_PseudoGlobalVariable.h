#pragma once

#include <new>
#include "decomp.h"
#include "nn/nex/nex_MemoryManager.h"
#include "nn/nex/nex_PseudoGlobalVariableList.h"
#include "nn/nex/nex_PseudoGlobalVariableRoot.h"
#include "nn/nex/nex_PseudoSingleton.h"

namespace nn {
namespace nex {
// Instantiations found in the binary:
//   nn::nex::PseudoGlobalVariable<SendMultiParameter>  typeinfo 0x008CE908  vtable 0x008FD8E4
//   nn::nex::PseudoGlobalVariable<bool>  typeinfo 0x008CE9F8  vtable 0x008FDBB4
//   nn::nex::PseudoGlobalVariable<nn::nex::AccountManagementServiceDDLDeclarations>  typeinfo 0x008CE9B0  vtable 0x008FDADC
//   nn::nex::PseudoGlobalVariable<nn::nex::AuthenticationFoundationDDLDeclarations>  typeinfo 0x008CE9BC  vtable 0x008FDB00
//   nn::nex::PseudoGlobalVariable<nn::nex::DDLDeclarations*>  typeinfo 0x008CE9D4  vtable 0x008FDB48
//   nn::nex::PseudoGlobalVariable<nn::nex::DataStoreDDLDeclarations>  typeinfo 0x008CE938  vtable 0x008FD974
//   nn::nex::PseudoGlobalVariable<nn::nex::DynamicRunTimeInterface*>  typeinfo 0x008CE9E0  vtable 0x008FDB6C
//   nn::nex::PseudoGlobalVariable<nn::nex::HealthServiceDDLDeclarations>  typeinfo 0x008CE95C  vtable 0x008FD9E0
//   nn::nex::PseudoGlobalVariable<nn::nex::MatchMakingServiceDDLDeclarations>  typeinfo 0x008CE974  vtable 0x008FDA28
//   nn::nex::PseudoGlobalVariable<nn::nex::MatchmakeExtensionDDLDeclarations>  typeinfo 0x008CE980  vtable 0x008FDA4C
//   nn::nex::PseudoGlobalVariable<nn::nex::MessagingServiceDDLDeclarations>  typeinfo 0x008CE968  vtable 0x008FDA04
//   nn::nex::PseudoGlobalVariable<nn::nex::NATTraversalDDLDeclarations>  typeinfo 0x008CE950  vtable 0x008FD9BC
//   nn::nex::PseudoGlobalVariable<nn::nex::NintendoAuthenticationDDLDeclarations>  typeinfo 0x008CE998  vtable 0x008FDA94
//   nn::nex::PseudoGlobalVariable<nn::nex::NintendoProtocolFoundationDDLDeclarations>  typeinfo 0x008CE9C8  vtable 0x008FDB24
//   nn::nex::PseudoGlobalVariable<nn::nex::OnlineCoreDDLDeclarations>  typeinfo 0x008CE944  vtable 0x008FD998
//   nn::nex::PseudoGlobalVariable<nn::nex::ProtocolFoundationDDLDeclarations>  typeinfo 0x008CE98C  vtable 0x008FDA70
//   nn::nex::PseudoGlobalVariable<nn::nex::STLExtDDLDeclarations>  typeinfo 0x008CE920  vtable 0x008FD92C
//   nn::nex::PseudoGlobalVariable<nn::nex::SecureConnectionServiceDDLDeclarations>  typeinfo 0x008CE9A4  vtable 0x008FDAB8
//   nn::nex::PseudoGlobalVariable<nn::nex::StreamSettings>  typeinfo 0x008CE914  vtable 0x008FD908
//   nn::nex::PseudoGlobalVariable<nn::nex::UtilityDDLDeclarations>  typeinfo 0x008CE92C  vtable 0x008FD950
//   nn::nex::PseudoGlobalVariable<unsigned char>  typeinfo 0x008CEA04  vtable 0x008FDBD8
//   nn::nex::PseudoGlobalVariable<unsigned int>  typeinfo 0x008CEA10  vtable 0x008FDBFC
//   nn::nex::PseudoGlobalVariable<unsigned short>  typeinfo 0x008CEA1C  vtable 0x008FDC20
//   nn::nex::PseudoGlobalVariable<wchar_t*>  typeinfo 0x008CE9EC  vtable 0x008FDB90
//
// A global with a value per context: the value of context 0 is m_Value, the other contexts have
// copies of the default value in m_pValues (when there is more than one context). The functions are
// inline with copies per type out of line (vtable); the names are ours.
template <typename T0>
class PseudoGlobalVariable : public ::nn::nex::PseudoGlobalVariableRoot
{
public:
    // (inline in the static initializers, with copies out of line per type)
    PseudoGlobalVariable() : m_pValues(nullptr) { Register(); }
    explicit PseudoGlobalVariable(const T0& value) : m_pValues(nullptr), m_DefaultValue(value) { Register(); }
    // (inline) the value of context 0 is the default value; the variable goes into the list
    void Register()
    {
        m_Value = m_DefaultValue;
        PseudoGlobalVariableList::s_Instance.AddVariable(this);
        if (s_ContextCount > 1) {
            AllocateValues();
        }
    }
    virtual ~PseudoGlobalVariable()
    {
        PseudoGlobalVariableList::s_Instance.RemoveVariable(this);
        FreeValues();
    }
    virtual void AllocateValues()
    {
        m_pValues = static_cast<T0*>(MemoryManager::Allocate(s_ContextCount * sizeof(T0)));
        for (u32 i = 0; i < s_ContextCount; i++) {
            ::new (static_cast<void*>(&m_pValues[i])) T0(m_DefaultValue);
        }
    }
    virtual void FreeValues()
    {
        if (m_pValues != nullptr) {
            for (u32 i = 0; i < s_ContextCount; i++) {
                m_pValues[i].~T0();
            }
            MemoryManager::Free(m_pValues);
            m_pValues = nullptr;
        }
    }
    virtual void ResetValue(u32 context) { m_pValues[context] = m_DefaultValue; }
    virtual PseudoGlobalVariableRoot* GetNext() { return m_pNext; }
    virtual void SetNext(PseudoGlobalVariableRoot* pNext) { m_pNext = pNext; }

    // (inline) the value of the current context
    T0& Get()
    {
        u32 context = PseudoSingleton::GetCurrentContext();
        if (context != 0) {
            return m_pValues[context];
        }
        return m_Value;
    }

    T0* m_pValues;     // 0x08
    T0 m_Value;        // (context 0)
    T0 m_DefaultValue;
};
} // namespace nex
} // namespace nn
