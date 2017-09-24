
// UART passthrough  with debug code
//
// Suzanna Lucarotti (c) 13/9/2017
//
// for use with the UoS3 Cubesat
//
// should effectively remove module, allowing testing of interfaces through board, with codes coming back converted into hex for clarity


#include "../firmware.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#include <stdlib.h>
#include <stdarg.h>

#define GPS_SERIAL UART_PC104_HEADER
#define CAM_SERIAL UART_CAM_HEADER

 // the useful hardware constants (only used here so not moved away as yet)
 
// Wrappers for printing to serial port - always to the one specified as output, placed here close to code for clarity

#define DISP3(x,y,z) UART_putstr(DEBUG_SERIAL,x,y,z); // x and z are strings, y is a number shown as signed base10 16bit
#define DISP2(y,z) UART_putstr(DEBUG_SERIAL,NULL,y,z); 
#define DISP1(x) UART_puts(DEBUG_SERIAL,x);

 //////////////////////////////////////////////////////////////////
 // the actual main code
 //////////////////////////////////////////////////////////////////

int main(void)
{  
  Board_init(); // start the board
 // WDT_kick(); // kick the watchdog 
 setupwatchdoginterrupt();
// UART at 115200, unreliable at 9600

  UART_init(CAM_SERIAL, 115200); // UART_puts(CAM_SERIAL,"UART_passthrough");
  UART_init(GPS_SERIAL, 115200); // UART_puts(GPS_SERIAL,"UART_passthrough");

   while(1) // infinite loop
 {
    char c; // echo back to sending port
    if (UART_getc_nonblocking(GPS_SERIAL,&c)) {UART_putc_nonblocking(CAM_SERIAL,c); } // input
    if (UART_getc_nonblocking(CAM_SERIAL,&c)) {UART_putc_nonblocking(GPS_SERIAL,c); } //output
   }
}


