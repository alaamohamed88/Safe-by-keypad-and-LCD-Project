/*
 * EEPROM_Driver.c
 *
 * Created: 24/06/2023 04:57:59 م
 *  Author: Alaa Mohamed
 */ 

#include "EEPROM_Driver.h"

void EEPROM_Write(unsigned short EEPROM_address,unsigned char stored_data)
{
	MyEEARL = EEPROM_address;
	MyEEARH = EEPROM_address >> 8;
	MyEEDR = stored_data;
	SET_BIT(MyEECR,MyEEMWE);
	SET_BIT(MyEECR,MyEEWE);
	while(READ_BIT(MyEECR,MyEEWE)==1);
}

unsigned char EEPROM_Read(unsigned short EEPROM_address)
{
	MyEEARL = EEPROM_address;
	MyEEARH = EEPROM_address >> 8;
	SET_BIT(MyEECR,MyEERE);
	return MyEEDR;
}