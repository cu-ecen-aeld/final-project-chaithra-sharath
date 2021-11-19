/*
 * Reference:  https://simonprickett.dev/gpio-access-in-c-with-raspberry-pi-traffic-lights/
 * 
 * 
 * 
 */
/*
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
}*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gpiod.h>
struct gpiod_chip *chip;
struct gpiod_line *line;
int rv, value;

//define GPIO 0 as input from solar cell
#define Solar_IN 0

//define GPIO 2 to reflect whatever is read at GPIO0
#define OUT 2

//The LED array is 16x16
#define array_ind 4

//Declare variables requried
int i=0, j=0;
int buffer[array_ind][array_ind];

int readgpio();
int gpiowrite(int pin, int value);

int main(void) {

	
	
		
	//Measure voltage with a delay of 500ms as programmed at the array
	while(i<array_ind) {
	
		int read;
		
		//this command reads the solar cell output
		//read=digitalRead(Solar_IN);
		read = readgpio();
		

		//Write itback to GPIO 2
		//digitalWrite(OUT, read);
		gpiowrite(OUT, read);
		
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

}	
int readgpio()
{
    chip= gpiod_chip_open("/dev/gpiochip0");

    if (!chip)
     return -1;

    line = gpiod_chip_get_line(chip, Solar_IN);

    if (!line) 
    {
     gpiod_chip_close(chip);
     return -1; 
    }

    rv = gpiod_line_request_input(line, "foobar");

    if (rv) 
    {
     gpiod_chip_close(chip);
     return -1;
    }

    value = gpiod_line_get_value(line);
    //printf("GPIO%d value is %d\n", Solar_IN, value);
    gpiod_chip_close(chip);

    return value;
}	
	
int gpiowrite(int pin, int value)
{
    chip= gpiod_chip_open("/dev/gpiochip0");

    if (!chip)
     return -1;

    line = gpiod_chip_get_line(chip, pin);

    if (!line) 
    {
     gpiod_chip_close(chip);
     return -1; 
    }

    rv = gpiod_line_request_output(line, "foobar", 1);

    if (rv) 
    {
     gpiod_chip_close(chip);
     return -1;
    }

    value = gpiod_line_set_value(line, value);
    //printf("GPIO%d value is set to %d\n", pin, value);
    sleep(1);
    gpiod_chip_close(chip);

    return 0;
}
