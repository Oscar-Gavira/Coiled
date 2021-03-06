/* wrap.c */

/*
This Software is distributed with the following X11 License,
sometimes also known as MIT license.

Copyright (c) 2010 Miguel A. Ballicora

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

#include "wrap.h"

#define LZMA86

#if defined(LZMA86)
#include "lzma/Lzma86Dec.h"
#endif

#if !defined(NDEBUG)
#define NDEBUG
#endif
#ifdef DEBUG
#undef NDEBUG
#endif
#include "assert.h"

/* external, so the compiler can be silenced */
size_t TB_DUMMY_unused;

/***********************************************************************************************************/

extern int
lzma_decode
(const unsigned char *in_start, size_t in_len, unsigned char *out_start, size_t *pout_len, size_t out_max)
{
		size_t nn = out_max;
		int x = Lzma86_Decode(out_start, &nn, in_start, &in_len);
		*pout_len = nn;
		return x == SZ_OK;
}

/***********************************************************************************************************/

#define TRUE 1
#define FALSE 0
















