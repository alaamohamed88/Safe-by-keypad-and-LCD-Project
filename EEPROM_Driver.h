/*
 * EEPROM_Driver.h
 *
 * Created: 24/06/2023 04:54:39 م
 *  Author: Alaa Mohamed
 */ 


#ifndef EEPROM_DRIVER_H_
#define EEPROM_DRIVER_H_

#include "DIO_Driver.h"

#define MyEEARL *(volatile unsigned char*) 0x3E
#define MyEEARH *(volatile unsigned char*) 0x3F
#define MyEEDR *(volatile unsigned char*) 0x3D
#define MyEECR *(volatile unsigned char*) 0x3C
#define MyEERE 0
#define MyEEWE 1
#define MyEEMWE 2

void EEPROM_Write(unsigned short EEPROM_address,unsigned char stored_data);
unsigned char EEPROM_Read(unsigned short EEPROM_address);

#endif /* EEPROM_DRIVER_H_ */