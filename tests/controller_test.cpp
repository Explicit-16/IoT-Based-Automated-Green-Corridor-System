#include <cassert>
#include <array>
#include <iostream>
#include "../firmware/green_corridor/green_corridor.ino"

void expectDirection(int r,int y,int g,int red,int yellow,int green){
 assert(pinValues[r]==red && pinValues[y]==yellow && pinValues[g]==green);
}
void expectState(int state){
 // 0=NS, 1=EW, 2=all yellow, 3=all red
 for(auto pins : {std::array<int,3>{N_R,N_Y,N_G},std::array<int,3>{S_R,S_Y,S_G}})
  expectDirection(pins[0],pins[1],pins[2],state==1||state==3,state==2,state==0);
 for(auto pins : {std::array<int,3>{E_R,E_Y,E_G},std::array<int,3>{W_R,W_Y,W_G}})
  expectDirection(pins[0],pins[1],pins[2],state==0||state==3,state==2,state==1);
}
void reset(){phase=0;lastChange=0;priorityMode=false;priorityDirection=0;priorityStart=0;lastState=-1;fakeNow=0;}
void send(const std::string& msg){char topic[] = MQTT_TOPIC;callback(topic,(byte*)msg.data(),msg.size());}
int main(){
 reset();setup();expectState(3);assert(fakeNow==3000);assert(client.host==MQTT_HOST && client.port==MQTT_PORT);
 std::cout<<"PASS startup all-red and broker configuration\n";
 client.online=false;reconnect();assert(client.id==MQTT_CLIENT_ID && client.topic==MQTT_TOPIC);
 std::cout<<"PASS MQTT client ID and subscription configuration\n";
 reset();normalTraffic();expectState(0);fakeNow=9999;normalTraffic();expectState(0);
 fakeNow=10000;normalTraffic();expectState(2);fakeNow=11999;normalTraffic();expectState(2);
 fakeNow=12000;normalTraffic();normalTraffic();expectState(1);
 fakeNow=22000;normalTraffic();expectState(2);fakeNow=24000;normalTraffic();normalTraffic();expectState(0);
 std::cout<<"PASS normal cycle and timing boundaries\n";
 for(const auto& msg : {std::string("NS"),std::string("EW")}){
  reset();send(msg);loop();expectState(2);fakeNow=999;loop();expectState(2);
  fakeNow=1000;loop();expectState(msg=="NS"?0:1);fakeNow=10999;loop();expectState(msg=="NS"?0:1);
  fakeNow=11000;loop();assert(!priorityMode && lastChange==11000);loop();expectState(0);
 }
 std::cout<<"PASS NS and EW priority windows and normal return\n";
 reset();phase=1;send("NS");fakeNow=11000;loop();loop();expectState(1);
 std::cout<<"PASS prior normal phase retained after priority\n";
 reset();send("EW");fakeNow=1000;loop();expectState(1);send("BAD");assert(priorityMode && priorityDirection==1);loop();expectState(2);
 std::cout<<"PASS characterized defect: invalid payload reuses previous direction\n";
 reset();send("NS");fakeNow=900;send("NS");fakeNow=1000;loop();expectState(2);
 std::cout<<"PASS characterized behavior: repeated command restarts timer\n";
 std::cout<<"All host tests passed. Hardware, network transport and ESP32 build not tested.\n";
}
