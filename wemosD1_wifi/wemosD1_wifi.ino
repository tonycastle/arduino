#include <WiFi.h>
#include <PubSubClient.h>
#include <BH1750FVI.h>
#include <Wire.h>

//SSID of your network
char ssid[] = "TopcatTC"; //SSID of your Wi-Fi router
char pass[] = "Qzp3pfnt5enP"; //Password of your Wi-Fi router
const char* mqtt_server = "192.168.0.178";  //MQTT  server address not yet known

WiFiClient espClient;

PubSubClient client(espClient);
// Create the Lightsensor instance
BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);

void readLightLevel(){
  uint16_t lux = LightSensor.GetLightIntensity();
  float ppf = lux / 69.0;
  char ppfStr[15];
  dtostrf(ppf,6,2,ppfStr); 
  Serial.print("Light (lux): ");
  Serial.print(lux);
  Serial.print("     PPF:");
  Serial.println(ppfStr);
  client.publish("/grow/sensor/ec",ppfStr);  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ECMeter")) {
      Serial.println("MQTT connected");
      // Once connected, publish an announcement...
      while(true){
        readLightLevel();
        delay(1000);
      }
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup()
{
  Serial.begin(115200);
  delay(10);
  
  //connectt o the wifi network
  setup_wifi();

  //setup MQTT
  client.setServer(mqtt_server, 1883); //thiis id the ip address of the host running openHab and the default mqtt port

   LightSensor.begin();  
}

//function to create the wifi conection
void setup_wifi()
{
   // Connect to Wi-Fi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to...");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wi-Fi connected successfully");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void loop () {
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
