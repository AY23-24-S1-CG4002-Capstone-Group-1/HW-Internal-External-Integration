#include "Bluno_Variables.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>


byte incomingData[20];
int index = 0;

bool hasReceivedHelloPacket = false;
bool hasReceivedConnPacket = false;

bool isReadyToSendData = false;

uint8_t calculateCRC8(void* hello_packet, int packet_length) {
  CRC8 crc(7, 0, 0, false, false);

  crc.reset();
  crc.add((uint8_t*)hello_packet, packet_length);  

  return crc.calc();
}

Hello_Packet computeHelloPacketResponse() {
  Hello_Packet hello_packet_response;

  hello_packet_response.Device_ID = BLUNO_1_DEVICE_ID;
  hello_packet_response.Packet_ID = HELLO_PACKET_ID;
  hello_packet_response.Padding_1 = 0;
  hello_packet_response.Padding_2 = 0;
  hello_packet_response.Padding_3 = 0;
  hello_packet_response.Padding_4 = 0;
  hello_packet_response.Padding_5 = 0;
  hello_packet_response.Padding_6 = 0;
  hello_packet_response.Padding_7 = 0;
  hello_packet_response.Padding_8 = 0;
  hello_packet_response.Padding_9 = 0;
  hello_packet_response.Checksum = calculateCRC8(&hello_packet_response, 19);

  return hello_packet_response;
}

Data_Packet computeDataPacketResponse(struct Glove_Data glove_data) {
  Data_Packet data_packet;
  data_packet.Device_ID = BLUNO_1_DEVICE_ID;
  data_packet.Packet_ID = DATA_PACKET_ID;
  
  data_packet.GyroscopeX = glove_data.GyroX;
  data_packet.GyroscopeY = glove_data.GyroY;
  data_packet.GyroscopeZ = glove_data.GyroZ;

  data_packet.AccelerometerX = glove_data.AccX;
  data_packet.AccelerometerY = glove_data.AccY;
  data_packet.AccelerometerZ = glove_data.AccZ;

  data_packet.FlexValue = glove_data.Flex1;
  data_packet.FlexValue2 = glove_data.Flex2;
  data_packet.Padding_1 = 0;

  data_packet.Checksum = calculateCRC8(&data_packet, 19);

  return data_packet;
}

int mapAcc(float accValue) {
  accValue = accValue * 10;
  int map_accValue = map(accValue, -78.4, 78.4, 0, 1023);
  map_accValue = constrain(accValue, 0, 1023); 

  return map_accValue;
}

int mapGyro(float gyroValue) {
  gyroValue = gyroValue * 100;
  int map_gyroValue = map(gyroValue, -500, 500, 0, 1023);
  map_gyroValue = constrain(gyroValue, 0, 1023);

  return map_gyroValue; 
}
