#include <Arduino.h>

const int SerialPort = 9600;
const float dry_reading = 630.0;
const float wet_reading = 330.0;

struct planter
{
  char * Description;
  char * RelayNode;
  int SensorPin;
  int PumpPin;
  int QuantityPerIrrigation;          // Wanted amount of Mililiter (ml) per irrigation
  int IrrigationTimeToAmountRatio; // amount of miliseconds needed to gain 1 Mililiter (ml). should be tested per hose
  int IrrigationInterval;
  int IrrigationDuration;
//  int IrrigationDuration =  QuantityPerIrrigation * IrrigationTimeToAmountRatio;
};
//++++++++++++++++++++ DEFINED PLANTERS ++++++++++++++++++++//

planter Plantes[] =
{
  {"NANA + CHILLI", "IN2", A0, 2, 40, 54, 86400000 * 4, 40*54}, //FINILIZED DO NO CHANGE!
  {"TROPICAL", "IN3", A2, 3, 20, 54, 86400000 * 7, 20*54},     //FINILIZED DO NO CHANGE!
  {"Rosemarie", "IN4", A1, 4, 40, 54, 86400000 * 7 , 40*54}   //FINILIZED DO NO CHANGE!
};

//++++++++++++++++++++ DEFINED PLANTERS ++++++++++++++++++++//

void setup() {
  Serial.begin(SerialPort); // open serial port
  // Reset Pumps
  for (int pinNumber = 2; pinNumber <= 4; pinNumber++) {
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
  }

}

void loop() {
  Serial.println("[>] Starting watering protocol");

  delay(10*1000);
  Serial.println(Plantes[0].Description);
  pump_water(Plantes[0].PumpPin, Plantes[0].IrrigationDuration, Plantes[0].RelayNode);

  delay(10*1000);
  Serial.println(Plantes[2].Description);
  pump_water(Plantes[1].PumpPin, Plantes[1].IrrigationDuration, Plantes[1].RelayNode);

  delay(10*1000);
  Serial.println(Plantes[2].Description);
  pump_water(Plantes[2].PumpPin, Plantes[2].IrrigationDuration, Plantes[2].RelayNode);

  Serial.println("[X] All done for today. restarting in 1 week");
  delay(86400000 *7); //wait another day
}

float measure_humidity(int sensor) {
  int moisture_sensor_reading = analogRead(sensor);
  float moisture_value = map(moisture_sensor_reading, dry_reading, wet_reading, 0, 100);
  float humidity = constrain(moisture_value, 0, 100); // Keep values between 0% and 100%, and not beyond
  Serial.print("[+] humidity %: ");
  Serial.print(humidity);
  Serial.print("\r\n");
  return humidity;
}

void pump_water(float pumpNo, float pump_duration, char * desc) {
  Serial.println("----");
  Serial.print("[+] Started Pump ");
  Serial.println(desc);
  Serial.println(".");

  digitalWrite(pumpNo, HIGH);
  delay(pump_duration);
  digitalWrite(pumpNo, LOW);

  Serial.print("[+] Stopped Pump ");
  Serial.println(desc);
  Serial.println(".");
  Serial.print("[+] time:");
  Serial.print(pump_duration / 1000);
  Serial.println("secs.");
}
