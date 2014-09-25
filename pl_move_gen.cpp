//
//  pl_move_gen.cpp
//  Shallow Cyan
//
//  Created by Andrew D. Gibson on 3/13/14.
//  Copyright (c) 2014 Andrew D. Gibson Personal. All rights reserved.
//

#include "pl_move_gen.h"


// -- UTILITY ARRAYS -- \\

//	Array "fillUpAttacks" is used for sliding piece move generation.
//	It takes a pieces location in a rank and the occupied bitboard of that rank, 
//	and returns a full bitboard of that pieces possible moves.    
cBitboard fillUpAttacks[8][64] = {
    {0xfefefefefefefefe, 0x202020202020202, 0x606060606060606, 0x202020202020202, 0xe0e0e0e0e0e0e0e, 0x202020202020202, 0x606060606060606, 0x202020202020202, 0x1e1e1e1e1e1e1e1e, 0x202020202020202, 0x606060606060606, 0x202020202020202, 0xe0e0e0e0e0e0e0e, 0x202020202020202, 0x606060606060606, 0x202020202020202, 0x3e3e3e3e3e3e3e3e, 0x202020202020202, 0x606060606060606, 0x202020202020202, 0xe0e0e0e0e0e0e0e, 0x202020202020202, 0x606060606060606, 0x202020202020202, 0x1e1e1e1e1e1e1e1e, 0x202020202020202, 0x606060606060606, 0x202020202020202, 0xe0e0e0e0e0e0e0e, 0x202020202020202, 0x606060606060606, 0x202020202020202, 0x7e7e7e7e7e7e7e7e, 0x202020202020202, 0x606060606060606, 0x202020202020202, 0xe0e0e0e0e0e0e0e, 0x202020202020202, 0x606060606060606, 0x202020202020202, 0x1e1e1e1e1e1e1e1e, 0x202020202020202, 0x606060606060606, 0x202020202020202, 0xe0e0e0e0e0e0e0e, 0x202020202020202, 0x606060606060606, 0x202020202020202, 0x3e3e3e3e3e3e3e3e, 0x202020202020202, 0x606060606060606, 0x202020202020202, 0xe0e0e0e0e0e0e0e, 0x202020202020202, 0x606060606060606, 0x202020202020202, 0x1e1e1e1e1e1e1e1e, 0x202020202020202, 0x606060606060606, 0x202020202020202, 0xe0e0e0e0e0e0e0e, 0x202020202020202, 0x606060606060606, 0x202020202020202, },
{0xfdfdfdfdfdfdfdfd, 0xfdfdfdfdfdfdfdfd, 0x505050505050505, 0x505050505050505, 0xd0d0d0d0d0d0d0d, 0xd0d0d0d0d0d0d0d, 0x505050505050505, 0x505050505050505, 0x1d1d1d1d1d1d1d1d, 0x1d1d1d1d1d1d1d1d, 0x505050505050505, 0x505050505050505, 0xd0d0d0d0d0d0d0d, 0xd0d0d0d0d0d0d0d, 0x505050505050505, 0x505050505050505, 0x3d3d3d3d3d3d3d3d, 0x3d3d3d3d3d3d3d3d, 0x505050505050505, 0x505050505050505, 0xd0d0d0d0d0d0d0d, 0xd0d0d0d0d0d0d0d, 0x505050505050505, 0x505050505050505, 0x1d1d1d1d1d1d1d1d, 0x1d1d1d1d1d1d1d1d, 0x505050505050505, 0x505050505050505, 0xd0d0d0d0d0d0d0d, 0xd0d0d0d0d0d0d0d, 0x505050505050505, 0x505050505050505, 0x7d7d7d7d7d7d7d7d, 0x7d7d7d7d7d7d7d7d, 0x505050505050505, 0x505050505050505, 0xd0d0d0d0d0d0d0d, 0xd0d0d0d0d0d0d0d, 0x505050505050505, 0x505050505050505, 0x1d1d1d1d1d1d1d1d, 0x1d1d1d1d1d1d1d1d, 0x505050505050505, 0x505050505050505, 0xd0d0d0d0d0d0d0d, 0xd0d0d0d0d0d0d0d, 0x505050505050505, 0x505050505050505, 0x3d3d3d3d3d3d3d3d, 0x3d3d3d3d3d3d3d3d, 0x505050505050505, 0x505050505050505, 0xd0d0d0d0d0d0d0d, 0xd0d0d0d0d0d0d0d, 0x505050505050505, 0x505050505050505, 0x1d1d1d1d1d1d1d1d, 0x1d1d1d1d1d1d1d1d, 0x505050505050505, 0x505050505050505, 0xd0d0d0d0d0d0d0d, 0xd0d0d0d0d0d0d0d, 0x505050505050505, 0x505050505050505, },
{0xfbfbfbfbfbfbfbfb, 0xfafafafafafafafa, 0xfbfbfbfbfbfbfbfb, 0xfafafafafafafafa, 0xb0b0b0b0b0b0b0b, 0xa0a0a0a0a0a0a0a, 0xb0b0b0b0b0b0b0b, 0xa0a0a0a0a0a0a0a, 0x1b1b1b1b1b1b1b1b, 0x1a1a1a1a1a1a1a1a, 0x1b1b1b1b1b1b1b1b, 0x1a1a1a1a1a1a1a1a, 0xb0b0b0b0b0b0b0b, 0xa0a0a0a0a0a0a0a, 0xb0b0b0b0b0b0b0b, 0xa0a0a0a0a0a0a0a, 0x3b3b3b3b3b3b3b3b, 0x3a3a3a3a3a3a3a3a, 0x3b3b3b3b3b3b3b3b, 0x3a3a3a3a3a3a3a3a, 0xb0b0b0b0b0b0b0b, 0xa0a0a0a0a0a0a0a, 0xb0b0b0b0b0b0b0b, 0xa0a0a0a0a0a0a0a, 0x1b1b1b1b1b1b1b1b, 0x1a1a1a1a1a1a1a1a, 0x1b1b1b1b1b1b1b1b, 0x1a1a1a1a1a1a1a1a, 0xb0b0b0b0b0b0b0b, 0xa0a0a0a0a0a0a0a, 0xb0b0b0b0b0b0b0b, 0xa0a0a0a0a0a0a0a, 0x7b7b7b7b7b7b7b7b, 0x7a7a7a7a7a7a7a7a, 0x7b7b7b7b7b7b7b7b, 0x7a7a7a7a7a7a7a7a, 0xb0b0b0b0b0b0b0b, 0xa0a0a0a0a0a0a0a, 0xb0b0b0b0b0b0b0b, 0xa0a0a0a0a0a0a0a, 0x1b1b1b1b1b1b1b1b, 0x1a1a1a1a1a1a1a1a, 0x1b1b1b1b1b1b1b1b, 0x1a1a1a1a1a1a1a1a, 0xb0b0b0b0b0b0b0b, 0xa0a0a0a0a0a0a0a, 0xb0b0b0b0b0b0b0b, 0xa0a0a0a0a0a0a0a, 0x3b3b3b3b3b3b3b3b, 0x3a3a3a3a3a3a3a3a, 0x3b3b3b3b3b3b3b3b, 0x3a3a3a3a3a3a3a3a, 0xb0b0b0b0b0b0b0b, 0xa0a0a0a0a0a0a0a, 0xb0b0b0b0b0b0b0b, 0xa0a0a0a0a0a0a0a, 0x1b1b1b1b1b1b1b1b, 0x1a1a1a1a1a1a1a1a, 0x1b1b1b1b1b1b1b1b, 0x1a1a1a1a1a1a1a1a, 0xb0b0b0b0b0b0b0b, 0xa0a0a0a0a0a0a0a, 0xb0b0b0b0b0b0b0b, 0xa0a0a0a0a0a0a0a, },
{0xf7f7f7f7f7f7f7f7, 0xf6f6f6f6f6f6f6f6, 0xf4f4f4f4f4f4f4f4, 0xf4f4f4f4f4f4f4f4, 0xf7f7f7f7f7f7f7f7, 0xf6f6f6f6f6f6f6f6, 0xf4f4f4f4f4f4f4f4, 0xf4f4f4f4f4f4f4f4, 0x1717171717171717, 0x1616161616161616, 0x1414141414141414, 0x1414141414141414, 0x1717171717171717, 0x1616161616161616, 0x1414141414141414, 0x1414141414141414, 0x3737373737373737, 0x3636363636363636, 0x3434343434343434, 0x3434343434343434, 0x3737373737373737, 0x3636363636363636, 0x3434343434343434, 0x3434343434343434, 0x1717171717171717, 0x1616161616161616, 0x1414141414141414, 0x1414141414141414, 0x1717171717171717, 0x1616161616161616, 0x1414141414141414, 0x1414141414141414, 0x7777777777777777, 0x7676767676767676, 0x7474747474747474, 0x7474747474747474, 0x7777777777777777, 0x7676767676767676, 0x7474747474747474, 0x7474747474747474, 0x1717171717171717, 0x1616161616161616, 0x1414141414141414, 0x1414141414141414, 0x1717171717171717, 0x1616161616161616, 0x1414141414141414, 0x1414141414141414, 0x3737373737373737, 0x3636363636363636, 0x3434343434343434, 0x3434343434343434, 0x3737373737373737, 0x3636363636363636, 0x3434343434343434, 0x3434343434343434, 0x1717171717171717, 0x1616161616161616, 0x1414141414141414, 0x1414141414141414, 0x1717171717171717, 0x1616161616161616, 0x1414141414141414, 0x1414141414141414, },
{0xefefefefefefefef, 0xeeeeeeeeeeeeeeee, 0xecececececececec, 0xecececececececec, 0xe8e8e8e8e8e8e8e8, 0xe8e8e8e8e8e8e8e8, 0xe8e8e8e8e8e8e8e8, 0xe8e8e8e8e8e8e8e8, 0xefefefefefefefef, 0xeeeeeeeeeeeeeeee, 0xecececececececec, 0xecececececececec, 0xe8e8e8e8e8e8e8e8, 0xe8e8e8e8e8e8e8e8, 0xe8e8e8e8e8e8e8e8, 0xe8e8e8e8e8e8e8e8, 0x2f2f2f2f2f2f2f2f, 0x2e2e2e2e2e2e2e2e, 0x2c2c2c2c2c2c2c2c, 0x2c2c2c2c2c2c2c2c, 0x2828282828282828, 0x2828282828282828, 0x2828282828282828, 0x2828282828282828, 0x2f2f2f2f2f2f2f2f, 0x2e2e2e2e2e2e2e2e, 0x2c2c2c2c2c2c2c2c, 0x2c2c2c2c2c2c2c2c, 0x2828282828282828, 0x2828282828282828, 0x2828282828282828, 0x2828282828282828, 0x6f6f6f6f6f6f6f6f, 0x6e6e6e6e6e6e6e6e, 0x6c6c6c6c6c6c6c6c, 0x6c6c6c6c6c6c6c6c, 0x6868686868686868, 0x6868686868686868, 0x6868686868686868, 0x6868686868686868, 0x6f6f6f6f6f6f6f6f, 0x6e6e6e6e6e6e6e6e, 0x6c6c6c6c6c6c6c6c, 0x6c6c6c6c6c6c6c6c, 0x6868686868686868, 0x6868686868686868, 0x6868686868686868, 0x6868686868686868, 0x2f2f2f2f2f2f2f2f, 0x2e2e2e2e2e2e2e2e, 0x2c2c2c2c2c2c2c2c, 0x2c2c2c2c2c2c2c2c, 0x2828282828282828, 0x2828282828282828, 0x2828282828282828, 0x2828282828282828, 0x2f2f2f2f2f2f2f2f, 0x2e2e2e2e2e2e2e2e, 0x2c2c2c2c2c2c2c2c, 0x2c2c2c2c2c2c2c2c, 0x2828282828282828, 0x2828282828282828, 0x2828282828282828, 0x2828282828282828, },
{0xdfdfdfdfdfdfdfdf, 0xdededededededede, 0xdcdcdcdcdcdcdcdc, 0xdcdcdcdcdcdcdcdc, 0xd8d8d8d8d8d8d8d8, 0xd8d8d8d8d8d8d8d8, 0xd8d8d8d8d8d8d8d8, 0xd8d8d8d8d8d8d8d8, 0xd0d0d0d0d0d0d0d0, 0xd0d0d0d0d0d0d0d0, 0xd0d0d0d0d0d0d0d0, 0xd0d0d0d0d0d0d0d0, 0xd0d0d0d0d0d0d0d0, 0xd0d0d0d0d0d0d0d0, 0xd0d0d0d0d0d0d0d0, 0xd0d0d0d0d0d0d0d0, 0xdfdfdfdfdfdfdfdf, 0xdededededededede, 0xdcdcdcdcdcdcdcdc, 0xdcdcdcdcdcdcdcdc, 0xd8d8d8d8d8d8d8d8, 0xd8d8d8d8d8d8d8d8, 0xd8d8d8d8d8d8d8d8, 0xd8d8d8d8d8d8d8d8, 0xd0d0d0d0d0d0d0d0, 0xd0d0d0d0d0d0d0d0, 0xd0d0d0d0d0d0d0d0, 0xd0d0d0d0d0d0d0d0, 0xd0d0d0d0d0d0d0d0, 0xd0d0d0d0d0d0d0d0, 0xd0d0d0d0d0d0d0d0, 0xd0d0d0d0d0d0d0d0, 0x5f5f5f5f5f5f5f5f, 0x5e5e5e5e5e5e5e5e, 0x5c5c5c5c5c5c5c5c, 0x5c5c5c5c5c5c5c5c, 0x5858585858585858, 0x5858585858585858, 0x5858585858585858, 0x5858585858585858, 0x5050505050505050, 0x5050505050505050, 0x5050505050505050, 0x5050505050505050, 0x5050505050505050, 0x5050505050505050, 0x5050505050505050, 0x5050505050505050, 0x5f5f5f5f5f5f5f5f, 0x5e5e5e5e5e5e5e5e, 0x5c5c5c5c5c5c5c5c, 0x5c5c5c5c5c5c5c5c, 0x5858585858585858, 0x5858585858585858, 0x5858585858585858, 0x5858585858585858, 0x5050505050505050, 0x5050505050505050, 0x5050505050505050, 0x5050505050505050, 0x5050505050505050, 0x5050505050505050, 0x5050505050505050, 0x5050505050505050, },
{0xbfbfbfbfbfbfbfbf, 0xbebebebebebebebe, 0xbcbcbcbcbcbcbcbc, 0xbcbcbcbcbcbcbcbc, 0xb8b8b8b8b8b8b8b8, 0xb8b8b8b8b8b8b8b8, 0xb8b8b8b8b8b8b8b8, 0xb8b8b8b8b8b8b8b8, 0xb0b0b0b0b0b0b0b0, 0xb0b0b0b0b0b0b0b0, 0xb0b0b0b0b0b0b0b0, 0xb0b0b0b0b0b0b0b0, 0xb0b0b0b0b0b0b0b0, 0xb0b0b0b0b0b0b0b0, 0xb0b0b0b0b0b0b0b0, 0xb0b0b0b0b0b0b0b0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xbfbfbfbfbfbfbfbf, 0xbebebebebebebebe, 0xbcbcbcbcbcbcbcbc, 0xbcbcbcbcbcbcbcbc, 0xb8b8b8b8b8b8b8b8, 0xb8b8b8b8b8b8b8b8, 0xb8b8b8b8b8b8b8b8, 0xb8b8b8b8b8b8b8b8, 0xb0b0b0b0b0b0b0b0, 0xb0b0b0b0b0b0b0b0, 0xb0b0b0b0b0b0b0b0, 0xb0b0b0b0b0b0b0b0, 0xb0b0b0b0b0b0b0b0, 0xb0b0b0b0b0b0b0b0, 0xb0b0b0b0b0b0b0b0, 0xb0b0b0b0b0b0b0b0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, 0xa0a0a0a0a0a0a0a0, },
{0x7f7f7f7f7f7f7f7f, 0x7e7e7e7e7e7e7e7e, 0x7c7c7c7c7c7c7c7c, 0x7c7c7c7c7c7c7c7c, 0x7878787878787878, 0x7878787878787878, 0x7878787878787878, 0x7878787878787878, 0x7070707070707070, 0x7070707070707070, 0x7070707070707070, 0x7070707070707070, 0x7070707070707070, 0x7070707070707070, 0x7070707070707070, 0x7070707070707070, 0x6060606060606060, 0x6060606060606060, 0x6060606060606060, 0x6060606060606060, 0x6060606060606060, 0x6060606060606060, 0x6060606060606060, 0x6060606060606060, 0x6060606060606060, 0x6060606060606060, 0x6060606060606060, 0x6060606060606060, 0x6060606060606060, 0x6060606060606060, 0x6060606060606060, 0x6060606060606060, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, 0x4040404040404040, }
};


