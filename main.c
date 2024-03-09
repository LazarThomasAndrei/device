#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* endpoint = "YOUR_ENDPOINT_URL";

const int trigPin1 = 23;
const int echoPin1 = 22;
const int trigPin2 = 33;
const int echoPin2 = 32;

int distance1;
int distance2;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  
  distance1 = getDistance(trigPin1, echoPin1);
  distance2 = getDistance(trigPin2, echoPin2);

  
  if (distance1 < 30 && distance2 < 30) {
    // Send a request to the endpoint
    HTTPClient http;
    http.begin(endpoint);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP response code: ");
      Serial.println(httpResponseCode);
      String response = http.getString();
      Serial.println(response);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
  
  delay(1000); 
}

int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;

  return distance;
}
