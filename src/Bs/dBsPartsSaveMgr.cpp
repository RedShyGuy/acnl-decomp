#include "Bs/dBsPartsSaveMgr.h"

#include "Other/dHeapInit.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvBestFriendList.h"
#include "Sv/dSvGlobals.h"
#include "Sv/dSvHolder.h"
#include "Sv/dSvSaveIo.h"
#include "sead/seadExpHeap.h"
#include "sead/seadNew.h"

// the statics of mvplayer_pls.dat (see dSvGlobals.cpp for its save number, data flag and data)
// 0x00983004
s32 g_SvMovePlayerPlsDetail = -1;
// 0x00983025
bool g_SvIsSavingMovePlayerPls = false;
// (guard 0x00983008; built in the static initializers of the files that use it)
// 0x00AF8818
SvStepInner g_SvMovePlayerPlsRequest(SvStepInner::FLAG_KEEP_ERROR);

// 0x00983028
template <> u32 BsPartsSaveMgr<SvBestFriendList>::s_SaveNo = 4;
// 0x0098302C
template <> s32 BsPartsSaveMgr<SvBestFriendList>::s_Detail = -1;
// 0x00983034
template <> bool BsPartsSaveMgr<SvBestFriendList>::s_HasData = false;
// 0x00983038
template <> SvBestFriendList* BsPartsSaveMgr<SvBestFriendList>::s_pData = NULL;
// 0x0098303C
template <> const char* BsPartsSaveMgr<SvBestFriendList>::s_FileNames[4] = {"/friend1.dat", "/friend2.dat", "/friend3.dat", "/friend4.dat"};
// 0x0098304C
template <> bool BsPartsSaveMgr<SvBestFriendList>::s_IsSaving = false;
// 0x00983050
template <> u32 BsPartsSaveMgr<svholder::Mail>::s_SaveNo = 4;
// 0x00983054
template <> s32 BsPartsSaveMgr<svholder::Mail>::s_Detail = -1;
// 0x0098305C
template <> bool BsPartsSaveMgr<svholder::Mail>::s_HasData = false;
// 0x00983060
template <> svholder::Mail* BsPartsSaveMgr<svholder::Mail>::s_pData = NULL;
// 0x00983064
template <> const char* BsPartsSaveMgr<svholder::Mail>::s_FileNames[4] = {"/mail1.dat", "/mail2.dat", "/mail3.dat", "/mail4.dat"};
// 0x00983075
template <> bool BsPartsSaveMgr<svholder::Mail>::s_IsSaving = false;
// 0x00983078
template <> u32 BsPartsSaveMgr<svholder::MyDesign>::s_SaveNo = 4;
// 0x0098307C
template <> s32 BsPartsSaveMgr<svholder::MyDesign>::s_Detail = -1;
// 0x00983084
template <> bool BsPartsSaveMgr<svholder::MyDesign>::s_HasData = false;
// 0x00983088
template <> svholder::MyDesign* BsPartsSaveMgr<svholder::MyDesign>::s_pData = NULL;
// 0x0098308C
template <> const char* BsPartsSaveMgr<svholder::MyDesign>::s_FileNames[4] = {"/mydsgn1.dat", "/mydsgn2.dat", "/mydsgn3.dat", "/mydsgn4.dat"};
// 0x0098309D
template <> bool BsPartsSaveMgr<svholder::MyDesign>::s_IsSaving = false;

// (the constants are defined in dBsPartsSaveMgrConstants.cpp, so that they are read from memory as in the original)
template <> const s32 BsPartsSaveMgr<SvBestFriendList>::ALIGNMENT;
template <> const u32 BsPartsSaveMgr<SvBestFriendList>::LOAD_FLAGS;
template <> const u32 BsPartsSaveMgr<SvBestFriendList>::SAVE_FLAGS;
template <> const s32 BsPartsSaveMgr<svholder::Mail>::ALIGNMENT;
template <> const u32 BsPartsSaveMgr<svholder::Mail>::LOAD_FLAGS;
template <> const u32 BsPartsSaveMgr<svholder::Mail>::SAVE_FLAGS;
template <> const s32 BsPartsSaveMgr<svholder::MyDesign>::ALIGNMENT;
template <> const u32 BsPartsSaveMgr<svholder::MyDesign>::LOAD_FLAGS;
template <> const u32 BsPartsSaveMgr<svholder::MyDesign>::SAVE_FLAGS;

