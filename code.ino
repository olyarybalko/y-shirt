// UNICE 2017 Olga MELNYK & Ahlam BOUGHAZI //

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM9DS0.h>
#include <Adafruit_NeoPixel.h>

#define PIN            12
#define NUMPIXELS      1
#define seuil          140.0
Adafruit_NeoPixel   pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_LSM9DS0    lsm = Adafruit_LSM9DS0(1000);
int couleur_courant = -1;
int delayval = 50;
sensors_event_t accel, mag, gyro, temp;

void configureSensor(void)
{
    lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_2G);
    lsm.setupMag(lsm.LSM9DS0_MAGGAIN_2GAUSS);
    lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_245DPS);
}

void software_Reset()
{
    asm volatile ("  jmp 0");
}

void  print_out_accelleration_data()
{
    Serial.print(millis());
    Serial.print(" ");
    Serial.print(accel.acceleration.x);
    Serial.print(" ");
    Serial.print(accel.acceleration.y);
    Serial.print(" ");
    Serial.print(accel.acceleration.z);
    Serial.print(" ");
}

double compute_tilt()
{
    double tmp_a, tmp_tilt;
    tmp_a = (1/9.81)*sqrt(accel.acceleration.x*accel.acceleration.x + accel.acceleration.y*accel.acceleration.y + accel.acceleration.z*accel.acceleration.z);
    Serial.print(tmp_a);
    Serial.print(F(" "));
    tmp_tilt = 180*(acos(accel.acceleration.y/9.81)/3.1415);
    Serial.print(tmp_tilt);
    Serial.print(F(" "));
    return tmp_tilt;
}

void analyse_tilt (double current_tilt)
{
    if (current_tilt  <=  seuil )
    {
        Serial.print(1);
        if (couleur_courant == 0)
        {
            pixels.setPixelColor(0, pixels.Color(250,0,0));
            pixels.show();
        }
        couleur_courant = 1;
    }
    else if (current_tilt  >  seuil  )
    {
        Serial.print(0);
        if (couleur_courant == 1)
        {
            pixels.setPixelColor(0, pixels.Color(0,150,0));
            pixels.show();
            couleur_courant = 0;
        }
    }
    Serial.println();
    delay(delayval);
}

void setup()
{
    Serial.begin(9600);
    Serial.println(F("LSM9DS0 9DOF Sensor Test\n"));
    if(!lsm.begin())
    {
        Serial.print(F("Ooops, no LSM9DS0 detected ... Check your wiring or I2C ADDR!"));
        software_Reset();
    }
    Serial.println(F("Found LSM9DS0 9DOF"));
    configureSensor();
    pixels.begin();
}

void loop()
{
    double tilt=.0;
    lsm.getEvent(&accel, &mag, &gyro, &temp);
    print_out_accelleration_data();
    tilt = compute_tilt();
    analyse_tilt(tilt);
}
