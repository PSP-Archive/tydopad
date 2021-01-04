#include <pspkernel.h> 
#include <pspdebug.h> 
#include <pspdisplay.h> 
#include <pspctrl.h> 
#include <pspsircs.h> 
#include <stdlib.h> 
#include <string.h>
#include <psprtc.h>
#include <time.h>  
 
PSP_MODULE_INFO("TyDoPad beta", 0, 1, 1); 
 
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU); 

#define printf   pspDebugScreenPrintf 
 
int exit_callback(void) 
{ 
   sceKernelExitGame(); 

   return 0; 
} 
 
void CallbackThread(void *arg) 
{ 
   int cbid; 

   cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL); 
   sceKernelRegisterExitCallback(cbid); 

   sceKernelSleepThreadCB(); 
} 
 
int SetupCallbacks(void) 
{ 
   int thid = 0; 

   thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0); 
   if(thid >= 0) 
   { 
      sceKernelStartThread(thid, 0, 0); 
   } 

   return thid; 
} 

unsigned char getIrPress(int fd, int textfd, int *shift)
{
  unsigned char p, key = 0;
  int len = 0;
  int i = 0;
  int readKeys = 0;
  
  SceCtrlData pad; 
  sceCtrlReadBufferPositive(&pad, 1);
  sceDisplayWaitVblankStart();
  
  if (pad.Buttons & PSP_CTRL_START) 
    {  
       key = '\n';
       sceIoWrite(textfd, &key, 1);
       sceIoWrite(textfd, &key, 1);
       sceIoWrite(textfd, &key, 1);
       sceIoClose(textfd);
    }
  
  while (i < 6)
    {
      len = sceIoRead(fd, &p, 1);
      if (len == 1)
      {
        // printf("%3d ", p);
        i++;
      }
      if (i == 3)
        key = p;
        
      readKeys++;
      if (readKeys > 40)
      {
        sceCtrlReadBufferPositive(&pad, 1);
        if (pad.Buttons & PSP_CTRL_START) 
        {  
           return 200; // save
           /* 
           */
        }
        readKeys = 0;
      }
      
    }
  
if (*shift == 0)
  switch (key) {
    case 14: return '`';
    case 21: return 'q';
    case 22: return '1';
    case 26: return 'z';    
    case 27: return 's';
    case 28: return 'a';
    case 29: return 'w';
    case 30: return '2';
    case 33: return 'c';
    case 34: return 'x';
    case 35: return 'd';
    case 36: return 'e';
    case 37: return '4';
    case 38: return '3';
    case 41: return ' ';
    case 42: return 'v';
    case 43: return 'f';
    case 44: return 't';
    case 45: return 'r';
    case 46: return '5';
    case 49: return 'n';
    case 50: return 'b';
    case 51: return 'h';
    case 52: return 'g';
    case 53: return 'y';    
    case 54: return '6';
    case 58: return 'm';
    case 59: return 'j';
    case 60: return 'u';
    case 61: return '7';
    case 62: return '8';
    case 66: return 'k';
    case 67: return 'i';
    case 68: return 'o';
    case 69: return '0';
    case 70: return '9';
    case 72: return ',';
    case 73: return '.';
    case 74: return '/';
    case 75: return 'l';
    case 76: return ';';
    case 77: return 'p';
    case 78: return '-';
    case 82: return '\'';
    case 84: return '[';
    case 85: return '=';
    case 90: return '\n';
    case 91: return ']';
    case 93: return '\\';
    case 102: return 8;   // backspace
    // case : return '';
    case 18: *shift = 1;
    case 89: *shift = 1;
    default: return 0;
  }
  
if (*shift == 1)
  switch (key) {
    case 14: return '~';
    case 21: return 'Q';
    case 22: return '!';
    case 26: return 'Z';    
    case 27: return 'S';
    case 28: return 'A';
    case 29: return 'W';
    case 30: return '@';
    case 33: return 'C';
    case 34: return 'X';
    case 35: return 'D';
    case 36: return 'E';
    case 37: return '$';
    case 38: return '#';
    case 41: return ' ';
    case 42: return 'V';
    case 43: return 'F';
    case 44: return 'T';
    case 45: return 'R';
    case 46: return '%';
    case 49: return 'N';
    case 50: return 'B';
    case 51: return 'H';
    case 52: return 'G';
    case 53: return 'Y';
    case 54: return '^';
    case 58: return 'M';
    case 59: return 'J';
    case 60: return 'U';
    case 61: return '&';
    case 62: return '*';
    case 66: return 'K';
    case 67: return 'I';
    case 68: return 'O';
    case 69: return ')';
    case 70: return '(';
    case 72: return '<';
    case 73: return '>';
    case 74: return '?';
    case 75: return 'L';
    case 76: return ':';
    case 77: return 'P';
    case 78: return '_';
    case 82: return '"';
    case 84: return '{';
    case 85: return '+';
    case 90: return '\n';
    case 91: return '}';
    case 93: return '|';
    case 102: return 8;   // backspace
    // case : return '';
    case 146: *shift = 0;
    case 217: *shift = 0;
    default: return 0;
  }
  
  
  return 0;
}


