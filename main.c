/* FreeRTOS 8.2 Tiva Demo
 *
 * main.c
 *
 * Andy Kobyljanec
 *
 * This is a simple demonstration project of FreeRTOS 8.2 on the Tiva Launchpad
 * EK-TM4C1294XL.  TivaWare driverlib sourcecode is included.
 */

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "i2c.h"
#include "drivers/pinout.h"
#include "utils/uartstdio.h"


// TivaWare includes
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

// FreeRTOS includes
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"


typedef struct queues
{
    float result;  /*temperature value*/
    unsigned int timestamp;  /*timestamp*/
    char device[20]; /*LED,Temp_Sensor*/
    char led_statement[100];/*LED Status updates*/

  }log;

//struct queues log;
QueueHandle_t msgq=NULL;
TaskHandle_t alert;

// Demo Task declarations
void demoLEDTask(void *pvParameters);
void sensor_init(void);
void Temp_senosr_task(void *pvParameters);
uint32_t I2CReceive(uint32_t slave_addr, uint8_t reg);
uint32_t read_temp(uint32_t slave_addr, uint8_t reg);
void logtask(void *pvParameters);
void ConfigureUART(void);
void tempinit(void);
uint32_t state;
uint32_t g_ui32SysClock;

void
ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, g_ui32SysClock);
}

void sensor_init(void)
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

uint32_t I2CReceive(uint32_t slave_addr, uint8_t reg)
{
    sensor_init();
    uint32_t raw_value,lsb,msb;
    //specify that we are writing (a register address) to the
    //slave device
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);

    //specify register to be read
    I2CMasterDataPut(I2C0_BASE, reg);

    //send control byte and register address byte to slave device
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    //wait for MCU to finish transaction
    while(I2CMasterBusy(I2C0_BASE));

    //specify that we are going to read from slave device
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, true);

    //send control byte and read from the register we
    //specified
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

    //wait for MCU to finish transaction
    while(I2CMasterBusy(I2C0_BASE));

    msb = I2CMasterDataGet(I2C0_BASE);

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);

    lsb = I2CMasterDataGet(I2C0_BASE);

    raw_value = ((msb << 8) | lsb) >> 4;


     return raw_value;

}

// Main function
int main(void)
      {
    // Initialize system clock to 120 MHz
    uint32_t output_clock_rate_hz;
    output_clock_rate_hz = ROM_SysCtlClockFreqSet(
                               (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                               SYSTEM_CLOCK);
    ASSERT(output_clock_rate_hz == SYSTEM_CLOCK);

    // Initialize the GPIO pins for the Launchpad
    PinoutSet(false, false);

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);


    //Create demo tasks
    xTaskCreate(demoLEDTask, (const portCHAR *)"LEDs",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);


    xTaskCreate(Temp_senosr_task, (const portCHAR *)"Temperature",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    xTaskCreate(logtask, (const portCHAR *)"Logger",
                   configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    vTaskStartScheduler();


    return 0;
}


// Flash the LEDs on the launchpad
void demoLEDTask(void *pvParameters)
{
     log led;
    UARTStdioConfig(0, 115200, SYSTEM_CLOCK);
while(1){
        UARTprintf("\r\nLED Task");
        strcpy(led.device,"LED TASK");

        state^=0x02;
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, state);
        vTaskDelay(50 / portTICK_PERIOD_MS); //delay for 10Hz freq
        state^=0x02;
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, state);
        vTaskDelay(50 / portTICK_PERIOD_MS); //delay for 10Hz freq
        strcpy(led.led_statement, "LED D1 & D2 blinking at 10Hz");
        led.timestamp = (xTaskGetTickCount() * portTICK_PERIOD_MS);
        if( xQueueSend( msgq, ( void * ) &led, ( TickType_t ) 10 ) != pdPASS )
                    {
                        UARTprintf("\r\n Failed Send from LED task!");
                    }
}

}


 //Write text over the Stellaris debug interface UART port


void Temp_senosr_task(void *pvParameters)
{
    log temp;

    const TickType_t delt = 1000 / portTICK_PERIOD_MS; /*each second, a reading shall be given*/
    while(1)
    {
        strcpy(temp.device,"TEMP TASK");
        UARTprintf("TEMP TASK\n\r");
        temp.result= ((read_temp(0x48, 0x00)) * 0.0625);
        UARTprintf("%d\n\r",temp.result);
        temp.timestamp = (xTaskGetTickCount() * portTICK_PERIOD_MS);



        if((uint8_t)(temp.result) >= 27)
         {

             /*notify*/
            if(xTaskNotifyGive(alert))
            {
                //   UARTprintf("\r\n Sending Alert %d",(uint8_t)(temp.value*100));

             }
            else
                UARTprintf("\r\n Could not notify task");
         }
         if( xQueueSend( msgq, ( void * ) &temp, ( TickType_t ) 10 ) != pdPASS )
         {
            UARTprintf("\r\n Failed Send from Temperature task!");
         }


        vTaskDelay(delt);
    }

}




void logtask(void *pvParameters)
{
    msgq = xQueueCreate( 15, sizeof(log));

       if( msgq == NULL )
           {
                UARTprintf("\r\n Queue not created!");
           }

int str1,str2;

    UARTStdioConfig(0, 115200, SYSTEM_CLOCK);
     log logger;
     float value=24.4;
while(1){
    if( xQueueReceive( msgq, (void *) &logger , ( TickType_t ) 10 ) )
    {

        UARTprintf("\r\nQueue Task");
       // if(logger.device == "LED Task")
        str1=strcmp(logger.device,"LED TASK");
        if(str1==0){
            UARTprintf("\r\n Timestamp[%d]\n\r"
                    " Task name:%s\n\r"
                    "Status:%s\n\r",logger.timestamp,logger.device,logger.led_statement);


        }

//        str2=strcmp(logger.device,"TEMP TASK");
//                if(str2==0){
//                    UARTprintf("\r\n Timestamp[%d]\n\r"
//                            " Task name:%s\n\r"
//                            "Temperature=%f\n\r",logger.timestamp,logger.device,logger.result);
//
//                }

    }

    }
}

/*  ASSERT() Error function
 *
 *  failed ASSERTS() from driverlib/debug.h are executed in this function
 */
void __error__(char *pcFilename, uint32_t ui32Line)
{
    // Place a breakpoint here to capture errors until logging routine is finished
    while (1)
    {
    }
}