template <typename T0>
oml::framework::Result BsPartsSaveMgr<T0>::Initialize()
{
    if (m_InstanceNum == 0) {
        if (s_Request.m_Result != SvStepInner::RESULT_RUNNING && !s_HasData) {
            s_Request.Reset();
            s_Request.m_Flags = LOAD_FLAGS;
            if (s_pData == NULL) {
                s_pData = new (HeapInit::s_pStageHeap, ALIGNMENT) T0();
                s_SaveNo = SvGetPlayerSaveNo();
                if (!SvIsValidPlayerSaveNo(static_cast<PlayerSaveNumber>(s_SaveNo))) {
                    s_SaveNo = 0;
                }
            }
        }
        m_InstanceNum++;
    }
    if (s_HasData) {
        return oml::framework::RESULT_SUCCESS;
    }
    const s32 result = s_Request.m_Result;
    if (result == 0) {
        s_Request.Start();
        return oml::framework::RESULT_RETRY;
    }
    if (!s_Request.m_IsDone) {
        return oml::framework::RESULT_RETRY;
    }
    // no file: a new one
    if (result != SvStepInner::RESULT_OK) {
        s_pData->SetUpNew();
        s_pData->checksum = s_pData->CalcChecksum();
    }
    s_HasData = true;
    if (result == 8) {
        g_SvErrorCode = -1;
    }
    s_Detail = s_Request.m_Detail;
    return oml::framework::RESULT_SUCCESS;
}

template <typename T0>
oml::framework::Result BsPartsSaveMgr<T0>::Finalize()
{
    if (m_InstanceNum == 1) {
        if (s_Request.m_Result != SvStepInner::RESULT_RUNNING && s_HasData) {
            s_Request.Reset();
            s_Request.m_Flags = SAVE_FLAGS;
        }
        m_InstanceNum--;
    }
    if (!s_HasData) {
        return oml::framework::RESULT_SUCCESS;
    }
    if (!s_Request.m_IsDone) {
        if (s_Request.m_Result == 0) {
            s_Request.Start();
        }
        return oml::framework::RESULT_RETRY;
    }
    s_Detail = s_Request.m_Detail;
    delete s_pData;
    s_pData = NULL;
    s_SaveNo = 4;
    s_HasData = false;
    return oml::framework::RESULT_SUCCESS;
}

template <typename T0>
oml::framework::Result BsPartsSaveMgr<T0>::Calc()
{
    return oml::framework::RESULT_SUCCESS;
}

template <typename T0>
oml::framework::Result BsPartsSaveMgr<T0>::Draw()
{
    return oml::framework::RESULT_SUCCESS;
}

template <typename T0>
void BsPartsSaveMgr<T0>::BeginSave()
{
    s_IsSaving = true;
}

template <typename T0>
s32 BsPartsSaveMgr<T0>::Write()
{
    if (!s_HasData) {
        return SvSaveIo::RESULT_FAILED;
    }
    return SvSaveIo::WriteFile(s_FileNames[s_SaveNo], 0, s_pData, sizeof(T0), true);
}

template <typename T0>
void BsPartsSaveMgr<T0>::EndSave()
{
    s_IsSaving = false;
}

// 0x00260C6C slot 0x00
// 0x00260C5C (deleting dtor)
template BsPartsSaveMgr<SvBestFriendList>::~BsPartsSaveMgr();
// 0x007CBBD4 (name after the reference symbols)
template oml::framework::Result BsPartsSaveMgr<SvBestFriendList>::Initialize();
// 0x007CBD48 (name after the reference symbols)
template oml::framework::Result BsPartsSaveMgr<SvBestFriendList>::Finalize();
// 0x007CBD40 (name after the reference symbols)
template oml::framework::Result BsPartsSaveMgr<SvBestFriendList>::Calc();
// 0x007CBBCC (name after the reference symbols)
template oml::framework::Result BsPartsSaveMgr<SvBestFriendList>::Draw();
// 0x007CBE8C (name is ours)
template void BsPartsSaveMgr<SvBestFriendList>::BeginSave();
// 0x007CBE30 (name is ours)
template s32 BsPartsSaveMgr<SvBestFriendList>::Write();
// 0x007CBD2C (name is ours)
template void BsPartsSaveMgr<SvBestFriendList>::EndSave();

