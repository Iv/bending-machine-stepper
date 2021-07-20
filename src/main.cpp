#include <Arduino.h>
#include <Ticker.h>
#include <AccelStepper.h>

AccelStepper stepper(AccelStepper::DRIVER, 2, 3); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

const int RANGE = 40000;

bool stop = false;


void readCommand();

Ticker command_reader_ticker(&readCommand, 50);


void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  stepper.setEnablePin(4);
  stepper.setPinsInverted(true, true, false);
  stepper.setMaxSpeed(4000);
  stepper.setAcceleration(1000);
  stepper.moveTo(RANGE);

  command_reader_ticker.start();
}

void set_rand_position() {
  stepper.moveTo(rand() % RANGE);
  Serial.println(stepper.targetPosition());
}

void loop() {
  // put your main code here, to run repeatedly:

  if (stepper.distanceToGo() == 0 && !stop){
    set_rand_position();
  }

  stepper.run();
  command_reader_ticker.update();
}

void readCommand(){
  if (Serial.available() > 0) {
    int command = Serial.read();

    switch (command)
    {
    case 's':
      stop = !stop;
      if(stop) {
        Serial.println("Stop!");
        stepper.stop();
      } else { 
        Serial.println("Start!");
        set_rand_position();
      }
      break;
    
    default:
      break;
    }
  }
}



