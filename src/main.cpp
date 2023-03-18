#include <Arduino.h>
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>
typedef struct
{
  // 原始模拟信号值
  uint16_t a0a, a1a, a2a, a3a, a4a, a5a;
  // 解析出来的电压值
  // float a0v, a1v, a2v, a3v, a4v, a5v;
} AnalogValue;
//
AnalogValue readAValue()
{

  // 官网介绍其最大精度是10位, 也就是说用一个 short 保存即可
  // https://www.arduino.cc/reference/en/language/functions/analog-io/analogread
  //
  uint16_t a0a = analogRead(A0);
  uint16_t a1a = analogRead(A1);
  uint16_t a2a = analogRead(A2);
  uint16_t a3a = analogRead(A3);
  uint16_t a4a = analogRead(A4);
  uint16_t a5a = analogRead(A5);
  AnalogValue av = {
      .a0a = a0a, //
      .a1a = a1a, //
      .a2a = a2a, //
      .a3a = a3a, //
      .a4a = a4a, //
      .a5a = a5a, //
                  // .a0v = a0a * (5.0 / 1023.0), //
                  // .a1v = a1a * (5.0 / 1023.0), //
                  // .a2v = a2a * (5.0 / 1023.0), //
                  // .a3v = a3a * (5.0 / 1023.0), //
                  // .a4v = a4a * (5.0 / 1023.0), //
                  // .a5v = a5a * (5.0 / 1023.0), //
  };
  return av;
}

// JSON生成
// char *AnalogToJson(AnalogValue *analogValue)
// {
//   char *json = (char *)malloc(256);
//   sprintf(json, "{\"a0a\":%d,\"a1a\":%d,\"a2a\":%d,\"a3a\":%d,\"a4a\":%d,\"a5a\":%d,\"a0v\":%f,\"a1v\":%f,\"a2v\":%f,\"a3v\":%f,\"a4v\":%f,\"a5v\":%f}",
//           analogValue->a0a, analogValue->a1a, analogValue->a2a, analogValue->a3a, analogValue->a4a, analogValue->a5a);
//   return json;
// }

void setup()
{
  Serial.begin(9600);
  ModbusRTUServer.begin(0x01, 9600);
  ModbusRTUServer.configureHoldingRegisters(0, sizeof(AnalogValue));
}

void loop()
{
  int packetReceived = ModbusRTUServer.poll();
  if (packetReceived)
  {
    AnalogValue av = readAValue();
    ModbusRTUServer.holdingRegisterWrite(0, av.a0a);
    ModbusRTUServer.holdingRegisterWrite(1, av.a1a);
    ModbusRTUServer.holdingRegisterWrite(2, av.a2a);
    ModbusRTUServer.holdingRegisterWrite(3, av.a3a);
    ModbusRTUServer.holdingRegisterWrite(4, av.a4a);
    ModbusRTUServer.holdingRegisterWrite(5, av.a5a);
  }
  delay(30);
}