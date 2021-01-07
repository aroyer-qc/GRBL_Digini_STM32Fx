// picoPNG
// version 20170207 (further clean-up, C code should not be in header file. by aroyer)
// version 20100508 (combined picopng.h / picopng.c into picopng.h by Lubomir I. Ivanov)
// version 20080503 (cleaned up and ported to c by kaitek)
// version 20100508 (modified the code to be used in axonlib by lii)
// Copyright (c) 2005-2008 Lode Vandevenne
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//   1. The origin of this software must not be misrepresented; you must not
//      claim that you wrote the original software. If you use this software
//      in a product, an acknowledgment in the product documentation would be
//      appreciated but is not required.
//   2. Altered source versions must be plainly marked as such, and must not be
//      misrepresented as being the original software.
//   3. This notice may not be removed or altered from any source distribution.

#ifndef _PICOPNG_H
#define _PICOPNG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
	size_t          size;
	size_t          allocsize;
	union
	{
	    void*       v;
	    uint32_t*   u32;
	    uint8_t*    u8;
	} data;
} vector_t;

typedef struct
{
	uint32_t    width, height;
	uint32_t    colorType, bitDepth;
	uint32_t    compressionMethod, filterMethod, interlaceMethod;
	uint32_t    key_r, key_g, key_b;
	bool        key_defined;        // is a transparent color key given?
	vector_t*   palette;
	vector_t    Image;
	vector_t    IDAT;
	vector_t    ScanLines;
    uint8_t*    pOut;
    uint8_t*    pIn;
    size_t      DataSize;

} PNG_Info_t;


//PNG_Info_t* PNG_decode(uint8_t *out, const uint8_t *in, uint32_t size);
size_t PNG_decode(PNG_Info_t* pInfo);

void png_alloc_free_all();

/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif // _PICOPNG_H
