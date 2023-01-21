#include "Arduino.h"
#include "LoRa_E22.h"
#include <SoftwareSerial.h>





SoftwareSerial portlora(10, 11);


LoRa_E22 e22ttl(&portlora);
#define M0 7
#define M1 6


typedef struct {
  byte altitude[10];
  byte Altitude2[10];
  byte Latitude[10];
  byte Longtitude[10];
  byte Temperature[10];
  byte Pressure[10];
  byte Heading[10];
  byte Parasut[10];
  byte Parasut1[10];
  byte x[10];
  byte y[10];
  byte z[10];
} Signal;
Signal data;



void setup() {
   pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  Serial.begin(9600);
  e22ttl.begin();
  delay(500);
}

void loop() {

while (e22ttl.available() > 1) {
ResponseStructContainer rsc = e22ttl.receiveMessage(sizeof(Signal));
data = *(Signal*) rsc.data;
rsc.close();

Serial.print(F("Altitude M: "));
Serial.println(*(float*)data.altitude);
Serial.println();
Serial.print(F("Altitude2 M: "));
Serial.println(*(float*)data.Altitude2);
Serial.println();
Serial.print(F("Latitude: "));
Serial.println(*(float*)data.Latitude);
Serial.println();
Serial.print(F("Longtitude: "));
Serial.println(*(float*)data.Longtitude);
Serial.println();
Serial.print(F("Temperature C: "));
Serial.println(*(float*)data.Temperature);
Serial.println();
Serial.print(F("Pressure PA: "));
Serial.println(*(float*)data.Pressure);
Serial.println();
Serial.print(F("Heading D: "));
Serial.println(*(float*)data.Heading);
Serial.println();
Serial.print(F("Parasut "));
Serial.println(*(float*)data.Parasut);
Serial.println();
Serial.print(F("Parasut1 "));
Serial.println(*(float*)data.Parasut1);
Serial.println();
Serial.print(F("x "));
Serial.println(*(float*)data.x);
Serial.println();
Serial.print(F("y "));
Serial.println(*(float*)data.y);
Serial.println();
Serial.print(F("z "));
Serial.println(*(float*)data.z);
Serial.println();


//Serial.print(F("***VERILER***"));


Serial.write(0x22);
Serial.write(0xff);
Serial.write(0xff);
Serial.write(0xff);

Serial.print("t18.txt=");
Serial.write(0x22);
Serial.print(*(float*)data.altitude);
Serial.write(0x22);
Serial.write(0xff);
Serial.write(0xff);
Serial.write(0xff);

Serial.print("t1.txt=");
Serial.write(0x22);
Serial.print(*(float*)data.Altitude2);
Serial.write(0x22);
Serial.write(0xff);
Serial.write(0xff);
Serial.write(0xff);

Serial.print("t2.txt=");
Serial.write(0x22);
Serial.print(*(float*)data.Latitude);
Serial.write(0x22);
Serial.write(0xff);
Serial.write(0xff);
Serial.write(0xff);

Serial.print("t3.txt=");
Serial.write(0x22);
Serial.print(*(float*)data.Longtitude);
Serial.write(0x22);
Serial.write(0xff);
Serial.write(0xff);
Serial.write(0xff);

Serial.print("t4.txt=");
Serial.write(0x22);
Serial.print(*(float*)data.Temperature);
Serial.write(0x22);
Serial.write(0xff);
Serial.write(0xff);
Serial.write(0xff);

Serial.print("t5.txt=");
Serial.write(0x22);
Serial.print(*(float*)data.Pressure);
Serial.write(0x22);
Serial.write(0xff);
Serial.write(0xff);
Serial.write(0xff);

Serial.print("t6.txt=");
Serial.write(0x22);
Serial.print(*(float*)data.Heading);
Serial.write(0x22);
Serial.write(0xff);
Serial.write(0xff);
Serial.write(0xff);

Serial.print("t8.txt=");
Serial.write(0x22);
Serial.print(*(float*)data.Parasut);
Serial.write(0x22);
Serial.write(0xff);
Serial.write(0xff);
Serial.write(0xff);

Serial.print("t9.txt=");
Serial.write(0x22);
Serial.print(*(float*)data.Parasut1);
Serial.write(0x22);
Serial.write(0xff);
Serial.write(0xff);
Serial.write(0xff);

Serial.print("h0.val=");

Serial.print(50+(*(int*)data.y)*5-8);

Serial.write(0xff);
Serial.write(0xff);
Serial.write(0xff);

Serial.print("h1.val=");

Serial.print(50+(*(int*)data.x)*5);

Serial.write(0xff);
Serial.write(0xff);
Serial.write(0xff);

Serial.print("h2.val=");

Serial.print(50+(*(int*)data.z)*4);

Serial.write(0xff);
Serial.write(0xff);
Serial.write(0xff);
}

}
