// Luan Banh
// Payload creation and manipulation

#include "main_config.h"

#define PACKET struct PACKET

struct PACKET{
  uint64_t  time;
  int32_t   primary_temperature;
  int32_t   secondary_temperature;
  uint8_t   battery_percentage;
  short     data_len;
};

void print_payload_info(char *payload, int data_len)
{
    printf("\nPayload: %.8s%.4s%.4s%.2s%.1s%.*s\n", 
                payload, payload+8, payload+12, payload+16, payload+17, data_len, payload+19);

    char unpack_time[9], unpack_tSensor[5], unpack_tTherm[5], unpack_batt[2], unpack_len[3];
    memset(unpack_time, '\0', sizeof(unpack_time));
    memset(unpack_tSensor, '\0', sizeof(unpack_tSensor));
    memset(unpack_tTherm, '\0', sizeof(unpack_tTherm));
    memset(unpack_batt, '\0', sizeof(unpack_batt));
    memset(unpack_len, '\0', sizeof(unpack_len));
    
    sprintf(unpack_time,  "%.8s", payload);
    sprintf(unpack_tSensor, "%.4s", payload+8);
    sprintf(unpack_tTherm, "%.4s", payload+12);
    sprintf(unpack_batt,  "%.1s", payload+16);
    sprintf(unpack_len,   "%.2s", payload+17);
    
    PRINTF("Unpack:\nTime: %llu\nGNIMO: %d\nTherm: %d\nBattery: %d\nLen: %hu", 
            *(uint64_t*) unpack_time, *(int32_t*) unpack_tSensor, *(int32_t*) unpack_tTherm,
            *(uint8_t*) unpack_batt, *(short*) unpack_len);
    if (data_len > 0)
    {
        char data[data_len+1];
        memset(data, '\0', sizeof(data));
        sprintf(data,  "%.*s", data_len, payload+19);
        PRINTF("\nData: %s\n", data);
    }
    PRINTF("\n\n");
}

void print_packet_info(PACKET *pPacket)
{
    PACKET packet = *pPacket;
    PRINTF("\nPacket Size: %ld\n", sizeof(PACKET) + packet.data_len);
    PRINTF("Time: %llu\nGNIMO: %d\nTherm: %d\nBattery: %d\nLen: %hu\n\n", 
            packet.time, packet.primary_temperature, packet.secondary_temperature, 
            packet.battery_percentage, packet.data_len);
}

PACKET *create_test_packet()
{
    PACKET *packet = (PACKET *)malloc(sizeof(PACKET));
    packet->time = 0x3030303030303030;
    packet->primary_temperature = 0x31313131;
    packet->secondary_temperature = 0x32323232;
    packet->battery_percentage = 0x33;
    packet->data_len = 0x3434;
    print_packet_info(packet);

    return packet;
}