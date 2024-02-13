//-------------------------------------------------------------------------------------------------
//
//  File : App_cfg.h
//
//-------------------------------------------------------------------------------------------------
//
// Copyright(c) 2020 Alain Royer.
// Email: aroyer.qc@gmail.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
// AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------------------------------
//
//  Note : This file contain only define for configuration of the application
//
//-------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------
//  block memory declaration section
//-------------------------------------------------------------------------------------------------

#define MEMORY_POOL_USE_STAT                        DEF_ENABLED
#define MEMORY_POOL_RESTRICT_ALLOC_TO_BLOCK_SIZE    DEF_DISABLED    // If Enable we allow only closest block size that are greater to be allocated

// Add here all group of memory block needed and their size

// Memory block 0:      general all around block
// Memory block 1:      general all around block
// Memory block 2:      general all around block
// Memory block 3:      general all around block
// Memory block 4:      general all around block

//               Enum ID             Memory Group Name   Allocation Group Name   Nb of block     Size of block
#define MEM_BLOCK_DEF(X_MEM_BLOCK)\
    X_MEM_BLOCK( MEM_BLOCK,          Block,              AllocBlock,             64,            16U              )\
    X_MEM_BLOCK( MEM_BLOCK_0,        Block_0,            AllocBlock_0,           64,            64U              )\
    X_MEM_BLOCK( MEM_BLOCK_1,        Block_1,            AllocBlock_1,           64,            128U             )\
    X_MEM_BLOCK( MEM_BLOCK_2,        Block_2,            AllocBlock_2,           64,            256U             )\
    X_MEM_BLOCK( MEM_BLOCK_3,        Block_3,            AllocBlock_3,           8,             1024U            )\
    X_MEM_BLOCK( MEM_BLOCK_4,        Block_4,            AllocBlock_4,           2,             2048U            )\

//-------------------------------------------------------------------------------------------------