// Array "rotateRightBB" is used for rook column move generation
// It takes the first 8 bits (a1-h1) of an otherwise clear bitboard and rotates them clockwise (a1-a8).
cBitboard rotateRightBB[256] = {
	0x0, 0x100000000000000, 0x1000000000000, 0x101000000000000, 0x10000000000, 0x100010000000000, 0x1010000000000, 0x101010000000000, 
	0x100000000, 0x100000100000000, 0x1000100000000, 0x101000100000000, 0x10100000000, 0x100010100000000, 0x1010100000000, 0x101010100000000, 
	0x1000000, 0x100000001000000, 0x1000001000000, 0x101000001000000, 0x10001000000, 0x100010001000000, 0x1010001000000, 0x101010001000000, 
	0x101000000, 0x100000101000000, 0x1000101000000, 0x101000101000000, 0x10101000000, 0x100010101000000, 0x1010101000000, 0x101010101000000, 
	0x10000, 0x100000000010000, 0x1000000010000, 0x101000000010000, 0x10000010000, 0x100010000010000, 0x1010000010000, 0x101010000010000, 
	0x100010000, 0x100000100010000, 0x1000100010000, 0x101000100010000, 0x10100010000, 0x100010100010000, 0x1010100010000, 0x101010100010000, 
	0x1010000, 0x100000001010000, 0x1000001010000, 0x101000001010000, 0x10001010000, 0x100010001010000, 0x1010001010000, 0x101010001010000, 
	0x101010000, 0x100000101010000, 0x1000101010000, 0x101000101010000, 0x10101010000, 0x100010101010000, 0x1010101010000, 0x101010101010000, 
	0x100, 0x100000000000100, 0x1000000000100, 0x101000000000100, 0x10000000100, 0x100010000000100, 0x1010000000100, 0x101010000000100, 0x100000100, 
	0x100000100000100, 0x1000100000100, 0x101000100000100, 0x10100000100, 0x100010100000100, 0x1010100000100, 0x101010100000100, 0x1000100, 
	0x100000001000100, 0x1000001000100, 0x101000001000100, 0x10001000100, 0x100010001000100, 0x1010001000100, 0x101010001000100, 0x101000100,
 	0x100000101000100, 0x1000101000100, 0x101000101000100, 0x10101000100, 0x100010101000100, 0x1010101000100, 0x101010101000100, 0x10100, 
 	0x100000000010100, 0x1000000010100, 0x101000000010100, 0x10000010100, 0x100010000010100, 0x1010000010100, 0x101010000010100, 0x100010100, 
 	0x100000100010100, 0x1000100010100, 0x101000100010100, 0x10100010100, 0x100010100010100, 0x1010100010100, 0x101010100010100, 0x1010100, 
 	0x100000001010100, 0x1000001010100, 0x101000001010100, 0x10001010100, 0x100010001010100, 0x1010001010100, 0x101010001010100, 0x101010100, 
 	0x100000101010100, 0x1000101010100, 0x101000101010100, 0x10101010100, 0x100010101010100, 0x1010101010100, 0x101010101010100, 
 	static_cast<cBitboard>(0xf0f0f0f000000001), static_cast<cBitboard>(0xf1f0f0f000000001), static_cast<cBitboard>(0xf0f1f0f000000001), 
 	static_cast<cBitboard>(0xf1f1f0f000000001), static_cast<cBitboard>(0xf0f0f1f000000001), static_cast<cBitboard>(0xf1f0f1f000000001), 
 	static_cast<cBitboard>(0xf0f1f1f000000001), static_cast<cBitboard>(0xf1f1f1f000000001), static_cast<cBitboard>(0xf0f0f0f100000001), 
 	static_cast<cBitboard>(0xf1f0f0f100000001), static_cast<cBitboard>(0xf0f1f0f100000001), static_cast<cBitboard>(0xf1f1f0f100000001), 
 	static_cast<cBitboard>(0xf0f0f1f100000001), static_cast<cBitboard>(0xf1f0f1f100000001), static_cast<cBitboard>(0xf0f1f1f100000001), 
 	static_cast<cBitboard>(0xf1f1f1f100000001), static_cast<cBitboard>(0xf0f0f0f001000001), static_cast<cBitboard>(0xf1f0f0f001000001), 
 	static_cast<cBitboard>(0xf0f1f0f001000001), static_cast<cBitboard>(0xf1f1f0f001000001), static_cast<cBitboard>(0xf0f0f1f001000001), 
 	static_cast<cBitboard>(0xf1f0f1f001000001), static_cast<cBitboard>(0xf0f1f1f001000001), static_cast<cBitboard>(0xf1f1f1f001000001), 
 	static_cast<cBitboard>(0xf0f0f0f101000001), static_cast<cBitboard>(0xf1f0f0f101000001), static_cast<cBitboard>(0xf0f1f0f101000001), 
 	static_cast<cBitboard>(0xf1f1f0f101000001), static_cast<cBitboard>(0xf0f0f1f101000001), static_cast<cBitboard>(0xf1f0f1f101000001), 
 	static_cast<cBitboard>(0xf0f1f1f101000001), static_cast<cBitboard>(0xf1f1f1f101000001), static_cast<cBitboard>(0xf0f0f0f000010001), 
 	static_cast<cBitboard>(0xf1f0f0f000010001), static_cast<cBitboard>(0xf0f1f0f000010001), static_cast<cBitboard>(0xf1f1f0f000010001), 
 	static_cast<cBitboard>(0xf0f0f1f000010001), static_cast<cBitboard>(0xf1f0f1f000010001), static_cast<cBitboard>(0xf0f1f1f000010001), 
 	static_cast<cBitboard>(0xf1f1f1f000010001), static_cast<cBitboard>(0xf0f0f0f100010001), static_cast<cBitboard>(0xf1f0f0f100010001), 
 	static_cast<cBitboard>(0xf0f1f0f100010001), static_cast<cBitboard>(0xf1f1f0f100010001), static_cast<cBitboard>(0xf0f0f1f100010001), 
 	static_cast<cBitboard>(0xf1f0f1f100010001), static_cast<cBitboard>(0xf0f1f1f100010001), static_cast<cBitboard>(0xf1f1f1f100010001), 
 	static_cast<cBitboard>(0xf0f0f0f001010001), static_cast<cBitboard>(0xf1f0f0f001010001), static_cast<cBitboard>(0xf0f1f0f001010001), 
 	static_cast<cBitboard>(0xf1f1f0f001010001), static_cast<cBitboard>(0xf0f0f1f001010001), static_cast<cBitboard>(0xf1f0f1f001010001), 
 	static_cast<cBitboard>(0xf0f1f1f001010001), static_cast<cBitboard>(0xf1f1f1f001010001), static_cast<cBitboard>(0xf0f0f0f101010001), 
 	static_cast<cBitboard>(0xf1f0f0f101010001), static_cast<cBitboard>(0xf0f1f0f101010001), static_cast<cBitboard>(0xf1f1f0f101010001), 
 	static_cast<cBitboard>(0xf0f0f1f101010001), static_cast<cBitboard>(0xf1f0f1f101010001), static_cast<cBitboard>(0xf0f1f1f101010001), 
 	static_cast<cBitboard>(0xf1f1f1f101010001), static_cast<cBitboard>(0xf0f0f0f000000101), static_cast<cBitboard>(0xf1f0f0f000000101), 
 	static_cast<cBitboard>(0xf0f1f0f000000101), static_cast<cBitboard>(0xf1f1f0f000000101), static_cast<cBitboard>(0xf0f0f1f000000101), 
 	static_cast<cBitboard>(0xf1f0f1f000000101), static_cast<cBitboard>(0xf0f1f1f000000101), static_cast<cBitboard>(0xf1f1f1f000000101), 
 	static_cast<cBitboard>(0xf0f0f0f100000101), static_cast<cBitboard>(0xf1f0f0f100000101), static_cast<cBitboard>(0xf0f1f0f100000101), 
 	static_cast<cBitboard>(0xf1f1f0f100000101), static_cast<cBitboard>(0xf0f0f1f100000101), static_cast<cBitboard>(0xf1f0f1f100000101), 
 	static_cast<cBitboard>(0xf0f1f1f100000101), static_cast<cBitboard>(0xf1f1f1f100000101), static_cast<cBitboard>(0xf0f0f0f001000101), 
 	static_cast<cBitboard>(0xf1f0f0f001000101), static_cast<cBitboard>(0xf0f1f0f001000101), static_cast<cBitboard>(0xf1f1f0f001000101), 
 	static_cast<cBitboard>(0xf0f0f1f001000101), static_cast<cBitboard>(0xf1f0f1f001000101), static_cast<cBitboard>(0xf0f1f1f001000101), 
 	static_cast<cBitboard>(0xf1f1f1f001000101), static_cast<cBitboard>(0xf0f0f0f101000101), static_cast<cBitboard>(0xf1f0f0f101000101), 
 	static_cast<cBitboard>(0xf0f1f0f101000101), static_cast<cBitboard>(0xf1f1f0f101000101), static_cast<cBitboard>(0xf0f0f1f101000101), 
 	static_cast<cBitboard>(0xf1f0f1f101000101), static_cast<cBitboard>(0xf0f1f1f101000101), static_cast<cBitboard>(0xf1f1f1f101000101), 
 	static_cast<cBitboard>(0xf0f0f0f000010101), static_cast<cBitboard>(0xf1f0f0f000010101), static_cast<cBitboard>(0xf0f1f0f000010101), 
 	static_cast<cBitboard>(0xf1f1f0f000010101), static_cast<cBitboard>(0xf0f0f1f000010101), static_cast<cBitboard>(0xf1f0f1f000010101), 
 	static_cast<cBitboard>(0xf0f1f1f000010101), static_cast<cBitboard>(0xf1f1f1f000010101), static_cast<cBitboard>(0xf0f0f0f100010101), 
 	static_cast<cBitboard>(0xf1f0f0f100010101), static_cast<cBitboard>(0xf0f1f0f100010101), static_cast<cBitboard>(0xf1f1f0f100010101), 
 	static_cast<cBitboard>(0xf0f0f1f100010101), static_cast<cBitboard>(0xf1f0f1f100010101), static_cast<cBitboard>(0xf0f1f1f100010101), 
 	static_cast<cBitboard>(0xf1f1f1f100010101), static_cast<cBitboard>(0xf0f0f0f001010101), static_cast<cBitboard>(0xf1f0f0f001010101), 
 	static_cast<cBitboard>(0xf0f1f0f001010101), static_cast<cBitboard>(0xf1f1f0f001010101), static_cast<cBitboard>(0xf0f0f1f001010101), 
 	static_cast<cBitboard>(0xf1f0f1f001010101), static_cast<cBitboard>(0xf0f1f1f001010101), static_cast<cBitboard>(0xf1f1f1f001010101), 
 	static_cast<cBitboard>(0xf0f0f0f101010101), static_cast<cBitboard>(0xf1f0f0f101010101), static_cast<cBitboard>(0xf0f1f0f101010101), 
 	static_cast<cBitboard>(0xf1f1f0f101010101), static_cast<cBitboard>(0xf0f0f1f101010101), static_cast<cBitboard>(0xf1f0f1f101010101), 
 	static_cast<cBitboard>(0xf0f1f1f101010101), static_cast<cBitboard>(0xf1f1f1f101010101),
};