int main(void) 
{ 
   // char buffer[10]; 
   // unsigned char data;
   // SceCtrlData pad; 
   // u32 buttonsold = 0; 
   int x = 0, y = 2;
   int i = 0, t = 0;
   unsigned char c;
   int len;
   unsigned char text[4096];
   // text[1023] = '\0';
   int textfd;
   char header[100];
   pspTime time; 
	 sceRtcGetCurrentClockLocalTime(&time);

   SetupCallbacks(); 
   pspDebugScreenInit(); 

   sceCtrlSetSamplingCycle(0); 
   sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL); 

   printf(" TyDoPad v0.01 (Palm Universal Wireless Keyboard) by madruscoe\n"); 
   printf("---------------------------------------------------------------\n"); 

   int fd = sceIoOpen("irda0:", PSP_O_RDWR, 0); 
   int shift = 0;
   
   textfd = sceIoOpen("ms0:/TyDoPad_notes.txt", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_APPEND, 0777);
	 if (textfd < 0) {
		printf("Cannot open file ms0:/TyDoPad_notes.txt\n");
		return -1;
	 }
 
	 
	 snprintf(header, 95, "\r\n\r\n======================\r\n     TyDoPad note\r\n   %04d %02d-%02d %02d:%02d\r\n======================\r\n\r\n", time.year, time.month, time.day, time.hour, time.minutes);
	 while (header[i] != '\0')
	   {
        c = header[i];
        sceIoWrite(textfd, &c, 1);
        i++;
     }
   
   pspDebugScreenSetXY(x, y);

  while (1) 
  {
    int c = getIrPress(fd, textfd, &shift);
    if (c)
    { 
      
      if (x > 65) // 65
        { 
          printf("\n");
          x = 0;
          y++;
          // pspDebugScreenSetXY(x, y);
        }
      if (y > 32)   // 32
        {
          x = 66;
          y = 1;
          pspDebugScreenSetXY(x, y);
          printf("\n");
          y = 2;
          x = 0;
          // pspDebugScreenSetXY(x, y);
        }
      
      
      if (c == 200) // save
      {
        int i = 0;
        unsigned char c;
        printf("\n\nSaving...  ");
        
        text[t] = '\0';
        while (text[i] != '\0')
        {
          c = text[i];
          sceIoWrite(textfd, &c, 1);
          i++;
        }
        
        unsigned char key1 = '\r'; unsigned char key2 = '\n'; 
        sceIoWrite(textfd, &key1, 1); sceIoWrite(textfd, &key2, 1); sceIoWrite(textfd, &key1, 1); sceIoWrite(textfd, &key2, 1); sceIoWrite(textfd, &key1, 1); sceIoWrite(textfd, &key2, 1);  
        sceIoClose(textfd);
        printf("Saved. Now you can exit TyDoPad.");
      } else if (c == 8)
      {
        if ((t > 0) && (x > 0))
        {
          t--;
          x--;
          pspDebugScreenSetXY(x, y);
          printf(" ");
          pspDebugScreenSetXY(x-1, y);
        }
      } else
      {
        if ((t < 4093) && (c != 8))
        { 
          pspDebugScreenSetXY(x, y);
          
          if (c == '\n')
          { 
            x = -1;
            y++;
            text[t] = '\r';
            t++;
          }
          
          printf("%c", c);
          x++;
          pspDebugScreenSetXY(x, y);
          
          
          text[t] = c;
          t++;
          
        } else
        {
          printf("\nMaximum note size exceeded. Please save (START) and exit.");
        }
      }
      
      
    }
      
    sceDisplayWaitVblankStart(); 
  }

   return 0; 
}
