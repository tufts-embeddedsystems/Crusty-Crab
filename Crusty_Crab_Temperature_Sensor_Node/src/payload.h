// Luan Banh
// Payload creation and manipulation

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

struct PACKET{
  uint64_t  time;
  int32_t   primary_temperature;
  int32_t   secondary_temperature;
  uint8_t   battery_percentage;
  short     data_len;
};

#define PACKET struct PACKET

void print_packet_info(PACKET *pPacket)
{
    PACKET packet = *pPacket;
    PRINTF("\nPacket Size: %ld\n", sizeof(PACKET) + packet.data_len);
    PRINTF("Time: %llu\nGNIMO: %d\nTherm: %d\nBattery: %d\nLen: %hu\n\n", 
            packet.time, packet.primary_temperature, packet.secondary_temperature, 
            packet.battery_percentage, packet.data_len);
}