// Array "knightMovesFrom" is used for knight move generation
// It takes in a knight's location on the board and returns its possible moves.
cBitboard knightMovesFrom[64] = {
    // 1st Rank
    0x020400, 0x050800, 0xA1100, 0x142200, 0x284400, 0x508800, 0xA01000, 0x402000,
    // 2nd Rank
    0x02040004, 0x05080008, 0xA110011, 0x14220022, 0x28440044, 0x50880088, 0xA0100010, 0x40200020,
    // 3rd Rank
    0x0204000402, 0x0508000805, 0xA1100110A, 0x1422002214, 0x2844004428, 0x5088008850, 0xA0100010A0, 0x4020002040,
    // 4th Rank
    0x020400040200, 0x050800080500, 0xA1100110A00, 0x142200221400, 0x284400442800, 0x508800885000, 0xA0100010A000, 0x402000204000,
    // 5th Rank
    0x02040004020000, 0x05080008050000, 0xA1100110A0000, 0x14220022140000, 0x28440044280000, 0x50880088500000, 0xA0100010A00000, 0x40200020400000,
    // 6th Rank
    0x0204000402000000, 0x0508000805000000, 0xA1100110A000000, 0x1422002214000000, 0x2844004428000000, 0x5088008850000000, static_cast<cBitboard>(0xA0100010A0000000), 0x4020002040000000,
    // 7th Rank
    0x0400040200000000, 0x0800080500000000, 0x1100110A00000000, 0x2200221400000000, 0x4400442800000000, static_cast<cBitboard>(0x8800885000000000), 0x100010A000000000, 0x2000204000000000,
    // 8th Rank
    0x0004020000000000, 0x0008050000000000, 0x0110A0000000000, 0x0022140000000000, 0x0044280000000000, 0x0088500000000000, 0x0010A00000000000, 0x0020400000000000,
};


