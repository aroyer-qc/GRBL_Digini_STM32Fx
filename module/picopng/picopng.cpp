// picoPNG
// version 20170207 (further clean-up, remove axonlib ref., C code should not be in header file. by aroyer)
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

#include "picopng.h"
#include "stdlib.h"

// Added for auto commenting code (AR)
typedef enum
{
    PNG_OK                                              = 0,
    PNG_APPEARED_OUTSIDE_CODETREE                       = 11,
    PNG_DATA_LENGTH_SMALLER_THAN_IN_HEADER              = 27,
    PNG_NO_SIGNATURE                                    = 28,
    PNG_DOES_NOT_START_WITH_IHDR_CHUNK                  = 29,
	PNG_BUFFER_SIZE_TOO_SMALL_FOR_NEXT_CHUNK            = 30,
    PNG_NON_EXISTANT_COLOR_TYPE                         = 31,
    PNG_ONLY_COMPRESSION_METHOD_0_ALLOWED               = 32,
	PNG_ONLY_FILTER_METHOD_0_ALLOWED                    = 33,
	PNG_ONLY_INTERLACE_METHOD_0_ALLOWED                 = 34,
	PNG_BUFFER_SIZE_TOO_SMALL_FOR_NEXT_CHUNK_2          = 35,
	PNG_ERROR_NONEXISTENT_FILTER                        = 36,
    PNG_NOT_VALID_COLOR_TYPE                            = 37,
    PNG_PALETTE_TOO_BIG                                 = 38,
    PNG_MORE_ALPHA_THAN_PALETTE_ENTRIES                 = 39,
    PNG_CHUNK_MUST_BE_2_BYTES_FOR_GRAYSCALE_IMAGE       = 40,
    PNG_CHUNK_MUST_BE_6_BYTES_FOR_RGB_IMAGE             = 41,
    PNG_CHUNK_T_RNS_NOT_ALLOWED_FOR_OTHER_COLOR_MODELS  = 42,
	PNG_CHUNK_LENGTH_INVALID                            = 63,
    PNG_UNKNOWN_CRITICAL_CHUNK                          = 69,

} PNG_Error_e;


typedef struct png_alloc_node
{
	struct png_alloc_node* prev;
	struct png_alloc_node* next;
	void* addr;
	size_t size;
} png_alloc_node_t;


void png_alloc_free_all();


#define PNG_SIGNATURE	0x0a1a0a0d474e5089ull

#define CHUNK_IHDR		0x52444849
#define CHUNK_IDAT		0x54414449
#define CHUNK_IEND		0x444e4549
#define CHUNK_PLTE		0x45544c50
#define CHUNK_tRNS		0x534e5274

PNG_Error_e PNG_Error;

/******************************************************************************/

png_alloc_node_t *png_alloc_head = nullptr;
png_alloc_node_t *png_alloc_tail = nullptr;

/******************************************************************************/

png_alloc_node_t *png_alloc_find_node(void *addr)
{
	png_alloc_node_t *node;

	for(node = png_alloc_head; node; node = node->next)
	{
		if(node->addr == addr)
		{
			break;
		}
	}
	return node;
}

void png_alloc_add_node(void *addr, size_t size)
{
    png_alloc_node_t *node;

	if(png_alloc_find_node(addr))
	{
		return;
	}

    node = (png_alloc_node_t*)malloc(sizeof (png_alloc_node_t));
	node->addr = addr;
	node->size = size;
	node->prev = png_alloc_tail;
	node->next = nullptr;
	png_alloc_tail = node;

	if(node->prev)
	{
		node->prev->next = node;
	}

	if(!png_alloc_head)
	{
		png_alloc_head = node;
	}
}

void png_alloc_remove_node(png_alloc_node_t *node)
{
	if(node->prev)
	{
		node->prev->next = node->next;
	}

	if(node->next)
	{
		node->next->prev = node->prev;
	}

	if (node == png_alloc_head)
	{
		png_alloc_head = node->next;
	}

	if(node == png_alloc_tail)
	{
		png_alloc_tail = node->prev;
	}

	node->prev = nullptr;
    node->next = nullptr;
    node->addr = nullptr;

    free(node);
}

void* png_alloc_malloc(size_t size)
{
    void *addr = malloc(size);
	png_alloc_add_node(addr, size);
	return addr;
}

void* png_alloc_realloc(void *addr, size_t size)
{
	void* new_addr;

	if(!addr)
	{
		return png_alloc_malloc(size);
	}

    new_addr = realloc((char*)addr, size);

	if(new_addr != addr)
	{
		png_alloc_node_t *old_node;
		old_node = png_alloc_find_node(addr);
		png_alloc_remove_node(old_node);
		png_alloc_add_node(new_addr, size);
	}

	return new_addr;
}

void png_alloc_free(void *addr)
{
	png_alloc_node_t *node = png_alloc_find_node(addr);
	if(!node)
	{
		return;
	}

	png_alloc_remove_node(node);
    free(addr);
}

void png_alloc_free_all()
{
	while(png_alloc_tail)
    {
		void* addr = png_alloc_tail->addr;

		png_alloc_remove_node(png_alloc_tail);
        free(addr);
	}
}

/******************************************************************************/

__unused void vector_cleanup(vector_t *p)
{
	p->size = p->allocsize = 0;

	if(p->data.v)
	{
		png_alloc_free(p->data.v);
	}

	p->data.v = nullptr;
}

