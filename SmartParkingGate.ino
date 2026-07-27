#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Pin Definitions 
const int trigPin = 9;
const int echoPin = 10;
const int servoPin = 6;
const int buzzerPin = 5;

// Constants 
const int GATE_OPEN_ANGLE = 90;
const int GATE_CLOSE_ANGLE = 0;

const int DETECTION_DISTANCE = 15;

const unsigned long GATE_OPEN_TIME = 2000;

const int BUZZER_BEEP_TIME = 200;

// Objects
Servo gateServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Gate States
enum GateState
{
  IDLE,
  OPEN,
  WAIT,
  CLOSE
};

GateState currentState = IDLE;
GateState previousState = CLOSE;

// State Names
const char* stateNames[] =
{
  "IDLE",
  "OPEN",
  "WAIT",
  "CLOSE"
};

// Global Variables 
unsigned long gateTimer = 0;
bool vehicleDetected = false;

// Function Prototypes 
float readDistance();
void printStatus(float distance);
void updateGateState(float distance);
void updateLCD(float distance);
void beep();


void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  gateServo.attach(servoPin);
  gateServo.write(GATE_CLOSE_ANGLE);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Smart Gate");
  lcd.setCursor(0, 1);
  lcd.print("Initializing");

  delay(2000);

  lcd.clear();
}


void loop()
{
  float distance = readDistance();

  printStatus(distance);

  updateGateState(distance);

  updateLCD(distance);

  delay(100);
}


// READ DISTANCE
float readDistance()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0)
    return -1;

  return duration * 0.0343 / 2.0;
}


// SERIAL MONITOR
void printStatus(float distance)
{
  Serial.print("State: ");
  Serial.print(stateNames[currentState]);

  Serial.print(" | Distance: ");

  if (distance == -1)
  {
    Serial.println("Sensor Error");
  }
  else
  {
    Serial.print(distance);
    Serial.println(" cm");
  }
}


// LCD DISPLAY
void updateLCD(float distance)
{
  // Update only when state changes
  if (currentState != previousState)
  {
    lcd.clear();

    switch (currentState)
    {
      case IDLE:
        lcd.setCursor(0, 0);
        lcd.print("Gate: CLOSED");
        break;

      case OPEN:
        lcd.setCursor(0, 0);
        lcd.print("Vehicle Found");
        lcd.setCursor(0, 1);
        lcd.print("Opening...");
        break;

      case WAIT:
        lcd.setCursor(0, 0);
        lcd.print("Gate OPEN");
        lcd.setCursor(0, 1);
        lcd.print("Please Pass");
        break;

      case CLOSE:
        lcd.setCursor(0, 0);
        lcd.print("Closing Gate");
        lcd.setCursor(0, 1);
        lcd.print("Thank You");
        break;
    }

    previousState = currentState;
  }

  // Update only the distance while idle
  if (currentState == IDLE)
  {
    lcd.setCursor(0, 1);

    if (distance == -1)
    {
      lcd.print("Sensor Error   ");
    }
    else
    {
      lcd.print("Dist:");
      lcd.print(distance, 1);
      lcd.print("cm   ");
    }
  }
}


// STATE MACHINE
void updateGateState(float distance)
{
  switch (currentState)
  {
    case IDLE:

      gateServo.write(GATE_CLOSE_ANGLE);

      if (distance > DETECTION_DISTANCE || distance == -1)
      {
        vehicleDetected = false;
      }

      if (distance <= DETECTION_DISTANCE &&
          distance != -1 &&
          !vehicleDetected)
      {
        vehicleDetected = true;
        currentState = OPEN;
      }

      break;

    case OPEN:

      gateServo.write(GATE_OPEN_ANGLE);

      Serial.println(">> Gate Opened");

      beep();

      gateTimer = millis();

      currentState = WAIT;

      break;

    case WAIT:

    gateServo.write(GATE_OPEN_ANGLE);

    if (distance <= DETECTION_DISTANCE)
    {
        // Vehicle still present
        gateTimer = millis();   // Reset timer
    }

    if (millis() - gateTimer >= GATE_OPEN_TIME)
    {
        currentState = CLOSE;
    }

    break;

    case CLOSE:

      gateServo.write(GATE_CLOSE_ANGLE);

      Serial.println(">> Gate Closed");

      beep();

      currentState = IDLE;

      break;
  }
}


// BUZZER
void beep()
{
  digitalWrite(buzzerPin, HIGH);
  delay(BUZZER_BEEP_TIME);
  digitalWrite(buzzerPin, LOW);
}