// 0x00260C80 slot 0x00
// 0x00260C70 (deleting dtor)
template BsPartsSaveMgr<svholder::Mail>::~BsPartsSaveMgr();
// 0x007CBEA8 (name after the reference symbols)
template oml::framework::Result BsPartsSaveMgr<svholder::Mail>::Initialize();
// 0x007CC040 (name after the reference symbols)
template oml::framework::Result BsPartsSaveMgr<svholder::Mail>::Finalize();
// 0x007CC038 (name after the reference symbols)
template oml::framework::Result BsPartsSaveMgr<svholder::Mail>::Calc();
// 0x007CBEA0 (name after the reference symbols)
template oml::framework::Result BsPartsSaveMgr<svholder::Mail>::Draw();
// 0x007CC19C (name is ours)
template void BsPartsSaveMgr<svholder::Mail>::BeginSave();
// 0x007CC140 (name is ours)
template s32 BsPartsSaveMgr<svholder::Mail>::Write();
// 0x007CC024 (name is ours)
template void BsPartsSaveMgr<svholder::Mail>::EndSave();

// 0x00260C94 slot 0x00
// 0x00260C84 (deleting dtor)
template BsPartsSaveMgr<svholder::MyDesign>::~BsPartsSaveMgr();
// 0x007CC1B8 (name after the reference symbols)
template oml::framework::Result BsPartsSaveMgr<svholder::MyDesign>::Initialize();
// 0x007CC350 (name after the reference symbols)
template oml::framework::Result BsPartsSaveMgr<svholder::MyDesign>::Finalize();
// 0x007CC348 (name after the reference symbols)
template oml::framework::Result BsPartsSaveMgr<svholder::MyDesign>::Calc();
// 0x007CC1B0 (name after the reference symbols)
template oml::framework::Result BsPartsSaveMgr<svholder::MyDesign>::Draw();
// 0x007CC4AC (name is ours)
template void BsPartsSaveMgr<svholder::MyDesign>::BeginSave();
// 0x007CC450 (name is ours)
template s32 BsPartsSaveMgr<svholder::MyDesign>::Write();
// 0x007CC334 (name is ours)
template void BsPartsSaveMgr<svholder::MyDesign>::EndSave();

// 0x007CB800 (name is ours)
void SvFreeMovePlayerPls()
{
    if (g_SvHasMovePlayerPls) {
        delete g_pSvMovePlayerPls;
        g_pSvMovePlayerPls = NULL;
        g_SvMovePlayerPlsSaveNo = 4;
        g_SvHasMovePlayerPls = false;
    }
}

// 0x007CBA68 (name is ours)
void SvEndSaveMovePlayerPls()
{
    g_SvIsSavingMovePlayerPls = false;
}

// 0x007CBA7C (name is ours)
bool SvUpdateLoadMovePlayerPls(s32* result)
{
    s32 dummy = 0;
    if (result == NULL) {
        result = &dummy;
    }
    const s32 requestResult = g_SvMovePlayerPlsRequest.m_Result;
    if (g_SvHasMovePlayerPls) {
        *result = (requestResult == 8) ? static_cast<s32>(SvStepInner::RESULT_OK) : requestResult;
        return true;
    }
    if (requestResult == 0) {
        g_SvMovePlayerPlsRequest.Start();
        return false;
    }
    if (!g_SvMovePlayerPlsRequest.m_IsDone) {
        return false;
    }
    *result = requestResult;
    // no file: a new one
    if (requestResult != SvStepInner::RESULT_OK) {
        g_pSvMovePlayerPls->SetUpNew();
        g_pSvMovePlayerPls->UpdateChecksum();
    }
    g_SvHasMovePlayerPls = true;
    if (*result == 8) {
        *result = SvStepInner::RESULT_OK;
        g_SvErrorCode = -1;
    }
    g_SvMovePlayerPlsDetail = g_SvMovePlayerPlsRequest.m_Detail;
    return true;
}

// 0x007CBB5C (name is ours)
s32 SvWriteMovePlayerPls()
{
    if (!g_SvHasMovePlayerPls) {
        return SvSaveIo::RESULT_FAILED;
    }
    return SvSaveIo::WriteFile(g_SvMovePlayerPlsFileNames[g_SvMovePlayerPlsSaveNo], 0, g_pSvMovePlayerPls,
                               sizeof(SvMovePlayerPls), true);
}

// 0x007CBBB8 (name is ours)
void SvBeginSaveMovePlayerPls()
{
    g_SvIsSavingMovePlayerPls = true;
}

// the factories of the process classes
// 0x007EED18
template oml::framework::Process* oml::framework::Process::Create<BsPartsSaveMgr<SvBestFriendList> >();
// 0x007EED48
template oml::framework::Process* oml::framework::Process::Create<BsPartsSaveMgr<svholder::Mail> >();
// 0x007EED78
template oml::framework::Process* oml::framework::Process::Create<BsPartsSaveMgr<svholder::MyDesign> >();
