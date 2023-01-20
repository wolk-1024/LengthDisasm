/*
 *  Copyright (c) 2019-2020 Wolk-1024 <wolk1024@gmail.com>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included
 *  in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 *  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 */

#include "LengthDisasm.h"
#include <string.h>

static uint8_t FlagsTable[256] =
	{
		/* 00 */ OP_MODRM,
		/* 01 */ OP_MODRM,
		/* 02 */ OP_MODRM,
		/* 03 */ OP_MODRM,
		/* 04 */ OP_DATA_I8,
		/* 05 */ OP_DATA_I16_I32,
		/* 06 */ OP_NONE,
		/* 07 */ OP_NONE,
		/* 08 */ OP_MODRM,
		/* 09 */ OP_MODRM,
		/* 0A */ OP_MODRM,
		/* 0B */ OP_MODRM,
		/* 0C */ OP_DATA_I8,
		/* 0D */ OP_DATA_I16_I32,
		/* 0E */ OP_NONE,
		/* 0F */ OP_NONE,

		/* 10 */ OP_MODRM,
		/* 11 */ OP_MODRM,
		/* 12 */ OP_MODRM,
		/* 13 */ OP_MODRM,
		/* 14 */ OP_DATA_I8,
		/* 15 */ OP_DATA_I16_I32,
		/* 16 */ OP_NONE,
		/* 17 */ OP_NONE,
		/* 18 */ OP_MODRM,
		/* 19 */ OP_MODRM,
		/* 1A */ OP_MODRM,
		/* 1B */ OP_MODRM,
		/* 1C */ OP_DATA_I8,
		/* 1D */ OP_DATA_I16_I32,
		/* 1E */ OP_NONE,
		/* 1F */ OP_NONE,

		/* 20 */ OP_MODRM,
		/* 21 */ OP_MODRM,
		/* 22 */ OP_MODRM,
		/* 23 */ OP_MODRM,
		/* 24 */ OP_DATA_I8,
		/* 25 */ OP_DATA_I16_I32,
		/* 26 */ OP_PREFIX,
		/* 27 */ OP_NONE,
		/* 28 */ OP_MODRM,
		/* 29 */ OP_MODRM,
		/* 2A */ OP_MODRM,
		/* 2B */ OP_MODRM,
		/* 2C */ OP_DATA_I8,
		/* 2D */ OP_DATA_I16_I32,
		/* 2E */ OP_PREFIX,
		/* 2F */ OP_NONE,

		/* 30 */ OP_MODRM,
		/* 31 */ OP_MODRM,
		/* 32 */ OP_MODRM,
		/* 33 */ OP_MODRM,
		/* 34 */ OP_DATA_I8,
		/* 35 */ OP_DATA_I16_I32,
		/* 36 */ OP_PREFIX,
		/* 37 */ OP_NONE,
		/* 38 */ OP_MODRM,
		/* 39 */ OP_MODRM,
		/* 3A */ OP_MODRM,
		/* 3B */ OP_MODRM,
		/* 3C */ OP_DATA_I8,
		/* 3D */ OP_DATA_I16_I32,
		/* 3E */ OP_PREFIX,
		/* 3F */ OP_NONE,

		/* 40 */ OP_NONE,
		/* 41 */ OP_NONE,
		/* 42 */ OP_NONE,
		/* 43 */ OP_NONE,
		/* 44 */ OP_NONE,
		/* 45 */ OP_NONE,
		/* 46 */ OP_NONE,
		/* 47 */ OP_NONE,
		/* 48 */ OP_NONE,
		/* 49 */ OP_NONE,
		/* 4A */ OP_NONE,
		/* 4B */ OP_NONE,
		/* 4C */ OP_NONE,
		/* 4D */ OP_NONE,
		/* 4E */ OP_NONE,
		/* 4F */ OP_NONE,

		/* 50 */ OP_NONE,
		/* 51 */ OP_NONE,
		/* 52 */ OP_NONE,
		/* 53 */ OP_NONE,
		/* 54 */ OP_NONE,
		/* 55 */ OP_NONE,
		/* 56 */ OP_NONE,
		/* 57 */ OP_NONE,
		/* 58 */ OP_NONE,
		/* 59 */ OP_NONE,
		/* 5A */ OP_NONE,
		/* 5B */ OP_NONE,
		/* 5C */ OP_NONE,
		/* 5D */ OP_NONE,
		/* 5E */ OP_NONE,
		/* 5F */ OP_NONE,
		/* 60 */ OP_NONE,

		/* 61 */ OP_NONE,
		/* 62 */ OP_MODRM,
		/* 63 */ OP_MODRM,
		/* 64 */ OP_PREFIX,
		/* 65 */ OP_PREFIX,
		/* 66 */ OP_PREFIX,
		/* 67 */ OP_PREFIX,
		/* 68 */ OP_DATA_I16_I32,
		/* 69 */ OP_MODRM | OP_DATA_I16_I32,
		/* 6A */ OP_DATA_I8,
		/* 6B */ OP_MODRM | OP_DATA_I8,
		/* 6C */ OP_NONE,
		/* 6D */ OP_NONE,
		/* 6E */ OP_NONE,
		/* 6F */ OP_NONE,

		/* 70 */ OP_RELATIVE | OP_DATA_I8,
		/* 71 */ OP_RELATIVE | OP_DATA_I8,
		/* 72 */ OP_RELATIVE | OP_DATA_I8,
		/* 73 */ OP_RELATIVE | OP_DATA_I8,
		/* 74 */ OP_RELATIVE | OP_DATA_I8,
		/* 75 */ OP_RELATIVE | OP_DATA_I8,
		/* 76 */ OP_RELATIVE | OP_DATA_I8,
		/* 77 */ OP_RELATIVE | OP_DATA_I8,
		/* 78 */ OP_RELATIVE | OP_DATA_I8,
		/* 79 */ OP_RELATIVE | OP_DATA_I8,
		/* 7A */ OP_RELATIVE | OP_DATA_I8,
		/* 7B */ OP_RELATIVE | OP_DATA_I8,
		/* 7C */ OP_RELATIVE | OP_DATA_I8,
		/* 7D */ OP_RELATIVE | OP_DATA_I8,
		/* 7E */ OP_RELATIVE | OP_DATA_I8,
		/* 7F */ OP_RELATIVE | OP_DATA_I8,

		/* 80 */ OP_MODRM | OP_DATA_I8,
		/* 81 */ OP_MODRM | OP_DATA_I16_I32,
		/* 82 */ OP_MODRM | OP_DATA_I8,
		/* 83 */ OP_MODRM | OP_DATA_I8,
		/* 84 */ OP_MODRM,
		/* 85 */ OP_MODRM,
		/* 86 */ OP_MODRM,
		/* 87 */ OP_MODRM,
		/* 88 */ OP_MODRM,
		/* 89 */ OP_MODRM,
		/* 8A */ OP_MODRM,
		/* 8B */ OP_MODRM,
		/* 8C */ OP_MODRM,
		/* 8D */ OP_MODRM,
		/* 8E */ OP_MODRM,
		/* 8F */ OP_MODRM,

		/* 90 */ OP_NONE,
		/* 91 */ OP_NONE,
		/* 92 */ OP_NONE,
		/* 93 */ OP_NONE,
		/* 94 */ OP_NONE,
		/* 95 */ OP_NONE,
		/* 96 */ OP_NONE,
		/* 97 */ OP_NONE,
		/* 98 */ OP_NONE,
		/* 99 */ OP_NONE,
		/* 9A */ OP_DATA_I16 | OP_DATA_I16_I32,
		/* 9B */ OP_NONE,
		/* 9C */ OP_NONE,
		/* 9D */ OP_NONE,
		/* 9E */ OP_NONE,
		/* 9F */ OP_NONE,

		/* A0 */ OP_DATA_I8,
		/* A1 */ OP_DATA_I16_I32_I64,
		/* A2 */ OP_DATA_I8,
		/* A3 */ OP_DATA_I16_I32_I64,
		/* A4 */ OP_NONE,
		/* A5 */ OP_NONE,
		/* A6 */ OP_NONE,
		/* A7 */ OP_NONE,
		/* A8 */ OP_DATA_I8,
		/* A9 */ OP_DATA_I16_I32,
		/* AA */ OP_NONE,
		/* AB */ OP_NONE,
		/* AC */ OP_NONE,
		/* AD */ OP_NONE,
		/* AE */ OP_NONE,
		/* AF */ OP_NONE,

		/* B0 */ OP_DATA_I8,
		/* B1 */ OP_DATA_I8,
		/* B2 */ OP_DATA_I8,
		/* B3 */ OP_DATA_I8,
		/* B4 */ OP_DATA_I8,
		/* B5 */ OP_DATA_I8,
		/* B6 */ OP_DATA_I8,
		/* B7 */ OP_DATA_I8,
		/* B8 */ OP_DATA_I16_I32_I64,
		/* B9 */ OP_DATA_I16_I32_I64,
		/* BA */ OP_DATA_I16_I32_I64,
		/* BB */ OP_DATA_I16_I32_I64,
		/* BC */ OP_DATA_I16_I32_I64,
		/* BD */ OP_DATA_I16_I32_I64,
		/* BE */ OP_DATA_I16_I32_I64,
		/* BF */ OP_DATA_I16_I32_I64,

		/* C0 */ OP_MODRM | OP_DATA_I8,
		/* C1 */ OP_MODRM | OP_DATA_I8,
		/* C2 */ OP_DATA_I16,
		/* C3 */ OP_NONE,
		/* C4 */ OP_MODRM,
		/* C5 */ OP_MODRM,
		/* C6 */ OP_MODRM | OP_DATA_I8,
		/* C7 */ OP_MODRM | OP_DATA_I16_I32,
		/* C8 */ OP_DATA_I8 | OP_DATA_I16,
		/* C9 */ OP_NONE,
		/* CA */ OP_DATA_I16,
		/* CB */ OP_NONE,
		/* CC */ OP_NONE,
		/* CD */ OP_DATA_I8,
		/* CE */ OP_NONE,
		/* CF */ OP_NONE,

		/* D0 */ OP_MODRM,
		/* D1 */ OP_MODRM,
		/* D2 */ OP_MODRM,
		/* D3 */ OP_MODRM,
		/* D4 */ OP_DATA_I8,
		/* D5 */ OP_DATA_I8,
		/* D6 */ OP_NONE,
		/* D7 */ OP_NONE,
		/* D8 */ OP_MODRM,
		/* D9 */ OP_MODRM,
		/* DA */ OP_MODRM,
		/* DB */ OP_MODRM,
		/* DC */ OP_MODRM,
		/* DD */ OP_MODRM,
		/* DE */ OP_MODRM,
		/* DF */ OP_MODRM,

		/* E0 */ OP_RELATIVE | OP_DATA_I8,
		/* E1 */ OP_RELATIVE | OP_DATA_I8,
		/* E2 */ OP_RELATIVE | OP_DATA_I8,
		/* E3 */ OP_RELATIVE | OP_DATA_I8,
		/* E4 */ OP_DATA_I8,
		/* E5 */ OP_DATA_I8,
		/* E6 */ OP_DATA_I8,
		/* E7 */ OP_DATA_I8,
		/* E8 */ OP_RELATIVE | OP_DATA_I16_I32,
		/* E9 */ OP_RELATIVE | OP_DATA_I16_I32,
		/* EA */ OP_DATA_I16 | OP_DATA_I16_I32,
		/* EB */ OP_RELATIVE | OP_DATA_I8,
		/* EC */ OP_NONE,
		/* ED */ OP_NONE,
		/* EE */ OP_NONE,
		/* EF */ OP_NONE,

		/* F0 */ OP_PREFIX,
		/* F1 */ OP_NONE,
		/* F2 */ OP_PREFIX,
		/* F3 */ OP_PREFIX,
		/* F4 */ OP_NONE,
		/* F5 */ OP_NONE,
		/* F6 */ OP_MODRM,
		/* F7 */ OP_MODRM,
		/* F8 */ OP_NONE,
		/* F9 */ OP_NONE,
		/* FA */ OP_NONE,
		/* FB */ OP_NONE,
		/* FC */ OP_NONE,
		/* FD */ OP_NONE,
		/* FE */ OP_MODRM,
		/* FF */ OP_MODRM};

