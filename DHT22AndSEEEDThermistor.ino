// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// Depends on the following Arduino libraries:
// - Adafruit Unified Sensor Library: https://github.com/adafruit/Adafruit_Sensor
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN            2         // Pin which is connected to the DHT sensor.

// Uncomment the type of sensor in use:
//#define DHTTYPE           DHT11     // DHT 11 
#define DHTTYPE           DHT22     // DHT 22 (AM2302)
//#define DHTTYPE           DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

double F_temp;

void setup() {
  Serial.begin(9600); 
  // Initialize device.

  dht.begin();
  Serial.println("DHTxx Unified Sensor Example");
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Temperature");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");  
  Serial.println("------------------------------------");
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Humidity");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");  
  Serial.println("------------------------------------");
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;

  pinMode(LED_BUILTIN, OUTPUT);
  
}

void loop() {
  // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.print("Temperature: ");
    F_temp = event.temperature*(9.0/5.0)+32;

    float ThermistorTempX = 0;
    ThermistorTempX = ThermistorTemp();
    
    
    //if(F_temp >= 77.0 || F_temp <= 75) {
    if(ThermistorTempX <= 79) {
      digitalWrite(LED_BUILTIN, HIGH);
    };
      
    if(ThermistorTempX >= 82) { 

      digitalWrite(LED_BUILTIN, LOW);
      
    };
    
      
    
    Serial.print(F_temp);
    Serial.println(" *F");
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
  }
}
///////////////////////////////////////////
float ThermistorTemp(){
  //Temperature from the SEEED Thermistor
  // SEN118A2B

  float A0val = 0;
  float A1val = 0;
  float V2 = 0;
  float Rtherm = 0;
  

  A0val = float(analogRead(0));
  A1val = float(analogRead(1));

  V2 = A0val - A1val;
  Rtherm = float(A1val/V2)*10000;


  //Steinhart
  #define BCoefficient 3950
  #define TNominal 25
  
  float steinhart;
  float steinhartf;

  steinhart = Rtherm/10000; 
  steinhart = log(steinhart);
  steinhart /= BCoefficient;
  steinhart += 1.0 / (TNominal + 273.15);
  steinhart = 1.0 / steinhart; //invert
  steinhart -= 273.15; //convert to C

  //Convert to Farenheit
  steinhartf = steinhart*9/5+32;
  

  Serial.println("_____________");
  Serial.print("A0val =");
  Serial.println(A0val); 
  Serial.print("A1val = ");
  Serial.println(A1val);
  Serial.print("Thermistor Value = ");
  Serial.println(Rtherm);
  Serial.print("Temp in Celsius = ");
  Serial.println(steinhart);
  Serial.print("Temp in Farenheit = ");
  Serial.println(steinhartf);

  return steinhartf;
  
}
