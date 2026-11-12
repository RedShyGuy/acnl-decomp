#include "Sv/dSvHolder.h"

#include "Other/dHeapInit.h"
#include "Other/dSaveProcess.h"
#include "Sv/dSvGlobals.h"
#include "Sv/dSvSaveIo.h"
#include "sead/seadExpHeap.h"
#include "sead/seadNew.h"

// The holder of exhibition.dat (the Happy Home Showcase; game code, all names are ours).

// 0x00953638
u8 g_SvIsSavingExhibition = 0;
// 0x0095363C
SvExhibition* g_pSvExhibition = NULL;
// 0x009B4AC0
SvExhibitionHolder g_SvExhibitionHolder;
// (constructs g_SvExhibitionHolder and registers its destructor)
// 0x0079C740 (static initializer)

namespace {
const char* const EXHIBITION_FILE_NAME = "/exhibition.dat";
} // namespace

// 0x002CDA88 (name is ours)
void SvExhibitionHolder::SetLoaded(bool isNew)
{
    if (isNew) {
        m_pData->SetUpNew();
    }
    g_pSvExhibition = m_pData;
    m_IsLoaded = true;
}

// 0x002CDAB8 (name is ours)
SvExhibition* SvExhibitionHolder::GetData()
{
    return g_SvIsSavingExhibition ? NULL : g_pSvExhibition;
}

// 0x002CDAD4 (name is ours)
void SvExhibitionHolder::SetNoCommit()
{
    m_Request.m_Flags = SvStepInner::FLAG_NO_COMMIT | (7 << 11);
}

// 0x002CDAEC (name is ours)
s32 SvExhibitionHolder::Read()
{
    return SvSaveIo::ReadFile(EXHIBITION_FILE_NAME, 0, m_pData, sizeof(SvExhibition));
}

// 0x002CDB08 (name is ours)
s32 SvExhibitionHolder::WriteFile()
{
    return g_SvExhibitionHolder.Write();
}

// 0x002CDB14 (name is ours)
s32 SvExhibitionHolder::Write()
{
    return SvSaveIo::WriteFile(EXHIBITION_FILE_NAME, 0, m_pData, sizeof(SvExhibition), true);
}

// 0x002CDB40 (name is ours)
bool SvExhibitionHolder::Allocate()
{
    if (m_pData != NULL) {
        return false;
    }
    if (HeapInit::s_pUnknown953CB4 != NULL) {
        m_pData = new (HeapInit::s_pUnknown953CB4, 4) SvExhibition();
    } else {
        m_pData = new (HeapInit::s_pStageHeap, 4) SvExhibition();
    }
    return true;
}

// 0x002CDBB0 (name is ours)
void SvExhibitionHolder::Free()
{
    if (m_pData != NULL) {
        delete m_pData;
        m_pData = NULL;
        g_pSvExhibition = NULL;
        m_IsLoaded = false;
    }
    u8* object = Func537B10();
    if (object != NULL) {
        object[0xA1] = 0;
    }
}

// 0x002CDBF8 (name is ours)
void SvExhibitionHolder::EndSave()
{
    g_SvIsSavingExhibition = 0;
}

// 0x002CDC0C (name is ours)
void SvExhibitionHolder::PrepareSave(SvStepInner::StepCallback afterLoad, SvStepInner::StepCallback beforeCommit,
                                     SvStepInner::Callback onEnd)
{
    m_Request.Reset();
    m_Request.m_Flags = 7 << 11;
    m_Request.m_AfterLoad = afterLoad;
    m_Request.m_BeforeCommit = beforeCommit;
    m_Request.m_OnEnd = onEnd;
}

// 0x002CDC44 (name is ours)
bool SvExhibitionHolder::UpdateLoad(s32* result)
{
    if (m_IsLoaded) {
        *result = SvStepInner::RESULT_OK;
        return true;
    }
    if (Allocate()) {
        m_Request.Reset();
        m_Request.m_Flags = SvStepInner::FLAG_CHECK | SaveProcess::LOAD_FILE_EXHIBITION;
        m_Request.Start();
    }
    if (!m_Request.m_IsDone) {
        return false;
    }
    *result = m_Request.m_Result;
    // no file: a new one
    SetLoaded(*result != SvStepInner::RESULT_OK);
    if (*result == 8) {
        *result = SvStepInner::RESULT_OK;
        g_SvErrorCode = -1;
    }
    m_Detail = m_Request.m_Detail;
    return true;
}

// 0x002CDD48 (name is ours)
bool SvExhibitionHolder::UpdateSave(s32* result)
{
    if (m_Request.m_IsDone) {
        *result = m_Request.m_Result;
        m_Detail = m_Request.m_Detail;
        return true;
    }
    if (m_Request.m_Result == 0) {
        m_Request.Start();
    }
    return false;
}

// 0x0071E4B0 (name is ours)
bool SvExhibitionHolder::IsValid()
{
    return m_pData->IsChecksumValid(true);
}

// 0x002CDD84 (name is ours)
void SvExhibitionHolder::BeginSave()
{
    g_SvIsSavingExhibition = 1;
}

// 0x002CDD98 (name is ours)
SvExhibitionHolder::~SvExhibitionHolder()
{
    // (only the members are destroyed)
}
