/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/


#pragma pack(1)

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
  char      data[];                // 0
};
 
int main ()
{
    char *buff = "this is a test string";
    
    time_t seconds;
     
    seconds = time(NULL);
    printf("Seconds since January 1, 1970 = %ld\n", seconds);
     
    int temp = 25;
    int batt = 100;
    char *data = "Hello";
    int len = strlen(data)+1;
    
    
    unsigned char foo[sizeof(struct PACKET)+len * sizeof(char)];
    
    struct PACKET packet = {seconds, temp, temp, batt, len};
    memcpy(packet.data, data, len);
    
    printf("Packet Data: %s\n", packet.data);

    printf("Packet Size: %ld\n", sizeof(packet) + len);

    memcpy(foo, &packet, sizeof(packet) + len * sizeof(char));

    void *ptr = foo;
    struct PACKET *unpack = malloc(sizeof(packet) + len * sizeof(char));
    unpack = (struct PACKET *)ptr;
    
    printf("\nUnpack:\n%ld, %d, %d, %d, %d, %s\n", unpack->time, 
                                        unpack->primary_temperature, 
                                        unpack->secondary_temperature,
                                        unpack->battery_percentage, 
                                        unpack->data_len, 
                                        unpack->data);

    return(0);
}
