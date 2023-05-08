#include "eeprom.h"
#include <string.h>


static uint8_t EepromData[EEPROM_SIZE];


void EE_Initialize(void)
{
	// TODO AR Remove temp memcpy(EepromData, (uint8_t*)EEPROM_START_ADDRESS, EEPROM_SIZE);
}

uint8_t EE_ReadByte(uint16_t VirtAddress)
{
	return 0;// TODO AR Remove temp   EepromData[VirtAddress];
}

void EE_WriteByte(uint16_t VirtAddress, uint8_t Data)
{
	EepromData[VirtAddress] = Data;
}

uint8_t EE_ReadByteArray(uint8_t *DataOut, uint16_t VirtAddress, uint16_t size)
{
	uint8_t data, checksum = 0;

	for(; size > 0; size--) {
// TODO AR Remove temp		data = EE_ReadByte(VirtAddress++);
		checksum = (checksum << 1) | (checksum >> 7);
		checksum += data;
		*(DataOut++) = data;
	}

// TODO AR Remove temp	data = EE_ReadByte(VirtAddress);
	if(data == checksum) {
		return 1;
	}

	return 0;
}

void EE_WriteByteArray(uint16_t VirtAddress, uint8_t *DataIn, uint16_t size)
{
	unsigned char checksum = 0;

	for(; size > 0; size--) {
		checksum = (checksum << 1) | (checksum >> 7);
		checksum += *DataIn;
// TODO AR Remove temp		EE_WriteByte(VirtAddress++, *(DataIn++));
	}

// TODO AR Remove temp	EE_WriteByte(VirtAddress, checksum);
}

void EE_Program(void)
{
	// TODO AR Remove tempEE_Erase();

// TODO AR Remove temp	FLASH_Unlock();

	for(uint16_t i = 0; i < EEPROM_SIZE; ++i) {
// TODO AR Remove temp		FLASH_ProgramByte(EEPROM_START_ADDRESS + i, EepromData[i]);
	}

// TODO AR Remove temp	FLASH_Lock();
}

void EE_Erase(void)
{
// TODO AR Remove temp	FLASH_Unlock();

// TODO AR Remove temp	FLASH_EraseSector(FLASH_SECTOR, VOLTAGE_RANGE);

// TODO AR Remove temp	FLASH_Lock();
}
