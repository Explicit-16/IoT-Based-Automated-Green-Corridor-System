// ESP32 traffic-signal controller for our IoT green corridor project.
// Wi-Fi and MQTT settings are configured locally in config.h.
// See docs/LIMITATIONS.md before running.
#include <WiFi.h>
#include <PubSubClient.h>

// Local configuration is intentionally excluded from Git.
#include "config.h"
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// ===== MQTT =====
WiFiClient espClient;
PubSubClient client(espClient);

// ===== PINS =====
#define N_R 5
#define N_Y 18
#define N_G 19

#define S_R 21
#define S_Y 22
#define S_G 23

#define E_R 13
#define E_Y 12
#define E_G 14

#define W_R 25
#define W_Y 32
#define W_G 33

// ===== TIMING =====
unsigned long GREEN_TIME = 10000;
unsigned long YELLOW_TIME = 2000;

// ===== STATE TRACKING =====
int lastState = -1;
// 0 = NS GREEN
// 1 = EW GREEN
// 2 = YELLOW
// 3 = RED

// ===== VARIABLES =====
unsigned long lastChange = 0;
int phase = 0;

bool priorityMode = false;
unsigned long priorityStart = 0;
int priorityDirection = 0;

// ===== SIGNAL FUNCTIONS =====
void allRed() {
  if (lastState != 3) {
    Serial.println("🛑 ALL RED");
    lastState = 3;
  }

  digitalWrite(N_R, HIGH); digitalWrite(N_Y, LOW); digitalWrite(N_G, LOW);
  digitalWrite(S_R, HIGH); digitalWrite(S_Y, LOW); digitalWrite(S_G, LOW);
  digitalWrite(E_R, HIGH); digitalWrite(E_Y, LOW); digitalWrite(E_G, LOW);
  digitalWrite(W_R, HIGH); digitalWrite(W_Y, LOW); digitalWrite(W_G, LOW);
}

void allYellow() {
  if (lastState != 2) {
    Serial.println("⚠ ALL YELLOW");
    lastState = 2;
  }

  digitalWrite(N_R, LOW); digitalWrite(N_Y, HIGH); digitalWrite(N_G, LOW);
  digitalWrite(S_R, LOW); digitalWrite(S_Y, HIGH); digitalWrite(S_G, LOW);
  digitalWrite(E_R, LOW); digitalWrite(E_Y, HIGH); digitalWrite(E_G, LOW);
  digitalWrite(W_R, LOW); digitalWrite(W_Y, HIGH); digitalWrite(W_G, LOW);
}

void setNS() {
  if (lastState != 0) {
    Serial.println("🚦 NS GREEN | EW RED");
    lastState = 0;
  }

  digitalWrite(N_R, LOW); digitalWrite(N_Y, LOW); digitalWrite(N_G, HIGH);
  digitalWrite(S_R, LOW); digitalWrite(S_Y, LOW); digitalWrite(S_G, HIGH);

  digitalWrite(E_R, HIGH); digitalWrite(E_Y, LOW); digitalWrite(E_G, LOW);
  digitalWrite(W_R, HIGH); digitalWrite(W_Y, LOW); digitalWrite(W_G, LOW);
}

void setEW() {
  if (lastState != 1) {
    Serial.println("🚦 EW GREEN | NS RED");
    lastState = 1;
  }

  digitalWrite(E_R, LOW); digitalWrite(E_Y, LOW); digitalWrite(E_G, HIGH);
  digitalWrite(W_R, LOW); digitalWrite(W_Y, LOW); digitalWrite(W_G, HIGH);

  digitalWrite(N_R, HIGH); digitalWrite(N_Y, LOW); digitalWrite(N_G, LOW);
  digitalWrite(S_R, HIGH); digitalWrite(S_Y, LOW); digitalWrite(S_G, LOW);
}

// ===== NORMAL TRAFFIC =====
void normalTraffic() {
  unsigned long now = millis();

  if (phase == 0) {
    if (now - lastChange < GREEN_TIME) {
      setNS();
    }
    else if (now - lastChange < GREEN_TIME + YELLOW_TIME) {
      allYellow();
    }
    else {
      phase = 1;
      lastChange = now;
    }
  }
  else {
    if (now - lastChange < GREEN_TIME) {
      setEW();
    }
    else if (now - lastChange < GREEN_TIME + YELLOW_TIME) {
      allYellow();
    }
    else {
      phase = 0;
      lastChange = now;
    }
  }
}

// ===== MQTT CALLBACK =====
void callback(char* topic, byte* payload, unsigned int len) {
  String msg = "";

  for (int i = 0; i < len; i++) {
    msg += (char)payload[i];
  }

  Serial.println("\n🚑 PRIORITY RECEIVED: " + msg);

  priorityMode = true;
  priorityStart = millis();

  if (msg == "NS") {
    priorityDirection = 0;
    Serial.println("➡ PRIORITY: NORTH-SOUTH");
  }
  else if (msg == "EW") {
    priorityDirection = 1;
    Serial.println("➡ PRIORITY: EAST-WEST");
  }
}

// ===== MQTT RECONNECT =====
void reconnect() {
  while (!client.connected()) {
    Serial.print("🔌 Connecting MQTT...");

    if (client.connect(MQTT_CLIENT_ID)) {
      Serial.println("✅ Connected");
      client.subscribe(MQTT_TOPIC);
    } else {
      Serial.print("❌ Failed: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);

  pinMode(N_R, OUTPUT); pinMode(N_Y, OUTPUT); pinMode(N_G, OUTPUT);
  pinMode(S_R, OUTPUT); pinMode(S_Y, OUTPUT); pinMode(S_G, OUTPUT);
  pinMode(E_R, OUTPUT); pinMode(E_Y, OUTPUT); pinMode(E_G, OUTPUT);
  pinMode(W_R, OUTPUT); pinMode(W_Y, OUTPUT); pinMode(W_G, OUTPUT);

  // SAFE START
  allRed();
  delay(3000);

  // WIFI
  Serial.println("📡 Connecting WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ WiFi Connected");
  Serial.println(WiFi.localIP());

  // MQTT
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(callback);

  lastChange = millis();
}

// ===== LOOP =====
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // 🚑 PRIORITY MODE
  if (priorityMode) {
    unsigned long now = millis();

    if (now - priorityStart < 1000) {
      allYellow();
    }
    else if (now - priorityStart < 11000) {
      if (priorityDirection == 0) setNS();
      else setEW();
    }
    else {
      Serial.println("🔄 Back to normal mode");
      priorityMode = false;
      lastChange = millis();
    }

    return;
  }

  // 🟢 NORMAL TRAFFIC
  normalTraffic();
}
