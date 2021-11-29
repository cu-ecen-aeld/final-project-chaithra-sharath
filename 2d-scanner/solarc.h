#ifndef __SOLARC_H__
#define __SOLARC_H__


#include "test_gpio_read.h"
#include "test_gpio_write.h"

//The LED array is 16x16
#define array_ind 256

/**
 *
 */
//int * solar_read(void); 
int* solar_read(int * buffer) ;

#endif 