uint32_t vector32_resize(vector_t *p, size_t size)
{	// returns 1 if success, 0 if failure ==> nothing done

	if(size * sizeof (uint32_t) > p->allocsize)
	{
		size_t newsize = size * sizeof (uint32_t) * 2;
		void *data = png_alloc_realloc(p->data.v, newsize);
		if(data)
		{
			p->allocsize = newsize;
			p->data.v = data;
			p->size = size;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		p->size = size;
	}

	return 1;
}

uint32_t vector32_resizev(vector_t *p, size_t size, uint32_t value)
{	// resize and give all new elements the value
	size_t oldsize = p->size, i;

	if(!vector32_resize(p, size))
	{
		return 0;
	}

	for(i = oldsize; i < size; i++)
	{
		(p->data.u32)[i] = value;
	}

	return 1;
}

void vector32_init(vector_t *p)
{
	p->data.v = nullptr;
	p->size = p->allocsize = 0;
}

vector_t* vector32_new(size_t size, uint32_t value)
{
	vector_t *p = (vector_t*)png_alloc_malloc(sizeof(vector_t));
	vector32_init(p);

	if((size != 0) && (vector32_resizev(p, size, value) == 0))
	{
		return nullptr;
	}

	return p;
}

/******************************************************************************/
// TODO maybe we can use a common resize

uint32_t vector8_resize(vector_t *p, size_t size)
{
  // returns 1 if success, 0 if failure ==> nothing done
	// xxx: the use of sizeof uint32_t here seems like a bug (this descends from the lodepng vector
	// compatibility functions which do the same). without this there is corruption in certain cases,
	// so this was probably done to cover up allocation bug(s) in the original picopng code!
	if(size * sizeof (uint32_t) > p->allocsize)
	{
		size_t newsize = size * sizeof (uint32_t) * 2;
		void *data = png_alloc_realloc(p->data.v, newsize);
		if(data)
		{
			p->allocsize = newsize;
			p->data.v = data;
			p->size = size;
		}
		else
		{
			return 0; // error: not enough memory
		}
	}
	else
	{
		p->size = size;
	}

	return 1;
}

uint32_t vector8_resizev(vector_t *p, size_t size, uint8_t value)
{	// resize and give all new elements the value
	size_t oldsize = p->size, i;

	if (!vector8_resize(p, size))
	{
		return 0;
	}

	for (i = oldsize; i < size; i++)
	{
		(p->data.u8)[i] = value;
	}

	return 1;
}

void vector8_init(vector_t *p)
{
	p->data.v = nullptr;
	p->size   = p->allocsize = 0;
}

vector_t *vector8_new(size_t size, uint8_t value)
{
	vector_t *p = (vector_t*) png_alloc_malloc(sizeof (vector_t));
	vector8_init(p);

	if((size != 0) && (vector8_resizev(p, size, value) == 0))
	{
		return nullptr;
	}

	return p;
}

vector_t *vector8_copy(vector_t *p)
{
	vector_t *q = vector8_new(p->size, 0);
	uint32_t n;

	for(n = 0; n < q->size; n++)
	{
	    (q->data.u8)[n] = (p->data.u8)[n];
	}

	return q;
}

/******************************************************************************/

const uint16_t LENBASE   [29] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258 };
const uint8_t  LENEXTRA  [29] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0 };
const uint16_t DISTBASE  [30] = { 1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577 };
const uint8_t  DISTEXTRA [30] = { 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13 };
const uint8_t  CLCL      [19] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 }; // code length code lengths

/******************************************************************************/

typedef struct
{
	// 2D representation of a huffman tree: The one dimension is "0" or "1", the other contains all
	// nodes and leaves of the tree.
	vector_t *tree2d;
} HuffmanTree;

HuffmanTree *HuffmanTree_new()
{
	HuffmanTree *tree = (HuffmanTree*) png_alloc_malloc(sizeof (HuffmanTree));
	tree->tree2d = NULL;
	return tree;
}

int HuffmanTree_makeFromLengths(HuffmanTree* tree, const vector_t* bitlen, uint32_t maxbitlen)
{	// make tree given the lengths
	uint32_t bits;
	uint32_t n;
    uint32_t i;
	uint32_t numcodes = (uint32_t) bitlen->size;
	uint32_t treepos = 0;
	uint32_t nodefilled = 0;

	vector_t* tree1d;
	vector_t* blcount;
	vector_t* nextcode;

	tree1d   = vector32_new(numcodes, 0);
	blcount  = vector32_new(maxbitlen + 1, 0);
	nextcode = vector32_new(maxbitlen + 1, 0);

	for(bits = 0; bits < numcodes; bits++)
	{
		(blcount->data.u32)[(bitlen->data.u32)[bits]]++; // count number of instances of each code length
	}

	for(bits = 1; bits <= maxbitlen; bits++)
	{
		(nextcode->data.u32)[bits] = ((nextcode->data.u32)[bits - 1] + (blcount->data.u32)[bits - 1]) << 1;
	}

	for(n = 0; n < numcodes; n++)
	{
		if((bitlen->data.u32)[n] != 0)
		{
			(tree1d->data.u32)[n] = (nextcode->data.u32)[(bitlen->data.u32)[n]]++; // generate all the codes
		}
	}

	// 0x7fff here means the tree2d isn't filled there yet
	vector_t* tree2d = vector32_new(numcodes * 2, 0x7fff);
	tree->tree2d = tree2d;

	for(n = 0; n < numcodes; n++) // the codes
	{
		for(i = 0; i < (bitlen->data.u32)[n]; i++)// the bits for this code
		{
			uint32_t bit = ((tree1d->data.u32)[n] >> ((bitlen->data.u32)[n] - i - 1)) & 1;

			if(treepos > numcodes - 2)
			{
				return 55;
			}

			if((tree2d->data.u32)[2 * treepos + bit] == 0x7fff) // not yet filled in
			{
				if(i + 1 == (bitlen->data.u32)[n]) // last bit
				{
					(tree2d->data.u32)[2 * treepos + bit] = n;
					treepos = 0;
				}
				else
				{ // addresses are encoded as values > numcodes
					(tree2d->data.u32)[2 * treepos + bit] = ++nodefilled + numcodes;
					treepos = nodefilled;
				}
			}
			else // subtract numcodes from address to get address value
			{
				treepos = (tree2d->data.u32)[2 * treepos + bit] - numcodes;
			}
		}
	}
	return 0;
}

int HuffmanTree_decode(const HuffmanTree *tree, bool *decoded, uint32_t *result, size_t *treepos, uint32_t bit)
{	// Decodes a symbol from the tree
	const vector_t *tree2d = tree->tree2d;
	uint32_t numcodes = (uint32_t) tree2d->size / 2;

	if (*treepos >= numcodes)
	{
		return PNG_APPEARED_OUTSIDE_CODETREE;
	}

	*result  = (tree2d->data.u32)[2 * (*treepos) + bit];
	*decoded = (*result < numcodes);
	*treepos = *decoded ? 0 : *result - numcodes;

	return 0;
}

/******************************************************************************/

int Inflator_error;

uint32_t Zlib_readBitFromStream(size_t *bitp, const uint8_t *bits)
{
	uint32_t result = (bits[*bitp >> 3] >> (*bitp & 0x7)) & 1;
	(*bitp)++;

	return result;
}

