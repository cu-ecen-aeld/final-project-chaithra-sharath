//
// Nokia 5110 LCD display test program
//
// Written by Larry Bank 4/6/2016
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "nokia5110.h"



/*extern unsigned char ucFont[], ucSmallFont[];
static int iScreenOffset; // current write offset of screen data
static unsigned char ucScreen[504]; // local copy of the image buffer
static int file_spi = 0;
static int iDCPin, iResetPin, iLEDPin;


#define LOW 0
#define HIGH 1

static void nokiaWriteCommand(unsigned char);
int nokiaFill(unsigned char);

typedef enum
{
 MODE_DATA = 0,
 MODE_COMMAND
} DC_MODE;

*/
int main(int argc, char* argv[])
{
int rc;
int i, x, y;

	// SPI Channel, D/C, RESET, LED
	rc = nokiaInit(0, 37, 35, 13);
	if (rc != 0)
	{
		printf("Problem initializing nokia5110 library\n");
		return 0;
	}
	nokiaWriteString(2, 1, "Another", FONT_NORMAL);
	nokiaWriteString(2, 2, "BitBank", FONT_NORMAL);
	nokiaWriteString(1, 3, "FOSS tool", FONT_NORMAL);
	nokiaWriteString(2, 4, "Narrow Font", FONT_SMALL);
	// draw a box around the whole display
	for (x=0; x<84; x++)
	{
		nokiaSetPixel(x, 0, 1);
		nokiaSetPixel(x, 47, 1);
	}
	for (y=0; y<48; y++)
	{
		nokiaSetPixel(0, y, 1);
		nokiaSetPixel(83, y, 1);
	}
	usleep(3000000);
	for (i=0; i<10000; i++)
	{
		x = rand() & 0x7f;
		y = rand() & 0x3f;
		// cover entire display with black
		while (nokiaGetPixel(x,y))
		{
			x++;
			if (x >= 84)
			{
				y++;
				x = 0;
				if (y >= 48)
					break;
			}
		}
		nokiaSetPixel(x, y, 1);	
	}
	usleep(4000000);
	nokiaShutdown();

   return 0;
} /* main() */


