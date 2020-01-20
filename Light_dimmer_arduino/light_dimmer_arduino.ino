//code to provide pwwm control  - need to connect the optocoupler to a pwm pin
//need to refactor this also for esp8266
//need to also add MQTT to control this from openhab
//need to check the voltage and current output to find out what size resistor is needed to protect the opto

//Initializing LED Pin
int led_pin = 6;

void setup() {
  //Declaring LED pin as output
  pinMode(led_pin, OUTPUT);
}

void loop() {
  //Fading the LED
  for(int i=0; i<255; i++){
    analogWrite(led_pin, i);
    delay(5);
  }

  for(int i=255; i>0; i--){
    analogWrite(led_pin, i);
    delay(5);
  }
}