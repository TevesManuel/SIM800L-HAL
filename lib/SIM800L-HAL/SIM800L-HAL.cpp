#include <Arduino.h>
#include <SIM800L-HAL.hpp>

bool sendATCommand(SoftwareSerial * serial, const String& command, const String& expectedResponse) {
    serial->println(command);
    while(!serial->available());

    String response = "";
    while (serial->available()) {
        response = serial->readString();
        Serial.print("SIM800L response: ");
        Serial.println(response);
    }

    return response.indexOf(expectedResponse) != -1;
}

SIM800L::SIM800L(u8 rx, u8 tx, String apn) : serial(rx, tx)
{
    this->apn = apn;
}

void SIM800L::begin()
{
    this->serial.begin(9600);
    if(!sendATCommand(&this->serial, "AT", "OK"))
    {
        Serial.println("SIM800L AT ERROR");
    }
    if(!sendATCommand(&this->serial, "AT+CSQ", "OK"))
    {
        Serial.println("Poor signal");
    }
    sendATCommand(&this->serial, "AT+CREG", "OK");

    sendATCommand(&this->serial, "AT+CGATT=1", "OK"); // Attached to GPRS network
    sendATCommand(&this->serial, "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", "OK"); // Set the connection type
    sendATCommand(&this->serial, "AT+SAPBR=3,1,\"APN\",\"" + this->apn + "\"", "OK"); // Set an APN
    sendATCommand(&this->serial, "AT+SAPBR=1,1", "OK"); // Open the context of the APN
}

bool SIM800L::sendHTTP(String url, HTTPMethod method, String data="")
{
    if (sendATCommand(&this->serial, "AT+HTTPINIT", "OK"))
    {
        sendATCommand(&this->serial, "AT+HTTPPARA=\"CID\",1", "OK"); // Select the context of the APN
        sendATCommand(&this->serial, "AT+HTTPPARA=\"URL\",\"" + url + "\"", "OK"); // Set the URL
        if(method == GET)
        {
            if (sendATCommand(&this->serial, "AT+HTTPGET", "200")) { // Send petition
                Serial.println("Solicitud GET exitosa.");
            } else {
                Serial.println("Error en la solicitud GET.");
            }
        }
    }
}

SIM800L::~SIM800L()
{
}