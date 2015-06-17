// ####################################################################################################################
// ##### OLED CPP FILE ################################################################################################
// ####################################################################################################################
#include "OLED_I2C_128x64_Monochrome.h"
#include "OLED_I2C_128x64_Monochrome_Font.h"
#include <Wire.h>
#include <avr/pgmspace.h>
// REDEFINE NAME
#define OLED_I2C_128x64_Monochrome Display
// ####################################################################################################################
// ##### INITZIALIZE ##################################################################################################
// ####################################################################################################################
void Display::initialize()														//
{																				//
	Wire.begin();																//
	// upgrade to 400KHz! (only use when your other i2c device support this speed)
	if (I2C_400KHZ)																//
	{																			//
		// save I2C bitrate (default 100Khz)									//
		byte twbrbackup = TWBR;													//
		TWBR = 12; 																//
	}																			//
    setDisplayOff(); 															// display off
    sendCommand(0x8d);															//
	sendCommand(0x14);															//
    setBlackBackground();  														// default Set Normal Display
	setPageMode();																// default addressing mode
	clearDisplay();																//
    setDisplayOn();																// display on
}																				//
// ####################################################################################################################
// ##### SEND COMMAND #################################################################################################
// ####################################################################################################################
void Display::sendCommand(byte Parameter_Command)								//
{																				//
	Wire.beginTransmission(OLED_ADDRESS); 										// begin transmitting
	Wire.write(COMMAND_MODE);													// data mode
	Wire.write(Parameter_Command);												// send command
	Wire.endTransmission();   													// stop transmitting
}																				//
// ####################################################################################################################
// ##### SEND DATA ####################################################################################################
// ####################################################################################################################
void Display::sendData(byte Parameter_Data)										//
{																				//
	Wire.beginTransmission(OLED_ADDRESS); 										// begin transmitting
	Wire.write(OLED_DATA_MODE);													// data mode
	Wire.write(Parameter_Data);													// send data
	Wire.endTransmission();    													// stop transmitting
}																				//
// ####################################################################################################################
// ##### PRINT CHARACTER ##############################################################################################
// ####################################################################################################################
void Display::printChar(char Parameter_Char, byte Parameter_X, byte Parameter_Y)//
{																				//
	if (Parameter_X < 128)														//
	{																			//
		setCursor(Parameter_X, Parameter_Y);									//
	}																			//
	// IGNORE UNUSED ASCCII CHARACTERS 											//
    if (Parameter_Char < 32 || Parameter_Char > 127)							//
    {																			//
		Parameter_Char = '?';													// ?: characters that can't be displayed
    }																			//
    // READ BYTES FROM CODE MEMORY 												//
    for(byte I = 0; I < 8; I++) 												//
    {																			//
       sendData(pgm_read_byte(&Basic_Font[Parameter_Char - 32][I])); 			// font array starts at 0, ASCII starts at 32.
    }																			//
}																				//
// ####################################################################################################################
// ##### PRINT STRING #################################################################################################
// ####################################################################################################################
void Display::printString(const char *Parameter_String, byte Parameter_X, byte Parameter_Y)
{																				//
	if (Parameter_X < 128)														//
	{																			//
		setCursor(Parameter_X, Parameter_Y);									//
	}																			//
	// USE printChar TO PRINT A STRING 											//
	byte I = 0;																	//
    while(Parameter_String[I] && I < 128)										//
    {																			//
		printChar(Parameter_String[I++]);  										//
	}																			//
}																				//
// ####################################################################################################################
// ##### PRINT NUMBER (NON FLOAT VALUE) ###############################################################################
// ####################################################################################################################
byte Display::printNumber(long Parameter_LongNum, byte Parameter_X, byte Parameter_Y)
{																				//
	if (Parameter_X < 128)														//
	{																			//
		setCursor(Parameter_X, Parameter_Y);									//
	}																			//
	byte Buffer[10]   = "";														//
	byte Integer_Part = 0;														//
	byte Float_Part   = 0; 														// number of characters
																				//
	if (Parameter_LongNum < 0) 													//
	{																			//
		Float_Part++;															//
		printChar('-');															//
		Parameter_LongNum = -Parameter_LongNum;									//
	} 																			//
	else if (Parameter_LongNum == 0) 											//
	{																			//
		Float_Part++;															//
		printChar('0');															//
		return Float_Part;														//
	} 																			//
	while (Parameter_LongNum > 0) 												//
	{																			//
		Buffer[Integer_Part++] = Parameter_LongNum % 10;						//
		Parameter_LongNum /= 10;												//
	}																			//
																				//
	Float_Part += Integer_Part;													//
	for(; Integer_Part > 0; Integer_Part--) 									//
	{																			//
		printChar('0' + Buffer[Integer_Part - 1]);								//
	}																			//
	return Float_Part;															//
}																				//
// ####################################################################################################################
// ##### PRINT NUMBER (FLOAT VALUE) ###################################################################################
// ####################################################################################################################
byte Display::printNumber(float Parameter_FloatNum, byte Parameter_Precision, byte Parameter_X, byte Parameter_Y)
{																				//
	if (Parameter_X < 128)														//
	{																			//
		setCursor(Parameter_X, Parameter_Y);									//
	}																			//
	byte Number_Integer  = 0;													// Parameter_Precision - 6 maximum
	byte Number_Fraction = 0;													//
	byte Number_Extra    = 0;													//
																				//
	long Integer_Part = Parameter_FloatNum; 									// get the integer part
	float Float_Part  = Parameter_FloatNum - Integer_Part; 						// get the fractional part
																				//
	if (Integer_Part == 0 && Float_Part < 0.0)									//
	{																			//
		printChar('-');															//
		Number_Extra++;															//
		printChar('0');															//
		Number_Extra++;															//
		Float_Part *= -1;														//
	}																			//
	else if (Integer_Part < 0 && Float_Part < 0.0)								//
	{																			//
		Number_Integer = printNumber(Integer_Part); 							// count how many digits in integer part
		Float_Part *= -1;														//
	}																			//
	else																		//
	{																			//
		Number_Integer = printNumber(Integer_Part); 							// count how many digits in integer part
	}																			//
																				//
	if (Float_Part > 0.0)														// only when fractional part > 0, decimal point is shown
	{																			//
		printChar('.');															//
		Number_Extra++;															//
		long Float_Part_Shift = 1;												//
																				//
		if (Number_Integer + Parameter_Precision > 8) 							//
		{																		//
			Parameter_Precision = 8 - Number_Integer;							//
		}																		//
		for (byte I = 0; I < Parameter_Precision; I++)							//
		{																		//
			Float_Part_Shift *= 10;												//
		}																		//
		Number_Fraction = printNumber((long)(Float_Part * Float_Part_Shift)); 	// count how many digits in fractional part
	}																			//
	return Number_Integer + Number_Fraction + Number_Extra;						//
}																				//
// ####################################################################################################################
// ##### DRAW BITMAP ##################################################################################################
// ####################################################################################################################
void Display::drawBitmap(const byte *Parameter_Bitmaparray, byte Parameter_X, byte Parameter_Y, byte Parameter_Width, byte Parameter_Height)
{																				//
	// max width = 16															//
	// max height = 8															//
	setCursor(Parameter_X, Parameter_Y);										//
																				//
	byte Column = 0; 															//
	for(int I = 0; I < Parameter_Width * 8 * Parameter_Height; I++) 			//
	{  																			//
		sendData(pgm_read_byte(&Parameter_Bitmaparray[I]));						//
		if (++Column == Parameter_Width * 8) 									//
		{																		//
			Column = 0;															//
			setCursor(Parameter_X, ++Parameter_Y);								//
		} 																		//
	}																			//
}																				//
// ####################################################################################################################
// ##### SET CURSOR ###################################################################################################
// ####################################################################################################################
void Display::setCursor(byte Parameter_X, byte Parameter_Y)						//
{																				//
	// Y - 1 unit = 1 page (8 pixel rows)										//
	// X - 1 unit = 8 pixel columns												//
    sendCommand(0x00 + (8 * Parameter_X & 0x0F)); 								// set column lower address
    sendCommand(0x10 + ((8 * Parameter_X >> 4) & 0x0F)); 						// set column higher address
	sendCommand(0xB0 + Parameter_Y); 											// set page address
}																				//
// ####################################################################################################################
// ##### CLEAR DISPLAY ################################################################################################
// ####################################################################################################################
void Display::clearDisplay()													//
{																				//
	for(byte X = 0; X < 8; X++) 												//
	{																			//
		setCursor(0, X);     													//
		for(byte Y = 0; Y < 128; Y++)											// clear all columns
		{  																		//
			sendData(0);    													//
		}																		//
	}																			//
	setCursor(0, 0);  															//
}																				//
// ####################################################################################################################
// ##### ROTATE DISPLAY 180 DEGREES ###################################################################################
// ####################################################################################################################
void Display::rotateDisplay180()												//
{																				//
	sendCommand(COMMAND_MIRROR_VERTICAL);										//
	sendCommand(COMMAND_MIRROR_HORIZONTAL);										//
}																				//
// ####################################################################################################################
// ##### WHITE BACKGROUND #############################################################################################
// ####################################################################################################################
void Display::setWhiteBackground()												//
{																				//
	sendCommand(COMMAND_WHITE_BACKGROUND);										//
}																				//
// ####################################################################################################################
// ##### BLACK BACKGROUND #############################################################################################
// ####################################################################################################################
void Display::setBlackBackground()												//
{																				//
	sendCommand(COMMAND_BLACK_BACKGROUND);										//
}																				//
// ####################################################################################################################
// ##### DISPLAY POWER OFF ############################################################################################
// ####################################################################################################################
void Display::setDisplayOff()													//
{																				//
	sendCommand(COMMAND_DISPLAY_OFF);											//
}																				//
// ####################################################################################################################
// ##### DISPLAY POWER ON #############################################################################################
// ####################################################################################################################
void Display::setDisplayOn()													//
{																				//
	sendCommand(COMMAND_DISPLAY_ON);											//
}																				//
// ####################################################################################################################
// ##### BRIGHTNESS ###################################################################################################
// ####################################################################################################################
void Display::setBrightness(byte Parameter_Brightness)							//
{																				//
	sendCommand(COMMAND_SET_BRIGHTNESS);										//
	sendCommand(Parameter_Brightness); 											//
}																				//
// ####################################################################################################################
// ##### PAGE MODE ####################################################################################################
// ####################################################################################################################
void Display::setPageMode()														//
{																				//
	addressingMode = PAGE_ADDRESSING;											//
	sendCommand(0x20); 															//set addressing mode
	sendCommand(PAGE_ADDRESSING); 												//set page addressing mode
}																				//
// ####################################################################################################################
// ##### HORIZONTAL MODE ##############################################################################################
// ####################################################################################################################
void Display::setHorizontalMode()												//
{																				//
	addressingMode = HORIZONTAL_ADDRESSING;										//
	sendCommand(0x20); 															//set addressing mode
	sendCommand(HORIZONTAL_ADDRESSING); 										//set page addressing mode
}																				//
// ####################################################################################################################
// ##### PREINSTANTIAE OBJECTS ########################################################################################
// ####################################################################################################################
Display lcd;  																	//
// ####################################################################################################################
// ##### END OF CODE ##################################################################################################
// ####################################################################################################################