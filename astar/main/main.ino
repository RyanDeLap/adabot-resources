#include <Servo.h>
#include <AStar32U4.h>
#include <PololuRPiSlave.h>

/* This example program shows how to make the A-Star 32U4 Robot
   Controller into a Raspberry Pi I2C slave.  The RPi and A-Star can
   exchange data bidirectionally, allowing each device to do what it
   does best: high-level programming can be handled in a language such
   as Python on the RPi, while the A-Star takes charge of motor
   control, analog inputs, and other low-level I/O.

   The example and libraries are available for download at:

   https://github.com/pololu/pololu-rpi-slave-arduino-library

   You will need the corresponding Raspberry Pi code, which is
   available in that repository under the pi/ subfolder.  The Pi code
   sets up a simple Python-based web application as a control panel
   for your Raspberry Pi robot.
*/

// Custom data structure that we will use for interpreting the buffer.
// We recommend keeping this under 64 bytes total.  If you change the
// data format, make sure to update the corresponding code in
// a_star.py on the Raspberry Pi.

struct Data
{
  bool yellow, green, red;
  bool buttonA, buttonB, buttonC;

  int16_t leftMotor, rightMotor;
  uint16_t batteryMillivolts;
  uint16_t analog[6];

  bool playNotes;
  char notes[14];
  int16_t leftEncoder, rightEncoder;
  
};

PololuRPiSlave<struct Data, 5> slave; //Set this to either 5 or 20. Depends if you are getting data misreads... I forget what its supposed to be set too. 
PololuBuzzer buzzer;
AStar32U4Motors motors;
AStar32U4ButtonA buttonA;
AStar32U4ButtonB buttonB;
AStar32U4ButtonC buttonC;

volatile int encoderOneCount = 0;
void readEncoderOne() {
    if (digitalRead(0) == HIGH) {
        if (digitalRead(7) == LOW) {
            encoderOneCount = encoderOneCount + 1;
        }
        else {
            encoderOneCount = encoderOneCount - 1;
        }
    }
    else {
        if (digitalRead(7) == HIGH) {
            encoderOneCount = encoderOneCount + 1;
        }
        else {
            encoderOneCount = encoderOneCount - 1;
        }
    }
}

volatile int encoderTwoCount = 0;
void readEncoderTwo() {
    if (digitalRead(1) == HIGH) {
        if (digitalRead(8) == LOW) {
            encoderTwoCount = encoderTwoCount + 1;
        }
        else {
            encoderTwoCount = encoderTwoCount - 1;
        }
    }
    else {
        if (digitalRead(8) == HIGH) {
            encoderTwoCount = encoderTwoCount + 1;
        }
        else {
            encoderTwoCount = encoderTwoCount - 1;
        }
    }
}

void setup()
{
  // Set up the slave at I2C address 20.
  slave.init(20);
  
  //encoder 1 on pins 0 and 7
  pinMode(0, INPUT);
  digitalWrite(0, HIGH);
  pinMode(7, INPUT);
  digitalWrite(7, HIGH);

  //encoder 2 on pins 1 and 8
  pinMode(1, INPUT);
  digitalWrite(1, HIGH);
  pinMode(8, INPUT);
  digitalWrite(8, HIGH);

  attachInterrupt(digitalPinToInterrupt(0), readEncoderOne, CHANGE);
  attachInterrupt(digitalPinToInterrupt(1), readEncoderTwo, CHANGE);
    
}

void loop()
{
  // Call updateBuffer() before using the buffer, to get the latest
  // data including recent master writes.
  slave.updateBuffer();

  // Write various values into the data structure.
  slave.buffer.buttonA = buttonA.isPressed();
  slave.buffer.buttonB = buttonB.isPressed();
  slave.buffer.buttonC = buttonC.isPressed();

  // Change this to readBatteryMillivoltsLV() for the LV model.
  slave.buffer.batteryMillivolts = readBatteryMillivoltsSV();

  for (uint8_t i = 0; i < 6; i++)
  {
    slave.buffer.analog[i] = analogRead(i);
  }

  // READING the buffer is allowed before or after finalizeWrites().
  ledYellow(slave.buffer.yellow);
  ledGreen(slave.buffer.green);
  ledRed(slave.buffer.red);
  motors.setSpeeds(slave.buffer.leftMotor, slave.buffer.rightMotor);

  // Playing music involves both reading and writing, since we only
  // want to do it once.
  static bool startedPlaying = false;

  if (slave.buffer.playNotes && !startedPlaying)
  {
    buzzer.play(slave.buffer.notes);
    startedPlaying = true;
  }
  else if (startedPlaying && !buzzer.isPlaying())
  {
    slave.buffer.playNotes = false;
    startedPlaying = false;
  }
  
  slave.buffer.rightEncoder = encoderTwoCount;
  slave.buffer.leftEncoder = encoderOneCount;
  
  slave.finalizeWrites();
}