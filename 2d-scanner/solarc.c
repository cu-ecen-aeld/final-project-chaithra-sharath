#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gpiod.h>
#include "solarc.h"

//define GPIO 0 as input from solar cell
#define SOLAR_IN 0

//define GPIO 2 to reflect whatever is read at GPIO0
#define OUT 2



//Declare variables requried
int i=0, j=0;


int* solar_read(int buffer[array_ind][array_ind]) {
	//int buffer[array_ind][array_ind];
	//Measure voltage with a delay of 500ms as programmed at the array
	while(i<array_ind) {
	
		int read;
		
		//this command reads the solar cell output
		read = gpio_read(SOLAR_IN);

		//Write itback to GPIO 2
		gpio_write(OUT, read);

		//store the value in a buffer
		buffer[i][j]=read;
		
		//Increment the buffer index
		printf(" %d %d = %d\n", i,j, buffer[i][j]);
		j++;
		if(j==array_ind)
		{
			i++;
			j=0;
		}
	}
	
	return buffer;
}

