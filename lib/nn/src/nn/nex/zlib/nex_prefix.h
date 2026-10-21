/* The copy of zlib 1.2.5 that nex links ("1.2.5" in the binary). It has no symbols of its own, so
 * every function of it gets the prefix nexZlib_ (the names are ours; the cx library has its own
 * copy of zlib 1.2.7 with other code at other addresses). Included by zconf.h; the zlib sources of
 * this directory are otherwise those of zlib 1.2.5 (https://github.com/madler/zlib, tag v1.2.5;
 * license in zlib.h). */
#ifndef NEX_PREFIX_H
#define NEX_PREFIX_H

/* linked functions */
#define _dist_code            nexZlib__dist_code
#define _length_code          nexZlib__length_code
#define _tr_align             nexZlib__tr_align
#define _tr_flush_block       nexZlib__tr_flush_block
#define _tr_init              nexZlib__tr_init
#define _tr_stored_block      nexZlib__tr_stored_block
#define _tr_tally             nexZlib__tr_tally
#define adler32               nexZlib_adler32
#define adler32_combine       nexZlib_adler32_combine
#define adler32_combine64     nexZlib_adler32_combine64
#define crc32                 nexZlib_crc32
#define crc32_combine         nexZlib_crc32_combine
#define crc32_combine64       nexZlib_crc32_combine64
#define get_crc_table         nexZlib_get_crc_table
#define deflate               nexZlib_deflate
#define deflateBound          nexZlib_deflateBound
#define deflateCopy           nexZlib_deflateCopy
#define deflateEnd            nexZlib_deflateEnd
#define deflateInit2_         nexZlib_deflateInit2_
#define deflateInit_          nexZlib_deflateInit_
#define deflateParams         nexZlib_deflateParams
#define deflatePrime          nexZlib_deflatePrime
#define deflateReset          nexZlib_deflateReset
#define deflateSetDictionary  nexZlib_deflateSetDictionary
#define deflateSetHeader      nexZlib_deflateSetHeader
#define deflateTune           nexZlib_deflateTune
#define deflate_copyright     nexZlib_deflate_copyright
#define inflate               nexZlib_inflate
#define inflateCopy           nexZlib_inflateCopy
#define inflateEnd            nexZlib_inflateEnd
#define inflateGetHeader      nexZlib_inflateGetHeader
#define inflateInit2_         nexZlib_inflateInit2_
#define inflateInit_          nexZlib_inflateInit_
#define inflateMark           nexZlib_inflateMark
#define inflatePrime          nexZlib_inflatePrime
#define inflateReset          nexZlib_inflateReset
#define inflateReset2         nexZlib_inflateReset2
#define inflateSetDictionary  nexZlib_inflateSetDictionary
#define inflateSync           nexZlib_inflateSync
#define inflateSyncPoint      nexZlib_inflateSyncPoint
#define inflateUndermine      nexZlib_inflateUndermine
#define inflate_copyright     nexZlib_inflate_copyright
#define inflate_fast          nexZlib_inflate_fast
#define inflate_table         nexZlib_inflate_table
#define zError                nexZlib_zError
#define zcalloc               nexZlib_zcalloc
#define zcfree                nexZlib_zcfree
#define zlibCompileFlags      nexZlib_zlibCompileFlags
#define zlibVersion           nexZlib_zlibVersion
#define z_errmsg              nexZlib_z_errmsg

/* static functions (renamed too, so that they differ from those of the cx copy) */
#define fill_window           nexZlib_fill_window
#define flush_pending         nexZlib_flush_pending
#define deflate_stored        nexZlib_deflate_stored
#define deflate_fast          nexZlib_deflate_fast
#define deflate_slow          nexZlib_deflate_slow
#define deflate_rle           nexZlib_deflate_rle
#define deflate_huff          nexZlib_deflate_huff
#define build_tree            nexZlib_build_tree
#define compress_block        nexZlib_compress_block
#define scan_tree             nexZlib_scan_tree
#define send_tree             nexZlib_send_tree
#define updatewindow          nexZlib_updatewindow

#endif
