#include <Arduino.h>

#include <SIM800L-HAL.hpp>

#define APN "internet.movil"

SIM800L sim800l(9, 10, APN);

void setup()
{
    Serial.begin(9600);
    Serial.println("Hi");
    sim800l.begin();
    sim800l.sendHTTP("www.google.com", GET);
}

void loop()
{

}