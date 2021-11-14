/*
 * Reference:  
 * 
 * 
 * 
 */


#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define Solar_IN 0
#define OUT 2
#define array_ind 16


int main(void) {
	

	if (-1 == wiringPiSetup()) {
		printf("wiring Pi set up failed\n");
		return 1;
	}

	pinMode(Solar_IN, INPUT);
	pinMode(OUT, OUTPUT);
	

	
	int read, i=0, j=0;
	int buffer[array_ind][array_ind];

	while(i<array_ind) {
		
		read=digitalRead(Solar_IN);
		

		
		digitalWrite(OUT, read);
		
		
		buffer[i][j]=read;
		
		printf(" %d %d = %d\n", i,j, buffer[i][j]);
		j++;
		if(j==array_ind)
		{
			i++;
			j=0;
		}
		
		delay(500);

		
	}
	
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
