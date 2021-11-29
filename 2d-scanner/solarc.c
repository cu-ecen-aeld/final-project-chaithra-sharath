#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <gpiod.h>
#include "solarc.h"

//define GPIO 0 as input from solar cell
#define SOLAR_IN 0

//define GPIO 2 to reflect whatever is read at GPIO0
#define OUT 2



//Declare variables requried
int i=0, j=0;


void delay(unsigned milliseconds)
{
    clock_t end;
    clock_t begin;

    end = milliseconds * (CLOCKS_PER_SEC / 1000);
    begin = clock();
    while( (clock() - begin) < end);
}

int* solar_read(int *buffer) {
	//int buffer[array_ind][array_ind];
	//Measure voltage with a delay of 500ms as programmed at the array
	
	printf(" \n start time %d \n " ,(int)clock());
	
	while(i<array_ind) {
	
	for(int j=0; j<16; j++){	
		int read;
		
		//this command reads the solar cell output
		read = gpio_read(SOLAR_IN);

		//Write itback to GPIO 2
		gpio_write(OUT, read);

		
		buffer[i] = read;
		
		
		
		i++;
		
		delay(500);
	}
	
	for(int j=16; j>0; j--){	
	
		int read;
		
		//this command reads the solar cell output
		read = gpio_read(SOLAR_IN);

		//Write itback to GPIO 2
		gpio_write(OUT, read);

		
		buffer[i+j] = read;
		
		
		delay(500);
		
	}
	
	
	i=i+16;
	
	}
	printf(" \n stop time %d \n " ,(int)clock());
	return buffer;
}

