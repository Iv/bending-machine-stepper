#include <Arduino.h>
#include <Ticker.h>
#include <AccelStepper.h>
#include <OneButton.h>


const long SMALL_STEP = 200;
const float SPEED = 1000;
const long MAX_SPEED = 4000;
const long ACCELERATION = 1000;

bool stepper_l_stopping = false;
bool stepper_r_stopping = false;

AccelStepper stepper_l(AccelStepper::DRIVER, 2, 3); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepper_r(AccelStepper::DRIVER, 5, 6); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

OneButton btn_ll(8, true, true);
OneButton btn_lr(9, true, true);
OneButton btn_rl(10, true, true);
OneButton btn_rr(11, true, true);



void readCommand();
void btn_ll_click();
void btn_lr_click();
void btn_rl_click();
void btn_rr_click();


void btn_ll_long_press_start();
void btn_lr_long_press_start();
void btn_rl_long_press_start();
void btn_rr_long_press_start();


void btn_ll_long_press_stop();
void btn_lr_long_press_stop();
void btn_rl_long_press_stop();
void btn_rr_long_press_stop();


void btn_ll_long_press_press() {stepper_l.runSpeed();}
void btn_lr_long_press_press() {stepper_l.runSpeed();}
void btn_rl_long_press_press() {stepper_r.runSpeed();}
void btn_rr_long_press_press() {stepper_r.runSpeed();}


Ticker command_reader_ticker(&readCommand, 50);


void setup() {

  Serial.begin(9600);

  stepper_l.setEnablePin(4);
  stepper_l.setPinsInverted(true, true, false);
  stepper_l.setMaxSpeed(MAX_SPEED);
  stepper_l.setAcceleration(ACCELERATION);
  

  stepper_r.setEnablePin(7);
  stepper_r.setPinsInverted(true, true, false);
  stepper_r.setMaxSpeed(MAX_SPEED);
  stepper_r.setAcceleration(ACCELERATION);
  
  command_reader_ticker.start();

  btn_ll.attachClick(&btn_ll_click);
  btn_lr.attachClick(&btn_lr_click);
  btn_rl.attachClick(&btn_rl_click);  
  btn_rr.attachClick(&btn_rr_click);

  btn_ll.attachLongPressStart(&btn_ll_long_press_start);
  btn_lr.attachLongPressStart(&btn_lr_long_press_start);
  btn_rl.attachLongPressStart(&btn_rl_long_press_start);
  btn_rr.attachLongPressStart(&btn_rr_long_press_start);


  btn_ll.attachLongPressStop(&btn_ll_long_press_stop);
  btn_lr.attachLongPressStop(&btn_lr_long_press_stop);
  btn_rl.attachLongPressStop(&btn_rl_long_press_stop);
  btn_rr.attachLongPressStop(&btn_rr_long_press_stop);

  btn_ll.attachDuringLongPress(&btn_ll_long_press_press);
  btn_lr.attachDuringLongPress(&btn_lr_long_press_press);
  btn_rl.attachDuringLongPress(&btn_rl_long_press_press);
  btn_rr.attachDuringLongPress(&btn_rr_long_press_press);
}


void loop() {

  if(!btn_ll.isLongPressed() && !btn_lr.isLongPressed()) stepper_l.run();
  if(!btn_rl.isLongPressed() && !btn_rr.isLongPressed()) stepper_r.run();

  btn_ll.tick();
  btn_lr.tick();
  btn_rl.tick();
  btn_rr.tick();

  command_reader_ticker.update();
}

void readCommand(){
  if (Serial.available() > 0) {
    // int command = Serial.read();

    // switch (command)
    
    // default:
    //   break;
    // }
  }
}


void btn_ll_click(){
  stepper_l.moveTo(stepper_l.targetPosition() - SMALL_STEP);
}

void btn_lr_click(){
  stepper_l.moveTo(stepper_l.targetPosition() + SMALL_STEP);
}

void btn_rl_click() {
  stepper_r.moveTo(stepper_r.targetPosition() - SMALL_STEP);
}

void btn_rr_click(){
  stepper_r.moveTo(stepper_r.targetPosition() + SMALL_STEP);
}



void btn_ll_long_press_start() {
  if(stepper_l.isRunning()) stepper_l.stop();
  stepper_l.setSpeed(-SPEED);
}

void btn_lr_long_press_start(){
  if(stepper_l.isRunning()) stepper_l.stop();
  stepper_l.setSpeed(SPEED);
}

void btn_rl_long_press_start(){
  if(stepper_r.isRunning()) stepper_r.stop();
  stepper_r.setSpeed(-SPEED);
}
void btn_rr_long_press_start(){
  if(stepper_r.isRunning()) stepper_r.stop();
  stepper_r.setSpeed(SPEED);
}


void btn_ll_long_press_stop()
{
  stepper_l.stop();
  stepper_l_stopping = true;
}

void btn_lr_long_press_stop(){
  stepper_l.stop();
  stepper_l_stopping = true;
}

void btn_rl_long_press_stop(){
  stepper_r.stop();
  stepper_r_stopping = true;
}

void btn_rr_long_press_stop(){
  stepper_r.stop();
  stepper_r_stopping = true;
}
