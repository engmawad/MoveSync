#include <DHT.h>
#include <DHT_U.h>

#define m1b 2
#define m1f 4
#define m2b 7
#define m2f 3
#define sm1 5
#define sm2 6
#define TRIG 8
#define ECHO 9
#define DHTPIN 10
#define DHTTYPE DHT11
#define LDR_PIN  13
#define flame  11
#define MQ4_PIN A0


char command;
DHT dht(DHTPIN, DHTTYPE);
unsigned long sensorLastTime = 0;
int flameValue;

//Ultra
float getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH);
  return (duration * 0.034) / 2;
}


//Sensors
void updateSensors() {
  unsigned long currentTime = millis();
  if (currentTime - sensorLastTime >= 1000) {
    sensorLastTime = currentTime;

    //Distance
    float distance = getDistance();
    Serial.print(distance);
    Serial.print(",");

    //DHT11
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    if (!isnan(humidity) && !isnan(temperature)) {
      Serial.print(humidity);
      Serial.print(",");
      Serial.print(temperature);
      Serial.print(",");
    }

    //LDR
    int ldrState = digitalRead(LDR_PIN); 
    Serial.print(ldrState); 
    Serial.print(",");

    //flame
    flameValue = digitalRead(flame);
    Serial.print(flameValue);
    Serial.print(",");

    // MQ-4
    int mq4Value = analogRead(MQ4_PIN);
    Serial.print(mq4Value);
    Serial.println(",");
  }
}

void setup() {
  pinMode(m1b, OUTPUT);
  pinMode(m1f, OUTPUT);
  pinMode(m2b, OUTPUT);
  pinMode(m2f, OUTPUT);
  pinMode(sm1, OUTPUT);
  pinMode(sm2, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial.begin(9600);
  dht.begin();
}

void loop() {
    if (Serial.available()) {
        command = Serial.read();
        handleCommand(command);
    }

    
    updateSensors();
}


void handleCommand(char cmd) {
    if (cmd == 'B') { 
        digitalWrite(m1b, LOW);
        digitalWrite(m1f, HIGH);
        digitalWrite(m2b, LOW);
        digitalWrite(m2f, HIGH);
        analogWrite(sm1, 180);
        analogWrite(sm2, 180);
    } else if (cmd == 'F') { 
        digitalWrite(m1b, HIGH);
        digitalWrite(m1f, LOW);
        digitalWrite(m2b, HIGH);
        digitalWrite(m2f, LOW);
        analogWrite(sm1, 180);
        analogWrite(sm2, 180);
    } else if (cmd == 'R') { 
        digitalWrite(m1b, HIGH);
        digitalWrite(m1f, LOW);
        digitalWrite(m2b, LOW);
        digitalWrite(m2f, HIGH);
        analogWrite(sm1, 180);
        analogWrite(sm2, 180);
    } else if (cmd == 'L') { 
        digitalWrite(m1b, LOW);
        digitalWrite(m1f, HIGH);
        digitalWrite(m2b, HIGH);
        digitalWrite(m2f, LOW);
        analogWrite(sm1, 180);
        analogWrite(sm2, 180);
    } else if (cmd == 'S') {
        digitalWrite(m1b, LOW);
        digitalWrite(m1f, LOW);
        digitalWrite(m2b, LOW);
        digitalWrite(m2f, LOW);
        analogWrite(sm1, 0);
        analogWrite(sm2, 0);
    }
}

