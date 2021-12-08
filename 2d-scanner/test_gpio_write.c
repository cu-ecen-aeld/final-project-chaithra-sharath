/*********************************************************
 *
 *  Gpio_write.c - Test file to check working of lgpiod lib
 *  
 ********************************************************/

#include "test_gpio_write.h"

struct gpiod_chip *chip;
struct gpiod_line *line;
int rv, value;

/**
 * @brief: function to write at a gpio pin
 * @param: int pin
 * @return int 
 */
int gpio_write(int pin, int value)
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
    sleep(1);
    gpiod_chip_close(chip);

    return 0;
}
