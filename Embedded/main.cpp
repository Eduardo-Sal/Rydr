#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "awsprovisions.h" //Include the AWS provisions header which will contain AWS IoT credentials based on your created policy 

//WiFi Credentials TOBE replaced with IoT simcards
const char *ssid = "";  //Wifi name here ex: MySpectrumWifiCA, cap sensitive
const char *password = ""; //Password here, cap sensitive

//MQTT AWS 
#define AWS_IOT_PUBLISH_TOPIC   "" //AWS publish route usually <thingname>/pub
#define AWS_IOT_SUBSCRIBE_TOPIC "" //AWS sub route ex: <thingname>/sub

//Declare Wifi Object
WiFiClientSecure net;
PubSubClient client(net);

//GPS Simulation dummy variables
float latitude = 100.233301;  
float longitude = -100.233301; 

//Simulating GPS drift for MQTT testing
void simulateGPS(float &lat, float &lon) {
  lat += (random(-5, 5) * 0.0001); //Randomize latiude
  lon += (random(-5, 5) * 0.0001); //Randomize longitude
}

//Connect to WiFi
void connectWiFi() {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");
}

//Callback function for incoming MQTT messages
void messageHandler(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message received on topic ");
  Serial.print(topic);
  Serial.print(": ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

//Connect to AWS IoT Core 
//IMPORTANT this function heavily relies on certificates based on your IoT console which produces certificates which you will put in awsprovisions.h
void connectAWS() {
  //Configure WiFiClientSecure
  net.setCACert(rootCA);
  net.setCertificate(deviceCert);
  net.setPrivateKey(privateKey);

  //Connect to the MQTT broker on the AWS IoT endpoint
  client.setServer(awsEndpoint, port);
  client.setCallback(messageHandler);

  Serial.println("Connecting to AWS IoT Core");
  while (!client.connect("")) { //Put your Thing name in this empty string field
    Serial.print(".");
    delay(500);
  }

  if (client.connected()) {
    Serial.println("\nConnected to AWS IoT Core!");
    client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC); //Subscribe to the topic
    Serial.println("Subscribed to topic: " AWS_IOT_SUBSCRIBE_TOPIC);
  } else {
    Serial.println("AWS IoT Connection Failed!");
  }
}

//Publish a simulated GPS message to the AWS IoT Core
void publishGPS() {
  if (client.connected()) {
    char jsonBuffer[128];
    snprintf(jsonBuffer, sizeof(jsonBuffer), "{\"latitude\": %.6f, \"longitude\": %.6f}", latitude, longitude);
    client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
    Serial.println("Published GPS coordinates: " + String(jsonBuffer));
  } else {
    Serial.println("Failed to publish. MQTT client is not connected.");
  }
}

//Setup function
void setup() {
  Serial.begin(115200);
  connectWiFi();  // Connect to Wi-Fi
  connectAWS();   // Connect to AWS IoT Core
}

//Loop function
void loop() {
  simulateGPS(latitude, longitude);  // Simulate GPS changes
  publishGPS();                      // Publish GPS coordinates to AWS IoT
  client.loop();                     // Ensure the client is listening for messages
  delay(10000);                      // Publish every 10 seconds
}
