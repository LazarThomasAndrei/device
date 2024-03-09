#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


const char* ssid = "HGGuest";
const char* password = "haufegroup!";
const String serverAdress = "https://parking-api.razvan.network/spot/";
const char* uuid = "cafc63fa-be08-4a0e-9ad4-c37d98bf3c04" ; 

const int trigPin1 = 17;
const int echoPin1 = 16;
const int trigPin2 = 2 ;
const int echoPin2 = 4 ;
const int ledBlue = 15;
const int ledRed = 1 ;
const int ledYellow = 21 ; 
bool stare ;
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration1;
float distanceCm1;
float distanceCm2 ; 
long duration2 ;
void setup() {
  Serial.begin(115200); 
  pinMode(trigPin1, OUTPUT); 
  pinMode(echoPin1, INPUT); 
  pinMode(trigPin2, OUTPUT); 
  pinMode(echoPin2, INPUT); 
  pinMode (ledBlue, OUTPUT);
  pinMode (ledRed, OUTPUT);
  pinMode (ledYellow, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  JsonObject requestBody;
  requestBody["uuid"] = uuid;
  String data = serverAdress  + "create";
  if (makePostRequest(data, requestBody)) {
    Serial.println("POST request successful");
  } else {
    Serial.println("POST request failed");
  }

}

void loop() {
  
  digitalWrite (ledBlue, HIGH);	
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
  digitalWrite (ledBlue, LOW);	
  if (distanceCm2 < 30 && distanceCm1 < 30 )
  stare = 0  ;
  else
    stare = 1 ;
 JsonObject patchBody;
 patchBody["uuid"] = uuid;
 patchBody["battery"] = 100;
 patchBody["freeSpot"] = stare ;
 String data = serverAdress  + "notify";
  if (makePatchRequest(data, patchBody)) {
    Serial.println("PATCH request successful");
  } else {
    Serial.println("PATCH request failed");
  }
delay(200000);
}

bool makePostRequest(const String url, JsonObject& body) {
  HTTPClient http;

  // Begin HTTP POST request
  http.begin(url);

  // Add content type header
  http.addHeader("Content-Type", "application/json");

  // Serialize JSON to string
  String jsonString;
  serializeJson(body,jsonString );
  // Make the request with the serialized JSON as the body
  int httpResponseCode = http.POST(jsonString);

  // End HTTP POST request
  http.end();

  // Check response code
  if (httpResponseCode == HTTP_CODE_OK) {
    return true;
  } else {
    Serial.printf("POST request failed, HTTP response code: %d\n", httpResponseCode);
    return false;
  }
}

bool makePatchRequest(const String url, JsonObject& body) {
  HTTPClient http;

  // Begin HTTP PATCH request
  http.begin(url);

  // Add content type header
  http.addHeader("Content-Type", "application/json");

  // Serialize JSON to string
  String jsonString;
  serializeJson(body,jsonString );

  // Make the request with the serialized JSON as the body
  int httpResponseCode = http.PATCH(jsonString);

  // End HTTP PATCH request
  http.end();

  // Check response code
  if (httpResponseCode == HTTP_CODE_OK) {
    return true;
  } else {
    Serial.printf("PATCH request failed, HTTP response code: %d\n", httpResponseCode);
    return false;
  }
}