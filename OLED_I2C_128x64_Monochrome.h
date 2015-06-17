// ####################################################################################################################
// ##### OLED MONOCHROME HEADER FILE ##################################################################################
// ####################################################################################################################
/*
	DELOARTS RESEARCH INC. 2015

	THIS LIBRARY IS WRITTEN FOR AN 4-PIN I2C MONOCHROME DISPLAY WITH A RESOLUTION OF 128x64 PIXELS.
	THIS LIBRARAY IS ALSO TESTED WITH THE FOLLOWING DISPLAY:

		- 0,96" I2C OLED, Manufacturer: Heltec-Taobao

	THIS LIBRARAY IS NOT TESTED WITH ANY OTHER DISPLAYS, THEREFORE IT IS NOT GUARANTEED, THAT IT WILL
	WORK ON ANY OTHER DEVICE.

	WIRING NOTES (Wire.h on Arduino UNO):

		- SDA -> A4
		- SCL -> A5
		- VCC -> 3.3-5V
		- GND -> Ground
*/	
// ####################################################################################################################
// ##### DEFINE CODE ##################################################################################################
// ####################################################################################################################
#ifndef OLED_data_H																//
#define OLED_data_H																//
																				//
#include <Arduino.h>															//
#include <Wire.h>																//
																				//
#define OLED_Max_X						128										// 128 Pixels
#define OLED_Max_Y						64										// 64  Pixels
																				//
#define OLED_ADDRESS					0x3C									//
#define I2C_400KHZ						1										// 0 to use default 100Khz, 1 for 400Khz
// REGISTERS																	//
#define COMMAND_MODE					0x80									//
#define OLED_DATA_MODE					0x40									//
// COMMANDS 																	//
#define COMMAND_DISPLAY_OFF				0xAE									//
#define COMMAND_DISPLAY_ON				0xAF									//
#define COMMAND_BLACK_BACKGROUND 		0xA6									//
#define COMMAND_WHITE_BACKGROUND		0xA7									//
#define COMMAND_SET_BRIGHTNESS			0x81									//
																				//
#define COMMAND_MIRROR_VERTICAL			0xA0 | 0x1								//
#define COMMAND_MIRROR_HORIZONTAL  		0xC8									//
																				//
#define HORIZONTAL_ADDRESSING			0x00									//
#define PAGE_ADDRESSING					0x02									//
// ####################################################################################################################
// ##### CLASS OLED ###################################################################################################
// ####################################################################################################################
class Display 																	//
{																				//
	public:																		//
		byte addressingMode;													//
																				//
		void initialize();														//
																				//
		void sendCommand(byte Parameter_Command);								//
		void sendData(byte Parameter_Data);										//
																				//
		void printChar(char Parameter_Char, byte Parameter_X = 255, byte Parameter_Y = 255);
		void printString(const char *Parameter_String, byte Parameter_X = 255, byte Parameter_Y = 255);
		byte printNumber(long Parameter_LongNum, byte Parameter_X = 255, byte Parameter_Y = 255);
		byte printNumber(float Parameter_FloatNum, byte Parameter_Precision = 6, byte Parameter_X = 255, byte Parameter_Y = 255);
		// BITMAP NECESSARILY NEEDS TO BE STORED IN PROGRAM STORAGE !!			//
		void drawBitmap(const byte *Parameter_Bitmaparray, byte Parameter_X, byte Parameter_Y, byte Parameter_Width, byte Parameter_Height);
																				//
		void clearDisplay();													//
																				//
		void setCursor(byte Parameter_X, byte Parameter_Y);						//
		void setBlackBackground();												//
		void setWhiteBackground(); 												//
		void setDisplayOff();													//
		void setDisplayOn();													//
		void setPageMode();														//
		void setHorizontalMode();												//
		void setBrightness(byte Parameter_Brightness);							//
																				//
		void rotateDisplay180();												//
};																				//
extern Display lcd; 															// Display object 
#endif																			//
// ####################################################################################################################
// ##### END OF CODE ##################################################################################################
// ####################################################################################################################