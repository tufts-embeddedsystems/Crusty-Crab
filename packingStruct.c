/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

// C program to demonstrate
// example of time() function.
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
 
 
struct PACKET{
  time_t    time;                  // 8
  int32_t   primary_temperature;   // 4
  int32_t   secondary_temperature; // 4
  uint8_t   battery_percentage;    // 1
  short     data_len;              // 2
};

int main ()
{
    time_t seconds;
     
    seconds = time(NULL);
    // printf("Seconds since January 1, 1970 = %ld\n", seconds);
     
    int temp = -0x80000000;
    int batt = 0xFF;
    char *data = "OK";
    int len = strlen(data);

    struct PACKET packet = {0x3030303030303030, 0x31313131, 0x32323232, 0x33, 0x3434};

    printf("Packet Size: %ld\n", sizeof(struct PACKET) + len);

    char* pBuffer;
    void *voidptr = (void *)&packet;
    pBuffer = (char *)voidptr;
    pBuffer[sizeof(struct PACKET)] = '\0';
    printf("pBuffer: %s\n", pBuffer);
    
    unsigned char foo[sizeof(struct PACKET)+len];
    memset(foo, 0, sizeof(foo));
    sprintf(foo, "%.8s%.4s%.4s%.1s%.2s", pBuffer, pBuffer+8, pBuffer+12, pBuffer+16, pBuffer+18);
    if (len > 0)
        sprintf(foo+19, "%s", data);
    
    printf("Payload: %.8s%.4s%.4s%.1s%.2s%s\n", foo, foo+8, foo+12, foo+16, foo+17, foo+19);
    
    // printf("Time: ");
    // for (int i = 0; i < 8; i++)
    // {
    //     printf("%c", foo[i]);
    // }
    // printf("\n");
    
    
    // printf("Temp1: ");
    // for (int i = 0; i < 4; i++)
    // {
    //     printf("%c", foo[8+i]);
    // }
    // printf("\n");
    
    
    // printf("Temp2: ");
    // for (int i = 0; i < 4; i++)
    // {
    //     printf("%c", foo[12+i]);
    // }
    // printf("\n");
    
    // printf("Batt: ");
    // for (int i = 0; i < 1; i++)
    // {
    //     printf("%c", foo[16+i]);
    // }
    // printf("\n");
    
    // printf("Len: ");
    // for (int i = 0; i < 2; i++)
    // {
    //     printf("%c", foo[18+i]);
    // }
    // printf("\n");
    
    // printf("Data: ");
    // for (int i = 0; i < len; i++)
    // {
    //     printf("%c", foo[20+i]);
    // }
    // printf("\n");

    // void *ptr = foo;
    // struct PACKET *unpack = (struct PACKET *)ptr;
    
    char unpack_time[9], unpack_temp[5], unpack_batt[2], unpack_len[3];
    sprintf(unpack_time, "%.8s", foo);
    sprintf(unpack_temp, "%.4s", foo+8);
    sprintf(unpack_batt, "%.1s", foo+16);
    sprintf(unpack_len , "%.2s", foo+17);
    
    printf("Unpack:\n%lld, %d, %d, %d, %d\n", *(long long int*) unpack_time, 
                                        *(int*) unpack_temp, 
                                        *(int*) unpack_temp,
                                        *(uint8_t*) unpack_batt, 
                                        *(short*) unpack_len);

    return(0);
}