static uint8_t FlagsTableEx[256] =
	{
		/* 0F00 */ OP_MODRM,
		/* 0F01 */ OP_MODRM,
		/* 0F02 */ OP_MODRM,
		/* 0F03 */ OP_MODRM,
		/* 0F04 */ OP_INVALID,
		/* 0F05 */ OP_NONE,
		/* 0F06 */ OP_NONE,
		/* 0F07 */ OP_NONE,
		/* 0F08 */ OP_NONE,
		/* 0F09 */ OP_NONE,
		/* 0F0A */ OP_INVALID,
		/* 0F0B */ OP_NONE,
		/* 0F0C */ OP_INVALID,
		/* 0F0D */ OP_MODRM,
		/* 0F0E */ OP_INVALID,
		/* 0F0F */ OP_MODRM | OP_DATA_I8, // 3Dnow

		/* 0F10 */ OP_MODRM,
		/* 0F11 */ OP_MODRM,
		/* 0F12 */ OP_MODRM,
		/* 0F13 */ OP_MODRM,
		/* 0F14 */ OP_MODRM,
		/* 0F15 */ OP_MODRM,
		/* 0F16 */ OP_MODRM,
		/* 0F17 */ OP_MODRM,
		/* 0F18 */ OP_MODRM,
		/* 0F19 */ OP_INVALID,
		/* 0F1A */ OP_INVALID,
		/* 0F1B */ OP_INVALID,
		/* 0F1C */ OP_INVALID,
		/* 0F1D */ OP_INVALID,
		/* 0F1E */ OP_INVALID,
		/* 0F1F */ OP_MODRM,

		/* 0F20 */ OP_MODRM,
		/* 0F21 */ OP_MODRM,
		/* 0F22 */ OP_MODRM,
		/* 0F23 */ OP_MODRM,
		/* 0F24 */ OP_MODRM | OP_EXTENDED, // SSE5
		/* 0F25 */ OP_INVALID,
		/* 0F26 */ OP_MODRM,
		/* 0F27 */ OP_INVALID,
		/* 0F28 */ OP_MODRM,
		/* 0F29 */ OP_MODRM,
		/* 0F2A */ OP_MODRM,
		/* 0F2B */ OP_MODRM,
		/* 0F2C */ OP_MODRM,
		/* 0F2D */ OP_MODRM,
		/* 0F2E */ OP_MODRM,
		/* 0F2F */ OP_MODRM,

		/* 0F30 */ OP_NONE,
		/* 0F31 */ OP_NONE,
		/* 0F32 */ OP_NONE,
		/* 0F33 */ OP_NONE,
		/* 0F34 */ OP_NONE,
		/* 0F35 */ OP_NONE,
		/* 0F36 */ OP_INVALID,
		/* 0F37 */ OP_NONE,
		/* 0F38 */ OP_MODRM | OP_EXTENDED,
		/* 0F39 */ OP_INVALID,
		/* 0F3A */ OP_MODRM | OP_EXTENDED | OP_DATA_I8,
		/* 0F3B */ OP_INVALID,
		/* 0F3C */ OP_INVALID,
		/* 0F3D */ OP_INVALID,
		/* 0F3E */ OP_INVALID,
		/* 0F3F */ OP_INVALID,

		/* 0F40 */ OP_MODRM,
		/* 0F41 */ OP_MODRM,
		/* 0F42 */ OP_MODRM,
		/* 0F43 */ OP_MODRM,
		/* 0F44 */ OP_MODRM,
		/* 0F45 */ OP_MODRM,
		/* 0F46 */ OP_MODRM,
		/* 0F47 */ OP_MODRM,
		/* 0F48 */ OP_MODRM,
		/* 0F49 */ OP_MODRM,
		/* 0F4A */ OP_MODRM,
		/* 0F4B */ OP_MODRM,
		/* 0F4C */ OP_MODRM,
		/* 0F4D */ OP_MODRM,
		/* 0F4E */ OP_MODRM,
		/* 0F4F */ OP_MODRM,

		/* 0F50 */ OP_MODRM,
		/* 0F51 */ OP_MODRM,
		/* 0F52 */ OP_MODRM,
		/* 0F53 */ OP_MODRM,
		/* 0F54 */ OP_MODRM,
		/* 0F55 */ OP_MODRM,
		/* 0F56 */ OP_MODRM,
		/* 0F57 */ OP_MODRM,
		/* 0F58 */ OP_MODRM,
		/* 0F59 */ OP_MODRM,
		/* 0F5A */ OP_MODRM,
		/* 0F5B */ OP_MODRM,
		/* 0F5C */ OP_MODRM,
		/* 0F5D */ OP_MODRM,
		/* 0F5E */ OP_MODRM,
		/* 0F5F */ OP_MODRM,

		/* 0F60 */ OP_MODRM,
		/* 0F61 */ OP_MODRM,
		/* 0F62 */ OP_MODRM,
		/* 0F63 */ OP_MODRM,
		/* 0F64 */ OP_MODRM,
		/* 0F65 */ OP_MODRM,
		/* 0F66 */ OP_MODRM,
		/* 0F67 */ OP_MODRM,
		/* 0F68 */ OP_MODRM,
		/* 0F69 */ OP_MODRM,
		/* 0F6A */ OP_MODRM,
		/* 0F6B */ OP_MODRM,
		/* 0F6C */ OP_MODRM,
		/* 0F6D */ OP_MODRM,
		/* 0F6E */ OP_MODRM,
		/* 0F6F */ OP_MODRM,

		/* 0F70 */ OP_MODRM | OP_DATA_I8,
		/* 0F71 */ OP_MODRM | OP_DATA_I8,
		/* 0F72 */ OP_MODRM | OP_DATA_I8,
		/* 0F73 */ OP_MODRM | OP_DATA_I8,
		/* 0F74 */ OP_MODRM,
		/* 0F75 */ OP_MODRM,
		/* 0F76 */ OP_MODRM,
		/* 0F77 */ OP_NONE,
		/* 0F78 */ OP_MODRM,
		/* 0F79 */ OP_MODRM,
		/* 0F7A */ OP_INVALID,
		/* 0F7B */ OP_INVALID,
		/* 0F7C */ OP_MODRM,
		/* 0F7D */ OP_MODRM,
		/* 0F7E */ OP_MODRM,
		/* 0F7F */ OP_MODRM,

		/* 0F80 */ OP_RELATIVE | OP_DATA_I16_I32,
		/* 0F81 */ OP_RELATIVE | OP_DATA_I16_I32,
		/* 0F82 */ OP_RELATIVE | OP_DATA_I16_I32,
		/* 0F83 */ OP_RELATIVE | OP_DATA_I16_I32,
		/* 0F84 */ OP_RELATIVE | OP_DATA_I16_I32,
		/* 0F85 */ OP_RELATIVE | OP_DATA_I16_I32,
		/* 0F86 */ OP_RELATIVE | OP_DATA_I16_I32,
		/* 0F87 */ OP_RELATIVE | OP_DATA_I16_I32,
		/* 0F88 */ OP_RELATIVE | OP_DATA_I16_I32,
		/* 0F89 */ OP_RELATIVE | OP_DATA_I16_I32,
		/* 0F8A */ OP_RELATIVE | OP_DATA_I16_I32,
		/* 0F8B */ OP_RELATIVE | OP_DATA_I16_I32,
		/* 0F8C */ OP_RELATIVE | OP_DATA_I16_I32,
		/* 0F8D */ OP_RELATIVE | OP_DATA_I16_I32,
		/* 0F8E */ OP_RELATIVE | OP_DATA_I16_I32,
		/* 0F8F */ OP_RELATIVE | OP_DATA_I16_I32,

		/* 0F90 */ OP_MODRM,
		/* 0F91 */ OP_MODRM,
		/* 0F92 */ OP_MODRM,
		/* 0F93 */ OP_MODRM,
		/* 0F94 */ OP_MODRM,
		/* 0F95 */ OP_MODRM,
		/* 0F96 */ OP_MODRM,
		/* 0F97 */ OP_MODRM,
		/* 0F98 */ OP_MODRM,
		/* 0F99 */ OP_MODRM,
		/* 0F9A */ OP_MODRM,
		/* 0F9B */ OP_MODRM,
		/* 0F9C */ OP_MODRM,
		/* 0F9D */ OP_MODRM,
		/* 0F9E */ OP_MODRM,
		/* 0F9F */ OP_MODRM,

		/* 0FA0 */ OP_NONE,
		/* 0FA1 */ OP_NONE,
		/* 0FA2 */ OP_NONE,
		/* 0FA3 */ OP_MODRM,
		/* 0FA4 */ OP_MODRM | OP_DATA_I8,
		/* 0FA5 */ OP_MODRM,
		/* 0FA6 */ OP_INVALID,
		/* 0FA7 */ OP_INVALID,
		/* 0FA8 */ OP_NONE,
		/* 0FA9 */ OP_NONE,
		/* 0FAA */ OP_NONE,
		/* 0FAB */ OP_MODRM,
		/* 0FAC */ OP_MODRM | OP_DATA_I8,
		/* 0FAD */ OP_MODRM,
		/* 0FAE */ OP_MODRM,
		/* 0FAF */ OP_MODRM,

		/* 0FB0 */ OP_MODRM,
		/* 0FB1 */ OP_MODRM,
		/* 0FB2 */ OP_MODRM,
		/* 0FB3 */ OP_MODRM,
		/* 0FB4 */ OP_MODRM,
		/* 0FB5 */ OP_MODRM,
		/* 0FB6 */ OP_MODRM,
		/* 0FB7 */ OP_MODRM,
		/* 0FB8 */ OP_MODRM,
		/* 0FB9 */ OP_MODRM,
		/* 0FBA */ OP_MODRM | OP_DATA_I8,
		/* 0FBB */ OP_MODRM,
		/* 0FBC */ OP_MODRM,
		/* 0FBD */ OP_MODRM,
		/* 0FBE */ OP_MODRM,
		/* 0FBF */ OP_MODRM,

		/* 0FC0 */ OP_MODRM,
		/* 0FC1 */ OP_MODRM,
		/* 0FC2 */ OP_MODRM | OP_DATA_I8,
		/* 0FC3 */ OP_MODRM,
		/* 0FC4 */ OP_MODRM | OP_DATA_I8,
		/* 0FC5 */ OP_MODRM | OP_DATA_I8,
		/* 0FC6 */ OP_MODRM | OP_DATA_I8,
		/* 0FC7 */ OP_MODRM,
		/* 0FC8 */ OP_NONE,
		/* 0FC9 */ OP_NONE,
		/* 0FCA */ OP_NONE,
		/* 0FCB */ OP_NONE,
		/* 0FCC */ OP_NONE,
		/* 0FCD */ OP_NONE,
		/* 0FCE */ OP_NONE,
		/* 0FCF */ OP_NONE,

		/* 0FD0 */ OP_MODRM,
		/* 0FD1 */ OP_MODRM,
		/* 0FD2 */ OP_MODRM,
		/* 0FD3 */ OP_MODRM,
		/* 0FD4 */ OP_MODRM,
		/* 0FD5 */ OP_MODRM,
		/* 0FD6 */ OP_MODRM,
		/* 0FD7 */ OP_MODRM,
		/* 0FD8 */ OP_MODRM,
		/* 0FD9 */ OP_MODRM,
		/* 0FDA */ OP_MODRM,
		/* 0FDB */ OP_MODRM,
		/* 0FDC */ OP_MODRM,
		/* 0FDD */ OP_MODRM,
		/* 0FDE */ OP_MODRM,
		/* 0FDF */ OP_MODRM,

		/* 0FE0 */ OP_MODRM,
		/* 0FE1 */ OP_MODRM,
		/* 0FE2 */ OP_MODRM,
		/* 0FE3 */ OP_MODRM,
		/* 0FE4 */ OP_MODRM,
		/* 0FE5 */ OP_MODRM,
		/* 0FE6 */ OP_MODRM,
		/* 0FE7 */ OP_MODRM,
		/* 0FE8 */ OP_MODRM,
		/* 0FE9 */ OP_MODRM,
		/* 0FEA */ OP_MODRM,
		/* 0FEB */ OP_MODRM,
		/* 0FEC */ OP_MODRM,
		/* 0FED */ OP_MODRM,
		/* 0FEE */ OP_MODRM,
		/* 0FEF */ OP_MODRM,

		/* 0FF0 */ OP_MODRM,
		/* 0FF1 */ OP_MODRM,
		/* 0FF2 */ OP_MODRM,
		/* 0FF3 */ OP_MODRM,
		/* 0FF4 */ OP_MODRM,
		/* 0FF5 */ OP_MODRM,
		/* 0FF6 */ OP_MODRM,
		/* 0FF7 */ OP_MODRM,
		/* 0FF8 */ OP_MODRM,
		/* 0FF9 */ OP_MODRM,
		/* 0FFA */ OP_MODRM,
		/* 0FFB */ OP_MODRM,
		/* 0FFC */ OP_MODRM,
		/* 0FFD */ OP_MODRM,
		/* 0FFE */ OP_MODRM,
		/* 0FFF */ OP_INVALID,
};

