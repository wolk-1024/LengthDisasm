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

#ifndef _H_LDASM_
#define _H_LDASM_

#include <stdint.h>

#define OP_NONE 0x00
#define OP_DATA_I8 0x01
#define OP_DATA_I16 0x02
#define OP_DATA_I16_I32 0x04
#define OP_DATA_I16_I32_I64 0x08
#define OP_EXTENDED 0x10
#define OP_RELATIVE 0x20
#define OP_MODRM 0x40
#define OP_PREFIX 0x80
#define OP_INVALID 0xff

#define F_INVALID 0x00000001
#define F_PREFIX 0x00000002
#define F_REX 0x00000004
#define F_MODRM 0x00000008
#define F_SIB 0x00000010
#define F_DISP 0x00000020
#define F_IMM 0x00000040
#define F_RELATIVE 0x00000080

#define F_PREFIX_LOCK 0x00000100
#define F_PREFIX_REPNZ 0x00000200
#define F_PREFIX_REPX 0x00000400
#define F_PREFIX_SEG 0x00000800
#define F_PREFIX66 0x00001000
#define F_PREFIX67 0x00002000

#define MAX_PREFIXES 4			// Максимальное число префиксов на одну инструкцию.
#define MAX_OPCODE_SIZE 3		// Максимальный размер опкода инструкции.
#define MAX_INSTRUCTION_SIZE 15 // Максимальный размер инструкции.

typedef enum TPrefixes
{
	LockPrefix = 0xF0,
	RepneRepnzPrefix = 0xF2,
	RepeRepzPrefix = 0xF3,
	CSOverridePrefix = 0x2E,
	SSOverridePrefix = 0x36,
	DSOverridePrefix = 0x3E,
	ESOverridePrefix = 0x26,
	FSOverridePrefix = 0x64,
	GSOverridePrefix = 0x65,
	OperandSizeOverridePrefix = 0x66,
	AddressSizeOverridePrefix = 0x67
} TPrefixes;

typedef struct TLengthDisasm
{
	uint8_t Length;
	uint8_t PrefixesCount;
	TPrefixes Prefix[MAX_PREFIXES];
	union
	{
		uint8_t REXByte;
		struct
		{
			uint8_t B : 1;
			uint8_t X : 1;
			uint8_t R : 1;
			uint8_t W : 1;
		} REX;
	};
	uint8_t OpcodeOffset;
	uint8_t OpcodeSize;
	uint8_t Opcode[MAX_OPCODE_SIZE];
	uint8_t ModRMOffset;
	uint8_t ModRMByte;
	struct
	{
		uint8_t Mod : 2;
		uint8_t Reg : 3;
		uint8_t Rm : 3;
	} MODRM;
	uint8_t SIBOffset;
	uint8_t SIBByte;
	struct
	{
		uint8_t Scale : 2;
		uint8_t Index : 3;
		uint8_t Base : 3;
	} SIB;
	uint8_t DisplacementOffset;
	uint8_t DisplacementSize;
	union
	{
		uint8_t Displacement08;
		uint16_t Displacement16;
		uint32_t Displacement32;
	} AddressDisplacement;
	uint8_t ImmediateDataOffset;
	uint8_t ImmediateDataSize;
	union
	{
		uint8_t ImmediateData08;
		uint16_t ImmediateData16;
		uint32_t ImmediateData32;
		uint64_t ImmediateData64;
	} ImmediateData;
	uint32_t Flags;
} TLengthDisasm, *PLengthDisasm;

#ifdef __cplusplus
extern "C"
{
#endif

	uint8_t LengthDisasm(void *Address, uint8_t Is64Bit, PLengthDisasm Data);
	uint32_t GetSizeOfProc(void *Address, uint8_t Is64Bit);
	uint8_t LengthAssemble(void *Buffer, PLengthDisasm Data);

#ifdef __cplusplus
}
#endif

#endif /* _H_LDASM_ */