/*
 * Safe.c
 *
 * Created: 01/08/2023 02:52:47 م
 *  Author: Alaa Mohamed
 */ 


#define  F_CPU 8000000UL
#include <util/delay.h>
#include "EEPROM_Driver.h"
#include "LCD.h"
#include "KEYPAD.h"
#include "TIMER.h"

#define EEPROM_STATUS_LOCATION 0x10
#define PASSWORD_LOCATION1 0x11
#define PASSWORD_LOCATION2 0x12
#define PASSWORD_LOCATION3 0x13
#define PASSWORD_LOCATION4 0x14
#define ADMIN_PASSWORD_LOCATION1 0x21
#define ADMIN_PASSWORD_LOCATION2 0x22
#define ADMIN_PASSWORD_LOCATION3 0x23
#define ADMIN_PASSWORD_LOCATION4 0x24
#define DEFAULT 0xff
#define MAX_TRIES 3
unsigned char admin_password[4], password[4];


int main(void)
{
    unsigned char correct_flag = 0, admin_correct_flag = 0;
	unsigned char counter, character = NOT_PRESSED;
	unsigned char PIN_tries = MAX_TRIES, admin_tries = MAX_TRIES;	
	LCD_Initialize();
	KEYPAD_Initialize();
	
	EEPROM_Write(ADMIN_PASSWORD_LOCATION1,'1');
	EEPROM_Write(ADMIN_PASSWORD_LOCATION2,'2');
	EEPROM_Write(ADMIN_PASSWORD_LOCATION3,'3');	
	EEPROM_Write(ADMIN_PASSWORD_LOCATION4,'4');
	
	label: LCD_SendCommand(CLEAR_SCREEN);
	LCD_SendCommand(RETURN_HOME);
	LCD_SendString("1: Admin Mode");
	LCD_MoveCursor(2,1);
	LCD_SendString("2: Open Safe");
	do 
	{
		character = KEYPAD_ReadPress();
	} while (character==NOT_PRESSED);
	_delay_ms(200);
	
	if (character == '1')
	{
		while (admin_correct_flag == 0)
		{
			LCD_SendCommand(CLEAR_SCREEN);
			LCD_SendCommand(RETURN_HOME);
			LCD_SendString("Admin Password:");
			LCD_MoveCursor(2,1);
			for (counter = 0; counter < 4; counter++)
			{
				do
				{
					admin_password[counter] = KEYPAD_ReadPress();
				} while (admin_password[counter]==NOT_PRESSED);
				LCD_SendCharacter(admin_password[counter]);
				_delay_ms(500);
				LCD_MoveCursor(2,counter+1);
				LCD_SendCharacter('*');
			}
			if((EEPROM_Read(ADMIN_PASSWORD_LOCATION1)==admin_password[0])&&(EEPROM_Read(ADMIN_PASSWORD_LOCATION2)==admin_password[1])&&(EEPROM_Read(ADMIN_PASSWORD_LOCATION3)==admin_password[2])&&(EEPROM_Read(ADMIN_PASSWORD_LOCATION4)==admin_password[3]))
			{
				LCD_SendCommand(CLEAR_SCREEN);
				LCD_SendCommand(RETURN_HOME);
				LCD_SendString("Correct Password");
				admin_correct_flag = 1;
				_delay_ms(1000);
				label2:LCD_SendCommand(CLEAR_SCREEN);
				LCD_SendCommand(RETURN_HOME);
				LCD_SendString("1: Change Pass");
				LCD_MoveCursor(2,1);
				LCD_SendString("2: Change PIN");
				character = NOT_PRESSED;
				do
				{
					character = KEYPAD_ReadPress();
				} while (character==NOT_PRESSED);
				_delay_ms(200);
				if (character == '1')
				{
					admin_password[0]=admin_password[1]=admin_password[2]=admin_password[3]=NOT_PRESSED;
					LCD_SendCommand(CLEAR_SCREEN);
					LCD_SendCommand(RETURN_HOME);
					LCD_SendString("New Admin Pass: ");
					LCD_MoveCursor(2,1);
					for (counter = 0; counter < 4; counter++)
					{
						do
						{
							admin_password[counter] = KEYPAD_ReadPress();
						} while (admin_password[counter]==NOT_PRESSED);
						LCD_SendCharacter(admin_password[counter]);
						_delay_ms(500);
						LCD_MoveCursor(2,counter+1);
						LCD_SendCharacter('*');
						EEPROM_Write(ADMIN_PASSWORD_LOCATION1+counter,admin_password[counter]);
					}
					LCD_SendCommand(CLEAR_SCREEN);
					LCD_SendCommand(RETURN_HOME);
					LCD_SendString("New Password Set");
					_delay_ms(1000);
					label3:LCD_SendCommand(CLEAR_SCREEN);
					LCD_SendCommand(RETURN_HOME);
					LCD_SendString("1: Home Screen");
					LCD_MoveCursor(2,1);
					LCD_SendString("2: Last Page");
					character = NOT_PRESSED;
					do
					{
						character = KEYPAD_ReadPress();
					} while (character==NOT_PRESSED);
					_delay_ms(200);
					if (character == '1')
					{
						goto label;
					}
					else if (character == '2')
					{
						goto label2;
					}
					else
					{
						LCD_SendCommand(CLEAR_SCREEN);
						LCD_SendCommand(RETURN_HOME);
						LCD_SendString("Wrong Choice");
						_delay_ms(1000);
						LCD_SendCommand(CLEAR_SCREEN);
						LCD_SendCommand(RETURN_HOME);
						goto label3;
					}
				}
				else if(character == '2')
				{
					if (EEPROM_Read(EEPROM_STATUS_LOCATION) == DEFAULT)
					{
						LCD_SendCommand(CLEAR_SCREEN);
						LCD_SendCommand(RETURN_HOME);
						LCD_SendString("Set PIN:");
						LCD_MoveCursor(2,1);
						character = NOT_PRESSED;
						for (counter = 0; counter < 4; counter++)
						{
							do
							{
								character = KEYPAD_ReadPress();
							} while (character==NOT_PRESSED);
							LCD_SendCharacter(character);
							_delay_ms(500);
							LCD_MoveCursor(2,counter+1);
							LCD_SendCharacter('*');
							EEPROM_Write(PASSWORD_LOCATION1+counter,character);
						}
						EEPROM_Write(EEPROM_STATUS_LOCATION,0x00);
						LCD_SendCommand(CLEAR_SCREEN);
						LCD_SendCommand(RETURN_HOME);
						LCD_SendString("1: Home Screen");
						LCD_MoveCursor(2,1);
						LCD_SendString("2: Last Page");
						character = NOT_PRESSED;
						do
						{
							character = KEYPAD_ReadPress();
						} while (character==NOT_PRESSED);
						_delay_ms(200);
						if (character == '1')
						{
							goto label;
						}
						else if (character == '2')
						{
							goto label2;
						}
						else
						{
							LCD_SendCommand(CLEAR_SCREEN);
							LCD_SendCommand(RETURN_HOME);
							LCD_SendString("Wrong Choice");
							_delay_ms(1000);
							LCD_SendCommand(CLEAR_SCREEN);
							LCD_SendCommand(RETURN_HOME);
							goto label3;
						}
					}
					else
					{
						LCD_SendCommand(CLEAR_SCREEN);
						LCD_SendCommand(RETURN_HOME);
						LCD_SendString("Enter New PIN:");
						LCD_MoveCursor(2,1);
						character = NOT_PRESSED;
						for (counter = 0; counter < 4; counter++)
						{
							do
							{
								character = KEYPAD_ReadPress();
							} while (character==NOT_PRESSED);
							LCD_SendCharacter(character);
							_delay_ms(500);
							LCD_MoveCursor(2,counter+1);
							LCD_SendCharacter('*');
							EEPROM_Write(PASSWORD_LOCATION1+counter,character);
						}
						LCD_SendCommand(CLEAR_SCREEN);
						LCD_SendCommand(RETURN_HOME);
						LCD_SendString("1: Home Screen");
						LCD_MoveCursor(2,1);
						LCD_SendString("2: Last Page");
						character = NOT_PRESSED;
						do
						{
							character = KEYPAD_ReadPress();
						} while (character==NOT_PRESSED);
						_delay_ms(200);
						if (character == '1')
						{
							goto label;
						}
						else if (character == '2')
						{
							goto label2;
						}
						else
						{
							LCD_SendCommand(CLEAR_SCREEN);
							LCD_SendCommand(RETURN_HOME);
							LCD_SendString("Wrong Choice");
							_delay_ms(1000);
							LCD_SendCommand(CLEAR_SCREEN);
							LCD_SendCommand(RETURN_HOME);
							goto label3;
						}
					}
				}
				else
				{
					LCD_SendCommand(CLEAR_SCREEN);
					LCD_SendCommand(RETURN_HOME);
					LCD_SendString("Wrong Choice");
					_delay_ms(1000);
					LCD_SendCommand(CLEAR_SCREEN);
					LCD_SendCommand(RETURN_HOME);
					goto label2;
				}
			}
			else
			{
				admin_tries--;
				LCD_SendCommand(CLEAR_SCREEN);
				LCD_SendCommand(RETURN_HOME);
				LCD_SendString("Wrong Password");
				LCD_MoveCursor(2,1);
				LCD_SendString("Tries Left:");
				LCD_SendCharacter(admin_tries+48);
				_delay_ms(1000);
				if (admin_tries == 0)
				{
					LCD_SendCommand(CLEAR_SCREEN);
					LCD_SendCommand(RETURN_HOME);
					LCD_SendString("Safe is closed");
					break;
				}
			}							
		}
	}
		
	else if (character == '2')
	{
		if (EEPROM_Read(EEPROM_STATUS_LOCATION) == DEFAULT)
		{
			LCD_SendCommand(CLEAR_SCREEN);
			LCD_SendCommand(RETURN_HOME);
			LCD_SendString("Set PIN:");
			LCD_MoveCursor(2,1);
			character = NOT_PRESSED;
			for (counter = 0; counter < 4; counter++)
			{
				do
				{
					character = KEYPAD_ReadPress();
				} while (character==NOT_PRESSED);
				LCD_SendCharacter(character);
				_delay_ms(500);
				LCD_MoveCursor(2,counter+1);
				LCD_SendCharacter('*');
				EEPROM_Write(PASSWORD_LOCATION1+counter,character);
			}
			EEPROM_Write(EEPROM_STATUS_LOCATION,0x00);
			label4:LCD_SendCommand(CLEAR_SCREEN);
			LCD_SendCommand(RETURN_HOME);
			LCD_SendString("1: Home Screen");
			character = NOT_PRESSED;
			do
			{
				character = KEYPAD_ReadPress();
			} while (character==NOT_PRESSED);
			_delay_ms(200);
			if (character == '1')
			{
				goto label;
			}
			else
			{
				LCD_SendCommand(CLEAR_SCREEN);
				LCD_SendCommand(RETURN_HOME);
				LCD_SendString("Wrong Choice");
				_delay_ms(1000);
				LCD_SendCommand(CLEAR_SCREEN);
				LCD_SendCommand(RETURN_HOME);
				goto label4;
			}
		}
		while(correct_flag == 0)
		{
			LCD_SendCommand(CLEAR_SCREEN);
			LCD_SendCommand(RETURN_HOME);
			LCD_SendString("Enter PIN:");
			LCD_MoveCursor(2,1);
			for (counter = 0; counter < 4; counter++)
			{
				do
				{
					password[counter] = KEYPAD_ReadPress();
				} while (password[counter]==NOT_PRESSED);
				LCD_SendCharacter(password[counter]);
				_delay_ms(500);
				LCD_MoveCursor(2,counter+1);
				LCD_SendCharacter('*');
			}
			if ((EEPROM_Read(PASSWORD_LOCATION1)==password[0])&&(EEPROM_Read(PASSWORD_LOCATION2)==password[1])&&(EEPROM_Read(PASSWORD_LOCATION3)==password[2])&&(EEPROM_Read(PASSWORD_LOCATION4)==password[3]))
			{
				LCD_SendCommand(CLEAR_SCREEN);
				LCD_SendCommand(RETURN_HOME);
				LCD_SendString("Correct PIN");
				LCD_MoveCursor(2,1);
				LCD_SendString("Safe is opening");
				_delay_ms(1000);
				TIMER1_FASTPWMWave(2);
				correct_flag = 1;
			}
			else
			{
				PIN_tries--;
				LCD_SendCommand(CLEAR_SCREEN);
				LCD_SendCommand(RETURN_HOME);
				LCD_SendString("Wrong PIN");
				LCD_MoveCursor(2,1);
				LCD_SendString("Tries Left:");
				LCD_SendCharacter(PIN_tries+48);
				_delay_ms(1000);
				if (PIN_tries == 0)
				{
					LCD_SendCommand(CLEAR_SCREEN);
					LCD_SendCommand(RETURN_HOME);
					LCD_SendString("Safe is closed");
					break;
				}
				_delay_ms(1000);
			}
		}
	}
	else
	{
		LCD_SendCommand(CLEAR_SCREEN);
		LCD_SendCommand(RETURN_HOME);
		LCD_SendString("Wrong Choice");
		_delay_ms(1000);
		LCD_SendCommand(CLEAR_SCREEN);
		LCD_SendCommand(RETURN_HOME);
		goto label;
	}	
}