#define bittest(v, n) ((*v) & ((uint8_t)1 << (n)))

/*
							  Структура инструкции:

		 GRP 1, 2, 3, 4           B, X, R, W      7.....0  7.....0  7.....0
   +------------------------+--------------------+-------------------------+
   | Legacy Prefixes (opt.) |  REX Prefix (x64)  |  OPCODE (1, 2, 3 byte)  +--+
   +------------------------+--------------------+-------------------------+  |
																			  |
   +--------------------------------------------------------------------------+
   |
   |   +-----+-----+-----+   +-------+-------+------+
   +-->| Mod | Reg | R/M | + | Scale | Index | Base | + disp8/16/32 + imm8/16/32/64
	   +-----+-----+-----+   +-------+-------+------+     Address       Immediate
		 7-6   5-3   2-0        7-6     5-3    2-0      Displacement      Data

		  Mod R/M Byte               SIB Byte
*/

uint8_t LengthDisasm(void *Address, uint8_t Is64Bit, PLengthDisasm Data)
{
	if (!Address || !Data)
		return 0;

	memset(Data, 0x00, sizeof(TLengthDisasm));
	// memset(Data, 0x00, sizeof(PLengthDisasm));

	uint8_t OpFlag = 0;

	uint8_t *Ip = Address;

	while (FlagsTable[*Ip] & OP_PREFIX)
	{
		Data->Flags |= F_PREFIX;

		switch (*Ip)
		{
		case LockPrefix:
			Data->Flags |= F_PREFIX_LOCK;
			break;
		case RepneRepnzPrefix:
			Data->Flags |= F_PREFIX_REPNZ;
			break;
		case RepeRepzPrefix:
			Data->Flags |= F_PREFIX_REPX;
			break;
		case CSOverridePrefix:
		case SSOverridePrefix:
		case DSOverridePrefix:
		case ESOverridePrefix:
		case FSOverridePrefix:
		case GSOverridePrefix:
			Data->Flags |= F_PREFIX_SEG;
			break;
		case OperandSizeOverridePrefix:
			Data->Flags |= F_PREFIX66;
			break;
		case AddressSizeOverridePrefix:
			Data->Flags |= F_PREFIX67;
			break;
		}
		if (Data->Length > MAX_PREFIXES)
		{
			Data->Flags |= F_INVALID;
			return 0;
		}
		Data->Prefix[Data->PrefixesCount] = *Ip++;
		Data->PrefixesCount++;
		Data->Length++;
	}

	if (Is64Bit && ((*Ip & 0xF0) == 0x40)) // REX
	{
		Data->Flags |= F_REX;

		Data->REX.B = bittest(Ip, 0);
		Data->REX.X = bittest(Ip, 1);
		Data->REX.R = bittest(Ip, 2);
		Data->REX.W = bittest(Ip, 3);

		Data->REXByte = *Ip++;

		Data->Length++;
	}

	Data->OpcodeSize = 1;
	Data->OpcodeOffset = Data->Length;

	if (*Ip == 0x0F) // Двухбайтный опкод
	{
		Data->OpcodeSize++;

		OpFlag = FlagsTableEx[*(Ip + 1)];

		if (OpFlag == OP_INVALID)
		{
			Data->Flags |= F_INVALID;
			return 0;
		}
		if (OpFlag & OP_EXTENDED) // Трёхбайтовый
			Data->OpcodeSize++;
	}
	else // Однобайтовый
	{
		OpFlag = FlagsTable[*Ip];

		if (OpFlag == OP_INVALID)
		{
			Data->Flags |= F_INVALID;
			return 0;
		}
		if (*Ip >= 0xA0 && *Ip <= 0xA3)
		{
			if (Data->Flags & F_PREFIX67)
				Data->Flags |= F_PREFIX66;
			else
				Data->Flags &= ~F_PREFIX66;
		}
	}

	memcpy(&Data->Opcode, Ip, Data->OpcodeSize); //  Копируем опкод

	Data->Length += Data->OpcodeSize;
	Ip += Data->OpcodeSize;

	if (OpFlag & OP_MODRM)
	{
		Data->Flags |= F_MODRM;
		Data->ModRMByte = *Ip++;
		Data->ModRMOffset = (Data->OpcodeOffset + Data->OpcodeSize);
		Data->MODRM.Mod = (Data->ModRMByte >> 6);
		Data->MODRM.Reg = (Data->ModRMByte & 0x38) >> 3;
		Data->MODRM.Rm = (Data->ModRMByte & 7);

		Data->Length++;

		if (Data->MODRM.Reg <= 1)
		{
			if (Data->Opcode[0] == 0xF6)
				OpFlag |= OP_DATA_I8;
			if (Data->Opcode[0] == 0xF7)
				OpFlag |= OP_DATA_I16_I32_I64;
		}

		if (Data->MODRM.Mod != 3 && Data->MODRM.Rm == 4 && !(!Is64Bit && (Data->Flags & F_PREFIX67))) // SIB
		{
			Data->Flags |= F_SIB;
			Data->SIBByte = *Ip++;
			Data->SIBOffset = (Data->ModRMOffset + 1);
			Data->SIB.Scale = (Data->SIBByte >> 6);
			Data->SIB.Index = (Data->SIBByte & 0x38) >> 3;
			Data->SIB.Base = (Data->SIBByte & 7);

			Data->Length++;
		}

		switch (Data->MODRM.Mod)
		{
		case 0:
			if (Data->MODRM.Rm == 5)
			{
				Data->DisplacementSize = 4;
				if (Is64Bit)
					Data->Flags |= F_RELATIVE;
			}
			if (Data->SIB.Base == 5)
				Data->DisplacementSize = 4;

			if ((Data->MODRM.Rm == 6) && (Data->Flags & F_PREFIX67))
				Data->DisplacementSize = 2;
			break;
		case 1:
			Data->DisplacementSize = 1;
			break;
		case 2:
			if (Data->Flags & F_PREFIX67)
				Data->DisplacementSize = 2;
			else
				Data->DisplacementSize = 4;
		}
	}

	if (Data->DisplacementSize > 0)
	{
		Data->Flags |= F_DISP;
		Data->DisplacementOffset = Data->Length;
		Data->Length += Data->DisplacementSize;

		switch (Data->DisplacementSize)
		{
		case 1:
			Data->AddressDisplacement.Displacement08 = *Ip;
			break;
		case 2:
			Data->AddressDisplacement.Displacement16 = *(uint16_t *)Ip;
			break;
		case 4:
			Data->AddressDisplacement.Displacement32 = *(uint32_t *)Ip;
			break;
		}
		Ip += Data->DisplacementSize;
	}

	if (OpFlag & OP_DATA_I8)
	{
		Data->ImmediateDataSize = 1;
	}
	else if (OpFlag & OP_DATA_I16)
	{
		Data->ImmediateDataSize = 2;
	}
	else if (OpFlag & OP_DATA_I16_I32)
	{
		if (Data->Flags & F_PREFIX66)
			Data->ImmediateDataSize = 2;
		else
			Data->ImmediateDataSize = 4;
	}
	else if (OpFlag & OP_DATA_I16_I32_I64)
	{
		if (Data->Flags & F_PREFIX66)
		{
			if (Data->REXByte)
				Data->ImmediateDataSize = 4;
			else
				Data->ImmediateDataSize = 2;
		}
		if (Data->REX.W)
			Data->ImmediateDataSize = 8;
		else
			Data->ImmediateDataSize = 4;
	}

	if (Data->ImmediateDataSize > 0)
	{
		Data->Flags |= F_IMM;
		Data->ImmediateDataOffset = Data->Length;
		Data->Length += Data->ImmediateDataSize;

		if (OpFlag & OP_RELATIVE)
			Data->Flags |= F_RELATIVE;

		switch (Data->ImmediateDataSize)
		{
		case 1:
			Data->ImmediateData.ImmediateData08 = *Ip;
			break;
		case 2:
			Data->ImmediateData.ImmediateData16 = *(uint16_t *)Ip;
			break;
		case 4:
			Data->ImmediateData.ImmediateData32 = *(uint32_t *)Ip;
			break;
		case 8:
			Data->ImmediateData.ImmediateData64 = *(uint64_t *)Ip;
			break;
		}
		Ip += Data->ImmediateDataSize;
	}

	if (Data->Length > MAX_INSTRUCTION_SIZE)
	{
		Data->Flags |= F_INVALID;
		return 0;
	}
	return Data->Length;
}

