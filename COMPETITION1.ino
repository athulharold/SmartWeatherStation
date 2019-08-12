
#define BLYNK_PRINT Serial

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
int LDRpin=A0;
float Humidity,Temperature,Light;
int hotness=0;
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "a8756ed9b0f244f8aa62d700b2e84630";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Helloitsme";
char pass[] = "arya1999";

#define DHTPIN 2          // What digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void calculate()
{
   
    delayMicroseconds(20);
 
    
    
    Serial.print("Current humidity = ");
    Serial.print(dht.readHumidity());
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(dht.readTemperature()); 
    Serial.println("C  \n");
    int LDRValue = (float)analogRead(LDRpin); // read the value from the LDR
    Serial.println(LDRValue);      // print the value to the serial port
    Humidity = dht.readHumidity();
    Temperature = dht.readTemperature();
    Light = LDRValue;
    delay(5000);//Wait 5 seconds before accessing sensor again.
    int possibility;
    if(Humidity  > 70.0){
      if(Temperature >= -4.0 && Temperature < 10)
        if(Light < 500) possibility = 3;
        else if(Light < 700) possibility = 2;
        else if(Light < 800) possibility = 1;
        else if(Light < 1000) possibility = 0;
        else possibility = -1;
      else if(Temperature >= 10 && Temperature < 17)
        if(Light < 500)  possibility = 4; 
        else if(Light < 700) possibility = 3;
        else if(Light < 800) possibility = 2;
        else if(Light < 1000) possibility = 1;
        else possibility = 0;
      else if(Temperature >=17 && Temperature < 27)
        if(Light < 500) possibility = 5;
        else if(Light < 700) possibility = 4;
        else if(Light < 800) possibility = 3;
        else if(Light < 1000) possibility = 1;
        else possibility = 0;
      else if(Temperature >=27)
        if(Light < 500) possibility = 5;
        else if(Light < 700) possibility = 4;
        else if(Light < 800) possibility = 3;
        else if(Light < 1000) possibility = 1;
        else{
          possibility = 0; 
          hotness=1;
          digitalWrite(16,HIGH);
        }
    }
    
    else if(Humidity <= 65.0){
        possibility = 0;
        if(Temperature>30) {
          hotness=1;
          digitalWrite(16,HIGH);
        }
    }
   
    
    switch (possibility){
      case -1:
        if(hotness==1)
        {
          Blynk.virtualWrite(V0,"It's hot outside!!");
          //trigger
        }
        else Blynk.virtualWrite(V0," very low chance of rain");
        break;
      case 0:
        Serial.println("There is a very low possibility of rain \n");
        Blynk.virtualWrite(V0," very low chance of rain");
        break;
      case 1:
      
        Serial.print("There is a low possibility of rain \n");
        Blynk.virtualWrite(V0,"Low chances of rain \n");
        break;
      case 2:
        Serial.print("There is a slight chance of rain \n");
        Blynk.virtualWrite(V0,"Slight chance of rain \n");
        break;
      case 3:
        Serial.print("There is moderate chance of rain \n");
        Blynk.virtualWrite(V0,"Moderate chance of rain \n");
        break;
      case 4:
        Serial.print("There is a high possibility of rain \n");
        Blynk.virtualWrite(V0,"High chance of rain");
        break;
      case 5:
        Serial.print("There is a very high possibility of rain \n");
        Blynk.virtualWrite(V0,"Very high chance of rain");
    }
}

void sendSensor()
{
  calculate();
   
  if (isnan(Humidity) || isnan(Temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  } else {
    Serial.print(Humidity); Serial.print("   ");Serial.println(Temperature);
  
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, Humidity);
  Blynk.virtualWrite(V6,Temperature);
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  /*WiFi.begin("Helloitsme", "arya1999");   //WiFi connection
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println("Waiting for connection");
 
  }

  callIFTTT();
 */
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  dht.begin();
  

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}
/*void callIFTTT(){
  
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
 Serial.println("Connected");
   HTTPClient http;    //Declare object of class HTTPClient
 
   http.begin("http://maker.ifttt.com/trigger/gopakumar/with/key/bL7jokzxNCxT9tNzXeRJgKZY9TwgdfLJsHaNsasmghy");      //Specify request destination
   http.addHeader("Content-Type", "text/plain");  //Specify content-type header
 
   int httpCode = http.POST("Message from ESP8266");   //Send the request
   String payload = http.getString();                  //Get the response payload
 
   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload
 
   http.end();  //Close connection
 
 }else{
 
    Serial.println("Error in WiFi connection");   
 
 }
}
*/

void loop()
{
  
  Blynk.run();
  timer.run();
}
