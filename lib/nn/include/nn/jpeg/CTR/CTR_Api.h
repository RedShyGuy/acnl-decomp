#pragma once

#include "decomp.h"
#include "nn/jpeg/CTR/jpeg_Types.h"

// hand-written assembly in the original (copied in jpeg_Asm.cpp)
#define JPEG_ASM __attribute__((naked))

namespace nn {
namespace jpeg {
namespace CTR {
void JpegMpEncoderAsm_ConvertWorkToAsm(nn::jpeg::CTR::JpegMpEncoderContext*) JPEG_ASM; // 0x0014B9F4 | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_ConvertWorkToC(nn::jpeg::CTR::JpegMpEncoderContext*) JPEG_ASM; // 0x0014BA30 | nintendogs:bytes [tier B]
bool JpegMpEncoderAsm_ComponentSequential(nn::jpeg::CTR::JpegMpEncoderContext*, nn::jpeg::CTR::JpegMpEncoderComponentStructure*) JPEG_ASM; // 0x0014BA5C | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_Yuyv8ToYuv444(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014BC6C | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_Yuyv8ToYuv420(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014BD58 | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_Yuyv8ToYuv422(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014BF38 | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_CtrRgb565ToYuv444(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014C00C | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_CtrRgb565ToYuv420(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014C120 | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_CtrRgb565Block8ToYuv444(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014C3B0 | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_CtrRgb565Block8ToYuv420(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014C4E0 | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_CtrRgb565Block8ToYuv422(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014C6C8 | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_Rgb8ToYuv444(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014C800 | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_Rgb8ToYuv420(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014C920 | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_CtrRgb8Block8ToYuv444(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014CBCC | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_CtrRgb8Block8ToYuv420(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014CD00 | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_CtrRgb8Block8ToYuv422(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014CEF8 | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_Rgba8ToYuv444(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014D03C | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_Rgba8ToYuv420(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014D150 | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_CtrRgba8Block8ToYuv444(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014D3E4 | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_CtrRgba8Block8ToYuv420(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014D510 | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_CtrRgba8Block8ToYuv422(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014D6FC | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_Bgr8ToYuv444(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014D834 | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_Bgr8ToYuv420(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014D954 | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_Abgr8ToYuv444(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014DC00 | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_Abgr8ToYuv420(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014DD14 | nintendogs:bytes [tier B]
void JpegMpEncoderAsm_CtrRgb565ToYuv422(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014C2AC (name is ours, after the converter table)
void JpegMpEncoderAsm_Rgb8ToYuv422(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014CABC (name is ours, after the converter table)
void JpegMpEncoderAsm_Rgba8ToYuv422(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014D2E0 (name is ours, after the converter table)
void JpegMpEncoderAsm_Bgr8ToYuv422(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014DAF0 (name is ours, after the converter table)
void JpegMpEncoderAsm_Abgr8ToYuv422(nn::jpeg::CTR::JpegMpEncoderContext*, unsigned, unsigned) JPEG_ASM; // 0x0014DEA4 (name is ours, after the converter table)
void JpegMpEncoderAsm_ForwardDct(short*, const short*, short*) JPEG_ASM; // 0x0014DFA8 | nintendogs:bytes [tier B]

namespace detail {
struct JpegMpEncoderWorkObj;
struct JpegMpEncoderTemporarySettingObj;
} // namespace detail

// the encoder (names from the symbols)
bool InitializeJpegMpEncoderContext(JpegMpEncoderContext* context, u8* dst, u32 dstSize, const void* src, u32 width, u32 height, u32 quality, PixelSampling sampling, PixelFormat format, u32 stride, const detail::JpegMpEncoderTemporarySettingObj* settings); // 0x00471E08 | nintendogs:bytes [tier B]
u32 Yos_JPEGEncodeFast(JpegMpEncoderContext* context); // 0x00470CC8
u32 StartJpegEncoderCore(detail::JpegMpEncoderWorkObj* work, u8* dst, u32 dstSize, const void* src, u32 width, u32 height, u32 quality, PixelSampling sampling, PixelFormat format, bool isAddThumbnail, detail::JpegMpEncoderTemporarySettingObj* settings); // 0x004710F0
bool StartMpEncoderCore(detail::JpegMpEncoderWorkObj* work, u8* dst, u32 dstSize, const void* src, u32 width, u32 height, u32 quality, PixelSampling sampling, PixelFormat format, bool isAddThumbnail, detail::JpegMpEncoderTemporarySettingObj* settings); // 0x00470BC4 | nintendogs:bytes [tier B]

// resampling of the thumbnail into one MCU of CtrRgb565 (Yuyv8 stays YUYV), then the converter
void JpegMpEncoderCResampleYuyv8(JpegMpEncoderContext* context, u32 mcuX, u32 mcuY); // 0x00471C94 | nintendogs:bytes [tier B]
void JpegMpEncoderCResampleCtrRgb565(JpegMpEncoderContext* context, u32 mcuX, u32 mcuY); // 0x00472120 | nintendogs:bytes [tier B]
void JpegMpEncoderCResampleCtrRgb565Block8(JpegMpEncoderContext* context, u32 mcuX, u32 mcuY); // 0x00472710 | nintendogs:bytes [tier B]
void JpegMpEncoderCResampleRgb8(JpegMpEncoderContext* context, u32 mcuX, u32 mcuY); // 0x00471700 | nintendogs:bytes [tier B]
void JpegMpEncoderCResampleCtrRgb8Block8(JpegMpEncoderContext* context, u32 mcuX, u32 mcuY); // 0x004722A8 | nintendogs:bytes [tier B]
void JpegMpEncoderCResampleRgba8(JpegMpEncoderContext* context, u32 mcuX, u32 mcuY); // 0x00471AB0 | nintendogs:bytes [tier B]
void JpegMpEncoderCResampleCtrRgba8Block8(JpegMpEncoderContext* context, u32 mcuX, u32 mcuY); // 0x004724E0 | nintendogs:bytes [tier B]
void JpegMpEncoderCResampleBgr8(JpegMpEncoderContext* context, u32 mcuX, u32 mcuY); // 0x00471530 | nintendogs:bytes [tier B]
void JpegMpEncoderCResampleAbgr8(JpegMpEncoderContext* context, u32 mcuX, u32 mcuY); // 0x004718C8 | nintendogs:bytes [tier B]
} // namespace CTR
} // namespace jpeg
} // namespace nn