// Array "kingMovesFrom" is used for king move generation.
// It takes in a king's location on the board and returns its possible moves.
cBitboard kingMovesFrom[64] = {
    // 1st Rank
    0x0302, 0x0705, 0x0E0A, 0x1C14, 0x3828, 0x7050, 0xE0A0, 0xC040,
    // 2nd Rank
    0x030203, 0x070507, 0x0E0A0E, 0x1C141C, 0x382838, 0x705070, 0xE0A0E0, 0xC040C0,
    // 3rd Rank
    0x03020300, 0x07050700, 0x0E0A0E00, 0x1C141C00, 0x38283800, 0x70507000, 0xE0A0E000, 0xC040C000,
    // 4th Rank
    0x0302030000, 0x0705070000, 0x0E0A0E0000, 0x1C141C0000, 0x3828380000, 0x7050700000, 0xE0A0E00000, 0xC040C00000,
    // 5th Rank
    0x030203000000, 0x070507000000, 0x0E0A0E000000, 0x1C141C000000, 0x382838000000, 0x705070000000, 0xE0A0E0000000, 0xC040C0000000,
    // 6th Rank
    0x03020300000000, 0x07050700000000, 0x0E0A0E00000000, 0x1C141C00000000, 0x38283800000000, 0x70507000000000, 0xE0A0E000000000, 0xC040C000000000,
    // 7th Rank
    0x0302030000000000, 0x0705070000000000, 0x0E0A0E0000000000, 0x1C141C0000000000, 0x3828380000000000, 0x7050700000000000, static_cast<cBitboard>(0xE0A0E00000000000), static_cast<cBitboard>(0xC040C00000000000),
    // 8th Rank
    0x0203000000000000, 0x0507000000000000, 0x0A0E000000000000, 0x141C000000000000, 0x2838000000000000, 0x5070000000000000, static_cast<cBitboard>(0xA0E0000000000000), static_cast<cBitboard>(0x40C0000000000000)
};


