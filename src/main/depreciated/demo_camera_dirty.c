/**
 * File purpose:        Old camera demo
 * Last modification:   27/07/2019
 * Status:              For backup and reference - used to investigate and establish camera responses on different commands
 */

/* firmware.h contains all relevant headers */
#include "../firmware.h"

#include <stdio.h>
#include <stdlib.h>

//#define PAGE_LENGTH 5000 //Buffer_slot_size is 3392bytes, do changed the page size!
#define PAGE_LENGTH 106

static char LK_RESET[]     = {0x56, 0x00, 0x26, 0x00};
static char LK_RESET_RE[]    = {0x0d, 0x0a, 0x49, 0x6e, 0x69, 0x74, 0x20, 0x65, 0x6e, 0x64, 0x0d, 0x0a};
static char LK_RESOLUTION_1600[] = {0x56, 0x00, 0x54, 0x01, 0x21};
static char LK_RESOLUTION_RE[] = {0x76, 0x00, 0x54, 0x00, 0x00}; // v T; response from the camera after setting the resolution
static char LK_COMPRESSION[] = {0x56, 0x00, 0x31, 0x05, 0x01, 0x01, 0x12, 0x04, 0x10}; // Value in last byte
static char LK_COMPRESSION_RE[] = {0x76, 0x00, 0x31, 0x00, 0x00}; // value in last byte
static char LK_PICTURE[]   = {0x56, 0x00, 0x36, 0x01, 0x00};
static char LK_PICTURE_RE[]    = {0x76, 0x00, 0x36, 0x00, 0x00};
static char LK_STOP[] = {0x56, 0x00, 0x36, 0x01, 0x03};
static char LK_STOP_RE[] = {0x76, 0x00, 0x36, 0x00, 0x00};
static char LK_JPEGSIZE[]    = {0x56, 0x00, 0x34, 0x01, 0x00};
static char LK_JPEGSIZE_RE[] = {0x76, 0x00, 0x34, 0x00, 0x04}; //, 0x00, 0x00, 0x00, 0x00}; // then XH XL // assumption is last 4 bytes are size
static char LK_READPICTURE[] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00};
static char LK_PICTURE_TIME_dot1ms[]  = {0x00, 0x0a}; // 0.1 ms
static char LK_READPICTURE_RE[]  = {0x76, 0x00, 0x32, 0x00, 0x00};
static char LK_RESOLUTION_160[] = {0x56, 0x00, 0x54, 0x01, 0x22};
static char LK_RESOLUTION_320[] = {0x56, 0x00, 0x54, 0x01, 0x11};
static char LK_RESOLUTION_640[] = {0x56, 0x00, 0x54, 0x01, 0x00};
static char LK_RESOLUTION_800[] = {0x56, 0x00, 0x54, 0x01, 0x1D};
static uint32_t UART_getw4(uint8_t serial)
{
  char c1=UART_getc(serial),c2=UART_getc(serial),c3=UART_getc(serial),c4=UART_getc(serial);
  return (uint32_t)((c1 << 24) | (c2 << 16) | (c3 << 8) | (c4 << 0)); //shifting values of read char/hex numbers to create one 32bit number
}

