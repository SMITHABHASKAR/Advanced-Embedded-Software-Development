/*
 * i2c.h
 *
 *  Created on: Apr 10, 2019
 *      Author: Smitha Bhaskar
 */

#ifndef I2C_H_
#define I2C_H_


#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

void InitI2C0(void);
void tempinit(void);
uint32_t read_temp(uint32_t slave_addr, uint8_t reg);
void I2CSend(uint8_t slave_addr, uint8_t num_of_args, ...);
float I2CSendString(uint32_t slave_addr, uint8_t reg);
uint32_t I2CReceive(uint32_t slave_addr, uint8_t reg);

#define TEMPERATURE_REG 0x00
#define CONFIG_REG 0x01
#define THRESH_LOW 0x02
#define THRESH_HIGH 0x03
#define TMP102_ADDR 0x48
#define RESOLUTION 0.0625



#endif /* I2C_H_ */