// Array "diagonalMask" is used for bishop move generation.
// It takes in a bishop's location on the board 
// and returns the mask for its diagonal (bottom left to top right on the board).
cBitboard diagonalMask[64] = {
    static_cast<cBitboard>(0x8040201008040201), 0x80402010080402, 0x804020100804, 0x8040201008, 0x80402010, 0x804020, 0x8040, 0x80,
    0x4020100804020100, static_cast<cBitboard>(0x8040201008040201), 0x80402010080402, 0x804020100804, 0x8040201008, 0x80402010, 0x804020, 0x8040,
    0x2010080402010000, 0x4020100804020100, static_cast<cBitboard>(0x8040201008040201), 0x80402010080402, 0x804020100804, 0x8040201008, 0x80402010, 0x804020,
    0x1008040201000000, 0x2010080402010000, 0x4020100804020100, static_cast<cBitboard>(0x8040201008040201), 0x80402010080402, 0x804020100804, 0x8040201008, 0x80402010,
    0x0804020100000000, 0x1008040201000000, 0x2010080402010000, 0x4020100804020100, static_cast<cBitboard>(0x8040201008040201), 0x80402010080402, 0x804020100804, 0x8040201008,
    0x0402010000000000, 0x0804020100000000, 0x1008040201000000, 0x2010080402010000, 0x4020100804020100, static_cast<cBitboard>(0x8040201008040201), 0x80402010080402, 0x804020100804,
    0x0201000000000000, 0x0402010000000000, 0x0804020100000000, 0x1008040201000000, 0x2010080402010000, 0x4020100804020100, static_cast<cBitboard>(0x8040201008040201), 0x80402010080402,
    0x0100000000000000, 0x0201000000000000, 0x0402010000000000, 0x0804020100000000, 0x1008040201000000, 0x2010080402010000, 0x4020100804020100, static_cast<cBitboard>(0x8040201008040201),
};


