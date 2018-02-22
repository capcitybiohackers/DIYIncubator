#define THERMISTOR1 0
#define THERMISTOR2 1

#define HIGH_THRESHOLD  82
#define LOW_THRESHOLD   79

void setup() {
  Serial.begin(9600); 
  
  pinMode(THERMISTOR1, INPUT);
  pinMode(THERMISTOR2, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  float temp = getThermistorTemp();

  if(temp <= LOW_THRESHOLD) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else if(temp >= HIGH_THRESHOLD) { 
    digitalWrite(LED_BUILTIN, LOW);
  };  
}

float getThermistorTemp(){
  //Temperature from the SEEED Thermistor
  // SEN118A2B

  float A0val = 0;
  float A1val = 0;
  float V2 = 0;
  float Rtherm = 0;
  
  A0val = float(analogRead(THERMISTOR1));
  A1val = float(analogRead(THERMISTOR2));

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
