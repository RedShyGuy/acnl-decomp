#include "libms/libms.h"

namespace libms {

namespace {

// 0x00954350
bool s_IsInitialized;

} // namespace

// 0x005B2274 (name is ours)
void Initialize(LMS_AllocFunc alloc, LMS_FreeFunc free)
{
    if (!s_IsInitialized)
    {
        LMS_SetMemFuncs(alloc, free);
        s_IsInitialized = true;
    }
}

// 0x00133BBC (name is ours)
void Finalize()
{
    if (s_IsInitialized)
    {
        LMS_SetMemFuncs(NULL, NULL);
        s_IsInitialized = false;
    }
}

// 0x005B2264 (name is ours)
libms::Message::Message() : m_pBinary(NULL), m_pData(NULL)
{
}

// 0x005B2248 (name is ours)
void libms::Message::Initialize(const void* data)
{
    m_pData = data;
    m_pBinary = LMS_InitMessage(data);
}

// 0x001397DC (name is ours)
void libms::Message::Finalize()
{
    if (m_pBinary != NULL)
    {
        LMS_CloseMessage(m_pBinary);
        m_pBinary = NULL;
        m_pData = NULL;
    }
}

// 0x005B2328 (name is ours)
libms::Flow::Flow() : m_pBinary(NULL), m_pData(NULL)
{
}

// 0x005B22E4 (name is ours)
void libms::Flow::Initialize(const void* data)
{
    m_pData = data;
    m_pBinary = LMS_InitFlow(data);
}

// 0x005B2300 (name is ours)
void libms::Flow::Finalize()
{
    if (m_pBinary != NULL)
    {
        LMS_CloseFlow(m_pBinary);
        m_pBinary = NULL;
        m_pData = NULL;
    }
}

// 0x005B22B8 (name is ours)
libms::Project::Project() : m_pBinary(NULL), m_pData(NULL)
{
}

// 0x005B229C (name is ours)
void libms::Project::Initialize(const void* data)
{
    m_pData = data;
    m_pBinary = LMS_InitProject(data);
}

// 0x00133BEC (name is ours)
void libms::Project::Finalize()
{
    if (m_pBinary != NULL)
    {
        LMS_CloseProject(m_pBinary);
        m_pBinary = NULL;
        m_pData = NULL;
    }
}

} // namespace libms
