#include <ESP8266WiFi.h>
#include <GoProControl.h>
#include "Secrets.h"

int sensorValue = 0;
boolean fotoTirada = false;
boolean ligado = true;

int buttonState = 0;

#define SENSOR_PIN A0
#define MIN_VALUE 25
#define MAX_VALUE 800

GoProControl gp(GOPRO_SSID, GOPRO_PASS, HERO4);

void setup()
{
  gp.enableDebug(&Serial);
  gp.begin();
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  pinMode(14, INPUT);
}

void loop()
{
  if (ligado == true)
  {


    sensorValue = analogRead(SENSOR_PIN);
    float level = 100 - ((sensorValue - MIN_VALUE) * 100 / (MAX_VALUE - MIN_VALUE));
    sensorValue = level;

    Serial.println(sensorValue);

    if (gp.isConnected() == true)
    {
      gp.printStatus();
      delay(500);
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
    }
    else
    {
      gp.end();
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      gp.begin();
      delay(10000);
    }

    if (sensorValue > -28 && fotoTirada == false)
    {
      fotoTirada = gp.shoot();
      if (fotoTirada)
      {
        delay(500);
      }
    }
    if (sensorValue <= -28)
    {
      fotoTirada = false;
    }
  }
  else
  {
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }
}