/*
 */
uint32_t GetSizeOfProc(void *Address, uint8_t Is64Bit)
{
	TLengthDisasm Data = {0};

	uint8_t Size = 0;
	uint32_t Result = 0;
	uint8_t *Offset = Address;

	while (Size = LengthDisasm(Offset, Is64Bit, &Data))
	{
		Result += Size;

		if ((Data.Opcode[0] == 0xC3) || (Data.Opcode[0] == 0xC2)) // ret или retn
			break;

		Offset += Size;
	}
	return Result;
}

/*
 */
uint8_t LengthAssemble(void *Buffer, PLengthDisasm Data)
{
	if (!Buffer || !Data)
		return 0;

	uint8_t *pCode = Buffer;

	if (Data->Flags & F_PREFIX)
	{
		memcpy(pCode, (uint8_t *)&Data->Prefix, Data->PrefixesCount);

		pCode += Data->PrefixesCount;
	}

	if (Data->Flags & F_REX)
	{
		*pCode = Data->REXByte;

		pCode++;
	}

	memcpy(pCode, (uint8_t *)&Data->Opcode, Data->OpcodeSize);

	pCode += Data->OpcodeSize;

	if (Data->Flags & F_MODRM)
	{
		*pCode = Data->ModRMByte;

		pCode++;
	}

	if (Data->Flags & F_SIB)
	{
		*pCode = Data->SIBByte;

		pCode++;
	}

	if (Data->Flags & F_DISP)
	{
		memcpy(pCode, (uint8_t *)&Data->AddressDisplacement, Data->DisplacementSize);

		pCode += Data->DisplacementSize;
	}

	if (Data->Flags & F_IMM)
	{
		memcpy(pCode, (uint8_t *)&Data->ImmediateData, Data->ImmediateDataSize);

		pCode += Data->ImmediateDataSize;
	}
	return pCode - (uint8_t *)Buffer;
}