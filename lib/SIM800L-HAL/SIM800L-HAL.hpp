#ifndef TEVES_SIM800L_HAL
#define TEVES_SIM800L_HAL

#include <Arduino.h>
#include <SoftwareSerial.h>

#define u8 int8_t

typedef enum{
    GET,
    POST
} HTTPMethod;

class SIM800L
{
    private:
        SoftwareSerial serial;
        String apn;
    public:
        SIM800L(u8 rx, u8 tx, String apn);
        void begin();
        bool sendHTTP(String url, HTTPMethod method, String data="");
        ~SIM800L();
};


#endif