// Array "antiDiagonalMask" is used for bishop move generation.
// It takes in a bishops's location on the board  
// and returns the mask for its anti-Diagonal (top left to bottom right on the board).
cBitboard antiDiagonalMask[64] = { 
    0x01, 0x0102, 0x010204, 0x01020408, 0x0102040810, 0x010204081020, 0x01020408102040, 0x0102040810204080,
    0x0102, 0x010204, 0x01020408, 0x0102040810, 0x010204081020, 0x01020408102040, 0x0102040810204080, 0x0204081020408000,
    0x010204, 0x01020408, 0x0102040810, 0x010204081020, 0x01020408102040, 0x0102040810204080, 0x0204081020408000, 0x0408102040800000,
    0x01020408, 0x0102040810, 0x010204081020, 0x01020408102040, 0x0102040810204080, 0x0204081020408000, 0x0408102040800000, 0x0810204080000000,
    0x0102040810, 0x010204081020, 0x01020408102040, 0x0102040810204080, 0x0204081020408000, 0x0408102040800000, 0x0810204080000000, 0x1020408000000000,
    0x010204081020, 0x01020408102040, 0x0102040810204080, 0x0204081020408000, 0x0408102040800000, 0x0810204080000000, 0x1020408000000000, 0x2040800000000000,
    0x01020408102040, 0x102040810204080, 0x0204081020408000, 0x0408102040800000, 0x0810204080000000, 0x1020408000000000, 0x2040800000000000, 0x4080000000000000,
    0x0102040810204080, 0x0204081020408000, 0x0408102040800000, 0x0810204080000000, 0x1020408000000000, 0x2040800000000000, 0x4080000000000000, 0x8000000000000000
};



