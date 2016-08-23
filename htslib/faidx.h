/// @file htslib/faidx.h
/// FASTA random access.
/*
   Copyright (C) 2008, 2009, 2013, 2014, 2016 Genome Research Ltd.

   Author: Heng Li <lh3@sanger.ac.uk>

   Permission is hereby granted, free of charge, to any person obtaining
   a copy of this software and associated documentation files (the
   "Software"), to deal in the Software without restriction, including
   without limitation the rights to use, copy, modify, merge, publish,
   distribute, sublicense, and/or sell copies of the Software, and to
   permit persons to whom the Software is furnished to do so, subject to
   the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

#ifndef HTSLIB_FAIDX_H
#define HTSLIB_FAIDX_H

#include "hts_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @file

  Index FASTA files and extract subsequence.

  The fai file index columns are:
    - chromosome name
    - chromosome length: number of bases
    - offset: number of bytes to skip to get to the first base
        from the beginning of the file, including the length
        of the sequence description string (`>chr ..\n`)
    - line length: number of bases per line (excluding `\n`)
    - binary line length: number of bytes, including `\n`
 */

struct __faidx_t;
/// Opaque structure representing FASTA index
typedef struct __faidx_t faidx_t;

typedef struct {
  const char* key;
  struct faidx_keys* next;
} faidx_keys;

/// Build index for a FASTA or bgzip-compressed FASTA file.
/** @param  fn  FASTA file name
    @return     0 on success; or -1 on failure

File "fn.fai" will be generated.
*/
int fai_build(const char *fn) HTS_RESULT_USED;

/// Destroy a faidx_t struct
void fai_destroy(faidx_t *fai);

/// Load index from "fn.fai".
/** @param  fn  File name of the FASTA file
*/
faidx_t *fai_load(const char *fn);

/// Fetch the sequence in a region
/** @param  fai  Pointer to the faidx_t struct
    @param  reg  Region in the format "chr2:20,000-30,000"
    @param  len  Length of the region; -2 if seq not present, -1 general error
    @return      Pointer to the sequence; `NULL` on failure

The returned sequence is allocated by `malloc()` family and should be destroyed
by end users by calling `free()` on it.
*/
char *fai_fetch(const faidx_t *fai, const char *reg, int *len);

/// Fetch the number of sequences
/** @param  fai  Pointer to the faidx_t struct
    @return      The number of sequences
*/
int faidx_fetch_nseq(const faidx_t *fai) HTS_DEPRECATED("Please use faidx_nseq instead");

/// Fetch the sequence in a region
/** @param  fai  Pointer to the faidx_t struct
    @param  c_name Region name
    @param  p_beg_i  Beginning position number (zero-based)
    @param  p_end_i  End position number (zero-based)
    @param  len  Length of the region; -2 if c_name not present, -1 general error
    @return      Pointer to the sequence; null on failure

The returned sequence is allocated by `malloc()` family and should be destroyed
by end users by calling `free()` on it.
*/
char *faidx_fetch_seq(const faidx_t *fai, const char *c_name, int p_beg_i, int p_end_i, int *len);

/// Query if sequence is present
/**   @param  fai  Pointer to the faidx_t struct
      @param  seq  Sequence name
      @return      1 if present or 0 if absent
*/
int faidx_has_seq(const faidx_t *fai, const char *seq);

/// Return number of sequences in fai index
int faidx_nseq(const faidx_t *fai);

/// Return name of i-th sequence
const char *faidx_iseq(const faidx_t *fai, int i);

/// Return sequence length, -1 if not present
int faidx_seq_len(const faidx_t *fai, const char *seq);

/// Return a linked list of keys in the index
/**  @param  fai Pointer to the faidx_t struct
     @return     Linked list of keys in the index

The returned linked is allocated by `malloc()` family and should be
destroyed by end users by calling `free()` on the elements or
calling faidx_keys_destroy().
*/
faidx_keys* faidx_fetch_keys(const faidx_t *fai);

/// Destroy a linked list of keys
/**  @param  keys Pointer to the faidx_keys list

 */
void faidx_keys_destroy(faidx_keys* keys);

#ifdef __cplusplus
}
#endif

#endif
