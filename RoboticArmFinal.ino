#include <IRremote.h>
#include <Servo.h>

#define DELAY           30

#define IR_PIN      3
#define LEFT_SERVO  5
#define RIGHT_SERVO 10
#define CLAW_SERVO  6
#define BASE_SERVO  9

// btec rubbish remote - remote-1
#define POWER_1     0xFFA25D
#define MENU_1      0xFFE21D
#define TEST_1      0xFF22DD
#define RETURN_1    0xFFC23D
#define PLUS_VOL_1  0xFF02FD
#define MINUS_VOL_1 0xFF9867
#define PLAY_1      0xFFA857
#define BACK_1      0xFFE01F
#define FORWARD_1   0xFF906F
#define C_1         0xFFB04F
#define ZERO_1      0xFF6897
#define ONE_1       0xFF30CF
#define TWO_1       0xFF18E7
#define THREE_1     0xFF7A85
#define FOUR_1      0xFF10EF
#define FIVE_1      0xFF38C7
#define SIX_1       0xFF5AA5
#define SEVEN_1     0xFF42BD
#define EIGHT_1     0xFF4AB5
#define NINE_1      0xFF52AD

#define REMOTE      1

IRrecv irRecv(IR_PIN);
decode_results Signal;

Servo leftServo;
Servo rightServo;
Servo clawServo;
Servo baseServo;

unsigned int ANGLE_INCREMENT = 5;

static unsigned int leftServoMax    = 160;
static unsigned int leftServoMin    = 50;
static unsigned int rightServoMax   = 160;
static unsigned int rightServoMin   = 20;
static unsigned int clawServoMax    = 100;
static unsigned int clawServoMin    = 40;
static unsigned int baseServoMax    = 180;
static unsigned int baseServoMin    = 0;

static unsigned int leftServoReset  = 100;
static unsigned int rightServoReset = 90 ;
static unsigned int clawServoReset  = 65;
static unsigned int baseServoReset  = 90;

unsigned int leftServoAngle  = leftServoReset;
unsigned int rightServoAngle = rightServoReset;
unsigned int clawServoAngle  = clawServoReset;
unsigned int baseServoAngle  = baseServoReset;

void setup() {
  Serial.begin(9600);
  
  leftServo.attach(LEFT_SERVO);
  rightServo.attach(RIGHT_SERVO);
  clawServo.attach(CLAW_SERVO);
  baseServo.attach(BASE_SERVO);

  leftServo.write(leftServoAngle);
  delay(DELAY);
  rightServo.write(rightServoAngle);
  delay(DELAY);
  clawServo.write(clawServoAngle);
  delay(DELAY);
  baseServo.write(baseServoAngle);
  delay(DELAY);
  
  Serial.println("Servos attached!");
  
  irRecv.enableIRIn();
  
}

void loop() {
  if(irRecv.decode(&Signal)){
    
    Serial.print("0x");
    Serial.println(Signal.value, HEX);

    //Reset servo positions
    if(Signal.value == POWER_1){
      leftServoAngle  = leftServoReset;
      rightServoAngle = rightServoReset;
      clawServoAngle  = clawServoReset;
      baseServoAngle  = baseServoReset;
    }

    //Output the servo values
    else if(Signal.value == MENU_1){
      Serial.print("LEFT: ");
      Serial.println(leftServo.read());
      Serial.print("RIGHT: ");
      Serial.println(rightServo.read());
      Serial.print("CLAW: ");
      Serial.println(clawServo.read());
      Serial.print("BASE: ");
      Serial.println(baseServo.read());
    }
    
    //Change the speed of the movement
    else if(Signal.value == ONE_1){
      ANGLE_INCREMENT--;
      if(ANGLE_INCREMENT <= 0)
        ANGLE_INCREMENT = 1;
    }
    else if(Signal.value == TWO_1){
      ANGLE_INCREMENT++;
      if(ANGLE_INCREMENT > 15)
        ANGLE_INCREMENT = 15;
    }
    
    //Forward or Backward
    else if(Signal.value == PLUS_VOL_1){
      Serial.println("PLUS REGISTERED");
      leftServoAngle += ANGLE_INCREMENT;
      Serial.println(leftServoAngle);
      if(leftServoAngle > leftServoMax)
        leftServoAngle = leftServoMax;
    }
    else if(Signal.value == MINUS_VOL_1){
      Serial.println("MINUS REGISTERED");
      leftServoAngle -= ANGLE_INCREMENT;
      Serial.println(leftServoAngle);
      if(leftServoAngle < leftServoMin)
        leftServoAngle = leftServoMin;
    }
    
    // Left or Right
    else if(Signal.value == FORWARD_1){
      baseServoAngle += ANGLE_INCREMENT;
      if(baseServoAngle > baseServoMax)
        baseServoAngle = baseServoMax;
    }
    else if(Signal.value == BACK_1){
      baseServoAngle -= ANGLE_INCREMENT;
      if(baseServoAngle < baseServoMin)
        baseServoAngle = baseServoMin;
    }
    
    // Up and Down
    else if(Signal.value == RETURN_1){
      rightServoAngle += ANGLE_INCREMENT;
      if(rightServoAngle > rightServoMax)
        rightServoAngle = rightServoMax;
    }
    else if(Signal.value == TEST_1){
      rightServoAngle -= ANGLE_INCREMENT;
      if(rightServoAngle < rightServoMin)
        rightServoAngle = rightServoMin;
    }
    
    // Open and Close claw
    else if(Signal.value == ZERO_1){
      clawServoAngle -= ANGLE_INCREMENT;
      if(clawServoAngle < clawServoMin)
        clawServoAngle = clawServoMin;
    }
    else if(Signal.value == C_1){
      clawServoAngle += ANGLE_INCREMENT;
      if(clawServoAngle > clawServoMax)
        clawServoAngle = clawServoMax;
    
    }
    
    irRecv.resume();
  }
  
  rightServo.write(rightServoAngle);
  delay(DELAY);
  leftServo.write(leftServoAngle);
  delay(DELAY);
  baseServo.write(baseServoAngle);
  delay(DELAY);
  clawServo.write(clawServoAngle);
  delay(DELAY);
}
