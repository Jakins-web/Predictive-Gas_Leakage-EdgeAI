#include <WiFi.h>
#include <HTTPClient.h>
#include "ThingSpeak.h"

// --- CREDENTIALS ---
const char* ssid = "Akinola";
const char* password = "akinolamiolu";
unsigned long myChannelNumber = 3334760;
const char* myWriteAPIKey = "TIEKIT9CECVGZV18";
const char* discord_webhook = "https://discordapp.com/api/webhooks/1491935071496831167/3LLyhbr8k3o4hRxWyMcFwfxtQ_n6-6NW-NAXH_MedU7Zn2539_tQ8Wv52m2jQc2AhuR5";

// --- PIN DEFINITIONS ---
const int MQ2_PIN = 34;
const int LED_GREEN = 18;
const int LED_YELLOW = 19;
const int LED_RED = 21;
const int BUZZER = 25;

// --- THRESHOLDS ---
int warningLevel = 300;
int dangerLevel = 1000;

unsigned long lastLogTime = 0;
unsigned long logInterval = 20000; // 20 seconds

unsigned long lastAlertTime = 0;
unsigned long alertCooldown = 60000; // 1 minute cooldown

WiFiClient client;

void setup() {
  Serial.begin(115200);
  
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); 
    Serial.print(".");
  }
  
  ThingSpeak.begin(client);
  Serial.println("\nSystem Online");
}

void loop() {
  int gasValue = analogRead(MQ2_PIN);
  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  // --- ALARM LOGIC ---
  if (gasValue < warningLevel) {
    // NORMAL STATE
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);
    noTone(BUZZER);
  } 
  else if (gasValue >= warningLevel && gasValue < dangerLevel) {
    // WARNING STATE
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_RED, LOW);
    digitalWrite(BUZZER, HIGH); 
  } 
  else if (gasValue >= dangerLevel) {
    // DANGER STATE
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, HIGH);
    digitalWrite(BUZZER, HIGH); 

    // Trigger Discord Alert with Cooldown
    if (millis() - lastAlertTime > alertCooldown) {
      sendDiscordAlert(gasValue);
      lastAlertTime = millis();
    }
  }

  // --- THINGSPEAK LOGGING ---
  if (millis() - lastLogTime > logInterval) {
    ThingSpeak.setField(1, gasValue);
    int status = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    
    if (status == 200) {
      Serial.println("Logged to ThingSpeak successfully.");
    } else {
      Serial.println("Error logging to ThingSpeak: " + String(status));
    }
    lastLogTime = millis();
  }

  delay(2000); 
}

// Function to send Discord Notification
void sendDiscordAlert(int value) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(discord_webhook);
    http.addHeader("Content-Type", "application/json");

    // Construct the JSON message for Discord
    String message = "{"content": "⚠️ <@1240998257828565165> **GAS LEAK DETECTED!** \nCurrent Value: " + String(value) + ""}";
    int response = http.POST(message);
    if (response > 0) {
      Serial.println("Discord message sent!");
    }
    http.end();
  }
}