uint32_t Zlib_readBitsFromStream(size_t *bitp, const uint8_t *bits, size_t nbits)
{
	uint32_t i, result = 0;

	for (i = 0; i < nbits; i++)
		result += (Zlib_readBitFromStream(bitp, bits)) << i;

	return result;
}

void Inflator_generateFixedTrees(HuffmanTree *tree, HuffmanTree *treeD)
{	// get the tree of a deflated block with fixed tree
	size_t i;
	vector_t* bitlen;
	vector_t* bitlenD;
   //uint32_t* pBitLen;

	bitlen = vector32_new(288, 8);
	bitlenD = vector32_new(32, 5);

	for(i = 144; i <= 255; i++)
	{
		(bitlen->data.u32)[i] = 9;
	}

	for(i = 256; i <= 279; i++)
	{
		(bitlen->data.u32)[i] = 7;
	}

	HuffmanTree_makeFromLengths(tree, bitlen, 15);
	HuffmanTree_makeFromLengths(treeD, bitlenD, 15);
}

uint32_t Inflator_huffmanDecodeSymbol(const uint8_t *in, size_t *bp, const HuffmanTree *codetree, size_t inlength)
{	// decode a single symbol from given list of bits with given code tree. returns the symbol
	bool decoded = false;
	uint32_t ct = 0;
	size_t treepos = 0;

	for(;;)
	{
		if((*bp & 0x07) == 0 && (*bp >> 3) > inlength)
		{
			Inflator_error = 10; // error: end reached without endcode
			return 0;
		}

		Inflator_error = HuffmanTree_decode(codetree, &decoded, &ct, &treepos, Zlib_readBitFromStream(bp, in));

		if(Inflator_error)	return 0; // stop, an error happened
		if(decoded)         return ct;
	}
}

void Inflator_getTreeInflateDynamic(HuffmanTree *tree, HuffmanTree *treeD, const uint8_t *in, size_t *bp, size_t inlength)
{
	// get the tree of a deflated block with dynamic tree, the tree itself is also Huffman
	// compressed with a known tree
	size_t i, n;
	HuffmanTree *codelengthcodetree = HuffmanTree_new(); // the code tree for code length codes
	vector_t *bitlen;
    vector_t  *bitlenD;
	bitlen = vector32_new(288, 0);
	bitlenD = vector32_new(32, 0);

	if(*bp >> 3 >= inlength - 2)
	{
		Inflator_error = 49; // the bit pointer is or will go past the memory
		return;
	}

	size_t HLIT = Zlib_readBitsFromStream(bp, in, 5) + 257;	// number of literal/length codes + 257
	size_t HDIST = Zlib_readBitsFromStream(bp, in, 5) + 1;	// number of dist codes + 1
	size_t HCLEN = Zlib_readBitsFromStream(bp, in, 4) + 4;	// number of code length codes + 4
	vector_t *codelengthcode; // lengths of tree to decode the lengths of the dynamic tree
	codelengthcode = vector32_new(19, 0);

	for(i = 0; i < 19; i++)
	{
		(codelengthcode->data.u32)[CLCL[i]] = (i < HCLEN) ? Zlib_readBitsFromStream(bp, in, 3) : 0;
	}

	Inflator_error = HuffmanTree_makeFromLengths(codelengthcodetree, codelengthcode, 7);

	if(Inflator_error)
	{
		return;
	}

	size_t replength;

	for(i = 0; i < HLIT + HDIST;)
	{
		uint32_t code = Inflator_huffmanDecodeSymbol(in, bp, codelengthcodetree, inlength);
		if(Inflator_error)
		{
			return;
		}

		if(code <= 15) // a length code
		{
			if(i < HLIT)
			{
				(bitlen->data.u32)[i++] = code;
			}
			else
			{
				(bitlenD->data.u32)[i++ - HLIT] = code;
			}
		}
		else if(code == 16) // repeat previous
		{
			if(*bp >> 3 >= inlength)
			{
				Inflator_error = 50; // error, bit pointer jumps past memory
				return;
			}

			replength = 3 + Zlib_readBitsFromStream(bp, in, 2);
			uint32_t value; // set value to the previous code

			if((i - 1) < HLIT)
			{
				value = (bitlen->data.u32)[i - 1];
			}
			else
			{
				value = (bitlenD->data.u32)[i - HLIT - 1];
			}

			for(n = 0; n < replength; n++) // repeat this value in the next lengths
			{
				if(i >= HLIT + HDIST)
				{
					Inflator_error = 13; // error: i is larger than the amount of codes
					return;
				}

				if(i < HLIT)
				{
					(bitlen->data.u32)[i++] = value;
				}
				else
				{
					(bitlenD->data.u32)[i++ - HLIT] = value;
				}
			}
		}
		else if(code == 17) // repeat "0" 3-10 times
        {
 			if(*bp >> 3 >= inlength)
 			{
				Inflator_error = 50; // error, bit pointer jumps past memory
				return;
			}

			replength = 3 + Zlib_readBitsFromStream(bp, in, 3);
			for(n = 0; n < replength; n++) // repeat this value in the next lengths
			{
				if(i >= HLIT + HDIST)
				{
					Inflator_error = 14; // error: i is larger than the amount of codes
					return;
				}

				if(i < HLIT)
				{
					(bitlen->data.u32)[i++] = 0;
				}
				else
				{
				    (bitlenD->data.u32)[i++ - HLIT] = 0;
				}
			}
		}
		else if(code == 18) // repeat "0" 11-138 times
		{
			if(*bp >> 3 >= inlength)
			{
				Inflator_error = 50; // error, bit pointer jumps past memory
				return;
			}

			replength = 11 + Zlib_readBitsFromStream(bp, in, 7);
			for(n = 0; n < replength; n++) // repeat this value in the next lengths
			{
				if(i >= HLIT + HDIST)
				{
					Inflator_error = 15; // error: i is larger than the amount of codes
					return;
				}
				if(i < HLIT)
				{
					(bitlen->data.u32)[i++] = 0;
				}
				else
				{
					(bitlenD->data.u32)[i++ - HLIT] = 0;
				}
			}
		}
		else
		{
			Inflator_error = 16; // error: an nonexitent code appeared. This can never happen.
			return;
		}
	}
	if((bitlen->data.u32)[256] == 0)
	{
		Inflator_error = 64; // the length of the end code 256 must be larger than 0
		return;
	}

	// now we've finally got HLIT and HDIST, so generate the code trees, and the function is done
	Inflator_error = HuffmanTree_makeFromLengths(tree, bitlen, 15);
	if(Inflator_error)
	{
		return;
	}

	Inflator_error = HuffmanTree_makeFromLengths(treeD, bitlenD, 15);

	if(Inflator_error)
	{
		return;
	}
}

