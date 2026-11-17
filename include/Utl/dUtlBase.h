#pragma once

#include "decomp.h"
#include "oml/framework/dProcess.h"

// A base class of the game over T (class name after the RTTI; the rest is ours): Initialize and Finalize run three
// steps each that the derived class implements, and keep their progress in a byte behind T (in its tail padding).
// Instantiations found in the binary (each with its own copy of Initialize/Finalize, see dUtlBase.cpp):
//   UtlBase<AcStrc>  typeinfo 0x008CD3F8
//   UtlBase<Base>  typeinfo 0x008CD3EC
//   UtlBase<BsScene>  typeinfo 0x008CD404
//   UtlBase<DemoActor>  typeinfo 0x008CD410  vtable 0x008F9200
template <typename T>
class UtlBase : public T
{
public:
    // the bits of m_UtlFlags
    enum
    {
        UTL_FLAG_INITIALIZE_STARTED = 1 << 0, // StartInitialize succeeded
        UTL_FLAG_INITIALIZED = 1 << 1,        // EndInitialize succeeded
        UTL_FLAG_FINALIZE_STARTED = 1 << 2,   // StartFinalize ran
        UTL_FLAG_FINALIZE_UPDATED = 1 << 3,   // UpdateFinalize and EndFinalize ran
        UTL_FLAG_FINALIZE_FAILED = 1 << 4,    // a step of Finalize failed (Finalize gives RESULT_FAIL)
        UTL_FLAG_FINALIZING = 1 << 5,
    };

    UtlBase() : m_UtlFlags(0) {}
    virtual ~UtlBase() {}

    // StartInitialize once (false: RESULT_FAIL), then UpdateInitialize until it gives RESULT_SUCCESS, then
    // EndInitialize (false: RESULT_FAIL)
    oml::framework::Result Initialize() override;
    // StartFinalize once, UpdateFinalize until it gives no RESULT_RETRY, then EndFinalize; a failed step makes the
    // result RESULT_FAIL
    oml::framework::Result Finalize() override;

    virtual bool StartInitialize() = 0;
    virtual oml::framework::Result UpdateInitialize() = 0;
    virtual bool EndInitialize() = 0;
    virtual bool StartFinalize() = 0;
    virtual oml::framework::Result UpdateFinalize() = 0;
    virtual bool EndFinalize() = 0;

    u8 m_UtlFlags;
};

template <typename T>
oml::framework::Result UtlBase<T>::Initialize()
{
    oml::framework::Result result = T::Initialize();
    if (result != oml::framework::RESULT_SUCCESS) {
        return result;
    }
    if (!(m_UtlFlags & UTL_FLAG_INITIALIZE_STARTED)) {
        if (!StartInitialize()) {
            return oml::framework::RESULT_FAIL;
        }
        m_UtlFlags |= UTL_FLAG_INITIALIZE_STARTED;
    }
    result = UpdateInitialize();
    if (result != oml::framework::RESULT_SUCCESS) {
        return result;
    }
    if (!EndInitialize()) {
        return oml::framework::RESULT_FAIL;
    }
    m_UtlFlags |= UTL_FLAG_INITIALIZED;
    return result;
}

template <typename T>
oml::framework::Result UtlBase<T>::Finalize()
{
    m_UtlFlags |= UTL_FLAG_FINALIZING;
    if (!(m_UtlFlags & UTL_FLAG_FINALIZE_STARTED)) {
        if (!StartFinalize()) {
            m_UtlFlags |= UTL_FLAG_FINALIZE_FAILED;
        }
        m_UtlFlags |= UTL_FLAG_FINALIZE_STARTED;
    }
    if (!(m_UtlFlags & UTL_FLAG_FINALIZE_UPDATED)) {
        const oml::framework::Result update = UpdateFinalize();
        if (update == oml::framework::RESULT_RETRY) {
            return update;
        }
        if (update == oml::framework::RESULT_FAIL) {
            m_UtlFlags |= UTL_FLAG_FINALIZE_FAILED;
        }
        if (!EndFinalize()) {
            m_UtlFlags |= UTL_FLAG_FINALIZE_FAILED;
        }
        m_UtlFlags |= UTL_FLAG_FINALIZE_UPDATED;
    }
    oml::framework::Result result = T::Finalize();
    if (result != oml::framework::RESULT_SUCCESS) {
        return result;
    }
    if (m_UtlFlags & UTL_FLAG_FINALIZE_FAILED) {
        return oml::framework::RESULT_FAIL;
    }
    return result;
}
