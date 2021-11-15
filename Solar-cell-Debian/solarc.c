/*
 * Reference:  https://simonprickett.dev/gpio-access-in-c-with-raspberry-pi-traffic-lights/
 * 
 * 
 * 
 */

//Required Headers
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

//define GPIO 0 as input from solar cell
#define Solar_IN 0

//define GPIO 2 to reflect whatever is read at GPIO0
#define OUT 2

//The LED array is 16x16
#define array_ind 16


int main(void) {

	//Declare variables requried
	int read, i=0, j=0;
	int buffer[array_ind][array_ind];
	
	//Check if WiringPi set up is successful
	if (-1 == wiringPiSetup()) {
		printf("wiring Pi set up failed\n");
		return 1;
	}
	
	//set appropriate pin modes for the pins
	pinMode(Solar_IN, INPUT);
	pinMode(OUT, OUTPUT);
	

	
	
	//Measure voltage with a delay of 500ms as programmed at the array
	while(i<array_ind) {
		
		//this command reads the solar cell output
		read=digitalRead(Solar_IN);
		

		//Write itback to GPIO 2
		digitalWrite(OUT, read);
		
		//store the value in a buffer
		buffer[i][j]=read;
		
		//Increment the buffer index
		//printf(" %d %d = %d\n", i,j, buffer[i][j]);
		j++;
		if(j==array_ind)
		{
			i++;
			j=0;
		}
		
		delay(500);

		
	}

	//Print the array to show the blocked object
	
	for(i=0; i<array_ind; i++)
	{
		for(j=0;j<array_ind;j++)
		{
			printf(" %d ", buffer[i][j]);
		}
		printf("\n");
		
	}

	
	return 0;
}