void Inflator_inflateHuffmanBlock(vector_t *out, const uint8_t *in, size_t *bp, size_t *pos, size_t inlength, uint32_t btype)
{
	HuffmanTree* codetree;
	HuffmanTree* codetreeD; // the code tree for Huffman codes, dist codes
	codetree = HuffmanTree_new();
	codetreeD = HuffmanTree_new();

	if(btype == 1)
	{
		Inflator_generateFixedTrees(codetree, codetreeD);
	}
	else if(btype == 2)
	{
		Inflator_getTreeInflateDynamic(codetree, codetreeD, in, bp, inlength);

		if(Inflator_error)
		{
			return;
		}
	}
	for(;;)
	{
		uint32_t code = Inflator_huffmanDecodeSymbol(in, bp, codetree, inlength);

		if(Inflator_error)
		{
			return;
		}

		if(code == 256) // end code
		{
			return;
		}
		else if(code <= 255) // literal symbol
        {
        	if(*pos >= out->size)
        	{
				vector8_resize(out, (*pos + 1) * 2); // reserve more room
			}

			(out->data.u8)[(*pos)++] = (uint8_t)code;
		}
		else if(code >= 257 && code <= 285) // length code
        {
			size_t length       = LENBASE  [code - 257];
			size_t numextrabits = LENEXTRA [code - 257];

			if((*bp >> 3) >= inlength)
			{
				Inflator_error = 51; // error, bit pointer will jump past memory
				return;
			}

			length += Zlib_readBitsFromStream(bp, in, numextrabits);
			uint32_t codeD = Inflator_huffmanDecodeSymbol(in, bp, codetreeD, inlength);

			if(Inflator_error)
			{
				return;
			}

			if(codeD > 29)
			{
				Inflator_error = 18; // error: invalid dist code (30-31 are never used)
				return;
			}

			uint32_t dist = DISTBASE[codeD], numextrabitsD = DISTEXTRA[codeD];

			if((*bp >> 3) >= inlength)
			{
				Inflator_error = 51; // error, bit pointer will jump past memory
				return;
			}

			dist += Zlib_readBitsFromStream(bp, in, numextrabitsD);
			size_t start = *pos, back = start - dist; // backwards

			if(*pos + length >= out->size)
			{
				vector8_resize(out, (*pos + length) * 2); // reserve more room
			}

			for(size_t i = 0; i < length; i++)
			{
				(out->data.u8)[(*pos)++] = (out->data.u8)[back++];
				if (back >= start)
				{
					back = start - dist;
				}
			}
		}
	}
}

void Inflator_inflateNoCompression(vector_t *out, const uint8_t *in, size_t *bp, size_t *pos, size_t inlength)
{
	while ((*bp & 0x7) != 0)
	{
		(*bp)++; // go to first boundary of byte
	}

	size_t p = *bp / 8;

	if (p >= inlength - 4)
	{
		Inflator_error = 52; // error, bit pointer will jump past memory
		return;
	}

	uint32_t LEN = in[p] + 256 * in[p + 1], NLEN = in[p + 2] + 256 * in[p + 3];
	p += 4;

	if(LEN + NLEN != 65535)
	{
		Inflator_error = 21; // error: NLEN is not one's complement of LEN
		return;
	}

	if(*pos + LEN >= out->size)
	{
		vector8_resize(out, *pos + LEN);
	}

	if(p + LEN > inlength)
	{
		Inflator_error = 23; // error: reading outside of in buffer
		return;
	}

	uint32_t n;
	for(n = 0; n < LEN; n++)
	{
		(out->data.u8)[(*pos)++] = in[p++]; // read LEN bytes of literal data
	}

	*bp = p * 8;
}

void Inflator_inflate(vector_t *out, const vector_t *in, size_t inpos)
{
	size_t bp = 0, pos = 0; // bit pointer and byte pointer
	Inflator_error = 0;
	uint32_t BFINAL = 0;

	while(!BFINAL && !Inflator_error)
	{
		if(bp >> 3 >= in->size)
		{
			Inflator_error = 52; // error, bit pointer will jump past memory
			return;
		}

		BFINAL = Zlib_readBitFromStream(&bp, &(in->data.u8)[inpos]);
		uint32_t BTYPE = Zlib_readBitFromStream(&bp, &(in->data.u8)[inpos]);
		BTYPE += 2 * Zlib_readBitFromStream(&bp, &(in->data.u8)[inpos]);

		if(BTYPE == 3)
		{
			Inflator_error = 20; // error: invalid BTYPE
			return;
		}
		else if(BTYPE == 0)
		{
			Inflator_inflateNoCompression(out, &(in->data.u8)[inpos], &bp, &pos, in->size);
		}
		else
		{
			Inflator_inflateHuffmanBlock(out, &(in->data.u8)[inpos], &bp, &pos, in->size, BTYPE);
		}
	}

	// TODO (Alain#2#) nowhere it is used !!
	if(!Inflator_error)
	{
		vector8_resize(out, pos); // Only now we know the true size of out, resize it to that
	}
}

/******************************************************************************/

int Zlib_decompress(vector_t *out, const vector_t *in) // returns error value
{
	if(in->size < 2)
	{
		return 53; // error, size of zlib data too small
	}

	if(((in->data.u8)[0] * 256 + (in->data.u8)[1]) % 31 != 0)
	{
		// error: 256 * in->data[0] + in->data[1] must be a multiple of 31, the FCHECK value is
		// supposed to be made that way
		return 24;
	}

	uint32_t CM = (in->data.u8)[0] & 15, CINFO = ((in->data.u8)[0] >> 4) & 15, FDICT = ((in->data.u8)[1] >> 5) & 1;

	if(CM != 8 || CINFO > 7)
	{
		// error: only compression method 8: inflate with sliding window of 32k is supported by
		// the PNG spec
		return 25;
	}

	if(FDICT != 0)
	{
		// error: the specification of PNG says about the zlib stream: "The additional flags shall
		// not specify a preset dictionary."
		return 26;
	}

	Inflator_inflate(out, in, 2);

	return Inflator_error; // note: adler32 checksum was skipped and ignored
}

