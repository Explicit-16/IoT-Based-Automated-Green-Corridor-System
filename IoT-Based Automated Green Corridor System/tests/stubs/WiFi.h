#pragma once
#include <cstdint>
#include <string>
#include <map>
using byte = uint8_t;
using String = std::string;
constexpr int HIGH=1, LOW=0, OUTPUT=1, WL_CONNECTED=3;
inline unsigned long fakeNow=0;
inline std::map<int,int> pinValues;
inline unsigned long millis(){return fakeNow;}
inline void delay(unsigned long ms){fakeNow+=ms;}
inline void pinMode(int,int){}
inline void digitalWrite(int pin,int value){pinValues[pin]=value;}
struct SerialMock {
 void begin(int){}
 template<class T> void print(const T&){}
 template<class T> void println(const T&){}
};
inline SerialMock Serial;
struct WiFiClient {};
struct WiFiMock {
 void begin(const char*,const char*){}
 int status(){return WL_CONNECTED;}
 const char* localIP(){return "127.0.0.1";}
};
inline WiFiMock WiFi;
