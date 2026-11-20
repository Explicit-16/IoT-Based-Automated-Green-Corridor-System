#pragma once
#include "WiFi.h"
struct PubSubClient {
 bool online=true;
 std::string host, topic, id;
 int port=0;
 explicit PubSubClient(WiFiClient&){}
 bool connected(){return online;}
 bool connect(const char* value){id=value;online=true;return true;}
 bool subscribe(const char* value){topic=value;return true;}
 int state(){return 0;}
 void setServer(const char* value,int p){host=value;port=p;}
 void setCallback(void (*)(char*,byte*,unsigned int)){}
 void loop(){}
};
