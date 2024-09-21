// Team and Software Lead = Sovit Bhandari



// Pin definitions for button, IR sensors, and motors
const int BTN_PIN = 13; 
const int IR_LEFT_PIN = 7; 
const int IR_RIGHT_PIN = 8; 

const int MOTOR_LEFT_FORWARD_PIN = 5; 
const int MOTOR_LEFT_BACKWARD_PIN = 6; 
const int MOTOR_RIGHT_FORWARD_PIN = 9; 
const int MOTOR_RIGHT_BACKWARD_PIN = 10; 

// Variables to manage robot state and button debouncing
bool isRobotOn = false; 
bool isButtonPressed = false; 
int lastButtonState = LOW; 
unsigned long lastDebounceTime = 0; 
const unsigned long debounceDelay = 50; 

void setup() {
  // Initialize pins for inputs and outputs
  pinMode(BTN_PIN, INPUT_PULLUP); 
  pinMode(IR_LEFT_PIN, INPUT);
  pinMode(IR_RIGHT_PIN, INPUT);
  
  pinMode(MOTOR_LEFT_FORWARD_PIN, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD_PIN, OUTPUT);
  
  // Set up serial communication for debugging
  Serial.begin(9600); 
}

void loop() {
  // Debounce logic to handle button state changes
  int buttonState = digitalRead(BTN_PIN);
  if (buttonState != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (buttonState != isButtonPressed) {
      isButtonPressed = buttonState;
      if (isButtonPressed == HIGH) {
        isRobotOn = !isRobotOn; 
      }
    }
  }
  lastButtonState = buttonState;
  
  // Robot movement logic when it is turned on
  if (isRobotOn) {
    int irLeftValue = digitalRead(IR_LEFT_PIN);
    int irRightValue = digitalRead(IR_RIGHT_PIN);
    
    // Move forward if both sensors detect the line
    if (irLeftValue == LOW && irRightValue == LOW) {
      Serial.println("Forward");
      analogWrite(MOTOR_LEFT_FORWARD_PIN, 75);
      digitalWrite(MOTOR_LEFT_BACKWARD_PIN, LOW);
      analogWrite(MOTOR_RIGHT_FORWARD_PIN, 75);
      digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, LOW);
    } 
    // Turn left if only the right sensor detects the line
    else if (irLeftValue == HIGH && irRightValue == LOW) {
      Serial.println("Turning Left");
      analogWrite(MOTOR_LEFT_FORWARD_PIN, 0);  
      digitalWrite(MOTOR_LEFT_BACKWARD_PIN, LOW);
      analogWrite(MOTOR_RIGHT_FORWARD_PIN, 75); 
      digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, LOW);
    } 
    // Turn right if only the left sensor detects the line
    else if (irLeftValue == LOW && irRightValue == HIGH) {
      Serial.println("Turning Right");
      analogWrite(MOTOR_LEFT_FORWARD_PIN, 75);  
      digitalWrite(MOTOR_LEFT_BACKWARD_PIN, LOW);
      analogWrite(MOTOR_RIGHT_FORWARD_PIN, 0);  
      digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, LOW);
    } 
    // Stop the robot when no line is detected
    else {
      Serial.println("Stop");
      analogWrite(MOTOR_LEFT_FORWARD_PIN, 0);
      digitalWrite(MOTOR_LEFT_BACKWARD_PIN, LOW);
      analogWrite(MOTOR_RIGHT_FORWARD_PIN, 0);
      digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, LOW);
    }
  } else {
    // Stop all motors when the robot is off
    analogWrite(MOTOR_LEFT_FORWARD_PIN, 0);
    digitalWrite(MOTOR_LEFT_BACKWARD_PIN, LOW);
    analogWrite(MOTOR_RIGHT_FORWARD_PIN, 0);
    digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, LOW);
  }
}
