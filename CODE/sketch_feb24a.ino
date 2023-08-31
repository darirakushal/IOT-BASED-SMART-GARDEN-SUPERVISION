// IR SENSOR -->5(D1)
// MQ-135 SENSOR -->16(D0)
// DHT SENSOR -->4(D2)
// [R1] NIGTH LAMP -->0(D3)
// [R2] CHECK LIGHT -->2(D4)
// MOISTURE SENSOR -->12(D6)
// [R4] ALARM -->12(D7)
// [R3] MOTOR -->15(D8)
// RAIN SENSOR -->14(D5)


#include <Adafruit_Sensor.h>
#include "FirebaseESP8266.h"	// Install Firebase ESP8266 library
#include <ESP8266WiFi.h>
#include <DHT.h>		// Install DHT11 Library and Adafruit Unified Sensor Library


#define FIREBASE_HOST "temp-hymp-default-rtdb.firebaseio.com" //Without http:// or https:// schemes
#define FIREBASE_AUTH "sdgfM1S0Zh7piKSmWyyG9sJ2fP9o9ccVpMwHMxsh"
#define WIFI_SSID "RAVIDARIRA"
#define WIFI_PASSWORD "RSPK*989970"

//Data Pin's
#define DHTPIN 4		// DHT to D2
#define DHTTYPE    DHT11
DHT dht(DHTPIN, DHTTYPE);

//int moistureSensor 
const int moisturePin = 12;

//RELAY MODULE
int CHECKLIGHT = 2;	
int ALARM = 13;
int MOTOR = 15;
int NIGHTLAMP = 0;

//MQ-135 AIR QUALITY SENSOR
#define anInput     16                        //analog feed from MQ135
#define co2Zero     55  

//IR SENSORE
int SENSORPin = 5;

//RAIN SENSOR
int RainPin = 14;

//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseData ledData;
FirebaseJson json;


void setup()
{

  Serial.begin(9600);

  dht.begin();
  
  pinMode(CHECKLIGHT, OUTPUT);
  pinMode(ALARM, OUTPUT);
  pinMode(MOTOR, OUTPUT);
  pinMode(NIGHTLAMP, OUTPUT);  

  pinMode(moisturePin, INPUT);

  pinMode(anInput, INPUT);

  pinMode(SENSORPin, INPUT);

  pinMode(RainPin, INPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}

void sensorUpdate(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  // Read Moisture Sensor

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C  ,"));
  Serial.print(f);
  Serial.println(F("°F  "));

  //MQ-135
  int a =analogRead(anInput);
  Serial.println(a);
  int per =map(a,100,450,0,100);
  Serial.println(per);



  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/Temperature", t))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/Humidity", h))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }


//MQ 135
  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/MQ 135", per))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }


  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/AIR QUALITY", a))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
}

void loop() {
  sensorUpdate();

  //IR 
  int SENSORValue = analogRead(SENSORPin);

  int SENSORPercentage = (SENSORValue);

  Serial.print("IR SENSOR: ");
  Serial.print(SENSORPercentage);
  Serial.println(" ");

  if (Firebase.setInt(firebaseData, "/FirebaseIOT/IR SENSOR", SENSORPercentage))
  {
    Serial.println("IR SENSOR value uploaded to Firebase");
  } 
  else
  {
    Serial.println("Error uploading IR VALUE to Firebase");
    Serial.println(firebaseData.errorReason());
  }

  //rain 
    int RainValue = analogRead(RainPin);

  int RainPercentage = (RainValue);

  Serial.print("IR SENSOR: ");
  Serial.print(RainPercentage);
  Serial.println(" ");

  if (Firebase.setInt(firebaseData, "/FirebaseIOT/RAIN SENSOR", RainPercentage))
  {
    Serial.println("IR SENSOR value uploaded to Firebase");
  } 
  else
  {
    Serial.println("Error uploading IR VALUE to Firebase");
    Serial.println(firebaseData.errorReason());
  }
  
  
  if (Firebase.getString(ledData, "/FirebaseIOT/CHECK LIGHT"))
  {
    Serial.println(ledData.stringData());
    if (ledData.stringData() == "1") 
    {
    digitalWrite(CHECKLIGHT, HIGH);
    }
  else if (ledData.stringData() == "0")
  {
    digitalWrite(CHECKLIGHT, LOW);
  }
  }
  
    if (Firebase.getString(ledData, "/FirebaseIOT/ALARM"))
    {
    Serial.println(ledData.stringData());
    if (ledData.stringData() == "1") {
    digitalWrite(ALARM, HIGH);
    }
  else if (ledData.stringData() == "0")
    {
    digitalWrite(ALARM, LOW);
    }
  }

    if (Firebase.getString(ledData, "/FirebaseIOT/MOTOR")){
    Serial.println(ledData.stringData());
    if (ledData.stringData() == "1") {
    digitalWrite(MOTOR, HIGH);
    }
  else if (ledData.stringData() == "0"){
    digitalWrite(MOTOR, LOW);
    }
  }

    if (Firebase.getString(ledData, "/FirebaseIOT/NIGHT LAMP")){
    Serial.println(ledData.stringData());
    if (ledData.stringData() == "1") {
    digitalWrite(NIGHTLAMP, HIGH);
    }
  else if (ledData.stringData() == "0"){
    digitalWrite(NIGHTLAMP, LOW);
    }
  }

  // Read the moisture sensor value
  int moistureValue = analogRead(moisturePin);

  // Convert the analog value to percentage
  int moisturePercentage = map(moistureValue, 0, 1023, 0, 100);

  Serial.print("Moisture: ");
  Serial.print(moisturePercentage);
  Serial.println(" ");
  

  // Upload the moisture value to Firebase
  if (Firebase.setInt(firebaseData, "/FirebaseIOT/Moisture", moisturePercentage))
  {
    Serial.println("Moisture value uploaded to Firebase");
  } 
  else
  {
    Serial.println("Error uploading moisture value to Firebase");
    Serial.println(firebaseData.errorReason());
  }
  

  delay(100);
}  
