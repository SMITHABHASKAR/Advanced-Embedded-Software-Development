/*
 * i2c.c
 *
 *  Created on: Apr 9, 2019
 *      Author: Smitha Bhaskar
 */

#include "i2c.h"

//initialize I2C module 0
//Slightly modified version of TI's example code


void I2CSend(uint8_t slave_addr, uint8_t num_of_args, ...)
{

    int i;
    // Tell the master module what address it will place on the bus when
    // communicating with the slave.
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);

    //stores list of variable number of arguments
    va_list vargs;

    //specifies the va_list to "open" and the last fixed argument
    //so vargs knows where to start looking
    va_start(vargs, num_of_args);

    //put data to be sent into FIFO
    I2CMasterDataPut(I2C0_BASE, va_arg(vargs, uint32_t));

    //if there is only one argument, we only need to use the
    //single send I2C function
    if(num_of_args == 1)
    {
        //Initiate send of data from the MCU
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);

        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C0_BASE));

        //"close" variable argument list
        va_end(vargs);
    }

    //otherwise, we start transmission of multiple bytes on the
    //I2C bus
    else
    {
        //Initiate send of data from the MCU
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C0_BASE));

        //send num_of_args-2 pieces of data, using the
        //BURST_SEND_CONT command of the I2C module
        for( i = 1; i < (num_of_args - 1); i++)
        {
            //put next piece of data into I2C FIFO
            I2CMasterDataPut(I2C0_BASE, va_arg(vargs, uint32_t));
            //send next data that was just placed into FIFO
            I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);

            // Wait until MCU is done transferring.
            while(I2CMasterBusy(I2C0_BASE));
        }

        //put last piece of data into I2C FIFO
        I2CMasterDataPut(I2C0_BASE, va_arg(vargs, uint32_t));
        //send next data that was just placed into FIFO
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C0_BASE));

        //"close" variable args list
        va_end(vargs);
    }
}

//sends an array of data via I2C to the specified slave
float I2CSendString(uint32_t slave_addr, uint8_t reg)
{
    float total;

    uint32_t low,high;

        I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);

        I2CMasterDataPut(I2C0_BASE, reg);

        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);

        while(I2CMasterBusy(I2C0_BASE));

        I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, true);

        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

        while(I2CMasterBusy(I2C0_BASE));

        high = I2CMasterDataGet(I2C0_BASE);

        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);


        while(I2CMasterBusy(I2C0_BASE));

         low = I2CMasterDataGet(I2C0_BASE);

         total = ((high << 8) | low) >> 4;


        return total;

}

void tempinit(void)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);


    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);

    HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;
}
/*read temp register*/
uint32_t read_temp(uint32_t slave_addr, uint8_t reg)
{
    tempinit();
    UARTprintf("Temperature Sensor Initialized\n\r");
    float total; uint32_t low,high;
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);

    I2CMasterDataPut(I2C0_BASE, reg); /*read reg*/

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);

    while(I2CMasterBusy(I2C0_BASE));

    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, true); /*set TMP102 as slave address*/

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    while(I2CMasterBusy(I2C0_BASE));

    high = I2CMasterDataGet(I2C0_BASE);

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);


    while(I2CMasterBusy(I2C0_BASE));

     low = I2CMasterDataGet(I2C0_BASE);

     total = ((high << 8) | low) >> 4;


    return total;

}



