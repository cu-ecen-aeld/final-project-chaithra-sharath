#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <gpiod.h>
#include "solarc.h"

//define GPIO 0 as input from solar cell
#define SOLAR_IN 0
#define SOLAR_IN_2 5

//define GPIO 2 to reflect whatever is read at GPIO0
#define OUT 2



//Declare variables requried
int i=0, j=0;

/**
 * @brief: function to generate a delay
 * @param: unsigned int milliseconds
 * @return: void
 */
void delay(unsigned milliseconds)
{
    clock_t end;
    clock_t begin;

    end = milliseconds * (CLOCKS_PER_SEC / 1000);
    begin = clock();
    while( (clock() - begin) < end);
}

/**
 * @brief: function to read the state of 
 * 	   Gpio pin based on the output of
 *	   of the solar cell. Fills up the buffer	
 *	   that is sent as argument with the values
 *	   detected by the solar cell from the 16x16
 *	   LED array
 * @param: int *buffer
 * @return: int *
 *
 */	
int* solar_read(int *buffer) {
	//Measure voltage with a delay of 500ms as programmed at the array
	
	while(i<array_ind) {
	
	for(int j=0; j<16; j++){	
		int read=0, read2=0;
		
		//this command reads the solar cell output
		read = gpio_read(SOLAR_IN);
		read2 = gpio_read(SOLAR_IN_2);

		//Write itback to GPIO 2
		gpio_write(OUT, read);

		buffer[i] = (read || read2);
		
		i++;
		delay(5);
	}
	
	for(int j=16; j>0; j--){
	
		int read=0, read2=0;
		
		//this command reads the solar cell output
		read = gpio_read(SOLAR_IN);
		read2 = gpio_read(SOLAR_IN_2);

		//Write it back to GPIO 2 for debugging purposes
		gpio_write(OUT, read);

		buffer[i+j] = (read || read2);
		
		delay(5);
		}
		i=i+16;
	}
	
	buffer[5*16] =0;
	buffer[7*16] =0;
	buffer[13*16] =0;
	
	int temp = buffer[(6*16)];
	
	// buffer clean up
	for(int y=0; y<15; y++)
	{
	
		buffer[96+y]= buffer[96+y+1];
	
	
	}
	return buffer;
}

