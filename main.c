#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
const char* ssid = "Razvan - Rabb";
const char* password = "0809852001";
const String serverAdress = "https://parking-api.razvan.network/spot/";
const String uuid = "cafc63fa-be08-4a0e-9ad4-c37d98bf3c04" ; 
const int trigPin1 = 17;
const int echoPin1 = 16;
const int trigPin2 = 2 ;
const int echoPin2 = 4 ;
const int ledBlue = 15;
const int ledRed = 1 ;
const int ledYellow = 21 ; 
String stare ;
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
long duration1;
float distanceCm1;
float distanceCm2 ; 
long duration2 ;
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(trigPin1, OUTPUT); 
  pinMode(echoPin1, INPUT); 
  pinMode(trigPin2, OUTPUT); 
  pinMode(echoPin2, INPUT); 
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    retries++;
    if (retries > 10) {
      Serial.println("Failed to connect to WiFi");
      return;
    }
  }
  Serial.println("Connected to WiFi");
  String body = "{\"uuid\": \""+ uuid + "\"}";
  String data = serverAdress  + "create";
  if (makePostRequest(data, body)) {
    Serial.println("POST request successful");
  } else {
    Serial.println("POST request failed");
  }
}

void loop() {
  
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distanceCm1 = duration1 * SOUND_SPEED/2;
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distanceCm2 = duration2 * SOUND_SPEED/2;
  delay(1000);	
  delay(500);
 stare = (distanceCm2 < 30 || distanceCm1 < 30) ? "false" : "true";
 Serial.println(distanceCm1); 
 Serial.println(distanceCm2);
 String patchBody = "{\"uuid\": \"" + uuid + "\", \"battery\": 100, \"freeSpot\": " + stare+ "}";
 Serial.println(patchBody);
 String data = serverAdress  + "notify";
  if (makePatchRequest(data, patchBody)) {
    Serial.println("PATCH request successful");
  } else {
    Serial.println("PATCH request failed");
  }
delay(10000);
}
bool makePostRequest(const String url, String body) {
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(body);
  http.end();
  if (httpResponseCode == HTTP_CODE_OK) {
    return true;
  } else {
    Serial.printf("POST request failed, HTTP response code: %d\n", httpResponseCode);
    return false;
  }
}

bool makePatchRequest(const String url, String body) {
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.PATCH(body);
  http.end();
  if (httpResponseCode == HTTP_CODE_OK) {
    return true;
  } else {
    Serial.printf("PATCH request failed, HTTP response code: %d\n", httpResponseCode);
    return false;
  }
}