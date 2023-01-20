#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "LoRa_E22.h"

#include "I2Cdev.h"
#include "MPU6050.h"
#include <Adafruit_BMP085.h>
#include <HMC5883L_Simple.h>
#include <Wire.h>
//MPU6050 accelgyro;
MPU6050 mpu;
static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;

SoftwareSerial portgps(RXPin, TXPin);
SoftwareSerial portlora(11, 10);

LoRa_E22 e22ttl(&portlora);
#define M0 8
#define M1 7
#define p 5
#define p1 6
typedef struct {
  byte altitude[10];
  byte altitude2[10];
  byte latitude[10];
  byte longtitude[10];
  byte temperature[10];
  byte pressure[10];
  byte heading[10];
  byte parasut[10];
  byte parasut1[10];
  byte x[10];
  byte y[10];
  byte z[10];
} Signal;
Signal data;


Adafruit_BMP085 bmp;
HMC5883L_Simple Compass;




void setup()
{
 pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(p, OUTPUT);
  pinMode(p1, OUTPUT);
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);

  Serial.begin(9600);
  e22ttl.begin();
  delay(500);
  portgps.begin(GPSBaud);
  delay(500);

  Wire.begin();

  // initialize devices
  Serial.println("Initializing I2C devices...");

  // initialize bmp085
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }

   while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
//  accelgyro.initialize();
 // Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
 mpu.setI2CBypassEnabled(true); // set bypass mode for gateway to hmc5883L
  // If you want, you can set accelerometer offsets
  // mpu.setAccelOffsetX();
  // mpu.setAccelOffsetY();
  // mpu.setAccelOffsetZ();
  

  
  
  // initialize hmc5883l
  Compass.SetDeclination(23, 35, 'E');
  Compass.SetSamplingMode(COMPASS_SINGLE);
  Compass.SetScale(COMPASS_SCALE_130);
  Compass.SetOrientation(COMPASS_HORIZONTAL_X_NORTH);
}


void loop()
{
portgps.listen();
if (gps.location.isValid()) {
Serial.print("Altitude: ");

*(float*)(data.altitude) = (gps.altitude.meters());
Serial.println(*(float*)data.altitude);

Serial.print("Latitude: ");
*(float*)(data.latitude) = (gps.location.lat());
Serial.println(gps.location.lat());

  Serial.print("Longitude: ");
*(float*)(data.longtitude) = (gps.location.lng());
Serial.println(gps.location.lng());
}
else
 Serial.println( "error");

smartDelay(1000);

if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));

Serial.println();

*(float*)(data.altitude) = (gps.altitude.meters());
Serial.println(*(float*)data.altitude);
Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

 *(float*)(data.temperature) = (bmp.readTemperature());
Serial.println(*(float*)data.temperature);

  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");
  
   *(float*)(data.pressure) = (bmp.readPressure());
Serial.println(*(float*)data.pressure);

  // Calculate altitude assuming 'standard' barometric
  // pressure of 1013.25 millibar = 101325 Pascal
  Serial.print("Altitude = ");
  Serial.print(bmp.readAltitude());
  Serial.println(" meters");
  
  Serial.print("Pressure at sealevel (calculated) = ");
  Serial.print(bmp.readSealevelPressure());
  Serial.println(" Pa");
  
  Serial.print("Real altitude = ");
  Serial.print(bmp.readAltitude(101500));
  Serial.println(" meters");

  *(float*)(data.altitude2) = (bmp.readAltitude());
Serial.println(*(float*)data.altitude2);

  
  Vector rawAccel = mpu.readRawAccel();
  Vector normAccel = mpu.readNormalizeAccel();

  
*(int*)(data.x) = (normAccel.XAxis);
Serial.println(*(int*)data.x);
 *(int*)(data.y) = (normAccel.YAxis);
Serial.println(*(int*)data.y);
 *(int*)(data.z) = (normAccel.ZAxis);
Serial.println(*(int*)data.z);


  float heading = Compass.GetHeadingDegrees();
  Serial.print("Heading: \t");
  Serial.println( heading );

*(float*)(data.heading) = Compass.GetHeadingDegrees();
Serial.println(*(float*)data.heading);

*(float*)(data.parasut) = 0;
Serial.println(*(float*)data.parasut);

*(float*)(data.parasut1) = 0;
Serial.println(*(float*)data.parasut1);

if (bmp.readTemperature()>35.5) {
*(float*)(data.parasut) = 1;
Serial.println(*(float*)data.parasut);
 digitalWrite(p, HIGH);
 delay(5000);
}
else
 *(float*)(data.parasut1) = 0;
Serial.println(*(float*)data.parasut1);
 digitalWrite(p, LOW); 
   
portlora.listen();

ResponseStatus rs = e22ttl.sendFixedMessage(0, 2, 18, &data, sizeof(Signal));
Serial.println(rs.getResponseDescription());

}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (portgps.available())
      gps.encode(portgps.read());
  } while (millis() - start < ms);
}