int main(void)
{
  Board_init();
  watchdog_update = 0xFF;
  enable_watchdog_kick();
  RTC_init();

  UART_init(UART_GNSS,115200);
  UART_puts(UART_GNSS, "Camera Demo\r\n");
  UART_init(UART_CAMERA, 38400);
  int stop = 1;
  while(stop--)
  { 
    //variables
    watchdog_update = 0xFF;
    int index_rst=0, index_res=0, index_takepic=0, index_readsize=0,index_readdata=0, index_readdata2 =0, index_resolution = 0;;
    int rst = 0, res = 0, pic = 0, size=0, dat =0, dat2 = 0, dat3=0, resol=0;
    char reset_resp[20], res_resp[20]={0}, pic_res[20], size_resp[20]={0}, read_resp[20]={0}, read_resp2[20], resol_resp[20]={0};
    char *output = malloc(100*sizeof(char));
    char *output2 = malloc(100*sizeof(char));
    char *output3 = malloc(100*sizeof(char));
    char *output4 = malloc(100*sizeof(char));
    char *output5 = malloc(100*sizeof(char));
    char *output6 = malloc(100*sizeof(char));
    char *output7 = malloc(100*sizeof(char));
    char *output8 = malloc(100*sizeof(char));
    char *output9 = malloc(100*sizeof(char));
    uint32_t jpegsize;
    UART_puts(UART_GNSS, "\r\nSTART: \r\n");
    //reset command
    UART_putb(UART_CAMERA, LK_RESET, sizeof(LK_RESET));
    uint64_t start_timestamp;
    RTC_getTime_ms(&start_timestamp);
  while(!RTC_timerElapsed_ms(start_timestamp, 3000))
  { 
    if(UART_charsAvail(UART_CAMERA)){
    if(UART_getc_nonblocking(UART_CAMERA, &reset_resp[rst]))
    {
      if(reset_resp[rst] == LK_RESET_RE[index_rst])
      {
        index_rst++;
        if(index_rst == sizeof(LK_RESET_RE))
        {
          break;;
        }
      }
      else
      {
        index_rst = 0;
      }
    }
    rst++;
    }
  }

    watchdog_update = 0xFF;
    if(index_rst == sizeof(LK_RESET_RE)){UART_puts(UART_GNSS, "\r\nReset response matched\r\n");}
      else{UART_puts(UART_GNSS, "\r\nReset response not matched\r\n");}
    UART_putb(UART_GNSS, reset_resp, sizeof(LK_RESET_RE));
    for(int i = 0;i<sizeof(LK_RESET_RE);i++){
      sprintf(output, "0x%02"PRIx8" ", reset_resp[i]);
      UART_puts(UART_GNSS, output);
    }
    Delay_ms(3000);
  //change resolution
    UART_putb(UART_CAMERA,LK_RESOLUTION_1600, sizeof(LK_RESOLUTION_1600));
    Delay_ms(25);
    while(UART_charsAvail(UART_CAMERA)){
      UART_getc_nonblocking(UART_CAMERA, &resol_resp[resol]);
      if(resol_resp[resol]==LK_RESOLUTION_RE[index_resolution]){
        index_resolution++;
      }else
      {
        index_resolution=0;
      }
      resol++;
    }
    if(index_resolution==sizeof(LK_PICTURE_RE)){UART_puts(UART_GNSS,"\r\nResolution response matched\r\n");}
    else{UART_puts(UART_CAMERA,"\r\nResolution response not matched\r\n");}
    UART_putb(UART_GNSS,resol_resp, sizeof(resol_resp));
    for(int i =0;i<sizeof(LK_RESOLUTION_RE);i++){
      sprintf(output8,"0x%02"PRIx8" ",resol_resp[i]);
      UART_puts(UART_GNSS,output8);
    }
    Delay_ms(100);

  //take picture command 
    UART_putb(UART_CAMERA, LK_PICTURE, sizeof(LK_PICTURE));
    Delay_ms(200);
    while(UART_charsAvail(UART_CAMERA)){
      UART_getc_nonblocking(UART_CAMERA,&pic_res[pic]);
      if(pic_res[pic] == LK_PICTURE_RE[index_takepic]){
        index_takepic++;
      }else{
        index_takepic=0;
      }
      pic++;
    }
    if(index_takepic == sizeof(LK_PICTURE_RE)){UART_puts(UART_GNSS, "\r\nPicture Resp matched\r\n");}
    else{UART_puts(UART_GNSS, "\r\nPicture response not matched\r\n");}
    UART_putb(UART_GNSS, pic_res, sizeof(LK_PICTURE_RE));
    for(int i = 0;i<sizeof(LK_PICTURE_RE);i++){
      sprintf(output2, "0x%02"PRIx8" ", pic_res[i]);
      UART_puts(UART_GNSS, output2);
    }
    //read picture size
    UART_putb(UART_CAMERA, LK_JPEGSIZE, sizeof(LK_JPEGSIZE));
    Delay_ms(200);
    while(UART_charsAvail(UART_CAMERA)){
      UART_getc_nonblocking(UART_CAMERA,&size_resp[size]);
      if(size_resp[size]==LK_JPEGSIZE_RE[index_readsize]){
        index_readsize++;
      }
      else{
        index_readsize=0;
      }
      size++;
      if(index_readsize == sizeof(LK_JPEGSIZE_RE)){
        break;
      }
    }
    if(index_readsize == sizeof(LK_JPEGSIZE_RE)){
      UART_puts(UART_GNSS, "\r\nPicture Size Resp matched\r\n");
    }else{UART_puts(UART_GNSS, "\r\nPicture size response not matched\r\n");}
    jpegsize = UART_getw4(UART_CAMERA);
    UART_putb(UART_GNSS, size_resp, sizeof(size_resp));
    for(int i = 0; i<sizeof(LK_JPEGSIZE_RE);i++){
      sprintf(output3, "0x%02"PRIx8" ",size_resp[i]);
      UART_puts(UART_GNSS, output3);
    }
  UART_puts(UART_GNSS,"\n\r");
  sprintf(output4, "%u ", jpegsize);
  UART_puts(UART_GNSS, output4);


  UART_puts(UART_GNSS,"Before reading picture data");
  //reading the picture
  uint16_t pagesize = 0x820;
  bool endflag = false;
  uint32_t i = 0x0000;
  uint8_t response_begin[5]={0},response_end[6]={0};
  uint8_t data_buffer[2080] = {0}; //some space for beginning characters and ending characters

  LK_READPICTURE[12]= 0x08;
  LK_READPICTURE[13]= 0x20;
  LK_READPICTURE[14]= 0x00;
  LK_READPICTURE[15]= 0x0a;

  uint64_t timestamp;
  uint8_t k = 0, z=0;

while(!endflag){
  //while(x--){
  LK_READPICTURE[8]=i/0x100;
  LK_READPICTURE[9]=i%0x100;
  UART_puts(UART_GNSS,"\n\r");
  UART_putb(UART_CAMERA,LK_READPICTURE,sizeof(LK_READPICTURE));
  RTC_getTime_ms(&timestamp);
  while(!RTC_timerElapsed_ms(timestamp,2000)){
    if(UART_charsAvail(UART_CAMERA)){
      if(k<5){
        UART_getc_nonblocking(UART_CAMERA,&response_begin[k]);
        k++;
      }else if(k>=5 && dat2<pagesize && !endflag)
      {
        UART_getc_nonblocking(UART_CAMERA,&data_buffer[dat2]);
        if((data_buffer[dat2-1]==0xff && data_buffer[dat2]==0xd9)){
            endflag = true;
            dat2++;
        }
        else
        {
          dat2++; 
        }
      }else
      {
        UART_getc_nonblocking(UART_CAMERA,&response_end[z]);
        z++;
      }
    }
  }
  for(int i =0;i<dat2;i++){
    sprintf(output6, "0x%02"PRIx8" ",data_buffer[i]);
      UART_puts(UART_GNSS, output6);
  }/*
  UART_puts(UART_GNSS,"\n\r");
  for(int i =0;i<5;i++){
    sprintf(output7, "0x%02"PRIx8" ",response_begin[i]);
    UART_puts(UART_GNSS, output7);
  } 
  UART_puts(UART_GNSS,"\n\r");
  for(int i =0;i<6;i++){
    sprintf(output8, "0x%02"PRIx8" ",response_end[i]);
    UART_puts(UART_GNSS, output8);
  }*/
  dat2=0;
  i+=0x820;
  k=0;
}
UART_puts(UART_GNSS, "\r\nkupa");
    free(output9);
    free(output8);
    free(output2);
    free(output);
    free(output3);
    free(output4);
    free(output5);
    free(output6);
    free(output7);
    LED_off(LED_B);
    Delay_ms(2000);
  }
}