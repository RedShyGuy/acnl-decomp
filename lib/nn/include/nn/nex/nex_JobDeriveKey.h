#pragma once

#include "decomp.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_ChecksumAlgorithm.h"
#include "nn/nex/nex_Job.h"

namespace nn {
namespace nex {
class Key;
class KeyCache;

// ChecksumAlgorithm::DeriveKey as a job: at most 64 iterations per run (or until the deadline of
// the dispatch); the key goes to the call context and the cache. The member names are ours.
// RTTI N2nn3nex12JobDeriveKeyE @ 0x008CE0D4
// vtable 0x008FC4F8 (vptr 0x008FC500), offset_to_top 0, 12 entries
class JobDeriveKey : public ::nn::nex::Job
{
public:
    // (inline in ChecksumAlgorithm::DeriveKey; the job owns pAlgorithm)
    JobDeriveKey(CallContext* pContext, ChecksumAlgorithm* pAlgorithm, Buffer password, u32 iterations, Key* pKey, JobType type)
        : Job(type, DebugString()), m_CallId(pContext->m_Id), m_pAlgorithm(pAlgorithm), m_Iteration(0), m_Iterations(iterations), m_pKey(pKey),
          m_pKeyCache(nullptr)
    {
        m_Data = password;
        m_Password = password;
    }
    virtual ~JobDeriveKey(); // 0x0035D13C slot 0x00
    // 0x0035D0C8 slot 0x04 | mk7dlp:callseq (deleting dtor)
    virtual void Execute(); // 0x0035CE64 slot 0x0C | fefates:callseq

    // the iterations of one run (name is ours)
    static const u32 ITERATIONS_PER_RUN = 64;

    u32 m_CallId;                   // 0x3C
    ChecksumAlgorithm* m_pAlgorithm; // 0x40
    Buffer m_Data;                  // 0x44 (the checksum of the last iteration)
    u32 m_Iteration;                // 0x64
    u32 m_Iterations;               // 0x68
    Key* m_pKey;                    // 0x6C
    Buffer m_Password;              // 0x70
    KeyCache* m_pKeyCache;          // 0x90
};
ASSERT_OFFSET(JobDeriveKey, m_CallId, 0x3C);
ASSERT_OFFSET(JobDeriveKey, m_Password, 0x70);
ASSERT_SIZE(JobDeriveKey, 0x98);
} // namespace nex
} // namespace nn