// -- SINGLE PIECE MOVE GENERATION -- \\

// These move generation functions take in the location of a piece and return a bitboard of its moves,
// with no regard to whether it attacks friendly or enemy pieces. 


cBitboard moveGen::knight(int &knightSQ) {

    return knightMovesFrom[knightSQ];    

}


cBitboard moveGen::bishop(int &bishopSQ, cBitboard &occupied) {
   
    int column = bishopSQ & 7;
    
    cBitboard bFile = 0x0202020202020202;


    // Diagonal
    cBitboard diaMoves = (diagonalMask[bishopSQ] & occupied) * bFile >> 58;    
    diaMoves = diagonalMask[bishopSQ] & fillUpAttacks[column][diaMoves];
    

    // Anti Diagonal
    cBitboard antiDiaMoves = (antiDiagonalMask[bishopSQ] & occupied) * bFile >> 58;  
    antiDiaMoves = antiDiagonalMask[bishopSQ] & fillUpAttacks[column][antiDiaMoves];
     

    return diaMoves | antiDiaMoves;
    
}


cBitboard moveGen::rook(int &rookSQ, cBitboard &occupied) {
    
    int rank = rookSQ >> 3, column = rookSQ & 7;

    const cBitboard aFile = 0x0101010101010101;
    
    
    // Rank moves
    cBitboard rankMoves = (occupied >> ((rank*8)+1) ) & 0x3F;
    rankMoves = (fillUpAttacks[column][rankMoves] & 0xFF) << (rank*8);
    

    // Column moves
    cBitboard columnMoves = aFile & ( occupied >> column );

    columnMoves = ((0x8040201008040201 * columnMoves ) >> 57) & 0x3f; // Rotates the bitboard to allow it to access the attacks array
    
    columnMoves = (fillUpAttacks[7-rank][columnMoves]) & 0xFF;
    
    columnMoves = (rotateRightBB[columnMoves] & aFile) << column;

    
    return rankMoves | columnMoves;

}



