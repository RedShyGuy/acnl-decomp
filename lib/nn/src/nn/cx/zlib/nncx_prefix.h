/* The cx library of ACNL links zlib 1.2.7 ("1.2.7.f-NINTENDO-CTR-SDK-v1" in the binary) with the
 * prefix nncxZlib_ on its linked functions (the names are from the reference symbols). Included
 * by zconf.h; the zlib sources of this directory are otherwise those of zlib 1.2.7
 * (https://github.com/madler/zlib, tag v1.2.7; license in zlib.h). */
/* (zcalloc and zcfree have no names in the binary: they keep the zlib names) */
#ifndef NNCX_PREFIX_H
#define NNCX_PREFIX_H

#define _tr_align             nncxZlib__tr_align
#define _tr_flush_bits        nncxZlib__tr_flush_bits
#define _tr_flush_block       nncxZlib__tr_flush_block
#define _tr_init              nncxZlib__tr_init
#define _tr_stored_block      nncxZlib__tr_stored_block
#define _tr_tally             nncxZlib__tr_tally
#define adler32               nncxZlib_adler32
#define adler32_combine       nncxZlib_adler32_combine
#define adler32_combine64     nncxZlib_adler32_combine64
#define crc32                 nncxZlib_crc32
#define crc32_combine         nncxZlib_crc32_combine
#define crc32_combine64       nncxZlib_crc32_combine64
#define get_crc_table         nncxZlib_get_crc_table
#define deflate               nncxZlib_deflate
#define deflateBound          nncxZlib_deflateBound
#define deflateCopy           nncxZlib_deflateCopy
#define deflateEnd            nncxZlib_deflateEnd
#define deflateInit2_         nncxZlib_deflateInit2_
#define deflateInit_          nncxZlib_deflateInit_
#define deflateParams         nncxZlib_deflateParams
#define deflatePending        nncxZlib_deflatePending
#define deflatePrime          nncxZlib_deflatePrime
#define deflateReset          nncxZlib_deflateReset
#define deflateResetKeep      nncxZlib_deflateResetKeep
#define deflateSetDictionary  nncxZlib_deflateSetDictionary
#define deflateSetHeader      nncxZlib_deflateSetHeader
#define deflateTune           nncxZlib_deflateTune
#define deflate_copyright     nncxZlib_deflate_copyright
#define zError                nncxZlib_zError
#define zlibCompileFlags      nncxZlib_zlibCompileFlags
#define zlibVersion           nncxZlib_zlibVersion
#define z_errmsg              nncxZlib_z_errmsg
#define _dist_code            nncxZlib__dist_code
#define _length_code          nncxZlib__length_code

#endif
