// - - - - -
// ESPTIME - A Arduino library for maintaining an NTP synced clock on an ESP8266
// ESPTIME.cpp: Library implementation file
//
// Copyright (C) 2016 fluepke, kryptokommunist <fabian.luepke<at>student.hpi.de>,<marcus.ding<at>student.hpi.de>
// This work is licensed under a GNU style license.
//
// Last change: kryptokommunist
//
// Documentation and samples are available at https://github.com/nerdmanufaktur/flipdot
// - - - - -
#ifndef ESPTIME_H
#define ESPTIME_H

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>  //by Paul Stoffregen, not included in the Arduino IDE !!!
#include <Timezone.h> //by Jack Christensen, not included in the Arduino IDE !!!
#include <string>

#define NTP_PACKET_SIZE 48

class ESPTime
{
  public:
    ESPTime();
    void printTime();
    uint8_t getHour();
    uint8_t getMinute();
    uint8_t getSecond();
    time_t getNtpTime();
    const char* getFormattedTime();
    const char* getFormattedDate();
  private:
    //UDP
    unsigned int _localPort = 123;
    WiFiUDP _udp;
    char* _ntpServerName1 = "ntp1.t-online.de";
    char* _ntpServerName2 = "time.nist.gov";
    //Timezone
    //Central European Time (Frankfurt, Paris)
    TimeChangeRule _CEST = { "CEST", Last, Sun, Mar, 2, 120 };     //Central European Summer Time
    TimeChangeRule _CET = { "CET ", Last, Sun, Oct, 3, 60 };       //Central European Standard Time
    Timezone *_CE = new Timezone(_CEST, _CET);
    TimeChangeRule *tcr;        //pointer to the time change rule, use to get the TZ abbrev
    time_t _utc, _local;
    byte _packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets
    long _lastTimeUpdate;

    void _sendNTPpacket(IPAddress &address);
    bool updateTime(char* ntpServerName);

    // printing
    void sPrintI00(int val);
    void sPrintDigits(int val);

    // formatting
    std::string getFormattedDigits(int digits);
};

//comment out to disable debugging
//#define DBG

#if defined(DBG)
  #include "Streaming.h"
  #define DBG_OUTPUT_PORT(data) Serial data;
#else
  #define DBG_OUTPUT_PORT(data) ;
#endif


#endif
