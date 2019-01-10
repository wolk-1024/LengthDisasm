/*
*  Copyright (c) 2019 Wolk-1024 <wolk1024@gmail.com>
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _LDASM_
#define _LDASM_

#include <stdint.h>

#define OP_NONE             0x00
#define OP_DATA_I8          0x01
#define OP_DATA_I16         0x02
#define OP_DATA_I16_I32     0x04
#define OP_DATA_I16_I32_I64 0x08
#define OP_EXTENDED         0x10
#define OP_RELATIVE         0x20
#define OP_MODRM            0x40     
#define OP_PREFIX           0x80
#define OP_INVALID          0xff

#define	F_INVALID		    0x00000001
#define F_PREFIX		    0x00000002
#define	F_REX			    0x00000004
#define F_MODRM			    0x00000008
#define F_SIB			    0x00000010
#define F_DISP			    0x00000020
#define F_IMM			    0x00000040
#define F_RELATIVE		    0x00000080

#define F_PREFIX_LOCK       0x00000100
#define F_PREFIX_REPNZ      0x00000200
#define F_PREFIX_REPX       0x00000400
#define F_PREFIX_SEG        0x00000800
#define F_PREFIX66          0x00001000
#define F_PREFIX67          0x00002000

#define MAX_PREFIXES 4          // Максимальное число префиксов на одну инструкцию.
#define MAX_OPCODE_SIZE 3       // Максимальный размер опкода инструкции.
#define MAX_INSTRUCTION_SIZE 15 // Максимальный размер инструкции.

#define BitTest(A, B) (A & (1 << B)) > 0;

typedef enum TPrefixes
{
	LockPrefix                = 0xF0,
	RepneRepnzPrefix          = 0xF2,
	RepeRepzPrefix            = 0xF3,
	CSOverridePrefix          = 0x2E,
	SSOverridePrefix          = 0x36,
	DSOverridePrefix          = 0x3E,
	ESOverridePrefix          = 0x26,
	FSOverridePrefix          = 0x64,
	GSOverridePrefix          = 0x65,
	OperandSizeOverridePrefix = 0x66,
	AddressSizeOverridePrefix = 0x67
}
TPrefixes;

typedef struct TInstruction
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
		uint8_t Rm  : 3;
	} MODRM;
	uint8_t SIBOffset;
	uint8_t SIBByte;
	struct
	{
		uint8_t Scale : 2;
		uint8_t Index : 3;
		uint8_t Base  : 3;
	} SIB;
	uint8_t DisplacementOffset;
	uint8_t DisplacementSize;
	union
	{
		uint8_t  Displacement08;
		uint16_t Displacement16;
		uint32_t Displacement32;
	} AddressDisplacement;
	uint8_t ImmediateDataOffset;
	uint8_t ImmediateDataSize;
	union
	{
		uint8_t  ImmediateData08;
		uint16_t ImmediateData16;
		uint32_t ImmediateData32;
		uint64_t ImmediateData64;
	} ImmediateData;
	uint32_t Flags;
} TInstruction, *PInstruction;

#ifdef __cplusplus
extern "C" {
#endif

uint8_t LengthDisasm(void *Address, int Is64Bit, PInstruction Data);
uint32_t GetSizeOfProc(void *Address, int Is64Bit);

#ifdef __cplusplus
}
#endif

#endif /* _LDASM_ */