// -- BITBOARD MOVE GEN -- \\

// Unlike the single piece move generation functions, these functions take a
// bitboard of pieces and return a bitboard of all their combined moves.
// Again, they do so with no regard to whether they attack friendly or enemy pieces.


// pawnForward returns all the possible one square forward and two square forward moves for a set of pawns. 
cBitboard moveGen::pawnForward(cBitboard &pawnLoc, cBitboard &occupied, bool color) {
    
    int direction = color ? 1 : -1;
    

    // One move forward gen
    cBitboard singleMoves = shift(pawnLoc, 8 * direction);
    singleMoves &= ~occupied;
    

    // Two move forward gen
    cBitboard doubleMoves = color ? singleMoves & 0xFF0000 : singleMoves & 0xFF0000000000;
    doubleMoves = shift(doubleMoves, 8 * direction);
    doubleMoves &= ~occupied;


    return singleMoves | doubleMoves;
    
}


// pawnLeftAtt returns all the possible left attacks for a set of pawns, 
// regardless of whether pieces exist to attack on each of those squares.
cBitboard moveGen::pawnLeftAtt(cBitboard &pawnLoc, bool color) {
    
    int direction = color ? 1 : -1;
    
    cBitboard moves = pawnLoc & 0xFEFEFEFEFEFEFEFE; // Gets rid of left-most file to prevent left-most pawn captures wrapping around
    
    moves = shift(moves, (8 * direction)-1);
    
    return moves;
    
}


// pawnRightAtt returns all the possible right attacks for a set of pawns, 
// regardless of whether pieces exist to attack on each of those squares.
cBitboard moveGen::pawnRightAtt(cBitboard &pawnLoc, bool color) {
    
    int direction = color ? 1 : -1;
    
    cBitboard moves = pawnLoc & 0x7F7F7F7F7F7F7F7F;
    
    moves = shift(moves, (8 * direction)+1);
    
    return moves;
    
}


cBitboard moveGen::allKnight(cBitboard knightLoc) {
    
    cBitboard allMoves = 0;

    while (knightLoc) {
        
        int knightSQ = bitScanForward(knightLoc);  
        knightLoc &= knightLoc - 1;
        allMoves |= knight(knightSQ);
        
    }
    
    return allMoves;
}

cBitboard moveGen::allBishop(cBitboard bishopLoc, cBitboard &occupied) {
    
    cBitboard allMoves = 0;
    
    while (bishopLoc) {
        
        int bishopSQ = bitScanForward(bishopLoc);
        bishopLoc &= bishopLoc - 1;
        allMoves |= bishop(bishopSQ, occupied);
        
    }
    
    return allMoves;
    
}

cBitboard moveGen::allRook(cBitboard rookLoc, cBitboard &occupied) {
    
    cBitboard allMoves(0);
    
    while (rookLoc) {
        
        int rookSQ = bitScanForward(rookLoc);
        
        rookLoc &= rookLoc - 1;
        
        allMoves |= rook(rookSQ, occupied);
        
    }
    
    return allMoves;
}

cBitboard moveGen::allQueen(cBitboard queenLoc, cBitboard &occupied) {

    return allBishop(queenLoc, occupied) | allRook(queenLoc, occupied);

}

// king returns all the possible moves for what should always be
// a single populated bitboard.
cBitboard moveGen::king(cBitboard &kingLoc) {

    return kingMovesFrom[bitScanForward(kingLoc)];

}