/*


// Sets the D/C pin to data or command mode
void nokiaSetMode(int iMode)
{
	AIOWriteGPIO(iDCPin, (iMode == MODE_DATA));
} 

// Opens a file system handle to the SPI device
// Initializes the LCD controller into "horizontal write mode"
// Prepares the font data for the orientation of the display
// Parameters: GPIO pin numbers used for the DC/RST/LED control lines
//
int nokiaInit(int iChannel, int iDC, int iReset, int iLED)
{

        iDCPin = iDC;
        iResetPin = iReset;
        iLEDPin = iLED;

        if (!AIOInit())
	{
		fprintf(stderr, "Error initializing ArmbianIO library\n");
		return -1;
	}
	file_spi = AIOOpenSPI(iChannel, 4000000); // Initialize SPI channel at 4Mhz
	if (file_spi == -1)
	{
		fprintf(stderr, "Failed to open the SPI bus\n");
		file_spi = 0;
		return -1;
	}

        AIOAddGPIO(iDCPin, GPIO_OUT);
        AIOAddGPIO(iResetPin, GPIO_OUT);
	AIOAddGPIO(iLEDPin, GPIO_OUT);

	// Start by reseting the LCD controller
        AIOWriteGPIO(iResetPin, HIGH);
	usleep(50000);
	AIOWriteGPIO(iResetPin, LOW);
	usleep(5000);
	AIOWriteGPIO(iResetPin, HIGH); // take it out of reset
	nokiaBacklight(1); // turn on the backlight

	nokiaSetMode(MODE_COMMAND);
	nokiaWriteCommand(0x21); // set advanced commands
	nokiaWriteCommand(0xa4); // set LCD Vop (contrast)
	nokiaWriteCommand(0xb2); // set normal contrast
	nokiaWriteCommand(0x04); // set temperature coefficient
	nokiaWriteCommand(0x14); // set LCD bias mode 1:40
	nokiaWriteCommand(0x20); // set simple command mode
	nokiaWriteCommand(0x0c); // set display mode to normal

	nokiaFill(0); // erase memory
	return 0;

}

// Controls the LED backlight
void nokiaBacklight(int bOn)
{
	AIOWriteGPIO(iLEDPin, (bOn) ? LOW:HIGH);
} 

// Sends a command to turn off the LCD display
// Closes the SPI file handle
void nokiaShutdown()
{
	if (file_spi != 0)
	{
		nokiaSetMode(MODE_COMMAND);
		nokiaBacklight(0); // turn off the backlight
		nokiaWriteCommand(0x24); // power down
		AIOCloseSPI(file_spi);
		file_spi = 0;
	}
}

void nokiaWriteCommand(unsigned char c)
{
unsigned char buf[2];

	nokiaSetMode(MODE_COMMAND);
	buf[0] = c;
	AIOWriteSPI(file_spi, buf, 1);
} 

int nokiaSetContrast(unsigned char ucContrast)
{
        if (file_spi == 0)
                return -1;

	nokiaWriteCommand(0x21); // set advanced command mode
	nokiaWriteCommand(0xB0 | ucContrast); // set contrast
	nokiaWriteCommand(0x20); // set simple command mode
	return 0;
} 

// Send commands to position the "cursor" to the given
// row and column
static void nokiaSetPosition(int x, int y)
{
	nokiaWriteCommand(0x40 | y); // set Y
	nokiaWriteCommand(0x80 | x); // set X
	iScreenOffset = (y*84)+x;
}

// Write a block of pixel data to the LCD
// Length can be anything from 1 to 504 (whole display)
static void nokiaWriteDataBlock(unsigned char *ucBuf, int iLen)
{

	nokiaSetMode(MODE_DATA);
	AIOWriteSPI(file_spi, ucBuf, iLen);
	// Keep a copy in local buffer
	memcpy(&ucScreen[iScreenOffset], ucBuf, iLen);
	iScreenOffset += iLen;
}

// Set (or clear) an individual pixel
// The local copy of the frame buffer is used to avoid
// reading data from the display controller
int nokiaSetPixel(int x, int y, unsigned char ucColor)
{
int i;
unsigned char uc, ucOld;

	if (file_spi == 0)
		return -1;

	i = ((y >> 3) * 84) + x;
	if (i < 0 || i > 503) // off the screen
		return -1;
	uc = ucOld = ucScreen[i];
	uc &= ~(0x1 << (y & 7));
	if (ucColor)
	{
		uc |= (0x1 << (y & 7));
	}
	if (uc != ucOld) // pixel changed
	{
		nokiaSetPosition(x, y>>3);
		nokiaWriteDataBlock(&uc, 1);
	}
	return 0;
} 

//
// Read the pixel at the given x,y
// if the library is not initialized, or the coordinates are
// not within the valid range, it returns 0
//
int nokiaGetPixel(int x, int y)
{
int i;

   if (file_spi == 0)
	return 0; // invalid request returns "black" pixel
   i = ((y >> 3) * 84) + x;
   if (i < 0 || i > 503) // off the screen
	return 0;
   return (ucScreen[i] & (1<< (y & 7)));
} 

// Draw a string of small (8x8) or large (16x24) characters
// At the given col+row
int nokiaWriteString(int x, int y, char *szMsg, int iSize)
{
int i, iLen;
unsigned char *s;

	if (file_spi == 0) return -1; // not initialized

	iLen = strlen(szMsg);
	if (iSize == FONT_BIG) // draw 16x32 font
	{
		if (iLen+x > 5) iLen = 5-x;
		if (iLen < 0) return -1;
		for (i=0; i<iLen; i++)
		{
			s = &ucFont[9728 + (unsigned char)szMsg[i]*64];
			nokiaSetPosition(x+(i*16), y);
			nokiaWriteDataBlock(s, 16);
			nokiaSetPosition(x+(i*16), y+1);
			nokiaWriteDataBlock(s+16, 16);	
			nokiaSetPosition(x+(i*16), y+2);
			nokiaWriteDataBlock(s+32, 16);	
		}
	}
	else if (iSize == FONT_NORMAL) // draw 8x8 font
	{
		nokiaSetPosition(x*8, y);
		if ((8*iLen) + x*8 > 84) iLen = (84 - x)/8; // can't display it all
		if (iLen < 0)return -1;

		for (i=0; i<iLen; i++)
		{
			s = &ucFont[(unsigned char)szMsg[i] * 8];
			nokiaWriteDataBlock(s, 8); // write character pattern
		}	
	}
	else // small
	{
		nokiaSetPosition(x*6, y);
		if ((6*iLen) + x*6 > 84) iLen = (84 - x)/6;
		if (iLen < 0) return -1;
		for (i=0; i<iLen; i++)
		{
			s = &ucSmallFont[(unsigned char)szMsg[i]*6];
			nokiaWriteDataBlock(s, 6);
		}
	}
	return 0;
} 

// Fill the frame buffer with a byte pattern
// e.g. all off (0x00) or all on (0xff)
int nokiaFill(unsigned char ucData)
{
int y;
unsigned char temp[84];

	if (file_spi == 0) return -1; // not initialized

	memset(temp, ucData, 84);
	for (y=0; y<6; y++)
	{
		nokiaSetPosition(0,y); // set to (0,Y)
		nokiaWriteDataBlock(temp, 84); // fill with data byte
	} // for y
	return 0;
} 
*/