/******************************************************************************/

uint32_t PNG_readBitFromReversedStream(size_t *bitp, const uint8_t *bits)
{
	uint32_t result = (bits[*bitp >> 3] >> (7 - (*bitp & 0x7))) & 1;
	(*bitp)++;

	return result;
}

uint32_t PNG_readBitsFromReversedStream(size_t *bitp, const uint8_t *bits, uint32_t nbits)
{
	uint32_t i, result = 0;

	for (i = nbits - 1; i < nbits; i--)
	{
		result += ((PNG_readBitFromReversedStream(bitp, bits)) << i);
	}

	return result;
}

void PNG_setBitOfReversedStream(size_t *bitp, uint8_t *bits, uint32_t bit)
{
	bits[*bitp >> 3] |= (bit << (7 - (*bitp & 0x7)));
	(*bitp)++;
}

uint32_t PNG_read32bitInt(const uint8_t *buffer)
{
	return (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
}

int PNG_checkColorValidity(uint32_t colorType, uint32_t bd) // return type is a LodePNG error code
{
	if((colorType == 2 || colorType == 4 || colorType == 6))
	{
		if(!(bd == 8 || bd == 16))
		{
		    return PNG_NOT_VALID_COLOR_TYPE;
		}
	}
	else if(colorType == 0)
	{
		if(!(bd == 1 || bd == 2 || bd == 4 || bd == 8 || bd == 16))
		{
		    return PNG_NOT_VALID_COLOR_TYPE;
		}
	}
	else if(colorType == 3)
	{
		if(!(bd == 1 || bd == 2 || bd == 4 || bd == 8))
		{
		    return PNG_NOT_VALID_COLOR_TYPE;
		}
	}
	else
	{
	    return PNG_NON_EXISTANT_COLOR_TYPE;
	}

	return PNG_OK;
}

uint32_t PNG_getBpp(const PNG_Info_t *info)
{
	uint32_t bitDepth;
	uint32_t colorType;

	bitDepth = info->bitDepth;
	colorType = info->colorType;

	if(colorType == 2)
	{
		return (3 * bitDepth);
	}
	else if (colorType >= 4)
	{
		return (colorType - 2) * bitDepth;
	}
	else
	{
		return bitDepth;
	}
}

void PNG_readPngHeader(PNG_Info_t *pInfo)
{	// read the information from the header and store it in the Info
	if(pInfo->DataSize < 29)
    {
		PNG_Error = PNG_DATA_LENGTH_SMALLER_THAN_IN_HEADER;
		return;
	}

    if(*(uint64_t *)pInfo->pIn != PNG_SIGNATURE)
    {
		PNG_Error = PNG_NO_SIGNATURE;
		return;
	}

	if(*(uint32_t *)&pInfo->pIn[12] != CHUNK_IHDR)
	{
		PNG_Error = PNG_DOES_NOT_START_WITH_IHDR_CHUNK;
		return;
	}

	pInfo->width     = PNG_read32bitInt(&pInfo->pIn[16]);
	pInfo->height    = PNG_read32bitInt(&pInfo->pIn[20]);
	pInfo->bitDepth  = pInfo->pIn[24];
	pInfo->colorType = pInfo->pIn[25];
	pInfo->compressionMethod = pInfo->pIn[26];

	if(pInfo->pIn[26] != 0)
	{
		PNG_Error = PNG_ONLY_COMPRESSION_METHOD_0_ALLOWED;
		return;
	}

    pInfo->filterMethod = pInfo->pIn[27];
	if(pInfo->pIn[27] != 0)
	{
		PNG_Error = PNG_ONLY_FILTER_METHOD_0_ALLOWED;
		return;
	}
	pInfo->interlaceMethod = pInfo->pIn[28];
	if(pInfo->pIn[28] > 1)
	{
		PNG_Error = PNG_ONLY_INTERLACE_METHOD_0_ALLOWED;
		return;
	}

	PNG_Error = (PNG_Error_e)PNG_checkColorValidity(pInfo->colorType, pInfo->bitDepth);
}

int PNG_paethPredictor(int a, int b, int c) // Paeth predicter, used by PNG filter type 4
{
	int p, pa, pb, pc;
	p = a + b - c;
	pa = p > a ? (p - a) : (a - p);
	pb = p > b ? (p - b) : (b - p);
	pc = p > c ? (p - c) : (c - p);
	return (pa <= pb && pa <= pc) ? a : (pb <= pc ? b : c);
}

void PNG_unFilterScanline(uint8_t *recon, const uint8_t *scanline, const uint8_t *precon,
		size_t bytewidth, uint32_t filterType, size_t length)
{
	size_t i;

	switch (filterType)
	{
	    case 0:
	    {
            for(i = 0; i < length; i++)
    	    {
                recon[i] = scanline[i];
            }
    	}
		break;

    	case 1:
	    {
            for(i = 0; i < bytewidth; i++)
    	    {
                recon[i] = scanline[i];
            }

            for(i = bytewidth; i < length; i++)
    	    {
                recon[i] = scanline[i] + recon[i - bytewidth];
            }
    	}
		break;

	    case 2:
	    {
            if(precon)
    	    {
                for(i = 0; i < length; i++)
        	    {
                    recon[i] = scanline[i] + precon[i];
                }
            }
            else
    	    {
                for(i = 0; i < length; i++)
        	    {
                    recon[i] = scanline[i];
                }
            }
    	}
		break;

	    case 3:
	    {
            if(precon)
            {
                for (i = 0; i < bytewidth; i++)
        	    {
                    recon[i] = scanline[i] + precon[i] / 2;
                }

                for (i = bytewidth; i < length; i++)
        	    {
                    recon[i] = scanline[i] + ((recon[i - bytewidth] + precon[i]) / 2);
                }
            }
            else
            {
                for (i = 0; i < bytewidth; i++)
        	    {
                    recon[i] = scanline[i];
                }

                for (i = bytewidth; i < length; i++)
        	    {
                    recon[i] = scanline[i] + recon[i - bytewidth] / 2;
                }
            }
    	}
		break;

	    case 4:
	    {
            if(precon)
            {
                for (i = 0; i < bytewidth; i++)
        	    {
                    recon[i] = (uint8_t) (scanline[i] + PNG_paethPredictor(0, precon[i], 0));
                }

                for (i = bytewidth; i < length; i++)
        	    {
                    recon[i] = (uint8_t) (scanline[i] + PNG_paethPredictor(recon[i - bytewidth], precon[i], precon[i - bytewidth]));
                }
            }
            else
            {
                for (i = 0; i < bytewidth; i++)
        	    {
                    recon[i] = scanline[i];
                }

                for (i = bytewidth; i < length; i++)
        	    {
                    recon[i] = (uint8_t) (scanline[i] + PNG_paethPredictor(recon[i - bytewidth], 0, 0));
                }
            }
    	}
		break;

	    default:
	    {
		    PNG_Error = PNG_ERROR_NONEXISTENT_FILTER;
		    return;
    	}
	}
}

void PNG_adam7Pass(uint8_t *out, uint8_t *linen, uint8_t *lineo, const uint8_t *in, uint32_t w,
		size_t passleft, size_t passtop, size_t spacex, size_t spacey, size_t passw, size_t passh,
		uint32_t bpp)
{	// filter and reposition the pixels into the output when the image is Adam7 interlaced. This
	// function can only do it after the full image is already decoded. The out buffer must have
	// the correct allocated memory size already.
	if(passw == 0)
	{
		return;
	}

	size_t bytewidth = (bpp + 7) / 8, linelength = 1 + ((bpp * passw + 7) / 8);
	uint32_t y;

	for (y = 0; y < passh; y++)
	{
		size_t i, b;
		uint8_t filterType = in[y * linelength], *prevline = (y == 0) ? 0 : lineo;

		PNG_unFilterScanline(linen, &in[y * linelength + 1], prevline, bytewidth, filterType, (w * bpp + 7) / 8);

		if(PNG_Error != PNG_OK)
		{
			return;
		}

		if(bpp >= 8)
		{
			for (i = 0; i < passw; i++)
			{
				for (b = 0; b < bytewidth; b++) // b = current byte of this pixel
				{
					out[bytewidth * w * (passtop + spacey * y) + bytewidth * (passleft + spacex * i) + b] = linen[bytewidth * i + b];
				}
			}
		}
		else
		{
			for (i = 0; i < passw; i++)
			{
				size_t obp, bp;
				obp = bpp * w * (passtop + spacey * y) + bpp * (passleft + spacex * i);
				bp = i * bpp;

				for (b = 0; b < bpp; b++)
				{
					PNG_setBitOfReversedStream(&obp, out, PNG_readBitFromReversedStream(&bp, linen));
				}
			}
		}

		uint8_t *temp = linen;
		linen = lineo;
		lineo = temp; // swap the two buffer pointers "line old" and "line new"
	}
}

int PNG_convert(const PNG_Info_t *info, vector_t *out, const uint8_t *in)
{	// converts from any color type to 32-bit. return value = LodePNG error code
	size_t i;
	size_t c;
	uint32_t bitDepth;
	uint32_t colorType;

	bitDepth = info->bitDepth;
	colorType = info->colorType;
	size_t numpixels = info->width * info->height, bp = 0;
	vector8_resize(out, numpixels * 4);
	uint8_t* out_data = out->size ? out->data.u8 : 0;

	if(bitDepth == 8 && colorType == 0) // greyscale
	{
		for(i = 0; i < numpixels; i++)
		{
			out_data[4 * i + 0] = out_data[4 * i + 1] = out_data[4 * i + 2] = in[i];
			out_data[4 * i + 3] = (info->key_defined && (in[i] == info->key_r)) ? 0 : 255;
		}
	}
	else if((bitDepth == 8) && (colorType == 2)) // RGB color
	{
		for(i = 0; i < numpixels; i++)
		{
		    size_t j = i * 3;

			for(c = 0; c < 3; c++)
			{
				out_data[4 * i + c] = in[j + c];
			}

			out_data[4 * i + 3] = (info->key_defined && (in[j + 0] == info->key_r) && (in[j + 1] == info->key_g) && (in[j + 2] == info->key_b)) ? 0 : 255;
		}
	}
	else if((bitDepth == 8) && (colorType == 3)) // indexed color (palette)
	{
		for(i = 0; i < numpixels; i++)
		{
			if(4U * in[i] >= info->palette->size)
			{
				return 46;
			}

			for(c = 0; c < 4; c++) // get rgb colors from the palette
			{
				out_data[4 * i + c] = (info->palette->data.u8)[4 * in[i] + c];
			}
		}
	}
	else if(bitDepth == 8 && colorType == 4) // greyscale with alpha
	{
		for(i = 0; i < numpixels; i++)
		{
			out_data[4 * i + 0] = out_data[4 * i + 1] = out_data[4 * i + 2] = in[2 * i + 0];
			out_data[4 * i + 3] = in[2 * i + 1];
		}
	}
	else if(bitDepth == 8 && colorType == 6)
	{
		for(i = 0; i < numpixels; i++)
		{
			for(c = 0; c < 4; c++)
			{
				out_data[4 * i + c] = in[4 * i + c]; // RGB with alpha
			}
		}
	}
	else if(bitDepth == 16 && colorType == 0) // greyscale
	{
		for(i = 0; i < numpixels; i++)
		{
			out_data[4 * i + 0] = out_data[4 * i + 1] = out_data[4 * i + 2] = in[2 * i];
			out_data[4 * i + 3] = (info->key_defined && (256U * in[i] + in[i + 1] == info->key_r)) ? 0 : 255;
		}
	}
	else if(bitDepth == 16 && colorType == 2) // RGB color
	{
		for(i = 0; i < numpixels; i++)
		{
            size_t j = i * 6;

			for(c = 0; c < 3; c++)
            {
				out_data[4 * i + c] = in[6 * i + 2 * c];
			    out_data[4 * i + 3] = (info->key_defined &&	(256U * in[j + 0] + in[j + 1] == info->key_r) &&
					                                        (256U * in[j + 2] + in[j + 3] == info->key_g) &&
					                                        (256U * in[j + 4] + in[j + 5] == info->key_b)) ? 0 : 255;
			}
		}
	}
	else if(bitDepth == 16 && colorType == 4) // greyscale with alpha
	{
		for(i = 0; i < numpixels; i++)
		{
			out_data[4 * i + 0] = out_data[4 * i + 1] = out_data[4 * i + 2] = in[4 * i]; // msb
			out_data[4 * i + 3] = in[4 * i + 2];
		}
	}
	else if(bitDepth == 16 && colorType == 6)
	{
		for(i = 0; i < numpixels; i++)
		{
			for(c = 0; c < 4; c++)
			{
				out_data[4 * i + c] = in[8 * i + 2 * c]; // RGB with alpha
			}
		}
	}
	else if((bitDepth < 8) && (colorType == 0)) // greyscale
	{
		for(i = 0; i < numpixels; i++)
		{
		    size_t j = i << 2;

			uint32_t value = (PNG_readBitsFromReversedStream(&bp, in, bitDepth) * 255) / ((1 << bitDepth) - 1); // scale value from 0 to 255
			out_data[j + 0] = out_data[j + 1] = out_data[j + 2] = (uint8_t) value;
			out_data[j + 3] = (info->key_defined && value && (((1U << bitDepth) - 1U) == info->key_r) && ((1U << bitDepth) - 1U)) ? 0 : 255;
		}
	}
	else if(bitDepth < 8 && colorType == 3) // palette
	{
		for(i = 0; i < numpixels; i++)
		{
			uint32_t value = PNG_readBitsFromReversedStream(&bp, in, bitDepth);

			if(4 * value >= info->palette->size)
			{
				return 47;
			}

			for(c = 0; c < 4; c++) // get rgb colors from the palette
			{
				out_data[4 * i + c] = (info->palette->data.u8)[4 * value + c];
			}
		}
	}
	return 0;
}

void PNG_info_new(PNG_Info_t* pInfo)
{
	pInfo->palette = vector8_new(0, 0);
}

size_t PNG_decode(PNG_Info_t* pInfo)
{
	PNG_Error = PNG_OK;

	PNG_info_new(pInfo);
	PNG_readPngHeader(pInfo);

	if(PNG_Error != PNG_OK)
    {
        return 0;
    }

	size_t pos = 33; // first byte of the first chunk after the header
	bool IEND = false;//, known_type = true;
	pInfo->key_defined = false;
	// loop through the chunks, ignoring unknown chunks and stopping at IEND chunk. IDAT data is
	// put at the start of the in buffer

    pInfo->IDAT.size = 0;

	while(!IEND)
    {
		size_t i;
		size_t j;

		if(pos + 8 >= pInfo->DataSize)
        {
			PNG_Error = PNG_BUFFER_SIZE_TOO_SMALL_FOR_NEXT_CHUNK;
			return 0;
		}

		size_t chunkLength = PNG_read32bitInt(&pInfo->pIn[pos]);
		pos += 4;

		if(chunkLength > 0x7fffffff)
        {
			PNG_Error = PNG_CHUNK_LENGTH_INVALID;
			return 0;
		}

		if(pos + chunkLength >= pInfo->DataSize)
        {
			PNG_Error = PNG_BUFFER_SIZE_TOO_SMALL_FOR_NEXT_CHUNK_2;
			return 0;
		}

//		uint32_t chunkType = *(uint32_t *) &pInfo->pIn[pos];
		uint32_t chunkType;
		chunkType  = (uint32_t)pInfo->pIn[pos];
		chunkType |= (uint32_t)pInfo->pIn[pos + 1] << 8;
		chunkType |= (uint32_t)pInfo->pIn[pos + 2] << 16;
		chunkType |= (uint32_t)pInfo->pIn[pos + 3] << 24;

		if(chunkType == CHUNK_IDAT)// IDAT: compressed image data chunk
        {
			for(i = 0; i < chunkLength; i++)
			{
				(pInfo->IDAT.data.u32)[pInfo->IDAT.size + i] = (pInfo->pIn)[pos + 4 + i];
			}
            pInfo->IDAT.size += chunkLength;
			pos += (4 + chunkLength);
		}
		else if(chunkType == CHUNK_IEND)            // IEND
        {
			pos += 4;
			IEND = true;
		}
        else if(chunkType == CHUNK_PLTE)            // PLTE: palette chunk
        {
			pos += 4; // go after the 4 letters
			vector8_resize(pInfo->palette, 4 * (chunkLength / 3));

			if(pInfo->palette->size > (4 * 256))
			{
				PNG_Error = PNG_PALETTE_TOO_BIG;
				return 0;
			}

			for(i = 0; i < pInfo->palette->size; i += 4)
            {
				for(j = 0; j < 3; j++)
				{
					(pInfo->palette->data.u8)[i + j] = pInfo->pIn[pos++]; // RGB
				}
				(pInfo->palette->data.u8)[i + 3] = 255; // alpha
			}
		}
		else if(chunkType == CHUNK_tRNS)         // tRNS: palette transparency chunk
        {
			pos += 4; // go after the 4 letters

			if(pInfo->colorType == 3)
			{
				if((4 * chunkLength) > pInfo->palette->size)
				{
					PNG_Error = PNG_MORE_ALPHA_THAN_PALETTE_ENTRIES;
					return 0;
				}
				for(i = 0; i < chunkLength; i++)
				{
					(pInfo->palette->data.u8)[4 * i + 3] = pInfo->pIn[pos++];
				}
			}
			else if(pInfo->colorType == 0)
			{
				if(chunkLength != 2)
				{
					PNG_Error = PNG_CHUNK_MUST_BE_2_BYTES_FOR_GRAYSCALE_IMAGE;
					return 0;
				}

				pInfo->key_defined = true;
				pInfo->key_r = pInfo->key_g = pInfo->key_b = 256 * pInfo->pIn[pos] + pInfo->pIn[pos + 1];
				pos += 2;
			}
			else if(pInfo->colorType == 2)
			{
				if(chunkLength != 6)
				{
					PNG_Error = PNG_CHUNK_MUST_BE_6_BYTES_FOR_RGB_IMAGE;
					return 0;
				}

				pInfo->key_defined = true;
				pInfo->key_r = 256 * pInfo->pIn[pos] + pInfo->pIn[pos + 1];
				pos += 2;
				pInfo->key_g = 256 * pInfo->pIn[pos] + pInfo->pIn[pos + 1];
				pos += 2;
				pInfo->key_b = 256 * pInfo->pIn[pos] + pInfo->pIn[pos + 1];
				pos += 2;
			}
			else
			{
				PNG_Error = PNG_CHUNK_T_RNS_NOT_ALLOWED_FOR_OTHER_COLOR_MODELS;
				return 0;
			}
		}
		else
		{
		    // it's not an implemented chunk type, so ignore it: skip over the data
			if(!(pInfo->pIn[pos + 0] & 32))
			{
				// error: unknown critical chunk (5th bit of first byte of chunk type is 0)
				PNG_Error = PNG_UNKNOWN_CRITICAL_CHUNK;
				return 0;
			}

			pos += (chunkLength + 4); // skip 4 letters and uninterpreted data of unimplemented chunk
			//known_type = false;
		}
		pos += 4; // step over CRC (which is ignored)
	}

	uint32_t bpp = PNG_getBpp(pInfo);
    pInfo->ScanLines.size = ((pInfo->width * (pInfo->height * bpp + 7)) / 8) + pInfo->height;

	if((PNG_Error = (PNG_Error_e)Zlib_decompress(&pInfo->ScanLines, &pInfo->IDAT)) != 0)
	{
        // stop if the zlib decompressor returned an error
        return 0;
	}

    size_t bytewidth = (bpp + 7) / 8;
    size_t outlength = (pInfo->height * pInfo->width * bpp + 7) / 8;
	uint8_t *out_data = NULL;

	if(outlength != 0)
    {
        out_data = pInfo->Image.data.u8;
        pInfo->Image.size = outlength;       // time to fill the out buffer
    }

	if(pInfo->interlaceMethod == 0)  // no interlace, just filter
    {
		size_t y, obp, bp;
		size_t linestart, linelength;
		linestart = 0;
		// length in bytes of a scanline, excluding the filtertype byte
		linelength = (pInfo->width * bpp + 7) / 8;

		if(bpp >= 8) // byte per byte
        {
			for(y = 0; y < pInfo->height; y++)
			{
				uint32_t filterType = (pInfo->ScanLines.data.u32)[linestart];
				const uint8_t *prevline;

				prevline = (y == 0) ? 0 : &out_data[(y - 1) * pInfo->width * bytewidth];
				PNG_unFilterScanline(&out_data[linestart - y], &(pInfo->ScanLines.data.u8)[linestart + 1],	prevline, bytewidth, filterType, linelength);

				if (PNG_Error != PNG_OK)
                {
					return 0;
                }
				linestart += (1 + linelength); // go to start of next scanline
            }
		}
        else
        { // less than 8 bits per pixel, so fill it up bit per bit
			vector_t *templine; // only used if bpp < 8
			templine = vector8_new((pInfo->width * bpp + 7) >> 3, 0);

			for(y = 0, obp = 0; y < pInfo->height; y++)
			{
				uint32_t filterType = (pInfo->ScanLines.data.u32)[linestart];
				const uint8_t *prevline;

				prevline = (y == 0) ? 0 : &out_data[(y - 1) * pInfo->width * bytewidth];
				PNG_unFilterScanline(templine->data.u8, &(pInfo->ScanLines.data.u8)[linestart + 1], prevline, bytewidth, filterType, linelength);

				if(PNG_Error != PNG_OK)
				{
					return 0;
				}

				for(bp = 0; bp < pInfo->width * bpp;)
				{
					PNG_setBitOfReversedStream(&obp, out_data, PNG_readBitFromReversedStream(&bp, templine->data.u8));
				}
				linestart += (1 + linelength); // go to start of next scanline
			}
		}
	}
	else  // interlaceMethod is 1 (Adam7)
    {
		int i;
		size_t passw[7] =
		{
			(pInfo->width + 7) / 8, (pInfo->width + 3) / 8, (pInfo->width + 3) / 4,
			(pInfo->width + 1) / 4, (pInfo->width + 1) / 2, (pInfo->width + 0) / 2,
			(pInfo->width + 0) / 1
		};

		size_t passh[7] =
		{
			(pInfo->height + 7) / 8, (pInfo->height + 7) / 8, (pInfo->height + 3) / 8,
			(pInfo->height + 3) / 4, (pInfo->height + 1) / 4, (pInfo->height + 1) / 2,
			(pInfo->height + 0) / 2
		};

		size_t passstart[7] = { 0 };
		size_t pattern[28] = { 0, 4, 0, 2, 0, 1, 0, 0, 0, 4, 0, 2, 0, 1, 8, 8, 4, 4, 2, 2, 1, 8, 8, 8, 4, 4, 2, 2 }; // values for the adam7 passes

		for(i = 0; i < 6; i++)
        {
			passstart[i + 1] = passstart[i] + passh[i] * ((passw[i] ? 1 : 0) + (passw[i] * bpp + 7) / 8);
        }

		vector_t* scanlineo; // "old"
		vector_t* scanlinen; // and "new" scanline
		scanlineo = vector8_new((pInfo->width * bpp + 7) / 8, 0);
		scanlinen = vector8_new((pInfo->width * bpp + 7) / 8, 0);

		for(i = 0; i < 7; i++)
        {
			PNG_adam7Pass(out_data, scanlinen->data.u8, scanlineo->data.u8, &(pInfo->ScanLines.data.u8)[passstart[i]],
					      pInfo->width, pattern[i], pattern[i + 7], pattern[i + 14], pattern[i + 21],
					      passw[i], passh[i], bpp);
        }
	}
	if((pInfo->colorType != 6) || (pInfo->bitDepth != 8)) // conversion needed
    {
	//	vector8_t *copy = vector8_copy(&pInfo->Image); // xxx: is this copy necessary?
		//PNG_error = PNG_convert(pInfo, &pInfo->image, copy->data);
	}
	else
    {
        for(size_t i = 0; i < pInfo->Image.size; i += 4)
        {
uint8_t* pDataSrc = &pInfo->Image.data.u8[i];
uint8_t* pDataDst = &pInfo->pOut[i];
            // BGRA       -> RGBA
            // BLUE  + 0     RED   + 0
            // GREEN + 1     GREEN + 1
            // RED   + 2     BLUE  + 2
            // ALPHA + 3     ALPHA + 3

            *pDataDst       = pDataSrc[2];
            pDataDst++;
            *pDataDst       = pDataSrc[1];
            pDataDst++;
            *pDataDst        = pDataSrc[0];
            pDataDst++;
            *pDataDst        = pDataSrc[3];
/*
            pInfo->pOut[i]     = (pInfo->Image.data.u8)[i + 2];
            pInfo->pOut[i + 1] = (pInfo->Image.data.u8)[i + 1];
            pInfo->pOut[i + 2] = (pInfo->Image.data.u8)[i];
            pInfo->pOut[i + 3] = (pInfo->Image.data.u8)[i + 3];
*/
        }
	}

	return pInfo->Image.size;
}

