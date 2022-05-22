 #include <LiquidCrystal.h>

#define CONTRAST 60    // LCD screen constrast 
#define AIRVAL 750   //you need to replace this value with Value_1
#define WATERVAL 325  //you need to replace this value with Value_2
int soilMoistureValue = 0;
int soilMoisturePercent=0;
int targetMoisturePercent= 0;
int motorPin = 7;
int contrastPin = 10;
int desiredMoistureValue;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  pinMode(motorPin, OUTPUT);
  Serial.begin(9600);// open serial port, set the baud rate to 9600 bps
  analogWrite(contrastPin, CONTRAST);
  lcd.begin(16, 2);
  lcd.clear();
}

void loop() {

  // Collect and process sensor data
  desiredMoistureValue = analogRead(A1);
	soilMoistureValue = analogRead(A0);  
	soilMoisturePercent = map(soilMoistureValue, AIRVAL, WATERVAL, 0, 100);
  targetMoisturePercent = map(desiredMoistureValue, 0, 1023, 0, 100);
  

  if (soilMoisturePercent < targetMoisturePercent - 5){
    while (soilMoisturePercent < targetMoisturePercent + 10) {
      digitalWrite(motorPin, HIGH); // Turn on motor

      // Read sensors
      desiredMoistureValue = analogRead(A1);
      soilMoistureValue = analogRead(A0);  
      soilMoisturePercent = map(soilMoistureValue, AIRVAL, WATERVAL, 0, 100);
      targetMoisturePercent = map(desiredMoistureValue, 0, 1023, 0, 100);

      printMoistureValue(soilMoisturePercent, targetMoisturePercent);
      //Serial.println(soilMoistureValue);
    }
  }
  
  analogWrite(motorPin, 0);
  printMoistureValue(soilMoisturePercent, targetMoisturePercent);
  //Serial.println(soilMoistureValue);
}


void printMoistureValue(int smp, int dmv) {
  //Serial.print(smp);
  //Serial.println("%");
  
  if (smp < 0) {
    smp = 0;
  } else if (smp > 100) {
    smp = 100;
  }

  lcd.setCursor(0, 0);
  lcd.print("Current:");
  lcd.setCursor(9, 0);
  lcd.print("Goal:");
  lcd.setCursor(5, 1);
  lcd.print(smp);
  lcd.print(" %");
  lcd.setCursor(10, 1);
  lcd.print(dmv);
  lcd.print(" %");
  delay(250);
  lcd.clear();
}
